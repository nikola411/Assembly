#include "driver.hpp"
#include "parser.hpp"

Driver::Driver (bool debug, Assembly* assembly)
  : trace_parsing (debug), trace_scanning (debug)
{
  this->assembly = assembly;
}

int Driver::parse (const std::string &f)
{
  file = f;
  location.initialize (&file);
  scan_begin ();
  yy::parser parse(*this, *this->assembly);
  parse.set_debug_level(trace_parsing);
  int res = parse();
  scan_end ();

  return res;
}
