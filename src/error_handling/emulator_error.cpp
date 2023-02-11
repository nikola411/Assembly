#include "emulator_error.hpp"

EmulatorError::EmulatorError()
{
}

EmulatorError::EmulatorError(std::string msg, int byte_no, unsigned char byte) :
    msg(msg), byte_no(byte_no), byte(byte)
{
}

EmulatorError::~EmulatorError()
{
}

std::string EmulatorError::to_string()
{
    std::string err = "";
    err += msg;
    err += "[" + byte_no;
    err += "]:" + byte;

    return err;
}

