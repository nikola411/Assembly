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
    #include <string>
    class Driver;
}

%param { Driver& drv }
%locations

%define parse.trace
%define parse.error verbose
%define parse.lac full
 
%code
{
    #include "driver.hpp"
    #include <iostream>
    #include <string>
    #include <cmath>
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
%token LITERAL SYMBOL;
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
%nterm expr
%nterm directive instruction jump
%nterm parameter_list
 
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
    GLOBAL symbol_list
    | EXTERN symbol_list
    | SECTION SYMBOL
    ;

symbol_list:
    symbol_list COMMA SYMBOL
    | SYMBOL
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