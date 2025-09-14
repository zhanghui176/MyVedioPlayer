#include "VideoDecoder.h"
#include <iostream>

VideoDecoder::VideoDecoder()
    : videoBuffer_(FrameBuffer<AVPacket*>(100, false)),
    audioBuffer_(FrameBuffer<AVPacket*>(200, false)),
    videoFrameBuffer_(FrameBuffer<AVFrame*>(10, false)),
    audioFrameBuffer_(FrameBuffer<AVFrame*>(10, false))
{
    avformat_network_init();
}

VideoDecoder::~VideoDecoder() {
    stop();
    avformat_network_deinit();
    avcodec_free_context(&vedioCodecCtx_);
    avcodec_free_context(&audioCodecCtx_);
}

bool VideoDecoder::openFile(const QString &filename) {
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

    vedioCodecPar_ = formatCtx_->streams[videoStreamIdx_]->codecpar;
    vedioCodec_ = avcodec_find_decoder(vedioCodecPar_->codec_id);
    vedioCodecCtx_ = avcodec_alloc_context3(vedioCodec_);
    avcodec_parameters_to_context(vedioCodecCtx_, vedioCodecPar_);
    avcodec_open2(vedioCodecCtx_, vedioCodec_, nullptr);

    audioCodecPar_ = formatCtx_->streams[audioStreamIdx_]->codecpar;
    audioCodec_ = avcodec_find_decoder(audioCodecPar_->codec_id);
    audioCodecCtx_ = avcodec_alloc_context3(audioCodec_);
    avcodec_parameters_to_context(audioCodecCtx_, audioCodecPar_);
    avcodec_open2(audioCodecCtx_, audioCodec_, nullptr);

    bytesPerSecond_ = audioCodecCtx_->sample_rate * audioCodecCtx_->ch_layout.nb_channels *
                      av_get_bytes_per_sample(audioCodecCtx_->sample_fmt);
    videoDurationIdeal_ = 1/av_q2d(formatCtx_->streams[videoStreamIdx_]->avg_frame_rate);
    std::cout << "audio sample rate: " << audioCodecCtx_->sample_rate
              << "| channels: " << audioCodecCtx_->ch_layout.nb_channels
              << "| bytesPerSecond: "<< bytesPerSecond_
              << "| videoDuration: " << videoDurationIdeal_ << std::endl;

    return true;
}

double VideoDecoder::getDuration() const {
    if (!formatCtx_) return 0.0;

    // 优先使用视频流时长
    if (videoStreamIdx_ >= 0) {
        AVStream *videoStream = formatCtx_->streams[videoStreamIdx_];
        if (videoStream->duration != AV_NOPTS_VALUE) {
            return videoStream->duration * av_q2d(videoStream->time_base);
        }
    }

    // 其次使用音频流时长
    if (audioStreamIdx_ >= 0) {
        AVStream *audioStream = formatCtx_->streams[audioStreamIdx_];
        if (audioStream->duration != AV_NOPTS_VALUE) {
            return audioStream->duration * av_q2d(audioStream->time_base);
        }
    }

    // 最后使用容器时长
    if (formatCtx_->duration != AV_NOPTS_VALUE) {
        return static_cast<double>(formatCtx_->duration) / AV_TIME_BASE;
    }

    return 0.0;
}

void VideoDecoder::start() {
    isRunning_ = true;
    demuxThread_ = QThread::create([this](){ demuxLoop(); });
    audioDecodeThread_ = QThread::create([this](){ audioDecodeLoop(); });
    videoDecodeThread_ = QThread::create([this](){ videoDecodeLoop(); });
    videoThread_ = QThread::create([this](){ videoThread(); });
    audioThread_ = QThread::create([this](){ audioThread(); });

    demuxThread_->start();
    audioDecodeThread_->start();
    videoDecodeThread_->start();
    videoThread_->start();
    audioThread_->start();
}

void VideoDecoder::stop() {
    isRunning_ = false;
    if (demuxThread_) { demuxThread_->quit(); demuxThread_->wait(); delete demuxThread_; demuxThread_ = nullptr; }
    if (audioDecodeThread_) { audioDecodeThread_->quit(); audioDecodeThread_->wait(); delete audioDecodeThread_; audioDecodeThread_ = nullptr; }
    if (videoDecodeThread_) { videoDecodeThread_->quit(); videoDecodeThread_->wait(); delete videoDecodeThread_; videoDecodeThread_ = nullptr; }
    if (videoThread_) { videoThread_->quit(); videoThread_->wait(); delete videoThread_; videoThread_ = nullptr; }
    if (audioThread_) { audioThread_->quit(); audioThread_->wait(); delete audioThread_; audioThread_ = nullptr; }
}

void VideoDecoder::pause() {
    {
        std::lock_guard<std::mutex> locker(pauseMutex_);
        isPaused_ = true;
        std::cout << "Pause here" << std::endl;
    }
    // 停止音频设备
    if (audioSink_) {
        audioSink_->suspend(); // 暂停音频输出
    }
    pauseCond_.notify_all();
}

void VideoDecoder::resume() {
    {
        std::lock_guard<std::mutex> locker(pauseMutex_);
        isPaused_ = false;
        std::cout << "Resume here" << std::endl;
    }

    audioFrameBuffer_.clear();
    {
        std::lock_guard<std::mutex> audioLk(audioDeviceMtx_);
        if (audioSink_) {
            // 彻底清理缓冲，重新 start
            audioSink_->stop();
            audioDevice_ = audioSink_->start();
            if (!audioDevice_) {
                std::cout << "重新启动音频设备失败" << std::endl;
            }
            audioSink_->resume();
        }
    }
    pauseCond_.notify_all(); // 唤醒所有等待的线程
}

void VideoDecoder::seek(double time)
{
    isSeeking_ = true;
    if (time == 0 || !formatCtx_) return;

    pause();
    QThread::msleep(3);

    int64_t ts = static_cast<int64_t>(time / av_q2d(formatCtx_->streams[videoStreamIdx_]->time_base));

    if (av_seek_frame(formatCtx_, videoStreamIdx_, ts, AVSEEK_FLAG_BACKWARD) < 0)
    {
        std::cout << "seek failed" << std::endl;
        resume();
        return;
    }

    videoBuffer_.clear();
    audioBuffer_.clear();
    videoFrameBuffer_.clear();
    audioFrameBuffer_.clear();

    {
        std::lock_guard<std::mutex> vediocodecLk(videoCodecMtx_);
        avcodec_flush_buffers(vedioCodecCtx_);
    }
    {
        std::lock_guard<std::mutex> vediocodecLk(audioCodecMtx_);
        avcodec_flush_buffers(audioCodecCtx_);
    }

    {
        std::unique_lock<std::mutex> locker(audioClockMutex_);
        audioClockPts_ = time;
    }

    {
        std::lock_guard<std::mutex> lk(startMtx_);
        isAudioReady_.store(false);
    }

    std::cout << "seek to " << time << " seconds" << std::endl;
    resume();
}

// 更新音频时钟
void VideoDecoder::updateAudioClock(double pts) {
    std::unique_lock<std::mutex> locker(audioClockMutex_);
    audioClockPts_ = pts;
}

double VideoDecoder::getAudioClock()
{
    std::unique_lock<std::mutex> locker(audioClockMutex_);

    if (!audioSink_ || bytesPerSecond_ <= 0)
        return audioClockPts_;

    // 输出设备缓冲还有多少数据没播放
    int buffered = audioSink_->bufferSize() - audioSink_->bytesFree();

    // 音频时钟 = 最后 pts - 缓冲未播放时间
    double buffer_delay = (double)buffered / bytesPerSecond_;
    return audioClockPts_ - buffer_delay;
}

// 通用暂停检查函数
inline void VideoDecoder::checkPause() {
    std::unique_lock<std::mutex> locker(pauseMutex_);
    pauseCond_.wait(locker, [this]{ return !isPaused_ && isRunning_; });
}

void VideoDecoder::demuxLoop() {
    AVPacket* pkt = av_packet_alloc();
    while (isRunning_) {
        checkPause();
        std::cout << "demuxLoop start to enqueue";
        if (av_read_frame(formatCtx_, pkt) < 0) break;

        if (pkt->stream_index == videoStreamIdx_) {
            videoBuffer_.enqueue(av_packet_clone(pkt));
            std::cout << "videoBuffer_.enqueue ... size is " << videoBuffer_.size() << std::endl;
        } else if (pkt->stream_index == audioStreamIdx_) {
            audioBuffer_.enqueue(av_packet_clone(pkt));
            std::cout << "audioBuffer_.enqueue ... size is " << audioBuffer_.size() << std::endl;
        }
        av_packet_unref(pkt);
    }
    av_packet_free(&pkt);
}

void VideoDecoder::audioDecodeLoop() {
    while (isRunning_) {
        checkPause();
        auto aPktOpt = audioBuffer_.dequeue();
        std::cout << "audioDecodeLoop audioBuffer_ start to dequeue, audioBuffer_ size is " << videoBuffer_.size() << std::endl;;
        AVPacket* aPkt;

        if (!aPktOpt)
            continue;
        aPkt = aPktOpt.value();
        if(!aPkt)
        {
            std::cout << "aPkt is nullptr " << std::endl;
            continue;
        }

        {
            std::lock_guard<std::mutex> codecLk(audioCodecMtx_);
            if (avcodec_send_packet(audioCodecCtx_, aPkt) == 0) {
                AVFrame *aFrame = av_frame_alloc();
                std::cout << "audioFrameBuffer_ enqueue 1: " << aFrame << std::endl;
                while (avcodec_receive_frame(audioCodecCtx_, aFrame) == 0) {
                    std::cout << "audioFrameBuffer_ enqueue 2 " << std::endl;

                    if (audioFrameBuffer_.size() >= 5) QThread::msleep(5);
                    audioFrameBuffer_.enqueue(av_frame_clone(aFrame));
                }
                av_frame_free(&aFrame);
            }
        }
        av_packet_free(&aPkt);
    }
}


void VideoDecoder::videoDecodeLoop() {
    while (isRunning_) {
        checkPause();
        auto vPktOpt = videoBuffer_.dequeue();
        std::cout << "videoDecodeLoop videoBuffer start to dequeue, videoBuffer size is " << videoBuffer_.size() << std::endl;
        AVPacket* vPkt;

        if (!vPktOpt)
            continue;
        vPkt = vPktOpt.value();
        if(!vPkt)
        {
            std::cout << "vPkt is nullptr " << std::endl;
            continue;
        }

        {
            std::lock_guard<std::mutex> codecLk(videoCodecMtx_);
            if (avcodec_send_packet(vedioCodecCtx_, vPkt) == 0) {
                AVFrame *frame = av_frame_alloc();
                while (avcodec_receive_frame(vedioCodecCtx_, frame) == 0) {
                    std::cout << "videoDecodeLoop videoFrameBuffer start to enqueue , size is " << videoFrameBuffer_.size() << std::endl;
                    videoFrameBuffer_.enqueue(av_frame_clone(frame));
                }
                av_frame_free(&frame);
            }
        }
        av_packet_free(&vPkt);
    }
}

void VideoDecoder::videoThread()
{
    SwsContext *swsctx = nullptr;
    double lastPts = 0;
    double videoDuration = 0;
    while (isRunning_)
    {
        checkPause();
        auto frameOpt = videoFrameBuffer_.dequeue();
        AVFrame* frame;
        if (frameOpt)
        {
            frame = frameOpt.value();
        }
        else
        {
            continue;
        }

        {
            std::unique_lock<std::mutex> lk(startMtx_);
            if (!isAudioReady_.load()) {
                startCond_.wait_for(lk, std::chrono::milliseconds(3000),
                                  [&]{ return isAudioReady_.load(); });
            }
        }

        if (isSeeking_)
        {
            if (!frame->flags & AV_PKT_FLAG_KEY)
            {
                continue;
            }
            else
            {
                isSeeking_ = false;
            }
        }
/*
        if (!swsctx) {
            swsctx = sws_getContext(vedioCodecCtx_->width, vedioCodecCtx_->height, vedioCodecCtx_->pix_fmt,
                                    vedioCodecCtx_->width, vedioCodecCtx_->height, AV_PIX_FMT_RGB32,
                                    SWS_BILINEAR, nullptr, nullptr, nullptr);
        }
*/
        double pts = (frame->pts == AV_NOPTS_VALUE) ?
                         frame->best_effort_timestamp * av_q2d(formatCtx_->streams[videoStreamIdx_]->time_base) :
                         frame->pts * av_q2d(formatCtx_->streams[videoStreamIdx_]->time_base);
        videoDuration = (pts - lastPts);
        lastPts = pts;
        if (fabs(videoDuration-videoDurationIdeal_) > videoDurationIdeal_)
        {
            videoDuration = videoDurationIdeal_;
            std::cout << "set as ideal videoDuration";
        }

        // --- step2: 音视频时钟差 ---
        double diff = (pts - getAudioClock()) * 1000;
        double sleepTime = 0;
        sleepTime = (pts + videoDuration - getAudioClock()) * 1000;

        std::cout << "Video PTS:" << pts
                 << " | Audio Clock:" << getAudioClock()
                  << "| videoDuration:" << videoDuration
                 << " | Diff:" << diff << "ms"
                  << "| sleeptime:" << sleepTime << "ms"
                  << " | Frame Type:" << av_get_picture_type_char(frame->pict_type) << std::endl;

        if (diff < -35)
        {
            std::cout << "dropped because of diff < -35" << std::endl;
            continue;
        }

        if (sleepTime > 0 && sleepTime < 1000)
        {
            QThread::msleep(sleepTime);
        }
        else if (sleepTime < -10 || sleepTime > 2000)
        {
            std::cout << "dropped because of sleep < -10" << std::endl;
            continue;
        }

        int w = frame->width;
        int h = frame->height;
        const uint8_t* y = frame->data[0];
        int yStride = frame->linesize[0];
        const uint8_t* u = frame->data[1];
        int uStride = frame->linesize[1];
        const uint8_t* v = frame->data[2];
        int vStride = frame->linesize[2];

        emit frameReady(w, h, y, yStride, u, uStride, v, vStride);
        emit updateProgress(getAudioClock());
    }
    sws_freeContext(swsctx);
}

void VideoDecoder::audioThread()
{
    // 创建音频重采样器
    AVChannelLayout out_ch_layout;
    av_channel_layout_default(&out_ch_layout, 2); // 立体声

    AVChannelLayout in_ch_layout;
    av_channel_layout_copy(&in_ch_layout, &audioCodecCtx_->ch_layout); // 复制输入布局

    SwrContext *swr_ctx = NULL;
    int ret = swr_alloc_set_opts2(
        &swr_ctx,                   // SwrContext 指针
        &out_ch_layout,              // 输出声道布局
        AV_SAMPLE_FMT_S16,           // 输出采样格式
        44100,                       // 输出采样率
        &in_ch_layout,               // 输入声道布局
        audioCodecCtx_->sample_fmt,       // 输入采样格式
        audioCodecCtx_->sample_rate,      // 输入采样率
        0,                           // 日志偏移
        NULL                         // 日志上下文
        );
    swr_init(swr_ctx);

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
    audioSink_ = new QAudioSink(outputDevice, format);

    // 启动音频设备
    QIODevice* audioDevice = audioSink_->start();
    if (!audioDevice) {
        std::cout << "无法启动音频设备";
        delete audioSink_;
        return;
    }

    if (ret < 0 || !swr_ctx) {
        std::cout << "无法创建音频重采样器";
        return;
    }
    while (isRunning_)
    {
        checkPause();
        auto aPktOpt = audioFrameBuffer_.dequeue();
        AVFrame* aFrame;
        if (aPktOpt)
        {
            aFrame = aPktOpt.value();
        }
        else
        {
            continue;
        }

        double pts = (aFrame->pts == AV_NOPTS_VALUE) ?
                         aFrame->best_effort_timestamp * av_q2d(formatCtx_->streams[audioStreamIdx_]->time_base) :
                         aFrame->pts * av_q2d(formatCtx_->streams[audioStreamIdx_]->time_base);
        double duration = aFrame->nb_samples / (double)aFrame->sample_rate;


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
        /*
        std::cout << "SampleRate:" << format.sampleRate()
                 << "Channels:" << format.channelCount()
                 << "BytesPerFrame:" << format.bytesPerFrame()
                  << "SampleFormat:" << format.sampleFormat() << std::endl;

        std::cout << "FFmpeg frame channels: " << aFrame->ch_layout.nb_channels
                  << " sample_rate: " << aFrame->sample_rate
                  << " format: " << av_get_sample_fmt_name((AVSampleFormat)aFrame->format)
                  << std::endl;
        */

        int samples_converted = swr_convert(
            swr_ctx,
            &output_buffer,
            out_samples,
            (const uint8_t **)aFrame->data,
            aFrame->nb_samples
            );


        if (samples_converted > 0) {
            int data_size = samples_converted * format.channelCount() * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

            // std::cout << "samples_converted is " << samples_converted << ", data_size : " << data_size;
            int free_bytes = 0;
            int bufferSize = 0;

            {
                std::lock_guard<std::mutex> audioLk(audioDeviceMtx_);
                free_bytes = audioSink_->bytesFree();
                bufferSize = audioSink_->bufferSize();
            }
            int used = bufferSize - free_bytes;

                // 当前缓冲时长（ms）
            int msBuffered = (bytesPerSecond_ > 0)
                                 ? static_cast<int>((double)used / bytesPerSecond_ * 1000.0)
                                 : 0;

            // 避免缓冲超过 200ms 堆积 → 等待一小会儿
            if (msBuffered > 200) {
                int dynamicWait = qMin(msBuffered - 200, 50); // 最多等 50ms
                std::cout << "dynamicWait for " << dynamicWait << std::endl;
                QThread::msleep(dynamicWait);
                free_bytes = audioSink_->bytesFree();
            }

            // 写入前二次验证缓冲区
            qint64 bytesWritten = 0;
            if (free_bytes >= data_size) {
                {
                    std::lock_guard<std::mutex> audioLk(audioDeviceMtx_);
                    bytesWritten = audioDevice->write(
                        reinterpret_cast<const char*>(output_buffer),
                        data_size
                        );
                }

                updateAudioClock(pts);
                std::cout << "audio in:" << bytesWritten << "/" << data_size
                          << ", audio pts is " << pts << ", getAudioClock "  << getAudioClock() << std::endl;
            } else {
                std::cout << "drop audio frame" << std::endl;
            }

            {
                std::lock_guard<std::mutex> lk(startMtx_);
                if (!isAudioReady_.load()) {
                    // audio_first_pts = pts;
                    isAudioReady_.store(true);
                    startCond_.notify_all();
                }
            }


            // 写入音频设备
        }

        // 释放输出缓冲区
        if (output_buffer) {
            av_freep(&output_buffer);
        }
    }
    swr_free(&swr_ctx);
    av_channel_layout_uninit(&out_ch_layout);
    av_channel_layout_uninit(&in_ch_layout);
}
