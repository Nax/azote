#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <QOpenGLWidget>

struct Emulator;
class VideoWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    VideoWidget(Emulator* emulator, QWidget* parent = nullptr);
    virtual void initializeGL();
    virtual void paintGL();

private:
    Emulator*   _emulator;
};

#endif
