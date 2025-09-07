#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include "FrameBuffer.h"
#include <QQueue>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QAudioSink>
#include <QtConcurrent/QtConcurrent>

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/time.h>
#include <libavutil/frame.h>
#include <libswresample/swresample.h>
}

class VideoDecoder : public QObject {
    Q_OBJECT
public:
    explicit VideoDecoder();
    ~VideoDecoder();

    bool openFile(const QString &filename);
    void start();
    void stop();
    void pause(); // 暂停
    void resume(); // 继续
    void updateAudioClock(double pts);
    double getDuration() const;
    void seek(double time);

signals:
    void frameReady(int w, int h,
                     const uint8_t* y, int yStride,
                     const uint8_t* u, int uStride,
                     const uint8_t* v, int vStride);

    void updateProgress(double time);
private:
    void checkPause();
    void demuxLoop();
    void audioDecodeLoop();
    void videoDecodeLoop();
    double getAudioClock();
    void videoThread();
    void audioThread();
    double getMasterClock();

    AVFormatContext *formatCtx_ = nullptr;
    int videoStreamIdx_ = -1;
    int audioStreamIdx_ = -1;

    QThread *demuxThread_ = nullptr;
    QThread *audioDecodeThread_ = nullptr;
    QThread *videoDecodeThread_ = nullptr;
    QThread *audioThread_ = nullptr;
    QThread *videoThread_ = nullptr;

    std::atomic<bool> isRunning_{false};
    std::atomic<bool> isPaused_{false};

    std::atomic<bool> isAudioReady_{false};
    std::mutex startMtx_;
    std::condition_variable startCond_;

    QMutex pauseMutex_;
    QWaitCondition pauseCond_;

    FrameBuffer<AVPacket*> videoBuffer_;
    FrameBuffer<AVPacket*> audioBuffer_;

    FrameBuffer<AVFrame*> videoFrameBuffer_;
    FrameBuffer<AVFrame*> audioFrameBuffer_;

    AVCodecParameters *vedioCodecPar_;
    const AVCodec *vedioCodec_;
    AVCodecContext *vedioCodecCtx_;

    AVCodecParameters *audioCodecPar_;
    const AVCodec *audioCodec_;
    AVCodecContext *audioCodecCtx_;

    QIODevice *audioDevice_;
    int bytesPerSecond_ = 0;

    double audioClockPts_ = 0;
    double videoDuration_ = 0;

    double last_delay_= 0;
    double last_pts_ = 0;

    QAudioSink* audioSink_;

    QMutex audioClockMutex_;
};

#endif // VIDEODECODER_H
