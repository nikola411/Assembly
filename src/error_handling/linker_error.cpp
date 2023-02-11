#include "linker_error.hpp"



LinkerError::LinkerError(std::string msg, int line_no) :
    msg(msg), int_value(line_no), symbol("")
{
}

LinkerError::LinkerError(std::string msg, int table_entry, std::string symbol):
    msg(msg), int_value(table_entry), symbol(symbol)
{
}

LinkerError::LinkerError()
{
}

LinkerError::~LinkerError()
{
}

std::string LinkerError::to_string()
{
    std::string err = "";
    
    err += msg;
    if (symbol == "")
    {
        err += "line: " + int_value;
    }
    else
    {
        err += "entry_no: " + int_value;
        err += " symbol: " + symbol;
    }

    return err;
}
