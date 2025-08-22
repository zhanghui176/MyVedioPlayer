QT       += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    LoginDialog.cpp \
    VideoDecoder.cpp \
    VideoPlayer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    FrameBuffer.h \
    LoginDialog.h \
    VideoDecoder.h \
    VideoPlayer.h \
    mainwindow.h

FORMS += \
    VideoPlayer.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += "D:/lib/opencv/opencv-build/lib/libopencv_core454.dll.a" \
        "D:/lib/opencv/opencv-build/lib/libopencv_imgproc454.dll.a" \
        "D:/lib/opencv/opencv-build/lib/libopencv_highgui454.dll.a" \
        "D:/lib/opencv/opencv-build/lib/libopencv_videoio454.dll.a"
LIBS += "D:/lib/ffmpeg-7.1.1-full_build-shared/lib/libavcodec.dll.a" \
        "D:/lib/ffmpeg-7.1.1-full_build-shared/lib/libavformat.dll.a" \
        "D:/lib/ffmpeg-7.1.1-full_build-shared/lib/libswscale.dll.a" \
        "D:/lib/ffmpeg-7.1.1-full_build-shared/lib/libavutil.dll.a" \
        "D:/lib/ffmpeg-7.1.1-full_build-shared/lib/libswresample.dll.a"
INCLUDEPATH += "D:/lib/opencv/opencv-build/install/include"
INCLUDEPATH += "D:/lib/ffmpeg-7.1.1-full_build-shared/include"

message(QT version:$$[QT_VERSION])

