#include "driver.hpp"
#include "parser.hpp"

Driver::Driver (bool parsing, bool scanning, Assembly* assembly)
  : trace_parsing (parsing), trace_scanning (scanning)
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
