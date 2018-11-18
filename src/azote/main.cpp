#include <QApplication>
#include <azote/Emulator.h>
#include <azote/MainWindow.h>
#include <azote/azote.h>

int main(int argc, char** argv)
{
    Emulator emu;
    QApplication* app;
    MainWindow* mainWin;

    if (argc != 2)
        return 1;

    app = new QApplication(argc, argv);
    emu.state = azInit();
    azLoadFile(emu.state, argv[1]);
    mainWin = new MainWindow(&emu);
    mainWin->show();
    return app->exec();
}
