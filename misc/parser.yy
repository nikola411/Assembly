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

    using namespace ParserUtil;
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

%type <vector<ParserOperand>> NT_SymbolList;
%type <vector<ParserOperand>> NT_LiteralList;

%nterm NT_Directive;
%type <string> NT_DirectiveWithSymbolList;
%type <string> NT_DirectiveWithList;
%type <string> NT_DirectiveSingleArgument;
%type <eInstructionIdentifier> NT_DirectiveIdentifier;

%nterm NT_ProcessorInstruction;

%nterm NT_BranchInstruction;
%type <eInstructionIdentifier> NT_ConditionalJumpIdentifier;
%type <eInstructionIdentifier> NT_UnconditionalJumpIdentifier;
%type <ConditionalJumpOperands> NT_ConditionalJumpOperands;

%nterm NT_StackInstruction;
%type <eInstructionIdentifier> NT_StackInstructionIdentifier;

%nterm NT_DataInstruction;
%type <eInstructionIdentifier> NT_DataInstructionIdentifier;
%type <vector<ParserOperand>> NT_DataInstructionOperands;

%nterm NT_MemoryInstruction;
%nterm NT_SpecialInstruction;

%nterm NT_LabelAndComment;

%nterm <ParserOperand> NT_Operand;
%nterm <ParserOperand> NT_JumpOperand;

//--------------------TERMINALS-----------------------
%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END;
%token HALT INT IRET RET;
%token JMP CALL;
%token PUSH POP;
%token BEQ BNE BGT;
%token XCHG ADD SUB MUL DIV NOT AND OR XOR SHL SHR;
%token LD ST;
%token CSRRD CSRWR;
%token <string> LABEL;

%token <string> SYMBOL;
%token <string> LITERAL;
%token <string> GPR;
%token <string> CSR;

%token DOLLAR "$" R_BRACKET "]" L_BRACKET "[";
%token L_PAREN "(" R_PAREN ")";
%token PERCENT "%" PLUS "+" MINUS "-";
%token COMMA ",";
%token COMMENT;
%token EOF 0 "end of file";
 
%%

NT_Program:
    NT_Program NT_Line { assembly.FinishInstruction(); }
    | NT_Line { assembly.FinishInstruction(); }
    | EOF
    ;

NT_Line:
    NT_Directive
    | NT_ProcessorInstruction
    | NT_BranchInstruction
    | NT_StackInstruction
    | NT_DataInstruction
    | NT_MemoryInstruction
    | NT_SpecialInstruction
    | NT_LabelAndComment
    ;

NT_LabelAndComment:
    COMMENT
    | LABEL
    {
        assembly.SetInstruction(eInstructionIdentifier::LBL, eInstructionType::LABEL);
        assembly.SetOperand($1, eOperandType::SYM);
    }
    ;
//-------------------DIRECTIVES-------------------
NT_Directive:
    NT_DirectiveWithSymbolList
    | NT_DirectiveWithList
    | NT_DirectiveSingleArgument
    | END
    ;

NT_DirectiveWithList:
    WORD NT_LiteralList
    {
        assembly.SetInstruction(eInstructionIdentifier::WORD, eInstructionType::DIRECTIVE);
        assembly.SetMultipleOperands($2);
        
    }
    | WORD NT_SymbolList
    {
        assembly.SetInstruction(eInstructionIdentifier::WORD, eInstructionType::DIRECTIVE);
        assembly.SetMultipleOperands($2);
    }
    ;

NT_DirectiveSingleArgument:
    SECTION SYMBOL
    {
        assembly.SetInstruction(eInstructionIdentifier::SECTION, eInstructionType::DIRECTIVE);
        assembly.SetOperand($2, eOperandType::SYM);
    }
    | SKIP LITERAL
    {
        assembly.SetInstruction(eInstructionIdentifier::SKIP, eInstructionType::DIRECTIVE);
        assembly.SetOperand($2, eOperandType::LTR);
    }
    ;

NT_DirectiveWithSymbolList:
    NT_DirectiveIdentifier NT_SymbolList
    {
        assembly.SetInstruction($1, eInstructionType::DIRECTIVE);
        assembly.SetMultipleOperands($2);
    }
    ;

NT_DirectiveIdentifier:
    GLOBAL { $$ = eInstructionIdentifier::GLOBAL; }
    | EXTERN { $$ = eInstructionIdentifier::EXTERN; }
    ;
//---------------------DIRECTIVES-END----------------
//---------------------PROCESSOR-INSTR---------------
NT_ProcessorInstruction:
    HALT { assembly.SetInstruction(eInstructionIdentifier::HALT, eInstructionType::PROCESSOR); }
    | INT { assembly.SetInstruction(eInstructionIdentifier::INT, eInstructionType::PROCESSOR); }
    | IRET { assembly.SetInstruction(eInstructionIdentifier::IRET, eInstructionType::PROCESSOR); }
    | RET { assembly.SetInstruction(eInstructionIdentifier::RET, eInstructionType::PROCESSOR); }
    ;
//---------------------PROCESSOR-END-----------------
//---------------------BRANCH------------------------
NT_BranchInstruction:
    NT_ConditionalJumpIdentifier NT_ConditionalJumpOperands
    {
        assembly.SetInstruction($1, eInstructionType::BRANCH);
        assembly.SetOperand($2.gpr1, eOperandType::GPR);
        assembly.SetOperand($2.gpr2, eOperandType::GPR);
        assembly.SetOperand($2.operand);
    }
    | NT_UnconditionalJumpIdentifier NT_JumpOperand
    {
        assembly.SetInstruction($1, eInstructionType::BRANCH);
        assembly.SetOperand($2);
    }
    ;

NT_ConditionalJumpIdentifier:
    BEQ { $$ = eInstructionIdentifier::BEQ; }
    | BNE { $$ = eInstructionIdentifier::BNE; }
    | BGT { $$ = eInstructionIdentifier::BGT; }
    ;

NT_ConditionalJumpOperands:
    "%" GPR COMMA "%" GPR COMMA NT_JumpOperand
    {
        $$.gpr1 = $2;
        $$.gpr2 = $5;
        $$.operand = $7;
    }
    ;

NT_UnconditionalJumpIdentifier:
    JMP { $$ = eInstructionIdentifier::JMP; }
    | CALL { $$ = eInstructionIdentifier::CALL; }
    ;
//---------------------BRANCH-END--------------------
//---------------------DATA--------------------------
NT_DataInstruction:
    NT_DataInstructionIdentifier NT_DataInstructionOperands
    {
        if ($1 == eInstructionIdentifier::NOT && $2.size() > 1)
        {
            std::cout << "wrong number of argruments for instruction \n";
            return 1;
        }

        assembly.SetInstruction($1, eInstructionType::DATA);
        assembly.SetMultipleOperands($2);
    }
    ;

NT_DataInstructionIdentifier:
    XCHG { $$ = eInstructionIdentifier::XCHG; }
    | ADD { $$ = eInstructionIdentifier::ADD; }
    | SUB { $$ = eInstructionIdentifier::SUB; }
    | MUL { $$ = eInstructionIdentifier::MUL; }
    | DIV { $$ = eInstructionIdentifier::DIV; }
    | NOT { $$ = eInstructionIdentifier::NOT; }
    | AND { $$ = eInstructionIdentifier::AND; }
    | OR  { $$ = eInstructionIdentifier::OR;  }
    | XOR { $$ = eInstructionIdentifier::XOR; }
    | SHL { $$ = eInstructionIdentifier::SHL; }
    | SHR { $$ = eInstructionIdentifier::SHR; }
    ;

NT_DataInstructionOperands:
    "%" GPR "," "%" GPR
    {
        $$.push_back(ParserOperand($2, eOperandType::GPR));
        $$.push_back(ParserOperand($5, eOperandType::GPR));
    }
    | "%" GPR { $$.push_back(ParserOperand($2, eOperandType::GPR)); }
    ;
//---------------------DATA-END----------------------
//---------------------MEMORY------------------------
NT_MemoryInstruction:
    LD NT_Operand "," "%" GPR
    {
        assembly.SetInstruction(eInstructionIdentifier::LD, eInstructionType::MEMORY);
        assembly.SetOperand($2);
        assembly.SetOperand($5, eOperandType::GPR);
    }
    | ST "%" GPR "," NT_Operand
    {
        assembly.SetInstruction(eInstructionIdentifier::ST, eInstructionType::MEMORY);
        assembly.SetOperand($3, eOperandType::GPR);
        assembly.SetOperand($5);
    }
    ;
//---------------------MEMORY-END--------------------
//---------------------SPECIAL-----------------------
NT_SpecialInstruction:
    CSRRD "%" CSR "," "%" GPR
    {
        assembly.SetInstruction(eInstructionIdentifier::CSRRD, eInstructionType::SPECIAL);
        assembly.SetOperand($3, eOperandType::CSR);
        assembly.SetOperand($6, eOperandType::GPR);
    }
    | CSRWR "%" GPR "," "%" CSR
    {
        assembly.SetInstruction(eInstructionIdentifier::CSRWR, eInstructionType::SPECIAL);
        assembly.SetOperand($3, eOperandType::GPR);
        assembly.SetOperand($6, eOperandType::CSR);
    }
    ;
//---------------------SPECIAL-END-------------------
//---------------------STACK-------------------------
NT_StackInstruction:
    NT_StackInstructionIdentifier "%" GPR
    {
        assembly.SetInstruction($1, eInstructionType::STACK);
        assembly.SetOperand($3, eOperandType::GPR);
    }
    ;

NT_StackInstructionIdentifier:
    PUSH { $$ = eInstructionIdentifier::PUSH; }
    | POP { $$ = eInstructionIdentifier::POP; }
    ;
//---------------------STACK-END---------------------
//---------------------LISTS-------------------------
NT_SymbolList:
    NT_SymbolList COMMA SYMBOL { $1.push_back(ParserOperand($3, eOperandType::SYM)); $$ = $1; }
    | SYMBOL { $$.push_back(ParserOperand($1, eOperandType::SYM)); }
    ;

NT_LiteralList:
    NT_LiteralList COMMA LITERAL { $1.push_back(ParserOperand($3, eOperandType::LTR)); $$ = $1; }
    | LITERAL { $$.push_back(ParserOperand($1, eOperandType::LTR)); }
    ;

//---------------------LISTS-END---------------------
//---------------------OPERANDS----------------------
NT_JumpOperand:
    SYMBOL    { $$.value = $1; $$.type = eOperandType::SYM; }
    | LITERAL { $$.value = $1; $$.type = eOperandType::LTR; }
    ;

NT_Operand:
    "$" LITERAL { $$.value = $2; $$.type = eOperandType::LTR; $$.addressingType = eAddressingType::ADDR_DIRECT; }
    | "$" SYMBOL { $$.value = $2; $$.type = eOperandType::SYM; $$.addressingType = eAddressingType::ADDR_DIRECT; }
    | LITERAL { $$.value = $1; $$.type = eOperandType::LTR; $$.addressingType = eAddressingType::ADDR_MEMORY; }
    | SYMBOL { $$.value = $1; $$.type = eOperandType::SYM; $$.addressingType = eAddressingType::ADDR_MEMORY; }
    | "%" GPR { $$.value = $2; $$.type = eOperandType::GPR; $$.addressingType = eAddressingType::ADDR_DIRECT; }
    | "[" "%" GPR "]" { $$.value = $3; $$.type = eOperandType::GPR; $$.addressingType = eAddressingType::ADDR_MEMORY; }
    | "[" "%" GPR "+" LITERAL "]" { $$.value = $3; $$.offset = $5; $$.offsetType = eOperandType::LTR; $$.type = eOperandType::GPR; $$.addressingType = eAddressingType::ADDR_MEMORY_OFFSET; }
    | "[" "%" GPR "+" SYMBOL "]"  { $$.value = $3; $$.offset = $5; $$.offsetType = eOperandType::SYM; $$.type = eOperandType::GPR; $$.addressingType = eAddressingType::ADDR_MEMORY_OFFSET; }
    ;
//---------------------OPERANDS-END------------------

%%

void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}
