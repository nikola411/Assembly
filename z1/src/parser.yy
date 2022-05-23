%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.5.1"

%{
#include <iostream>
#include <string>
#include <cmath>
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
  
  SYMBOL
;
%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END;
%token EOF 0 "end of file"

// NONTERMINALS
%nterm program
%nterm expr
%nterm directives
 
%%

program :
    lines EOF
    | EOF
    ;

lines :
    line lines {std::cout << "Line! "; }
    | line
    ;

line :
    SYMBOL
    ;

 
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}