%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.5.1"

%{
   
%}

%language "C++"
%defines "./misc/parser/parser.hpp"
%output  "./misc/parser/parser.cpp"

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
    #include "instruction.hpp"
    #include "jump.hpp"
    
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
  DOLLAR  "$"
  PERCENT "%"
  LBRACKET "["
  RBRACKET "]"
  COLON ":"
;
%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END;
%token <string> LITERAL;
%token <string> SYMBOL;
%token <string> REGISTER;
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
%nterm line
%nterm <Directive*> directive
%nterm <Instruction*> instruction load_store_instructions two_argument_instructions one_argument_instructions zero_argument_instructions
// this here nterm jump_operand is "hacking" the pointer values so we can have smaller tree with less c code
%nterm <Jump*> jump_operand
%nterm <Jump*> jump
%nterm <vector<string>> symbol_list
%nterm <Directive*> label_list
%nterm <Instruction_type> load_store
%nterm <Instruction_type> instruction_type

 
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
    {
        $$ = new Directive();
        $$->set_type(Directive_type::GLOBAL);
        $$->set_arguments($2);
        assembly.add_new_line($$);
    }
    | EXTERN symbol_list
    {
        $$ = new Directive();
        $$->set_type(Directive_type::EXTERN);
        $$->set_arguments($2);
        assembly.add_new_line($$);
    }
    | SECTION SYMBOL 
    {
        $$ = new Directive();
        $$->set_type(Directive_type::SECTION);
        $$->add_argument($2);
        assembly.add_new_line($$);
    }
    | WORD label_list
    {
        $$ = $2;
        $$->set_type(Directive_type::WORD);
        assembly.add_new_line($$);
    }
    | SKIP LITERAL
    {
        $$ = new Directive();
        $$->set_type(Directive_type::SKIP);
        $$->add_argument($2);
        assembly.add_new_line($$);
    }
    | END
    {
        $$ = new Directive();
        $$->set_type(Directive_type::END);
        assembly.add_new_line($$);
        YYACCEPT;
    }
    | SYMBOL COLON
    {
        $$ = new Directive();
        $$ -> set_type(Directive_type::LABEL);
        $$ -> add_argument($1);
        assembly.add_new_line($$);
    }
    ;

symbol_list:
    symbol_list COMMA SYMBOL { $$ = $1; $$.emplace_back($3); }
    | SYMBOL { $$.emplace_back($1); }
    ;

label_list:
    label_list COMMA SYMBOL
    {
        $$ = $1;
        $$ -> add_argument($3, Label_type::SYMBOL);
    }
    | label_list COMMA LITERAL
    {
        $$ = $1;
        $$ -> add_argument($3, Label_type::LITERAL);
    }
    | LITERAL 
    {
        $$ = new Directive();
        $$ -> add_argument($1, Label_type::LITERAL);
    }
    | SYMBOL 
    {
        $$ = new Directive(); 
        $$ -> add_argument($1, Label_type::SYMBOL);
    }
    ;

jump:
    JMP jump_operand
    {
        $$ = $2; $$ -> set_jump_type(Jump_type::JMP);
        assembly.add_new_line($$);
    }
    | JEQ jump_operand
    {
        $$ = $2; $$ -> set_jump_type(Jump_type::JEQ);
        assembly.add_new_line($$);
    }
    | JNE jump_operand
    {
        $$ = $2; $$ -> set_jump_type(Jump_type::JNE);
        assembly.add_new_line($$);
    }
    | JGT jump_operand
    {
        $$ = $2; $$ -> set_jump_type(Jump_type::JGT);
        assembly.add_new_line($$);
    }
    ;
    
jump_operand:
    LITERAL 
    {
        $$ = new Jump();
        $$ -> set_operand ($1, Addressing_type::ABSOLUTE, Label_type::LITERAL);
    }
    | SYMBOL
    {
        $$ = new Jump();
        $$ -> set_operand ($1, Addressing_type::ABSOLUTE, Label_type::SYMBOL);
    }
    | PERCENT SYMBOL
    {
        $$ = new Jump();
        $$ -> set_operand ($2, Addressing_type::PC_RELATIVE, Label_type::SYMBOL);
    }
    | STAR LITERAL
    {
        $$ = new Jump();
        $$ -> set_operand ($2, Addressing_type::MEMORY, Label_type::LITERAL);
    }
    | STAR SYMBOL
    {
        $$ = new Jump();
        $$ -> set_operand ($2, Addressing_type::MEMORY, Label_type::SYMBOL);
    }
    | STAR REGISTER
    {
        $$ = new Jump();
        $$ -> set_operand ($2, Addressing_type::ABSOLUTE, Label_type::REGISTER);
    }       
    | STAR LBRACKET REGISTER RBRACKET
    {
        $$ = new Jump();
        $$ -> set_operand ($3, Addressing_type::MEMORY, Label_type::REGISTER);
    }   
    | STAR LBRACKET REGISTER PLUS LITERAL RBRACKET
    {
        $$ = new Jump();
        $$ -> set_operand ($3, Addressing_type::LITERAL_OFFSET, Label_type::REGISTER);
        $$ -> set_offset ($5);
    }        
    | STAR LBRACKET REGISTER PLUS SYMBOL RBRACKET
    {
        $$ = new Jump();
        $$ -> set_operand ($3, Addressing_type::SYMBOL_OFFSET, Label_type::REGISTER);
        $$ -> set_offset ($5);
    }
    ;   

instruction:
    two_argument_instructions { $$ = $1; }
    | one_argument_instructions { $$ = $1; }
    | zero_argument_instructions { $$ = $1; }
    ;

two_argument_instructions:
    load_store_instructions { $$ = $1; }
    | instruction_type REGISTER COMMA REGISTER
    {
        $$ = new Instruction($1, $2, $4);
        assembly.add_new_line($$);
    }
    ;


load_store_instructions:
    load_store REGISTER COMMA DOLLAR LITERAL
    {
        $$ = new Instruction($1, $2); 
        $$ -> set_second_argument(Addressing_type::ABSOLUTE, $5, Label_type::LITERAL);
        assembly.add_new_line($$);
    }
    | load_store REGISTER COMMA DOLLAR SYMBOL
    { 
        $$ = new Instruction($1, $2); 
        $$ -> set_second_argument(Addressing_type::ABSOLUTE, $5, Label_type::SYMBOL);
        assembly.add_new_line($$);
    }
    | load_store REGISTER COMMA LITERAL
    {
        $$ = new Instruction($1, $2);
        $$ -> set_second_argument(Addressing_type::MEMORY, $4, Label_type::LITERAL);
        assembly.add_new_line($$);
    }
    | load_store REGISTER COMMA SYMBOL 
    {
        $$ = new Instruction($1, $2);
        $$ -> set_second_argument(Addressing_type::MEMORY, $4, Label_type::SYMBOL);
        assembly.add_new_line($$);
    }
    | load_store REGISTER COMMA PERCENT SYMBOL
    {
        $$ = new Instruction($1, $2);
        $$ -> set_second_argument(Addressing_type::PC_RELATIVE, $5, Label_type::SYMBOL);
        assembly.add_new_line($$);
    }
    | load_store REGISTER COMMA REGISTER
    {
        $$ = new Instruction($1, $2); 
        $$ -> set_second_argument(Addressing_type::ABSOLUTE, $4, Label_type::REGISTER);
        assembly.add_new_line($$);
    }
    | load_store REGISTER COMMA LBRACKET REGISTER RBRACKET
    {
        $$ = new Instruction($1, $2);
        $$ -> set_second_argument(Addressing_type::MEMORY, $5, Label_type::REGISTER);
        assembly.add_new_line($$);
    }
    | load_store REGISTER COMMA LBRACKET REGISTER PLUS SYMBOL RBRACKET
    {
        $$ = new Instruction($1, $2); 
        $$ -> set_second_argument(Addressing_type::SYMBOL_OFFSET, $5, Label_type::REGISTER);
        $$ -> set_offset($7, Label_type::SYMBOL);
        assembly.add_new_line($$);
    }
    | load_store REGISTER COMMA LBRACKET REGISTER PLUS LITERAL RBRACKET
    {
        $$ = new Instruction($1, $2);
        $$ -> set_second_argument(Addressing_type::SYMBOL_OFFSET, $5, Label_type::REGISTER);
        $$ -> set_offset($7, Label_type::LITERAL);
        assembly.add_new_line($$);
    }
    ;

load_store:
    LDR { $$ = Instruction_type::LDR; }
    | STR { $$ = Instruction_type::STR; }
    ;

instruction_type:
    SHL { $$ = Instruction_type::SHL; }
    | SHR { $$ = Instruction_type::SHR; }
    | TEST { $$ = Instruction_type::TEST; }
    | XOR { $$ = Instruction_type::XOR; }
    | OR { $$ = Instruction_type::OR; }
    | AND { $$ = Instruction_type::AND; }
    | CMP { $$ = Instruction_type::CMP; }
    | DIV { $$ = Instruction_type::DIV; }
    | MUL { $$ = Instruction_type::MUL; }
    | SUB { $$ = Instruction_type::SUB; }
    | ADD { $$ = Instruction_type::ADD; }
    | XCHG { $$ = Instruction_type::XCHG; }
    ;

one_argument_instructions:
    POP REGISTER 
    {
        $$ = new Instruction(Instruction_type::POP, $2); 
        assembly.add_new_line($$);
    }
    | PUSH REGISTER
    {
        $$ = new Instruction(Instruction_type::PUSH, $2); 
        assembly.add_new_line($$);
    }
    | NOT REGISTER
    {
        $$ = new Instruction(Instruction_type::NOT, $2); 
        assembly.add_new_line($$);
    }
    | INT REGISTER
    {
        $$ = new Instruction(Instruction_type::INT, $2); 
        assembly.add_new_line($$);
    }
    | CALL REGISTER 
    {
        $$ = new Instruction(Instruction_type::CALL, $2); 
        assembly.add_new_line($$);
    }
    ;

zero_argument_instructions:
    RET
    {
        $$ = new Instruction(Instruction_type::RET);
        assembly.add_new_line($$);
    }
    | IRET 
    {
        $$ = new Instruction(Instruction_type::IRET);
        assembly.add_new_line($$);
    }
    | HALT 
    {
        $$ = new Instruction(Instruction_type::HALT);
        assembly.add_new_line($$);
    }
    ;
 
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}