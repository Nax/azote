#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <azote/VideoWidget.h>

struct Emulator;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(Emulator* emulator, QWidget* parent = nullptr);

private:
    Emulator*       _emulator;
    VideoWidget*    _video;
};

#endif
