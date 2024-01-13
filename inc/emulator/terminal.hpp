#include <termios.h>

#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <iostream>

struct termios config_out;
struct termios config_in;

int main()
{
    auto ret = tcgetattr(STDIN_FILENO, &config_in);
    if (ret < 0)
    {
        std::cerr << "STDIN error";
        return 0;
    }

    ret = tcgetattr(STDOUT_FILENO, &config_out);
    if (ret < 0)
    {
        std::cerr << "STDIN error";
        return 0;
    }

    ret = open("/dev/tty0", O_NOCTTY | O_RDWR);

}
