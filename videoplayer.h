#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QPushButton>
#include <QVBoxLayout>
#include "VideoDecoder.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

namespace Ui {
class videoPlayer;
}

class videoPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit videoPlayer(QWidget *parent = nullptr);
    ~videoPlayer();

private slots:
    void on_pushButton_clicked();

    void on_PlayPause_clicked();

    void on_volumeSlider_sliderMoved(int position);

    void on_ProgressSlider_sliderMoved(int position);

    void update_Frames(const QImage &frame);

    void on_openButton_clicked();

    void update_progress(double progress);

    void startDecode();

    void on_ProgressSlider_sliderPressed();

    void on_ProgressSlider_sliderReleased();

    void show_progress(double progress);

private:
    VideoDecoder decoder;
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *controlLayout;
    double videoDuration_;
    int finalPosition_ = 0;
    Ui::videoPlayer *ui;
    bool isPlaying_ = false;
    bool isStart_ = false;
    double videoFps_;
    QTimer *videoTimer_;
    cv::VideoCapture capture_;
    int frames_;
    AVFormatContext* formatCtx;
    bool isSeeking = false;
};

#endif // VIDEOPLAYER_H
