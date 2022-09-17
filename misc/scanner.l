%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hpp"
# include "parser.hpp"
%}

%option noyywrap nounput noinput batch debug outfile="./misc/scanner/scanner.cpp" header="./misc/scanner/scanner.hpp"

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

.global  return yy::parser::make_GLOBAL (loc);
.extern  return yy::parser::make_EXTERN (loc);
.section return yy::parser::make_SECTION(loc);
.word    return yy::parser::make_WORD   (loc);
.skip    return yy::parser::make_SKIP   (loc);
.ascii   return yy::parser::make_ASCII  (loc);
.equ     return yy::parser::make_EQU    (loc);
.end     return yy::parser::make_END    (loc);

halt  return yy::parser::make_HALT (loc);
int   return yy::parser::make_INT  (loc);
iret  return yy::parser::make_IRET (loc);
ret   return yy::parser::make_RET  (loc);

call return yy::parser::make_CALL (loc);
jmp  return yy::parser::make_JMP  (loc);
jeq  return yy::parser::make_JEQ  (loc);
jne  return yy::parser::make_JNE  (loc);
jgt  return yy::parser::make_JGT  (loc);

push return yy::parser::make_PUSH  (loc);
pop  return yy::parser::make_POP   (loc);
xchg return yy::parser::make_XCHG  (loc);

add return yy::parser::make_ADD (loc);
sub return yy::parser::make_SUB (loc);
div return yy::parser::make_DIV (loc);
mul return yy::parser::make_MUL (loc);

cmp  return yy::parser::make_CMP  (loc);
not  return yy::parser::make_NOT  (loc);
and  return yy::parser::make_AND  (loc);
or   return yy::parser::make_OR   (loc);
xor  return yy::parser::make_XOR  (loc);
test return yy::parser::make_TEST (loc);
shl  return yy::parser::make_SHL  (loc);
shr  return yy::parser::make_SHR  (loc);

ldr return yy::parser::make_LDR (loc);
str return yy::parser::make_STR (loc);

r[0-7]|sp|pc|psw          return yy::parser::make_REGISTER (yytext, loc);
[a-zA-Z\.][\_a-zA-Z0-9]*  return yy::parser::make_SYMBOL   (yytext, loc);
(0x[0-9ABCDEF]+)|[0-9]+   return yy::parser::make_LITERAL  (yytext, loc);

#.*        loc.step ();

"-"        return yy::parser::make_MINUS  (loc);
"+"        return yy::parser::make_PLUS   (loc);
"*"        return yy::parser::make_STAR   (loc);
"/"        return yy::parser::make_SLASH  (loc);
"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);
":="       return yy::parser::make_ASSIGN (loc);
"\,"       return yy::parser::make_COMMA (loc);
"\$"       return yy::parser::make_DOLLAR (loc);
"\%"       return yy::parser::make_PERCENT (loc);
"\["       return yy::parser::make_LBRACKET (loc);
"\]"       return yy::parser::make_RBRACKET (loc);
"\:"       return yy::parser::make_COLON(loc);

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

