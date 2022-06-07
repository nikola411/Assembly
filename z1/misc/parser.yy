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
  HASHTAG "#"
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

%token <string> COMMENT ;

%token EOF 0 "end of file"

// NONTERMINALS
%nterm program
%nterm line
%nterm <Directive*> directive
%nterm <Instruction*> instruction load_store_instructions two_operand_instructions one_operand_instructions zero_operand_instructions
// this here nterm jump_operand is "hacking" the pointer values so we can have smaller tree with less c code
%nterm <Jump*> jump_operand
%nterm <Jump*> jump
%nterm <vector<string>> symbol_list
%nterm <Directive*> label_list
%nterm <Instruction_type> load_store
%nterm <Instruction_type> instruction_type
%nterm <Instruction*> operand

 
%%

program:
    program line  
    | line
    | EOF  { assembly.finish_parsing(); }
    ;

line:
    directive 
    | instruction
    | jump
    | COMMENT
    ;

directive:
    GLOBAL symbol_list
    {
        $$ = new Directive();
        $$->set_type(Directive_type::GLOBAL);
        $$->set_operands($2);
        assembly.add_new_line($$);
    }
    | EXTERN symbol_list
    {
        $$ = new Directive();
        $$->set_type(Directive_type::EXTERN);
        $$->set_operands($2);
        assembly.add_new_line($$);
    }
    | SECTION SYMBOL 
    {
        $$ = new Directive();
        $$->set_type(Directive_type::SECTION);
        $$->add_operand($2);
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
        $$->add_operand($2);
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
        $$ -> add_operand($1);
        assembly.add_new_line($$);
    }
    | EQU SYMBOL COMMA LITERAL
    {

    }
    | ASCII
    {
        
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
        $$ -> add_operand($3, Label_type::SYMBOL);
    }
    | label_list COMMA LITERAL
    {
        $$ = $1;
        $$ -> add_operand($3, Label_type::LITERAL);
    }
    | LITERAL 
    {
        $$ = new Directive();
        $$ -> add_operand($1, Label_type::LITERAL);
    }
    | SYMBOL 
    {
        $$ = new Directive(); 
        $$ -> add_operand($1, Label_type::SYMBOL);
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
        $$ -> set_offset($5);
        $$ -> set_offset_type(Label_type::LITERAL);
        $$ -> set_offset ($5);
    }        
    | STAR LBRACKET REGISTER PLUS SYMBOL RBRACKET
    {
        $$ = new Jump();
        $$ -> set_operand ($3, Addressing_type::SYMBOL_OFFSET, Label_type::REGISTER);
        $$ -> set_offset($5);
        $$ -> set_offset_type(Label_type::SYMBOL);
        $$ -> set_offset ($5);
    }
    ;   

instruction:
    two_operand_instructions { $$ = $1; }
    | one_operand_instructions { $$ = $1; }
    | zero_operand_instructions { $$ = $1; }
    ;

two_operand_instructions:
    load_store_instructions { $$ = $1; }
    | instruction_type REGISTER COMMA REGISTER
    {
        $$ = new Instruction($1, $2, $4);
        assembly.add_new_line($$);
    }
    ;


load_store_instructions:
    load_store REGISTER COMMA operand
    {
        $$ = $4;
        $$ -> set_instruction_type($1);
        $$ -> set_first_operand($2);
        $$ -> set_number_of_operands(2);
        assembly.add_new_line($$);
    }
    ;

operand:
    DOLLAR LITERAL 
    {
        $$ = new Instruction();
        $$ -> set_second_operand($2, Label_type::LITERAL);
        $$ -> set_addressing_type(Addressing_type::ABSOLUTE);
    }
    | DOLLAR SYMBOL
    {
        $$ = new Instruction();
        $$ -> set_second_operand($2, Label_type::SYMBOL);
        $$ -> set_addressing_type(Addressing_type::ABSOLUTE);
    }
    | PERCENT SYMBOL
    {
        $$ = new Instruction();
        $$ -> set_second_operand($2, Label_type::SYMBOL);
        $$ -> set_addressing_type(Addressing_type::PC_RELATIVE);
    }
    | LITERAL
    {
        $$ = new Instruction();
        $$ -> set_second_operand($1, Label_type::LITERAL);
        $$ -> set_addressing_type(Addressing_type::MEMORY);
    }
    | SYMBOL
    {
        $$ = new Instruction();
        $$ -> set_second_operand($1, Label_type::SYMBOL);
        $$ -> set_addressing_type(Addressing_type::MEMORY);
    }
    | REGISTER
    {
        $$ = new Instruction();
        $$ -> set_second_operand($1, Label_type::REGISTER);
        $$ -> set_addressing_type(Addressing_type::ABSOLUTE);
    }       
    | LBRACKET REGISTER RBRACKET
    {
        $$ = new Instruction();
        $$ -> set_second_operand($2, Label_type::REGISTER);
        $$ -> set_addressing_type(Addressing_type::MEMORY);
    }   
    | LBRACKET REGISTER PLUS LITERAL RBRACKET
    {
        $$ = new Instruction();
        $$ -> set_second_operand($2, Label_type::REGISTER);
        $$ -> set_addressing_type(Addressing_type::LITERAL_OFFSET);
        $$ -> set_offset($4, Label_type::LITERAL);
    }        
    | LBRACKET REGISTER PLUS SYMBOL RBRACKET
    {
        $$ = new Instruction();
        $$ -> set_second_operand($2, Label_type::REGISTER);
        $$ -> set_addressing_type(Addressing_type::SYMBOL_OFFSET);
        $$ -> set_offset($4, Label_type::SYMBOL);
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

one_operand_instructions:
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
    | CALL operand 
    {
        $$ = $2;
        $$ -> set_instruction_type(Instruction_type::CALL);
        assembly.add_new_line($$);
    }
    ;

zero_operand_instructions:
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