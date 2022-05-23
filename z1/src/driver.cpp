#include "driver.hpp"
#include "parser.hpp"


Driver::Driver ()
  : trace_parsing (false), trace_scanning (false)
{
  variables["one"] = 1;
  variables["two"] = 2;
}

int
Driver::parse (const std::string &f)
{
  file = f;
  location.initialize (&file);
  scan_begin ();
  yy::parser parse(*this);
  parse.set_debug_level(trace_parsing);
  int res = parse();
  scan_end ();
  return res;
}
