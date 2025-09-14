#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

class OpenGLVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    explicit OpenGLVideoWidget(QWidget *parent = nullptr);
    ~OpenGLVideoWidget();

public slots:
    void setYUV420PFrame(int width, int height,
                         const uint8_t* yData, int yLineSize,
                         const uint8_t* uData, int uLineSize,
                         const uint8_t* vData, int vLineSize);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram *program = nullptr;
    GLuint vao=0, vbo=0;
    GLuint texY=0, texU=0, texV=0;

    int frameWidth_=0, frameHeight_=0;
    uint8_t *yPlane=nullptr, *uPlane=nullptr, *vPlane=nullptr;
};
