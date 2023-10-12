%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.5.1"
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
    #include "assembly.hpp"
    
    class Driver;
    class Assembly;

    using std::string;
    using std::vector;
    using std::pair;
}

%param { Driver& drv }
%param { Assembly& assembly }
%locations

%define parse.trace
%define parse.error verbose
%define parse.lac full
 
%code
{
    #include <memory>
    #include <vector>
    #include <string>

    #include "util.hpp"
    #include "driver.hpp"
}
 
%define api.token.prefix {TOK_}

//--------------------NONTERMINALS--------------------
%nterm NT_Program;
%nterm NT_Line;

%type <vector<pair<string, eOperandType>>> NT_SymbolList;
%type <vector<pair<string, eOperandType>>> NT_LiteralList;

%nterm NT_Directive;
%type <string> NT_DirectiveWithSymbolList;
%type <string> NT_DirectiveWithList;
%type <string> NT_DirectiveSingleArgument;
%type <eInstructionIdentifier> NT_DirectiveIdentifier;

%nterm NT_ZeroOperandInstruction;
%type <eInstructionIdentifier> NT_ZeroOperandInstructionIdentifer

%nterm NT_OneOperandInstruction
%type <eInstructionIdentifier> NT_OneOperandJumpIdentifier
%type <eInstructionIdentifier> NT_OneOperandInstructionIdentifier

%nterm NT_Operand
%nterm NT_Register

//--------------------TERMINALS-----------------------
%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END;
%token HALT INT IRET RET
%token JMP CALL
%token PUSH POP NOT

%token <string> SYMBOL;
%token <string> LITERAL;

%token COMMA ","
%token EOF 0 "end of file"
 
%%

NT_Program:
    NT_Program NT_Line  
    | NT_Line
    | EOF  { }
    ;

NT_Line:
    NT_Directive
    | NT_ZeroOperandInstruction
    ;

//-------------------DIRECTIVES-------------------
NT_Directive:
    NT_DirectiveWithSymbolList
    | NT_DirectiveWithList
    | NT_DirectiveSingleArgument
    END
    ;

NT_DirectiveWithList:
    WORD NT_LiteralList
    {
        assembly.SetInstruction(eInstructionIdentifier::WORD, eInstructionType::DIRECTIVE);
        assembly.SetMultipleOperands($2);
        assembly.FinishInstruction();
    }
    | WORD NT_SymbolList
    {
        assembly.SetInstruction(eInstructionIdentifier::WORD, eInstructionType::DIRECTIVE);
        assembly.SetMultipleOperands($2);
        assembly.FinishInstruction();
    }
    ;

NT_DirectiveSingleArgument:
    SECTION SYMBOL
    {
        assembly.SetInstruction(eInstructionIdentifier::SECTION, eInstructionType::DIRECTIVE);
        assembly.SetOperand($2, eOperandType::SYM);
        assembly.FinishInstruction();
    }
    | SKIP LITERAL
    {
        assembly.SetInstruction(eInstructionIdentifier::SKIP, eInstructionType::DIRECTIVE);
        assembly.SetOperand($2, eOperandType::LTR);
        assembly.FinishInstruction();
    }
    ;

NT_DirectiveWithSymbolList:
    NT_DirectiveIdentifier NT_SymbolList
    {
        assembly.SetInstruction($1, eInstructionType::DIRECTIVE);
        assembly.SetMultipleOperands($2);
        assembly.FinishInstruction();
    }
    ;

NT_DirectiveIdentifier:
    GLOBAL { $$ = eInstructionIdentifier::GLOBAL; }
    | EXTERN { $$ = eInstructionIdentifier::EXTERN; }
    ;

//---------------------DIRECTIVES-END----------------
//---------------------ZERO-OP-----------------------

NT_ZeroOperandInstruction:
    NT_ZeroOperandInstructionIdentifer
    {
        assembly.SetInstruction($1, eInstructionType::ZERO_OPERAND_INSTRUCTION);
        assembly.FinishInstruction();
    }
    ;

NT_ZeroOperandInstructionIdentifer:
    HALT { $$ = eInstructionIdentifer::HALT; }
    | INT { $$ = eInstructionIdentifer::INT; }
    | IRET { $$ = eInstructionIdentifer::IRET; }
    | RET { $$ = eInstructionIdentifer::RET; }
    ;

//---------------------ZERO-OP-END-------------------
//---------------------SINGLE-OP---------------------

NT_OneOperandInstruction:
    NT_OneOperandJumpIdentifier NT_Operand
    {

    }
    | NT_OneOperandInstructionIdentifier NT_Register
    {

    }
    ;

NT_OneOperandJumpIdentifier:
    JMP { assembly.SetInstruction(eInstructionIdentifier::JMP, eInstructionType::ONE_OPERAND_INSTRUCTION); }
    | CALL { assembly.SetInstruction(eInstructionIdentifier::CALL, eInstructionType::ONE_OPERAND_INSTRUCTION); }
    ;

NT_OneOperandInstructionIdentifier:
    PUSH
    | POP
    | NOT
    ;

//---------------------SINGLE-OP-END-----------------
//---------------------LISTS-------------------------
NT_SymbolList:
    NT_SymbolList COMMA SYMBOL { $1.push_back({ $3, eOperandType::SYM }); $$ = $1; }
    | SYMBOL { $$.push_back({ $1, eOperandType::SYM }); }
    ;

NT_LiteralList:
    NT_LiteralList COMMA LITERAL { $1.push_back({ $3, eOperandType::LTR }); $$ = $1; }
    | LITERAL { $$.push_back({ $1, eOperandType::LTR }); }
    ;

//---------------------LISTS-END---------------------
//---------------------------------------------------
NT_Operand:

NT_Register:

 
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}