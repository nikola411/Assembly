#include "terminal.hpp"
#include "emulator.hpp"

#include <iostream>

Terminal::Terminal()
{
    tcgetattr(STDOUT_FILENO, &orig_out);
    tcgetattr(STDIN_FILENO, &orig_in);

    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag = 0;
    stdio.c_oflag = 0;
    stdio.c_cflag = 0;
    stdio.c_lflag = 0;

    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void Terminal::cleanup()
{
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&orig_out);
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_in);
}

void Terminal::start()
{
    unsigned char c='D';    

    bool running = true;

    while (running)
    {
            // if (read(tty_fd,&c,1)>0)// if new data is available on the serial port, print it out
            // {
                
            //     write(STDOUT_FILENO,&c,1);
            // }             

        if (read(STDIN_FILENO,&c,1)>0) // if new data is available on the console, send it to the serial port
        {
            Emulator::memory[TERM_IN] = c;
        }

        
        if (c == 'q')
        {
            running = false;
        }    
    }
}

Terminal::~Terminal()
{
    cleanup();
}
