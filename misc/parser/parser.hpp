// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file ./misc/parser/parser.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_MISC_PARSER_PARSER_HPP_INCLUDED
# define YY_YY_MISC_PARSER_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 14 "./misc/parser.yy"

    #include "assembly.hpp"
    
    class Driver;
    class Assembly;

    using std::string;
    using std::vector;
    using std::pair;

    using namespace ParserUtil;

#line 62 "./misc/parser/parser.hpp"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"
#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

namespace yy {
#line 202 "./misc/parser/parser.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // NT_ConditionalJumpOperands
      char dummy1[sizeof (ConditionalJumpOperands)];

      // NT_JumpOperand
      // NT_Operand
      char dummy2[sizeof (ParserOperand)];

      // NT_DirectiveIdentifier
      // NT_ConditionalJumpIdentifier
      // NT_UnconditionalJumpIdentifier
      // NT_DataInstructionIdentifier
      // NT_StackInstructionIdentifier
      char dummy3[sizeof (eInstructionIdentifier)];

      // LABEL
      // SYMBOL
      // LITERAL
      // GPR
      // CSR
      // NT_DirectiveWithList
      // NT_DirectiveSingleArgument
      // NT_DirectiveWithSymbolList
      char dummy4[sizeof (string)];

      // NT_DataInstructionOperands
      // NT_SymbolList
      // NT_LiteralList
      char dummy5[sizeof (vector<ParserOperand>)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        TOK_YYEMPTY = -2,
    TOK_EOF = 0,                   // "end of file"
    TOK_YYerror = 1,               // error
    TOK_YYUNDEF = 2,               // "invalid token"
    TOK_GLOBAL = 3,                // GLOBAL
    TOK_EXTERN = 4,                // EXTERN
    TOK_SECTION = 5,               // SECTION
    TOK_WORD = 6,                  // WORD
    TOK_SKIP = 7,                  // SKIP
    TOK_ASCII = 8,                 // ASCII
    TOK_EQU = 9,                   // EQU
    TOK_END = 10,                  // END
    TOK_HALT = 11,                 // HALT
    TOK_INT = 12,                  // INT
    TOK_IRET = 13,                 // IRET
    TOK_RET = 14,                  // RET
    TOK_JMP = 15,                  // JMP
    TOK_CALL = 16,                 // CALL
    TOK_PUSH = 17,                 // PUSH
    TOK_POP = 18,                  // POP
    TOK_BEQ = 19,                  // BEQ
    TOK_BNE = 20,                  // BNE
    TOK_BGT = 21,                  // BGT
    TOK_XCHG = 22,                 // XCHG
    TOK_ADD = 23,                  // ADD
    TOK_SUB = 24,                  // SUB
    TOK_MUL = 25,                  // MUL
    TOK_DIV = 26,                  // DIV
    TOK_NOT = 27,                  // NOT
    TOK_AND = 28,                  // AND
    TOK_OR = 29,                   // OR
    TOK_XOR = 30,                  // XOR
    TOK_SHL = 31,                  // SHL
    TOK_SHR = 32,                  // SHR
    TOK_LD = 33,                   // LD
    TOK_ST = 34,                   // ST
    TOK_CSRRD = 35,                // CSRRD
    TOK_CSRWR = 36,                // CSRWR
    TOK_LABEL = 37,                // LABEL
    TOK_SYMBOL = 38,               // SYMBOL
    TOK_LITERAL = 39,              // LITERAL
    TOK_GPR = 40,                  // GPR
    TOK_CSR = 41,                  // CSR
    TOK_DOLLAR = 42,               // "$"
    TOK_R_BRACKET = 43,            // "]"
    TOK_L_BRACKET = 44,            // "["
    TOK_L_PAREN = 45,              // "("
    TOK_R_PAREN = 46,              // ")"
    TOK_PERCENT = 47,              // "%"
    TOK_PLUS = 48,                 // "+"
    TOK_MINUS = 49,                // "-"
    TOK_COMMA = 50,                // ","
    TOK_COMMENT = 51               // COMMENT
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 52, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_GLOBAL = 3,                            // GLOBAL
        S_EXTERN = 4,                            // EXTERN
        S_SECTION = 5,                           // SECTION
        S_WORD = 6,                              // WORD
        S_SKIP = 7,                              // SKIP
        S_ASCII = 8,                             // ASCII
        S_EQU = 9,                               // EQU
        S_END = 10,                              // END
        S_HALT = 11,                             // HALT
        S_INT = 12,                              // INT
        S_IRET = 13,                             // IRET
        S_RET = 14,                              // RET
        S_JMP = 15,                              // JMP
        S_CALL = 16,                             // CALL
        S_PUSH = 17,                             // PUSH
        S_POP = 18,                              // POP
        S_BEQ = 19,                              // BEQ
        S_BNE = 20,                              // BNE
        S_BGT = 21,                              // BGT
        S_XCHG = 22,                             // XCHG
        S_ADD = 23,                              // ADD
        S_SUB = 24,                              // SUB
        S_MUL = 25,                              // MUL
        S_DIV = 26,                              // DIV
        S_NOT = 27,                              // NOT
        S_AND = 28,                              // AND
        S_OR = 29,                               // OR
        S_XOR = 30,                              // XOR
        S_SHL = 31,                              // SHL
        S_SHR = 32,                              // SHR
        S_LD = 33,                               // LD
        S_ST = 34,                               // ST
        S_CSRRD = 35,                            // CSRRD
        S_CSRWR = 36,                            // CSRWR
        S_LABEL = 37,                            // LABEL
        S_SYMBOL = 38,                           // SYMBOL
        S_LITERAL = 39,                          // LITERAL
        S_GPR = 40,                              // GPR
        S_CSR = 41,                              // CSR
        S_DOLLAR = 42,                           // "$"
        S_R_BRACKET = 43,                        // "]"
        S_L_BRACKET = 44,                        // "["
        S_L_PAREN = 45,                          // "("
        S_R_PAREN = 46,                          // ")"
        S_PERCENT = 47,                          // "%"
        S_PLUS = 48,                             // "+"
        S_MINUS = 49,                            // "-"
        S_COMMA = 50,                            // ","
        S_COMMENT = 51,                          // COMMENT
        S_YYACCEPT = 52,                         // $accept
        S_NT_Program = 53,                       // NT_Program
        S_NT_Line = 54,                          // NT_Line
        S_NT_LabelAndComment = 55,               // NT_LabelAndComment
        S_NT_Directive = 56,                     // NT_Directive
        S_NT_DirectiveWithList = 57,             // NT_DirectiveWithList
        S_NT_DirectiveSingleArgument = 58,       // NT_DirectiveSingleArgument
        S_NT_DirectiveWithSymbolList = 59,       // NT_DirectiveWithSymbolList
        S_NT_DirectiveIdentifier = 60,           // NT_DirectiveIdentifier
        S_NT_ProcessorInstruction = 61,          // NT_ProcessorInstruction
        S_NT_BranchInstruction = 62,             // NT_BranchInstruction
        S_NT_ConditionalJumpIdentifier = 63,     // NT_ConditionalJumpIdentifier
        S_NT_ConditionalJumpOperands = 64,       // NT_ConditionalJumpOperands
        S_NT_UnconditionalJumpIdentifier = 65,   // NT_UnconditionalJumpIdentifier
        S_NT_DataInstruction = 66,               // NT_DataInstruction
        S_NT_DataInstructionIdentifier = 67,     // NT_DataInstructionIdentifier
        S_NT_DataInstructionOperands = 68,       // NT_DataInstructionOperands
        S_NT_MemoryInstruction = 69,             // NT_MemoryInstruction
        S_NT_SpecialInstruction = 70,            // NT_SpecialInstruction
        S_NT_StackInstruction = 71,              // NT_StackInstruction
        S_NT_StackInstructionIdentifier = 72,    // NT_StackInstructionIdentifier
        S_NT_SymbolList = 73,                    // NT_SymbolList
        S_NT_LiteralList = 74,                   // NT_LiteralList
        S_NT_JumpOperand = 75,                   // NT_JumpOperand
        S_NT_Operand = 76                        // NT_Operand
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_NT_ConditionalJumpOperands: // NT_ConditionalJumpOperands
        value.move< ConditionalJumpOperands > (std::move (that.value));
        break;

      case symbol_kind::S_NT_JumpOperand: // NT_JumpOperand
      case symbol_kind::S_NT_Operand: // NT_Operand
        value.move< ParserOperand > (std::move (that.value));
        break;

      case symbol_kind::S_NT_DirectiveIdentifier: // NT_DirectiveIdentifier
      case symbol_kind::S_NT_ConditionalJumpIdentifier: // NT_ConditionalJumpIdentifier
      case symbol_kind::S_NT_UnconditionalJumpIdentifier: // NT_UnconditionalJumpIdentifier
      case symbol_kind::S_NT_DataInstructionIdentifier: // NT_DataInstructionIdentifier
      case symbol_kind::S_NT_StackInstructionIdentifier: // NT_StackInstructionIdentifier
        value.move< eInstructionIdentifier > (std::move (that.value));
        break;

      case symbol_kind::S_LABEL: // LABEL
      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_LITERAL: // LITERAL
      case symbol_kind::S_GPR: // GPR
      case symbol_kind::S_CSR: // CSR
      case symbol_kind::S_NT_DirectiveWithList: // NT_DirectiveWithList
      case symbol_kind::S_NT_DirectiveSingleArgument: // NT_DirectiveSingleArgument
      case symbol_kind::S_NT_DirectiveWithSymbolList: // NT_DirectiveWithSymbolList
        value.move< string > (std::move (that.value));
        break;

      case symbol_kind::S_NT_DataInstructionOperands: // NT_DataInstructionOperands
      case symbol_kind::S_NT_SymbolList: // NT_SymbolList
      case symbol_kind::S_NT_LiteralList: // NT_LiteralList
        value.move< vector<ParserOperand> > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ConditionalJumpOperands&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ConditionalJumpOperands& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ParserOperand&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ParserOperand& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, eInstructionIdentifier&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const eInstructionIdentifier& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<ParserOperand>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<ParserOperand>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_NT_ConditionalJumpOperands: // NT_ConditionalJumpOperands
        value.template destroy< ConditionalJumpOperands > ();
        break;

      case symbol_kind::S_NT_JumpOperand: // NT_JumpOperand
      case symbol_kind::S_NT_Operand: // NT_Operand
        value.template destroy< ParserOperand > ();
        break;

      case symbol_kind::S_NT_DirectiveIdentifier: // NT_DirectiveIdentifier
      case symbol_kind::S_NT_ConditionalJumpIdentifier: // NT_ConditionalJumpIdentifier
      case symbol_kind::S_NT_UnconditionalJumpIdentifier: // NT_UnconditionalJumpIdentifier
      case symbol_kind::S_NT_DataInstructionIdentifier: // NT_DataInstructionIdentifier
      case symbol_kind::S_NT_StackInstructionIdentifier: // NT_StackInstructionIdentifier
        value.template destroy< eInstructionIdentifier > ();
        break;

      case symbol_kind::S_LABEL: // LABEL
      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_LITERAL: // LITERAL
      case symbol_kind::S_GPR: // GPR
      case symbol_kind::S_CSR: // CSR
      case symbol_kind::S_NT_DirectiveWithList: // NT_DirectiveWithList
      case symbol_kind::S_NT_DirectiveSingleArgument: // NT_DirectiveSingleArgument
      case symbol_kind::S_NT_DirectiveWithSymbolList: // NT_DirectiveWithSymbolList
        value.template destroy< string > ();
        break;

      case symbol_kind::S_NT_DataInstructionOperands: // NT_DataInstructionOperands
      case symbol_kind::S_NT_SymbolList: // NT_SymbolList
      case symbol_kind::S_NT_LiteralList: // NT_LiteralList
        value.template destroy< vector<ParserOperand> > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// The user-facing name of this symbol.
      std::string name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::TOK_EOF
                   || (token::TOK_YYerror <= tok && tok <= token::TOK_CSRWR)
                   || (token::TOK_DOLLAR <= tok && tok <= token::TOK_COMMENT));
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, string v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const string& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::TOK_LABEL <= tok && tok <= token::TOK_CSR));
#endif
      }
    };

    /// Build a parser object.
    parser (Driver& drv_yyarg, Assembly& assembly_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);

    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EOF (location_type l)
      {
        return symbol_type (token::TOK_EOF, std::move (l));
      }
#else
      static
      symbol_type
      make_EOF (const location_type& l)
      {
        return symbol_type (token::TOK_EOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror (location_type l)
      {
        return symbol_type (token::TOK_YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::TOK_YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::TOK_YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::TOK_YYUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GLOBAL (location_type l)
      {
        return symbol_type (token::TOK_GLOBAL, std::move (l));
      }
#else
      static
      symbol_type
      make_GLOBAL (const location_type& l)
      {
        return symbol_type (token::TOK_GLOBAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXTERN (location_type l)
      {
        return symbol_type (token::TOK_EXTERN, std::move (l));
      }
#else
      static
      symbol_type
      make_EXTERN (const location_type& l)
      {
        return symbol_type (token::TOK_EXTERN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SECTION (location_type l)
      {
        return symbol_type (token::TOK_SECTION, std::move (l));
      }
#else
      static
      symbol_type
      make_SECTION (const location_type& l)
      {
        return symbol_type (token::TOK_SECTION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WORD (location_type l)
      {
        return symbol_type (token::TOK_WORD, std::move (l));
      }
#else
      static
      symbol_type
      make_WORD (const location_type& l)
      {
        return symbol_type (token::TOK_WORD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SKIP (location_type l)
      {
        return symbol_type (token::TOK_SKIP, std::move (l));
      }
#else
      static
      symbol_type
      make_SKIP (const location_type& l)
      {
        return symbol_type (token::TOK_SKIP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASCII (location_type l)
      {
        return symbol_type (token::TOK_ASCII, std::move (l));
      }
#else
      static
      symbol_type
      make_ASCII (const location_type& l)
      {
        return symbol_type (token::TOK_ASCII, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQU (location_type l)
      {
        return symbol_type (token::TOK_EQU, std::move (l));
      }
#else
      static
      symbol_type
      make_EQU (const location_type& l)
      {
        return symbol_type (token::TOK_EQU, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END (location_type l)
      {
        return symbol_type (token::TOK_END, std::move (l));
      }
#else
      static
      symbol_type
      make_END (const location_type& l)
      {
        return symbol_type (token::TOK_END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HALT (location_type l)
      {
        return symbol_type (token::TOK_HALT, std::move (l));
      }
#else
      static
      symbol_type
      make_HALT (const location_type& l)
      {
        return symbol_type (token::TOK_HALT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT (location_type l)
      {
        return symbol_type (token::TOK_INT, std::move (l));
      }
#else
      static
      symbol_type
      make_INT (const location_type& l)
      {
        return symbol_type (token::TOK_INT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IRET (location_type l)
      {
        return symbol_type (token::TOK_IRET, std::move (l));
      }
#else
      static
      symbol_type
      make_IRET (const location_type& l)
      {
        return symbol_type (token::TOK_IRET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RET (location_type l)
      {
        return symbol_type (token::TOK_RET, std::move (l));
      }
#else
      static
      symbol_type
      make_RET (const location_type& l)
      {
        return symbol_type (token::TOK_RET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_JMP (location_type l)
      {
        return symbol_type (token::TOK_JMP, std::move (l));
      }
#else
      static
      symbol_type
      make_JMP (const location_type& l)
      {
        return symbol_type (token::TOK_JMP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CALL (location_type l)
      {
        return symbol_type (token::TOK_CALL, std::move (l));
      }
#else
      static
      symbol_type
      make_CALL (const location_type& l)
      {
        return symbol_type (token::TOK_CALL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PUSH (location_type l)
      {
        return symbol_type (token::TOK_PUSH, std::move (l));
      }
#else
      static
      symbol_type
      make_PUSH (const location_type& l)
      {
        return symbol_type (token::TOK_PUSH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_POP (location_type l)
      {
        return symbol_type (token::TOK_POP, std::move (l));
      }
#else
      static
      symbol_type
      make_POP (const location_type& l)
      {
        return symbol_type (token::TOK_POP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BEQ (location_type l)
      {
        return symbol_type (token::TOK_BEQ, std::move (l));
      }
#else
      static
      symbol_type
      make_BEQ (const location_type& l)
      {
        return symbol_type (token::TOK_BEQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BNE (location_type l)
      {
        return symbol_type (token::TOK_BNE, std::move (l));
      }
#else
      static
      symbol_type
      make_BNE (const location_type& l)
      {
        return symbol_type (token::TOK_BNE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BGT (location_type l)
      {
        return symbol_type (token::TOK_BGT, std::move (l));
      }
#else
      static
      symbol_type
      make_BGT (const location_type& l)
      {
        return symbol_type (token::TOK_BGT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_XCHG (location_type l)
      {
        return symbol_type (token::TOK_XCHG, std::move (l));
      }
#else
      static
      symbol_type
      make_XCHG (const location_type& l)
      {
        return symbol_type (token::TOK_XCHG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ADD (location_type l)
      {
        return symbol_type (token::TOK_ADD, std::move (l));
      }
#else
      static
      symbol_type
      make_ADD (const location_type& l)
      {
        return symbol_type (token::TOK_ADD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SUB (location_type l)
      {
        return symbol_type (token::TOK_SUB, std::move (l));
      }
#else
      static
      symbol_type
      make_SUB (const location_type& l)
      {
        return symbol_type (token::TOK_SUB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MUL (location_type l)
      {
        return symbol_type (token::TOK_MUL, std::move (l));
      }
#else
      static
      symbol_type
      make_MUL (const location_type& l)
      {
        return symbol_type (token::TOK_MUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIV (location_type l)
      {
        return symbol_type (token::TOK_DIV, std::move (l));
      }
#else
      static
      symbol_type
      make_DIV (const location_type& l)
      {
        return symbol_type (token::TOK_DIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NOT (location_type l)
      {
        return symbol_type (token::TOK_NOT, std::move (l));
      }
#else
      static
      symbol_type
      make_NOT (const location_type& l)
      {
        return symbol_type (token::TOK_NOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AND (location_type l)
      {
        return symbol_type (token::TOK_AND, std::move (l));
      }
#else
      static
      symbol_type
      make_AND (const location_type& l)
      {
        return symbol_type (token::TOK_AND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OR (location_type l)
      {
        return symbol_type (token::TOK_OR, std::move (l));
      }
#else
      static
      symbol_type
      make_OR (const location_type& l)
      {
        return symbol_type (token::TOK_OR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_XOR (location_type l)
      {
        return symbol_type (token::TOK_XOR, std::move (l));
      }
#else
      static
      symbol_type
      make_XOR (const location_type& l)
      {
        return symbol_type (token::TOK_XOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SHL (location_type l)
      {
        return symbol_type (token::TOK_SHL, std::move (l));
      }
#else
      static
      symbol_type
      make_SHL (const location_type& l)
      {
        return symbol_type (token::TOK_SHL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SHR (location_type l)
      {
        return symbol_type (token::TOK_SHR, std::move (l));
      }
#else
      static
      symbol_type
      make_SHR (const location_type& l)
      {
        return symbol_type (token::TOK_SHR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LD (location_type l)
      {
        return symbol_type (token::TOK_LD, std::move (l));
      }
#else
      static
      symbol_type
      make_LD (const location_type& l)
      {
        return symbol_type (token::TOK_LD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ST (location_type l)
      {
        return symbol_type (token::TOK_ST, std::move (l));
      }
#else
      static
      symbol_type
      make_ST (const location_type& l)
      {
        return symbol_type (token::TOK_ST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CSRRD (location_type l)
      {
        return symbol_type (token::TOK_CSRRD, std::move (l));
      }
#else
      static
      symbol_type
      make_CSRRD (const location_type& l)
      {
        return symbol_type (token::TOK_CSRRD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CSRWR (location_type l)
      {
        return symbol_type (token::TOK_CSRWR, std::move (l));
      }
#else
      static
      symbol_type
      make_CSRWR (const location_type& l)
      {
        return symbol_type (token::TOK_CSRWR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LABEL (string v, location_type l)
      {
        return symbol_type (token::TOK_LABEL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_LABEL (const string& v, const location_type& l)
      {
        return symbol_type (token::TOK_LABEL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SYMBOL (string v, location_type l)
      {
        return symbol_type (token::TOK_SYMBOL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_SYMBOL (const string& v, const location_type& l)
      {
        return symbol_type (token::TOK_SYMBOL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LITERAL (string v, location_type l)
      {
        return symbol_type (token::TOK_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_LITERAL (const string& v, const location_type& l)
      {
        return symbol_type (token::TOK_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GPR (string v, location_type l)
      {
        return symbol_type (token::TOK_GPR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_GPR (const string& v, const location_type& l)
      {
        return symbol_type (token::TOK_GPR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CSR (string v, location_type l)
      {
        return symbol_type (token::TOK_CSR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CSR (const string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CSR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOLLAR (location_type l)
      {
        return symbol_type (token::TOK_DOLLAR, std::move (l));
      }
#else
      static
      symbol_type
      make_DOLLAR (const location_type& l)
      {
        return symbol_type (token::TOK_DOLLAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_R_BRACKET (location_type l)
      {
        return symbol_type (token::TOK_R_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_R_BRACKET (const location_type& l)
      {
        return symbol_type (token::TOK_R_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_L_BRACKET (location_type l)
      {
        return symbol_type (token::TOK_L_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_L_BRACKET (const location_type& l)
      {
        return symbol_type (token::TOK_L_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_L_PAREN (location_type l)
      {
        return symbol_type (token::TOK_L_PAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_L_PAREN (const location_type& l)
      {
        return symbol_type (token::TOK_L_PAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_R_PAREN (location_type l)
      {
        return symbol_type (token::TOK_R_PAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_R_PAREN (const location_type& l)
      {
        return symbol_type (token::TOK_R_PAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PERCENT (location_type l)
      {
        return symbol_type (token::TOK_PERCENT, std::move (l));
      }
#else
      static
      symbol_type
      make_PERCENT (const location_type& l)
      {
        return symbol_type (token::TOK_PERCENT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS (location_type l)
      {
        return symbol_type (token::TOK_PLUS, std::move (l));
      }
#else
      static
      symbol_type
      make_PLUS (const location_type& l)
      {
        return symbol_type (token::TOK_PLUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS (location_type l)
      {
        return symbol_type (token::TOK_MINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_MINUS (const location_type& l)
      {
        return symbol_type (token::TOK_MINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::TOK_COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::TOK_COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMENT (location_type l)
      {
        return symbol_type (token::TOK_COMMENT, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMENT (const location_type& l)
      {
        return symbol_type (token::TOK_COMMENT, l);
      }
#endif


    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      const location_type& location () const YY_NOEXCEPT { return yyla_.location; }

      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const parser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif

    /// Check the lookahead yytoken.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_check_ (symbol_kind_type yytoken) const;
    /// Establish the initial context if no initial context currently exists.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_establish_ (symbol_kind_type yytoken);
    /// Discard any previous initial lookahead context because of event.
    /// \param event  the event which caused the lookahead to be discarded.
    ///               Only used for debbuging output.
    void yy_lac_discard_ (const char* event);

    /// Stored state numbers (used for stacks).
    typedef signed char state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const signed char yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const signed char yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const signed char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const signed char yytable_[];

    static const signed char yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;
    /// The stack for LAC.
    /// Logically, the yy_lac_stack's lifetime is confined to the function
    /// yy_lac_check_. We just store it as a member of this class to hold
    /// on to the memory and to avoid frequent reallocations.
    /// Since yy_lac_check_ is const, this member must be mutable.
    mutable std::vector<state_type> yylac_stack_;
    /// Whether an initial LAC context was established.
    bool yy_lac_established_;


    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 133,     ///< Last index in yytable_.
      yynnts_ = 25,  ///< Number of nonterminal symbols.
      yyfinal_ = 69 ///< Termination state number.
    };


    // User arguments.
    Driver& drv;
    Assembly& assembly;

  };

  inline
  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    return static_cast<symbol_kind_type> (t);
  }

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_NT_ConditionalJumpOperands: // NT_ConditionalJumpOperands
        value.copy< ConditionalJumpOperands > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NT_JumpOperand: // NT_JumpOperand
      case symbol_kind::S_NT_Operand: // NT_Operand
        value.copy< ParserOperand > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NT_DirectiveIdentifier: // NT_DirectiveIdentifier
      case symbol_kind::S_NT_ConditionalJumpIdentifier: // NT_ConditionalJumpIdentifier
      case symbol_kind::S_NT_UnconditionalJumpIdentifier: // NT_UnconditionalJumpIdentifier
      case symbol_kind::S_NT_DataInstructionIdentifier: // NT_DataInstructionIdentifier
      case symbol_kind::S_NT_StackInstructionIdentifier: // NT_StackInstructionIdentifier
        value.copy< eInstructionIdentifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LABEL: // LABEL
      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_LITERAL: // LITERAL
      case symbol_kind::S_GPR: // GPR
      case symbol_kind::S_CSR: // CSR
      case symbol_kind::S_NT_DirectiveWithList: // NT_DirectiveWithList
      case symbol_kind::S_NT_DirectiveSingleArgument: // NT_DirectiveSingleArgument
      case symbol_kind::S_NT_DirectiveWithSymbolList: // NT_DirectiveWithSymbolList
        value.copy< string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NT_DataInstructionOperands: // NT_DataInstructionOperands
      case symbol_kind::S_NT_SymbolList: // NT_SymbolList
      case symbol_kind::S_NT_LiteralList: // NT_LiteralList
        value.copy< vector<ParserOperand> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_NT_ConditionalJumpOperands: // NT_ConditionalJumpOperands
        value.move< ConditionalJumpOperands > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NT_JumpOperand: // NT_JumpOperand
      case symbol_kind::S_NT_Operand: // NT_Operand
        value.move< ParserOperand > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NT_DirectiveIdentifier: // NT_DirectiveIdentifier
      case symbol_kind::S_NT_ConditionalJumpIdentifier: // NT_ConditionalJumpIdentifier
      case symbol_kind::S_NT_UnconditionalJumpIdentifier: // NT_UnconditionalJumpIdentifier
      case symbol_kind::S_NT_DataInstructionIdentifier: // NT_DataInstructionIdentifier
      case symbol_kind::S_NT_StackInstructionIdentifier: // NT_StackInstructionIdentifier
        value.move< eInstructionIdentifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_LABEL: // LABEL
      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_LITERAL: // LITERAL
      case symbol_kind::S_GPR: // GPR
      case symbol_kind::S_CSR: // CSR
      case symbol_kind::S_NT_DirectiveWithList: // NT_DirectiveWithList
      case symbol_kind::S_NT_DirectiveSingleArgument: // NT_DirectiveSingleArgument
      case symbol_kind::S_NT_DirectiveWithSymbolList: // NT_DirectiveWithSymbolList
        value.move< string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NT_DataInstructionOperands: // NT_DataInstructionOperands
      case symbol_kind::S_NT_SymbolList: // NT_SymbolList
      case symbol_kind::S_NT_LiteralList: // NT_LiteralList
        value.move< vector<ParserOperand> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  inline
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  inline
  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  inline
  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  inline
  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  inline
  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


} // yy
#line 2350 "./misc/parser/parser.hpp"




#endif // !YY_YY_MISC_PARSER_PARSER_HPP_INCLUDED
