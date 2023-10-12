#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <string>
#include <map>
#include "parser.hpp"
#include "location.hh"

#include "assembly.hpp"

// Give Flex the prototype of yylex we want ...
# define YY_DECL \
  yy::parser::symbol_type yylex (Driver& drv, Assembly& asem)
// ... and declare it for the parser's sake.
YY_DECL;

class Driver
{
public:
  Driver(bool, Assembly*);

  void scan_begin();
  void scan_end();
  int parse (const std::string &f);

  yy::location location;

private:
  std::string file;
  bool trace_parsing;
  
  Assembly* assembly;
};

#endif