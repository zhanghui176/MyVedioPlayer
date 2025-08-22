#include "VideoPlayer.h"
#include "ui_VideoPlayer.h"
#include <QFileDialog>
#include <QMessageBox>

videoPlayer::videoPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::videoPlayer)
    , isPlaying_(false)
    , videoFps_(0)
{
    ui->setupUi(this);
    videoTimer_ = new QTimer(this);
    connect(videoTimer_, SIGNAL(timeout()), this, SLOT(startDecode()));
    mainWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(mainWidget);

    // 视频显示区域
    ui->videoLabel->setAlignment(Qt::AlignCenter);
    ui->videoLabel->setStyleSheet("background-color: black;");
    ui->videoLabel->setMinimumSize(640, 480);
    mainLayout->addWidget( ui->videoLabel, 1);

    // 控制区域
    controlLayout = new QHBoxLayout();

    ui->PlayPause->setEnabled(true);
    ui->ProgressSlider->setEnabled(true);

    controlLayout->addWidget(ui->openButton);
    controlLayout->addWidget(ui->PlayPause);
    controlLayout->addWidget(ui->ProgressSlider, 1);

    mainLayout->addLayout(controlLayout);

    setCentralWidget(mainWidget);

    connect(&decoder, &videoDecoder::frameReady,  // 2. 发送方
            this, &videoPlayer::update_Frames);  // 3. 接收方
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
    decoder.start();
}

void videoPlayer::update_Frames(const QImage &frame)
{
    if (!frame.isNull())
    {
         ui->videoLabel->setPixmap(QPixmap::fromImage(frame));
    }
    //ui->ProgressSlider->setValue(capture_.get(cv::CAP_PROP_POS_FRAMES));
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


void videoPlayer::on_ProgressSlider_sliderMoved(int position)
{

}


void videoPlayer::on_openButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Video", "", "Video Files (*.mp4 *.avi *.mov)");
    if(filename.isEmpty()) return;

    decoder.openFile(filename);
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

