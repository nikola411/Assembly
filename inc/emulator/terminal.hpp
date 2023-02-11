#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include "emulator.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

class Terminal
{
public:

    Terminal();

    void start();
    void cleanup();

    ~Terminal();

private:

    struct termios stdio;
    struct termios orig_in;
    struct termios orig_out;
};

#endif