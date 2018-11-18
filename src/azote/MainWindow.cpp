#include <azote/MainWindow.h>

MainWindow::MainWindow(Emulator* emulator, QWidget* parent)
: QWidget(parent)
, _emulator(emulator)
{
    setFixedSize(320, 240);
    _video = new VideoWidget(_emulator, this);
    _video->setFixedSize(320, 240);
    /*_video->makeCurrent();
    glViewport(0, 0, 320, 240);
    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);*/
}
