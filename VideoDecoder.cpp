#include "VideoDecoder.h"
#include <iostream>
#include <QtConcurrent/QtConcurrent>

videoDecoder::videoDecoder()
     :videoBuffer_(FrameBuffer<AVPacket*>(3, false)),
      audioBuffer_(FrameBuffer<AVPacket*>(9, false))
{
    avformat_network_init();
    audioTimer_.start();
}

videoDecoder::~videoDecoder() {
    stop();
    avformat_network_deinit();
}

bool videoDecoder::openFile(const QString &filename) {
    if (avformat_open_input(&formatCtx_, filename.toUtf8().constData(), nullptr, nullptr) != 0) {
        std::cout << "无法打开文件";
        return false;
    }

    if (avformat_find_stream_info(formatCtx_, nullptr) < 0) {
        std::cout << "无法获取流信息";
        return false;
    }

    for (uint i = 0; i < formatCtx_->nb_streams; i++) {
        AVStream* stream = formatCtx_->streams[i];
        AVCodecParameters* codecParams = stream->codecpar;

        if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIdx_ < 0) {
            videoStreamIdx_ = i;
        } else if (codecParams->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIdx_ < 0) {
            audioStreamIdx_ = i;
        }
    }

    if (videoStreamIdx_ < 0 && audioStreamIdx_ < 0) {
        return false;
    }

    return true;
}

void videoDecoder::start() {
    isRunning_ = true;
    demuxThread_ = QThread::create([this](){ demuxLoop(); });
    audioThread_ = QThread::create([this](){ audioDecodeLoop(); });
    videoThread_ = QThread::create([this](){ videoDecodeLoop(); });

    demuxThread_->start();
    audioThread_->start();
    videoThread_->start();
}

void videoDecoder::stop() {
    isRunning_ = false;
    if (demuxThread_) { demuxThread_->quit(); demuxThread_->wait(); delete demuxThread_; demuxThread_ = nullptr; }
    if (audioThread_) { audioThread_->quit(); audioThread_->wait(); delete audioThread_; audioThread_ = nullptr; }
    if (videoThread_) { videoThread_->quit(); videoThread_->wait(); delete videoThread_; videoThread_ = nullptr; }
}

void videoDecoder::pause() {
    isPaused_ = true;
}

void videoDecoder::resume() {
    {
        QMutexLocker locker(&pauseMutex_);
        isPaused_ = false;
    }
    pauseCond_.wakeAll(); // 唤醒所有等待的线程
}

// 通用暂停检查函数
inline void checkPause(QMutex &pauseMutex, QWaitCondition &pauseCond, std::atomic<bool> &isPaused, std::atomic<bool> &isRunning) {
    QMutexLocker locker(&pauseMutex);
    while (isPaused && isRunning) {
        pauseCond.wait(&pauseMutex);
    }
}

void videoDecoder::demuxLoop() {
    AVPacket* pkt = av_packet_alloc();
    while (isRunning_) {
        if (av_read_frame(formatCtx_, pkt) < 0) break;

        if (pkt->stream_index == videoStreamIdx_) {
            videoBuffer_.enqueue(av_packet_clone(pkt));
        } else if (pkt->stream_index == audioStreamIdx_) {
            audioBuffer_.enqueue(av_packet_clone(pkt));
        }
        av_packet_unref(pkt);
    }
    av_packet_free(&pkt);
}

void videoDecoder::audioDecodeLoop() {
    AVCodecParameters *aCodecPar = formatCtx_->streams[audioStreamIdx_]->codecpar;
    const AVCodec *aCodec = avcodec_find_decoder(aCodecPar->codec_id);
    AVCodecContext *aCtx = avcodec_alloc_context3(aCodec);
    avcodec_parameters_to_context(aCtx, aCodecPar);
    avcodec_open2(aCtx, aCodec, nullptr);

    // 创建音频重采样器
    AVChannelLayout out_ch_layout;
    av_channel_layout_default(&out_ch_layout, 2); // 立体声

    AVChannelLayout in_ch_layout;
    av_channel_layout_copy(&in_ch_layout, &aCtx->ch_layout); // 复制输入布局

    SwrContext *swr_ctx = NULL;
    int ret = swr_alloc_set_opts2(
        &swr_ctx,                   // SwrContext 指针
        &out_ch_layout,              // 输出声道布局
        AV_SAMPLE_FMT_S16,           // 输出采样格式
        44100,                       // 输出采样率
        &in_ch_layout,               // 输入声道布局
        aCtx->sample_fmt,       // 输入采样格式
        aCtx->sample_rate,      // 输入采样率
        0,                           // 日志偏移
        NULL                         // 日志上下文
        );
    swr_init(swr_ctx);
    AVFrame *aFrame = av_frame_alloc();

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16); // 16位采样格式

    // 获取默认音频设备
    QAudioDevice outputDevice = QMediaDevices::defaultAudioOutput();

    // 检查格式是否支持
    if (!outputDevice.isFormatSupported(format)) {
        qWarning() << "默认音频格式不支持，使用最接近的格式";
        format = outputDevice.preferredFormat();
    }

    // 创建音频输出 (Qt6 使用 QAudioSink)
    QAudioSink* audioSink = new QAudioSink(outputDevice, format);

    // 启动音频设备
    QIODevice* audioDevice = audioSink->start();
    if (!audioDevice) {
        std::cout << "无法启动音频设备";
        delete audioSink;
        return;
    }

    if (ret < 0 || !swr_ctx) {
        std::cout << "无法创建音频重采样器";
        return;
    }

    while (isRunning_) {
        auto aPktOpt = audioBuffer_.dequeue();
        AVPacket* aPkt;
        if (aPktOpt) aPkt = aPktOpt.value();

        if (avcodec_send_packet(aCtx, aPkt) == 0) {
            while (avcodec_receive_frame(aCtx, aFrame) == 0) {
                double pts = (aFrame->pts == AV_NOPTS_VALUE) ?
                                 aFrame->best_effort_timestamp * av_q2d(formatCtx_->streams[audioStreamIdx_]->time_base) :
                                 aFrame->pts * av_q2d(formatCtx_->streams[audioStreamIdx_]->time_base);
                double duration = aFrame->nb_samples / (double)aFrame->sample_rate;
                //std::cout << "audio pts : " << pts << std::endl;
                // 更新音频时钟
                updateAudioClock(pts, duration);

                // 存储音频帧信息用于同步
                {
                     //QMutexLocker lock(&audioFrameMutex_);
                    //audioFrames_.enqueue({pts, duration});
                }


                // 计算输出缓冲区大小
                int out_samples = swr_get_out_samples(swr_ctx, aFrame->nb_samples);
                if (out_samples <= 0) {
                    std::cout << "out_samples error" << std::endl;
                    continue;
                }

                // 分配输出缓冲区
                uint8_t *output_buffer = nullptr;
                int buffer_size = av_samples_alloc(&output_buffer, nullptr,
                                                   format.channelCount(),
                                                   out_samples,
                                                   AV_SAMPLE_FMT_S16, 0);
                if (buffer_size < 0) {
                    std::cout << "buffer_size error" << std::endl;
                    continue;
                }

                int samples_converted = swr_convert(
                    swr_ctx,
                    &output_buffer,
                    out_samples,
                    (const uint8_t **)aFrame->data,
                    aFrame->nb_samples
                    );

                if (samples_converted > 0) {
                    // 计算实际数据大小 (样本数 × 通道数 × 每个样本字节数)
                    int data_size = samples_converted * format.channelCount() * (format.bytesPerSample());

                    // 动态等待：按设备空闲比例调整
                    int free_bytes = audioSink->bytesFree();
                    if (free_bytes < data_size) {
                        // 计算缓冲区空闲比例 (0~1)
                        float free_ratio = static_cast<float>(free_bytes) / data_size;

                        // 动态等待公式：基础等待 + 空闲比例补偿
                        int dynamicWait = 10 + static_cast<int>((1 - free_ratio) * 40); // 10ms~50ms
                        dynamicWait = qBound(5, dynamicWait, 100); // 限制范围5ms~100ms
                        std::cout << "wait for " << dynamicWait << " ms " << std::endl;

                        QThread::msleep(dynamicWait);
                        free_bytes = audioSink->bytesFree(); // 重新检查
                    }

                    // 写入前二次验证缓冲区
                    if (free_bytes >= data_size) {
                        qint64 bytesWritten = audioDevice->write(
                            reinterpret_cast<const char*>(output_buffer),
                            data_size
                            );
                        std::cout << "audio in:" << bytesWritten << "/" << data_size << std::endl;

                        if (bytesWritten != data_size) {
                            std::cout << "audio not complete:" << bytesWritten << "/" << data_size << std::endl;
                        }
                    } else {
                        std::cout << "drop audio frame" << std::endl; // 记录异常
                    }

                    // 写入音频设备

                }

                // 释放输出缓冲区
                if (output_buffer) {
                    av_freep(&output_buffer);
                }
            }
        }
        av_packet_free(&aPkt);
    }

    av_frame_free(&aFrame);
    avcodec_free_context(&aCtx);
    swr_free(&swr_ctx);
    av_channel_layout_uninit(&out_ch_layout);
    av_channel_layout_uninit(&in_ch_layout);
}

void videoDecoder::updateAudioClock(double pts, double duration) {
    QMutexLocker lock(&audioClockMutex_);
    audioClock_ = pts + duration; // 更新为当前帧结束时间
}


void videoDecoder::videoDecodeLoop() {
    SwsContext *swsctx = nullptr;
    AVCodecParameters *vCodecPar = formatCtx_->streams[videoStreamIdx_]->codecpar;
    const AVCodec *vCodec = avcodec_find_decoder(vCodecPar->codec_id);
    AVCodecContext *vCtx = avcodec_alloc_context3(vCodec);
    avcodec_parameters_to_context(vCtx, vCodecPar);
    avcodec_open2(vCtx, vCodec, nullptr);

    AVFrame *frame = av_frame_alloc();

    while (isRunning_) {
        auto vPktOpt = videoBuffer_.dequeue();
        AVPacket* vPkt;
        if (vPktOpt) vPkt = vPktOpt.value();

        if (avcodec_send_packet(vCtx, vPkt) == 0) {
            while (avcodec_receive_frame(vCtx, frame) == 0) {
                double pts = (frame->pts == AV_NOPTS_VALUE) ?
                                 frame->best_effort_timestamp * av_q2d(formatCtx_->streams[videoStreamIdx_]->time_base) :
                                 frame->pts * av_q2d(formatCtx_->streams[videoStreamIdx_]->time_base);

                // 音视频同步逻辑
                double audioClock = getAudioClock();
                double diff = pts - audioClock;

                std::cout << "vedio emit, pts : " << pts << ",audioclock is " << audioClock << ",diff is " << diff << std::endl;

                // 处理不同步情况
                if (diff > 0.1) { // 视频超前>100ms
                    std::cout << "sleep " << diff << std::endl;
                    QThread::usleep(static_cast<size_t>(diff * 1000000));
                } else if (diff < -0.1) { // 视频落后>100ms
                    std::cout << "drop " << diff << std::endl;
                    continue; // 丢弃当前帧
                }

                if (!swsctx) {
                    swsctx = sws_getContext(vCtx->width, vCtx->height, vCtx->pix_fmt,
                                            vCtx->width, vCtx->height, AV_PIX_FMT_RGB32,
                                            SWS_BILINEAR, nullptr, nullptr, nullptr);
                }

                QImage img(vCtx->width, vCtx->height, QImage::Format_RGB32);
                uint8_t *dstData[4] = {img.bits(), nullptr, nullptr, nullptr};
                int dstLinesize[4] = {static_cast<int>(img.bytesPerLine()), 0, 0, 0};
                sws_scale(swsctx, frame->data, frame->linesize, 0, vCtx->height, dstData, dstLinesize);

                emit frameReady(img);
            }
        }
        av_packet_free(&vPkt);
    }

    sws_freeContext(swsctx);
    av_frame_free(&frame);
    avcodec_free_context(&vCtx);
}

double videoDecoder::getAudioClock() {
    QMutexLocker lock(&audioClockMutex_);
    if (!audioFrames_.isEmpty()) {
        // 计算当前音频播放位置：最新帧PTS + 已播放时长
        const AudioFrame& frame = audioFrames_.head();
        double playedTime = audioTimer_.elapsed() / 1000.0;
        return frame.pts + std::min(playedTime, frame.duration);
    }
    return audioClock_;
}
