%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring> 
    #include <string>
    #include "driver.hpp"
    #include "parser.hpp"
%}

%option noyywrap nounput noinput batch debug
%option outfile="./misc/scanner/scanner.cpp" header="./misc/scanner/scanner.hpp"

%{
    #define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
    yy::location& loc = drv.location;
    loc.step ();
%}

.global  return yy::parser::make_GLOBAL (loc);
.extern  return yy::parser::make_EXTERN (loc);
.section return yy::parser::make_SECTION(loc);
.word    return yy::parser::make_WORD   (loc);
.skip    return yy::parser::make_SKIP   (loc);
.ascii   return yy::parser::make_ASCII  (loc);
.equ     return yy::parser::make_EQU    (loc);
.end     return yy::parser::make_END    (loc);

[a-zA-Z]+ return yy::parser::make_SYMBOL (yytext, loc);
0x[0-9ABCDEF]+ return yy::parser::make_LITERAL(yytext, loc);

,         return yy::parser::make_COMMA(loc);

<<EOF>>    return yy::parser::make_EOF  (loc);

%%