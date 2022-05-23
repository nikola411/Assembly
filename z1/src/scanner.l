%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hpp"
# include "parser.hpp"
%}

%option noyywrap nounput noinput batch debug outfile="./src/scanner/scanner.cpp" header="./src/scanner/scanner.hpp"

%{
  // A number symbol corresponding to the value in S.
//   yy::parser::symbol_type make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
%}

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}

{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();

"-"        return yy::parser::make_MINUS  (loc);
"+"        return yy::parser::make_PLUS   (loc);
"*"        return yy::parser::make_STAR   (loc);
"/"        return yy::parser::make_SLASH  (loc);
"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);
":="       return yy::parser::make_ASSIGN (loc);

.global return yy::parser::make_GLOBAL (loc);
.extern return yy::parser::make_EXTERN (loc);
.word   return yy::parser::make_WORD   (loc);
.skip   return yy::parser::make_SKIP   (loc);
.equ    return yy::parser::make_EQU    (loc);
.end    return yy::parser::make_END    (loc);


"[\w]+" return yy::parser::make_SYMBOL (loc);


<<EOF>>    return yy::parser::make_EOF (loc);
%%

void Driver::scan_begin ()
{
    yy_flex_debug = trace_scanning;
    if (file.empty () || file == "-")
        yyin = stdin;
    else if (!(yyin = fopen (file.c_str (), "r")))
    {
        std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
        exit (EXIT_FAILURE);
    }
}

void Driver::scan_end ()
{
  fclose (yyin);
}

