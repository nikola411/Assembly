#ifndef ERROR_HANDLER_HPP
#define ERRPR_HANDLER_HPP

#include <string>

class ErrorHandler
{
public:
    ErrorHandler();
    virtual ~ErrorHandler();
    std::string to_string();
};


#endif