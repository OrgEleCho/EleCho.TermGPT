#pragma once
#include <cmath>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>

extern int terminal_width, terminal_height;
inline void get_terminal_size()
{
    struct winsize size;
    if(isatty(STDOUT_FILENO) == 0)
    {
        throw "stdout is not a terminal";
    }
    if(ioctl(STDIN_FILENO, TIOCGWINSZ, &size)<0)
    {
        throw "ioctl error";
    }
    terminal_width = size.ws_col;
    terminal_height = size.ws_row;
}
inline void sigwinch_handler(int sig)
{
    std::cout<<terminal_height<<" "<<terminal_width<<std::endl;
    try
    {
        get_terminal_size();
    }
    catch (const char* e)
    {
        printf("get_terminal_size error: %s\n", e);
    }
    catch (...)
    {
        printf("get_terminal_size error: unknown\n");
    }
}
inline void check_terminal_size_update()
{
    if(signal(SIGWINCH, sigwinch_handler) == SIG_ERR)
    {
        throw "signal error";
    }
    sigwinch_handler(0);
}
