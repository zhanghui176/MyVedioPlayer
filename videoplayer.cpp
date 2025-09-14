#include "VideoPlayer.h"
#include "ui_VideoPlayer.h"
#include "OpenGLVideoWidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <Utility.h>

videoPlayer::videoPlayer(QWidget *parent)
    : QMainWindow(parent)
    , decoder(VideoDecoder())
    , ui(new Ui::videoPlayer)
    , isPlaying_(false)
{
    ui->setupUi(this);
    videoTimer_ = new QTimer(this);
    connect(videoTimer_, SIGNAL(timeout()), this, SLOT(startDecode()));
    mainWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(mainWidget);

    // 视频显示区域
    ui->VedioWidget->setMinimumSize(640,480);
    ui->VedioWidget->setStyleSheet("background-color: black;");
    mainLayout->addWidget( ui->VedioWidget, 1);

    // ui->durationLabel->hide();

    // 控制区域
    controlLayout = new QHBoxLayout();

    ui->PlayPause->setEnabled(true);
    ui->ProgressSlider->setEnabled(true);

    controlLayout->addWidget(ui->openButton);
    controlLayout->addWidget(ui->PlayPause);
    controlLayout->addWidget(ui->ProgressSlider, 1);
    controlLayout->addWidget(ui->durationLabel);

    mainLayout->addLayout(controlLayout);

    setCentralWidget(mainWidget);

    // connect(&decoder, &VideoDecoder::frameReady,  // 2. 发送方
    //         this, &VideoDecoder::update_Frames);  // 3. 接收方
    ui->durationLabel->setText("00:00");

    // 获取提升后的控件指针
    OpenGLVideoWidget *glWidget = qobject_cast<OpenGLVideoWidget*>(
        ui->VedioWidget  // UI文件中提升的控件对象名
        );

    auto conn1 = connect(&decoder, &VideoDecoder::frameReady,
            glWidget, &OpenGLVideoWidget::setYUV420PFrame, Qt::QueuedConnection);

    auto conn2 = connect(&decoder, &VideoDecoder::updateProgress,
                        this, &videoPlayer::update_progress, Qt::QueuedConnection);

    if (!conn1 && !conn2) {
        std::cout << " connect failed" << std::endl;
    } else {
        std::cout << "conenct successfully" << std::endl;
    }

    // connect(ui->openButton, &QPushButton::clicked, this, &videoPlayer::on_open_clicked);
    // connect(ui->ProgressSlider, &QSlider::sliderMoved, this, &videoPlayer::on_ProgressSlider_sliderMoved);
    // connect(ui->volumeSlider, &QSlider::valueChanged, this, &videoPlayer::on_volumeSlider_sliderMoved);
}

videoPlayer::~videoPlayer()
{
    delete ui;
}

void videoPlayer::on_PlayPause_clicked()
{
std::cout << "on_PlayPause_clicked" << std::endl;
    if (!isStart_)
    {
        std::cout << "on_PlayPause_clicked  1 " << std::endl;
        decoder.start();
        isPlaying_ = true;
        isStart_ = true;
        return;
    }
    if (isPlaying_)
    {
        std::cout << "on_PlayPause_clicked  2 " << std::endl;
        decoder.pause();
        isPlaying_ = false;
    }
    else
    {
        std::cout << "on_PlayPause_clicked  3 " << std::endl;
        decoder.resume();
        isPlaying_ = true;
    }

}

void videoPlayer::update_Frames(const QImage &frame)
{
    if (!frame.isNull())
    {
        //ui->videoLabel->setPixmap(QPixmap::fromImage(frame));
    }
    //ui->ProgressSlider->setValue(capture_.get(cv::CAP_PROP_POS_FRAMES));
}

void videoPlayer::show_progress(double progress)
{
        ui->ProgressSlider->setValue(static_cast<int>(progress * 1000));
        auto strProgress = Utility::formatSecondToQTime(progress);
        auto strDuration = Utility::formatSecondToQTime(videoDuration_);
        auto string = strProgress + "/" + strDuration;
        ui->durationLabel->setText(string);
}

void videoPlayer::update_progress(double progress)
{
    if (!isSeeking)
    {
        show_progress(progress);
    }
}

void videoPlayer::startDecode()
{

}

void videoPlayer::on_pushButton_clicked()
{

}


void videoPlayer::on_volumeSlider_sliderMoved(int position)
{

}

void videoPlayer::on_ProgressSlider_sliderPressed()
{
    isSeeking = true;
}


void videoPlayer::on_ProgressSlider_sliderMoved(int position)
{
    show_progress(position/1000);
    finalPosition_ = position;
    // decoder.seek(time);
}

void videoPlayer::on_ProgressSlider_sliderReleased()
{
    double time = static_cast<double>(finalPosition_) / 1000;
    std::cout << "seek time, time is " << time;
    decoder.seek(time);
    isSeeking = false;
}

void videoPlayer::on_openButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Video", "", "Video Files (*.mp4 *.avi *.mov)");
    if(filename.isEmpty()) return;

    decoder.openFile(filename);
    videoDuration_ = decoder.getDuration();
    ui->ProgressSlider->setRange(0, static_cast<int>(videoDuration_ * 1000));
    auto videoDuration = Utility::formatSecondToQTime(videoDuration_);
    if (ui->durationLabel)
    {
        ui->durationLabel->setText(videoDuration);
    }

/*
    capture_.open(filename.toStdString());
    if(!capture_.isOpened()) {
        QMessageBox::critical(this, "Error", "Could not open video file");
        return;
    }
    videoFps_ = capture_.get(cv::CAP_PROP_FPS);
    frames_ = capture_.get(cv::CAP_PROP_FRAME_COUNT);
    ui->ProgressSlider->setMaximum(frames_);
    on_PlayPause_clicked();
*/

}



