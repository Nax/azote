#include <stdio.h>
#include <azote/Emulator.h>
#include <azote/VideoWidget.h>

VideoWidget::VideoWidget(Emulator* emulator, QWidget* parent)
: QOpenGLWidget(parent)
, _emulator(emulator)
{

}

void VideoWidget::initializeGL()
{

}

void VideoWidget::paintGL()
{
    puts("Paint GL");
    for (;;)
    azNextFrame(_emulator->state);
}
