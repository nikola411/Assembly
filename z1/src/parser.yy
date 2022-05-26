%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.5.1"

%{
   
%}

%language "C++"
%defines "./src/parser/parser.hpp"
%output  "./src/parser/parser.cpp"

%defines
%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert
 
%code requires
{
    #include "define.hpp"
    #include "directive.hpp"
    #include "assembly.hpp"
    
    #include <vector>
    #include <string>
    class Driver;
    class Assembly;

    using std::vector;
    using std::string;
}

%param { Driver& drv }
%param { Assembly& assembly }
%locations

%define parse.trace
%define parse.error verbose
%define parse.lac full
 
%code
{
    #include "driver.hpp"
    #include <iostream>
    #include <string>
    #include <vector>
    #include <cmath>
    #include "directive.hpp"
}
 
%define api.token.prefix {TOK_}
// TERMINALS
%token
  ASSIGN  ":="
  MINUS   "-"
  PLUS    "+"
  STAR    "*"
  SLASH   "/"
  LPAREN  "("
  RPAREN  ")"
  COMMA   ","
;
%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END;
%token <string> LITERAL;
%token <string> SYMBOL;
%token HALT INT IRET RET;
%token CALL;
%token JMP JEQ JNE JGT;
%token PUSH POP XCHG ;
%token ADD SUB DIV MUL ;
%token CMP NOT AND OR XOR TEST SHL SHR;
%token LDR STR;

%token EOF 0 "end of file"

// NONTERMINALS
%nterm program

%nterm <Directive*> directive
%nterm instruction jump
%nterm <vector<string>> symbol_list
%nterm <vector<string>> label_list
 
%%

program:
    program line  
    | line
    | EOF  
    ;

line:
    directive
    | instruction
    | jump
    ;

directive:
    GLOBAL symbol_list { $$ = new Directive(); $$->set_type(Directive_type::GLOBAL); $$->set_arguments($2); assembly.add_new_line($$); }
    | EXTERN symbol_list { $$ = new Directive(); $$->set_type(Directive_type::EXTERN); $$->set_arguments($2); assembly.add_new_line($$); }
    | SECTION SYMBOL { $$ = new Directive(); $$->set_type(Directive_type::SECTION); $$->add_argument($2); assembly.add_new_line($$); }
    | WORD label_list { $$ = new Directive(); $$->set_type(Directive_type::WORD); $$->set_arguments($2); assembly.add_new_line($$); }
    ;

symbol_list:
    symbol_list COMMA SYMBOL { $$ = $1; $$.emplace_back($3); }
    | SYMBOL { $$.emplace_back($1); }
    ;

label_list:
    symbol_list COMMA SYMBOL { $$ = $1; $$.emplace_back($3); }
    | symbol_list COMMA LITERAL { $$ = $1; $$.emplace_back($3); }
    | LITERAL { $$.emplace_back($1); }
    | SYMBOL { $$.emplace_back($1); }
    ;

jump:
    JMP
    | JEQ
    | JNE
    | JGT
    ;

instruction:
    LDR
    | STR
    ;
 
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}