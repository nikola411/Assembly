%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring> 
    #include <string>
    #include "Driver.hpp"
    #include "parser.hpp"
%}

%option noyywrap nounput noinput batch
%option outfile="./misc/scanner/scanner.cpp" header="./misc/scanner/scanner.hpp"

%{
    #define YY_USER_ACTION  loc.columns (yyleng);
%}

blank [ \t\r]

%%

%{
    yy::location& loc = drv.location;
    loc.step ();
%}

{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();
#.* 

.global  return yy::parser::make_GLOBAL (loc);
.extern  return yy::parser::make_EXTERN (loc);
.section return yy::parser::make_SECTION(loc);
.word    return yy::parser::make_WORD   (loc);
.skip    return yy::parser::make_SKIP   (loc);
.ascii   return yy::parser::make_ASCII  (loc);
.equ     return yy::parser::make_EQU    (loc);
.end     return yy::parser::make_END    (loc);

call return yy::parser::make_CALL(loc);
jmp  return yy::parser::make_JMP(loc);
beq  return yy::parser::make_BEQ(loc);
bne  return yy::parser::make_BNE(loc);
bgt  return yy::parser::make_BGT(loc);

xchg return yy::parser::make_XCHG(loc);
add  return yy::parser::make_ADD(loc);
sub  return yy::parser::make_SUB(loc);
mul  return yy::parser::make_MUL(loc);
div  return yy::parser::make_DIV(loc);
not  return yy::parser::make_NOT(loc);
and  return yy::parser::make_AND(loc);
or   return yy::parser::make_OR(loc);
xor  return yy::parser::make_XOR(loc);
shl  return yy::parser::make_SHL(loc);
shr  return yy::parser::make_SHR(loc);

push return yy::parser::make_PUSH(loc);
pop  return yy::parser::make_POP(loc);

ret  return yy::parser::make_RET(loc);
iret return yy::parser::make_IRET(loc);
int  return yy::parser::make_INT(loc);
halt return yy::parser::make_HALT(loc);

ld return yy::parser::make_LD(loc);
st return yy::parser::make_ST(loc);

csrrd return yy::parser::make_CSRRD(loc);
csrwr return yy::parser::make_CSRWR(loc);

\,  return yy::parser::make_COMMA(loc);
\[  return yy::parser::make_L_BRACKET(loc);
\]  return yy::parser::make_R_BRACKET(loc);
\(  return yy::parser::make_L_PAREN(loc);
\)  return yy::parser::make_R_PAREN(loc);
%   return yy::parser::make_PERCENT(loc);
\+  return yy::parser::make_PLUS(loc);
-   return yy::parser::make_MINUS(loc);

[a-zA-Z_0-9]+:            return yy::parser::make_LABEL(yytext, loc);
status|handler|cause   return yy::parser::make_CSR(yytext, loc);
r[0-9][0-5]{0,1}|sp|pc return yy::parser::make_GPR(yytext, loc);
(0x){0,1}[0-9ABCDEF]+  return yy::parser::make_LITERAL(yytext, loc);
[a-zA-Z_0-9]+             return yy::parser::make_SYMBOL (yytext, loc);

<<EOF>>    return yy::parser::make_EOF  (loc);

%%