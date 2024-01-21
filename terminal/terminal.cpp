#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h> // needed for memset
#include <string>

#include <thread>
#include <iostream>

void writer(int tty)
{
    char c = 'b';

    while(true)
    {
        if (read(STDIN_FILENO, &c, 1) > 0)
        {
            printf("%c", c);
            write(tty, &c, 1);
            sleep(1);

            
            if (c == 'q')
                break;
        }

        sleep(1);
    }
}

int main(int argc,char** argv)
{
    char terminalPath[] = "/dev/ttyNikola";

    struct termios old_tio;
    struct termios old_stdio;
    tcgetattr(STDIN_FILENO, &old_stdio);

    struct termios tio;
    struct termios stdio;
    int tty_fd;
    fd_set rdset;

    
    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL);       // make the reads non-blocking

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=ECHO;
    tio.c_cc[VMIN]=1;

    tty_fd=open(terminalPath, O_RDWR | O_NOCTTY | O_NONBLOCK);        // O_NONBLOCK might override VMIN and VTIME, so read() may return immediately.
    // cfsetospeed(&tio,B115200);            // 115200 baud
    // cfsetispeed(&tio,B115200);            // 115200 baud

    tcsetattr(tty_fd,TCSANOW,&tio);
    fcntl(tty_fd, F_SETFL, O_NONBLOCK);

    // std::thread thrd(writer, tty_fd);
    // std::thread thrd(writer, tty_fd);
    // thrd.detach();

    char c = 's';
    char b = 'b';
    while(true)
    {
        if (read(tty_fd,&c,1)>0)
        {
            write(STDOUT_FILENO,&c,1);  
        }
          // if new data is available on the console, send it to the serial port
        if (read(STDIN_FILENO, &c, 1) > 0)
        {
            write(tty_fd, &c, 1);
            if (c == 'q')
                break;
        }
                
    }
    close(tty_fd);

    tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio);
}