#ifndef EMULATOR_ERROR
#define EMULATOR_ERROR

#include "error_handler.hpp"

class EmulatorError : public ErrorHandler
{
public:
    EmulatorError();
    EmulatorError(std::string, int, unsigned char);
    ~EmulatorError();
    std::string to_string();
private:
    std::string msg;
    int byte_no;
    unsigned char byte;
};

#endif