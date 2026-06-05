#include "terminal.h"

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