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

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/time.h>
#include <libswresample/swresample.h>
}

class videoDecoder : public QObject {
    Q_OBJECT
public:
    explicit videoDecoder();
    ~videoDecoder();

    bool openFile(const QString &filename);
    void start();
    void stop();
    void pause(); // 暂停
    void resume(); // 继续
    void updateAudioClock(double pts, double duration);

signals:
    void frameReady(const QImage &frame);

private:
    void demuxLoop();
    void audioDecodeLoop();
    void videoDecodeLoop();
    double getAudioClock();
    void displayLoop();

    AVFormatContext *formatCtx_ = nullptr;
    int videoStreamIdx_ = -1;
    int audioStreamIdx_ = -1;

    QThread *demuxThread_ = nullptr;
    QThread *audioThread_ = nullptr;
    QThread *videoThread_ = nullptr;

    std::atomic<bool> isRunning_{false};
    std::atomic<bool> isPaused_{false};

    QMutex pauseMutex_;
    QWaitCondition pauseCond_;

    FrameBuffer<AVPacket*> videoBuffer_;
    FrameBuffer<AVPacket*> audioBuffer_;

    FrameBuffer<AVFrame*> videoFrameBuffer_;
    FrameBuffer<AVFrame*> audioFrameBuffer_;
    QWaitCondition videoCond_;
    QWaitCondition audioCond_;
    bool videoBufferNotEmpty_ = false;
    bool audioBufferNotEmpty_ = false;
    QMutex videoMutex_;
    QMutex audioMutex_;

    double audioClock_ = 0.0;   // 基础时钟
    QElapsedTimer audioTimer_;              // 音频计时器
    double currentAudioPts_ = 0.0;          // 当前播放帧的PTS
    double currentAudioDuration_ = 0.0;     // 当前播放帧持续时间

    double audioStartPts_ = 0.0;
    QElapsedTimer audioPlaybackTimer_;
    QMutex audioClockMutex_;

    // 视频同步相关
    double lastVideoPts_ = -1.0;
    double frameInterval_ = 0.033; // 默认30fps
    int consecutiveDrops_ = 0;

    // 音频帧队列
    struct AudioFrame {
        double pts;
        double duration;
    };
    QQueue<AudioFrame> audioFrames_;
    QMutex audioFrameMutex_;
};

#endif // VIDEODECODER_H
