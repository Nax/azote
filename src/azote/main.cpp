#include <azote/azote.h>

int main(int argc, char** argv)
{
    AzState* state;

    if (argc != 2)
        return 1;

    state = azInit();
    azLoadFile(state, argv[1]);
    azRun(state);
    azExit(state);
    return 0;
}
