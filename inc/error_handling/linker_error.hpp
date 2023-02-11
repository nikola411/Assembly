#ifndef LINKER_ERROR
#define LINKER_ERROR

#include "error_handler.hpp"

class LinkerError : public ErrorHandler
{
public:
    LinkerError(std::string msg, int line_no);
    LinkerError(std::string msg, int table_entry, std::string symbol);

    LinkerError();
    ~LinkerError();
    std::string to_string();
    
private:
    std::string msg; // message for the error
    int int_value; // line number or table entry
    std::string symbol; // symbol
};

#endif