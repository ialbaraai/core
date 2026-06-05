#include "terminal.h"

#ifdef _WIN32
#include <windows.h>

static DWORD old_mode;
static HANDLE stdin_handle;

void terminal_raw_mode_enter(void)
{
    stdin_handle = GetStdHandle(STD_INPUT_HANDLE);

    GetConsoleMode(stdin_handle, &old_mode);

    DWORD raw_mode = old_mode;
    raw_mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);

    SetConsoleMode(stdin_handle, raw_mode);
}

void terminal_raw_mode_exit(void)
{
    SetConsoleMode(stdin_handle, old_mode);
}
#else
#include <termios.h>
#include <unistd.h>

static struct termios _old;

void terminal_raw_mode_enter()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &_old);
    raw = _old;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
void terminal_raw_mode_exit()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_old);
}
#endif