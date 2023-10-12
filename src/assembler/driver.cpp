#include "driver.hpp"
#include "parser.hpp"
#include "scanner.hpp"

Driver::Driver (bool debug, Assembly* assembly)
  : trace_parsing (debug)
{
  this->assembly = assembly;
}

void Driver::scan_begin()
{
    if (!(yyin = fopen (file.c_str (), "r")))
    {
        std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
        exit (EXIT_FAILURE);
    }
}

void Driver::scan_end()
{
  fclose (yyin);
}

int Driver::parse (const std::string &f)
{
    if (f.size() == 0)
      return 0;

    file = f;

    location.initialize(&f);

    scan_begin();
    yy::parser parser(*this, *(this->assembly));
    int res = parser.parse();

    scan_end();

    return res;
}