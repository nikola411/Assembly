// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.



// First part of user prologue.
#line 4 "./misc/parser.yy"

   

#line 44 "./misc/parser/parser.cpp"


#include "parser.hpp"


// Unqualified %code blocks.
#line 44 "./misc/parser.yy"

    #include "driver.hpp"
    #include <iostream>
    #include <string>
    #include <vector>
    #include <cmath>
    #include "directive.hpp"

#line 60 "./misc/parser/parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 151 "./misc/parser/parser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (Driver& drv_yyarg, Assembly& assembly_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      drv (drv_yyarg),
      assembly (assembly_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 58: // directive
      case 67: // label_list
        value.YY_MOVE_OR_COPY< Directive* > (YY_MOVE (that.value));
        break;

      case 59: // instruction
      case 60: // load_store_instructions
      case 61: // two_operand_instructions
      case 62: // one_operand_instructions
      case 63: // zero_operand_instructions
      case 70: // operand
        value.YY_MOVE_OR_COPY< Instruction* > (YY_MOVE (that.value));
        break;

      case 68: // load_store
      case 69: // instruction_type
        value.YY_MOVE_OR_COPY< Instruction_type > (YY_MOVE (that.value));
        break;

      case 64: // jump_operand
      case 65: // jump
        value.YY_MOVE_OR_COPY< Jump* > (YY_MOVE (that.value));
        break;

      case 25: // LITERAL
      case 26: // SYMBOL
      case 27: // REGISTER
      case 54: // COMMENT
        value.YY_MOVE_OR_COPY< string > (YY_MOVE (that.value));
        break;

      case 66: // symbol_list
        value.YY_MOVE_OR_COPY< vector<string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 58: // directive
      case 67: // label_list
        value.move< Directive* > (YY_MOVE (that.value));
        break;

      case 59: // instruction
      case 60: // load_store_instructions
      case 61: // two_operand_instructions
      case 62: // one_operand_instructions
      case 63: // zero_operand_instructions
      case 70: // operand
        value.move< Instruction* > (YY_MOVE (that.value));
        break;

      case 68: // load_store
      case 69: // instruction_type
        value.move< Instruction_type > (YY_MOVE (that.value));
        break;

      case 64: // jump_operand
      case 65: // jump
        value.move< Jump* > (YY_MOVE (that.value));
        break;

      case 25: // LITERAL
      case 26: // SYMBOL
      case 27: // REGISTER
      case 54: // COMMENT
        value.move< string > (YY_MOVE (that.value));
        break;

      case 66: // symbol_list
        value.move< vector<string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 58: // directive
      case 67: // label_list
        value.copy< Directive* > (that.value);
        break;

      case 59: // instruction
      case 60: // load_store_instructions
      case 61: // two_operand_instructions
      case 62: // one_operand_instructions
      case 63: // zero_operand_instructions
      case 70: // operand
        value.copy< Instruction* > (that.value);
        break;

      case 68: // load_store
      case 69: // instruction_type
        value.copy< Instruction_type > (that.value);
        break;

      case 64: // jump_operand
      case 65: // jump
        value.copy< Jump* > (that.value);
        break;

      case 25: // LITERAL
      case 26: // SYMBOL
      case 27: // REGISTER
      case 54: // COMMENT
        value.copy< string > (that.value);
        break;

      case 66: // symbol_list
        value.copy< vector<string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 58: // directive
      case 67: // label_list
        value.move< Directive* > (that.value);
        break;

      case 59: // instruction
      case 60: // load_store_instructions
      case 61: // two_operand_instructions
      case 62: // one_operand_instructions
      case 63: // zero_operand_instructions
      case 70: // operand
        value.move< Instruction* > (that.value);
        break;

      case 68: // load_store
      case 69: // instruction_type
        value.move< Instruction_type > (that.value);
        break;

      case 64: // jump_operand
      case 65: // jump
        value.move< Jump* > (that.value);
        break;

      case 25: // LITERAL
      case 26: // SYMBOL
      case 27: // REGISTER
      case 54: // COMMENT
        value.move< string > (that.value);
        break;

      case 66: // symbol_list
        value.move< vector<string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    /// Discard the LAC context in case there still is one left from a
    /// previous invocation.
    yy_lac_discard_ ("init");

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv, assembly));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        if (!yy_lac_establish_ (yyla.type_get ()))
           goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.type_get ()))
           goto yyerrlab;

        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    yy_lac_discard_ ("shift");
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 58: // directive
      case 67: // label_list
        yylhs.value.emplace< Directive* > ();
        break;

      case 59: // instruction
      case 60: // load_store_instructions
      case 61: // two_operand_instructions
      case 62: // one_operand_instructions
      case 63: // zero_operand_instructions
      case 70: // operand
        yylhs.value.emplace< Instruction* > ();
        break;

      case 68: // load_store
      case 69: // instruction_type
        yylhs.value.emplace< Instruction_type > ();
        break;

      case 64: // jump_operand
      case 65: // jump
        yylhs.value.emplace< Jump* > ();
        break;

      case 25: // LITERAL
      case 26: // SYMBOL
      case 27: // REGISTER
      case 54: // COMMENT
        yylhs.value.emplace< string > ();
        break;

      case 66: // symbol_list
        yylhs.value.emplace< vector<string> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 4:
#line 107 "./misc/parser.yy"
           { assembly.finish_parsing(); }
#line 765 "./misc/parser/parser.cpp"
    break;

  case 9:
#line 119 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = new Directive();
        yylhs.value.as < Directive* > ()->set_type(Directive_type::GLOBAL);
        yylhs.value.as < Directive* > ()->set_operands(yystack_[0].value.as < vector<string> > ());
        assembly.add_new_line(yylhs.value.as < Directive* > ());
    }
#line 776 "./misc/parser/parser.cpp"
    break;

  case 10:
#line 126 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = new Directive();
        yylhs.value.as < Directive* > ()->set_type(Directive_type::EXTERN);
        yylhs.value.as < Directive* > ()->set_operands(yystack_[0].value.as < vector<string> > ());
        assembly.add_new_line(yylhs.value.as < Directive* > ());
    }
#line 787 "./misc/parser/parser.cpp"
    break;

  case 11:
#line 133 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = new Directive();
        yylhs.value.as < Directive* > ()->set_type(Directive_type::SECTION);
        yylhs.value.as < Directive* > ()->add_operand(yystack_[0].value.as < string > ());
        assembly.add_new_line(yylhs.value.as < Directive* > ());
    }
#line 798 "./misc/parser/parser.cpp"
    break;

  case 12:
#line 140 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = yystack_[0].value.as < Directive* > ();
        yylhs.value.as < Directive* > ()->set_type(Directive_type::WORD);
        assembly.add_new_line(yylhs.value.as < Directive* > ());
    }
#line 808 "./misc/parser/parser.cpp"
    break;

  case 13:
#line 146 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = new Directive();
        yylhs.value.as < Directive* > ()->set_type(Directive_type::SKIP);
        yylhs.value.as < Directive* > ()->add_operand(yystack_[0].value.as < string > ());
        assembly.add_new_line(yylhs.value.as < Directive* > ());
    }
#line 819 "./misc/parser/parser.cpp"
    break;

  case 14:
#line 153 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = new Directive();
        yylhs.value.as < Directive* > ()->set_type(Directive_type::END);
        assembly.add_new_line(yylhs.value.as < Directive* > ());
        YYACCEPT;
    }
#line 830 "./misc/parser/parser.cpp"
    break;

  case 15:
#line 160 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = new Directive();
        yylhs.value.as < Directive* > () -> set_type(Directive_type::LABEL);
        yylhs.value.as < Directive* > () -> add_operand(yystack_[1].value.as < string > ());
        assembly.add_new_line(yylhs.value.as < Directive* > ());
    }
#line 841 "./misc/parser/parser.cpp"
    break;

  case 16:
#line 167 "./misc/parser.yy"
    {

    }
#line 849 "./misc/parser/parser.cpp"
    break;

  case 17:
#line 171 "./misc/parser.yy"
    {
        
    }
#line 857 "./misc/parser/parser.cpp"
    break;

  case 18:
#line 177 "./misc/parser.yy"
                             { yylhs.value.as < vector<string> > () = yystack_[2].value.as < vector<string> > (); yylhs.value.as < vector<string> > ().emplace_back(yystack_[0].value.as < string > ()); }
#line 863 "./misc/parser/parser.cpp"
    break;

  case 19:
#line 178 "./misc/parser.yy"
             { yylhs.value.as < vector<string> > ().emplace_back(yystack_[0].value.as < string > ()); }
#line 869 "./misc/parser/parser.cpp"
    break;

  case 20:
#line 183 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = yystack_[2].value.as < Directive* > ();
        yylhs.value.as < Directive* > () -> add_operand(yystack_[0].value.as < string > (), Label_type::SYMBOL);
    }
#line 878 "./misc/parser/parser.cpp"
    break;

  case 21:
#line 188 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = yystack_[2].value.as < Directive* > ();
        yylhs.value.as < Directive* > () -> add_operand(yystack_[0].value.as < string > (), Label_type::LITERAL);
    }
#line 887 "./misc/parser/parser.cpp"
    break;

  case 22:
#line 193 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = new Directive();
        yylhs.value.as < Directive* > () -> add_operand(yystack_[0].value.as < string > (), Label_type::LITERAL);
    }
#line 896 "./misc/parser/parser.cpp"
    break;

  case 23:
#line 198 "./misc/parser.yy"
    {
        yylhs.value.as < Directive* > () = new Directive(); 
        yylhs.value.as < Directive* > () -> add_operand(yystack_[0].value.as < string > (), Label_type::SYMBOL);
    }
#line 905 "./misc/parser/parser.cpp"
    break;

  case 24:
#line 206 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = yystack_[0].value.as < Jump* > (); yylhs.value.as < Jump* > () -> set_jump_type(Jump_type::JMP);
        assembly.add_new_line(yylhs.value.as < Jump* > ());
    }
#line 914 "./misc/parser/parser.cpp"
    break;

  case 25:
#line 211 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = yystack_[0].value.as < Jump* > (); yylhs.value.as < Jump* > () -> set_jump_type(Jump_type::JEQ);
        assembly.add_new_line(yylhs.value.as < Jump* > ());
    }
#line 923 "./misc/parser/parser.cpp"
    break;

  case 26:
#line 216 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = yystack_[0].value.as < Jump* > (); yylhs.value.as < Jump* > () -> set_jump_type(Jump_type::JNE);
        assembly.add_new_line(yylhs.value.as < Jump* > ());
    }
#line 932 "./misc/parser/parser.cpp"
    break;

  case 27:
#line 221 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = yystack_[0].value.as < Jump* > (); yylhs.value.as < Jump* > () -> set_jump_type(Jump_type::JGT);
        assembly.add_new_line(yylhs.value.as < Jump* > ());
    }
#line 941 "./misc/parser/parser.cpp"
    break;

  case 28:
#line 229 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = new Jump();
        yylhs.value.as < Jump* > () -> set_operand (yystack_[0].value.as < string > (), Addressing_type::ABSOLUTE, Label_type::LITERAL);
    }
#line 950 "./misc/parser/parser.cpp"
    break;

  case 29:
#line 234 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = new Jump();
        yylhs.value.as < Jump* > () -> set_operand (yystack_[0].value.as < string > (), Addressing_type::ABSOLUTE, Label_type::SYMBOL);
    }
#line 959 "./misc/parser/parser.cpp"
    break;

  case 30:
#line 239 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = new Jump();
        yylhs.value.as < Jump* > () -> set_operand (yystack_[0].value.as < string > (), Addressing_type::PC_RELATIVE, Label_type::SYMBOL);
    }
#line 968 "./misc/parser/parser.cpp"
    break;

  case 31:
#line 244 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = new Jump();
        yylhs.value.as < Jump* > () -> set_operand (yystack_[0].value.as < string > (), Addressing_type::MEMORY, Label_type::LITERAL);
    }
#line 977 "./misc/parser/parser.cpp"
    break;

  case 32:
#line 249 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = new Jump();
        yylhs.value.as < Jump* > () -> set_operand (yystack_[0].value.as < string > (), Addressing_type::MEMORY, Label_type::SYMBOL);
    }
#line 986 "./misc/parser/parser.cpp"
    break;

  case 33:
#line 254 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = new Jump();
        yylhs.value.as < Jump* > () -> set_operand (yystack_[0].value.as < string > (), Addressing_type::ABSOLUTE, Label_type::REGISTER);
    }
#line 995 "./misc/parser/parser.cpp"
    break;

  case 34:
#line 259 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = new Jump();
        yylhs.value.as < Jump* > () -> set_operand (yystack_[1].value.as < string > (), Addressing_type::MEMORY, Label_type::REGISTER);
    }
#line 1004 "./misc/parser/parser.cpp"
    break;

  case 35:
#line 264 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = new Jump();
        yylhs.value.as < Jump* > () -> set_operand (yystack_[3].value.as < string > (), Addressing_type::LITERAL_OFFSET, Label_type::REGISTER);
        yylhs.value.as < Jump* > () -> set_offset(yystack_[1].value.as < string > ());
        yylhs.value.as < Jump* > () -> set_offset_type(Label_type::LITERAL);
        yylhs.value.as < Jump* > () -> set_offset (yystack_[1].value.as < string > ());
    }
#line 1016 "./misc/parser/parser.cpp"
    break;

  case 36:
#line 272 "./misc/parser.yy"
    {
        yylhs.value.as < Jump* > () = new Jump();
        yylhs.value.as < Jump* > () -> set_operand (yystack_[3].value.as < string > (), Addressing_type::SYMBOL_OFFSET, Label_type::REGISTER);
        yylhs.value.as < Jump* > () -> set_offset(yystack_[1].value.as < string > ());
        yylhs.value.as < Jump* > () -> set_offset_type(Label_type::SYMBOL);
        yylhs.value.as < Jump* > () -> set_offset (yystack_[1].value.as < string > ());
    }
#line 1028 "./misc/parser/parser.cpp"
    break;

  case 37:
#line 282 "./misc/parser.yy"
                             { yylhs.value.as < Instruction* > () = yystack_[0].value.as < Instruction* > (); }
#line 1034 "./misc/parser/parser.cpp"
    break;

  case 38:
#line 283 "./misc/parser.yy"
                               { yylhs.value.as < Instruction* > () = yystack_[0].value.as < Instruction* > (); }
#line 1040 "./misc/parser/parser.cpp"
    break;

  case 39:
#line 284 "./misc/parser.yy"
                                { yylhs.value.as < Instruction* > () = yystack_[0].value.as < Instruction* > (); }
#line 1046 "./misc/parser/parser.cpp"
    break;

  case 40:
#line 288 "./misc/parser.yy"
                            { yylhs.value.as < Instruction* > () = yystack_[0].value.as < Instruction* > (); }
#line 1052 "./misc/parser/parser.cpp"
    break;

  case 41:
#line 290 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction(yystack_[3].value.as < Instruction_type > (), yystack_[2].value.as < string > (), yystack_[0].value.as < string > ());
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1061 "./misc/parser/parser.cpp"
    break;

  case 42:
#line 299 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = yystack_[0].value.as < Instruction* > ();
        yylhs.value.as < Instruction* > () -> set_instruction_type(yystack_[3].value.as < Instruction_type > ());
        yylhs.value.as < Instruction* > () -> set_first_operand(yystack_[2].value.as < string > ());
        yylhs.value.as < Instruction* > () -> set_number_of_operands(2);
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1073 "./misc/parser/parser.cpp"
    break;

  case 43:
#line 310 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction();
        yylhs.value.as < Instruction* > () -> set_second_operand(yystack_[0].value.as < string > (), Label_type::LITERAL);
        yylhs.value.as < Instruction* > () -> set_addressing_type(Addressing_type::ABSOLUTE);
    }
#line 1083 "./misc/parser/parser.cpp"
    break;

  case 44:
#line 316 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction();
        yylhs.value.as < Instruction* > () -> set_second_operand(yystack_[0].value.as < string > (), Label_type::SYMBOL);
        yylhs.value.as < Instruction* > () -> set_addressing_type(Addressing_type::ABSOLUTE);
    }
#line 1093 "./misc/parser/parser.cpp"
    break;

  case 45:
#line 322 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction();
        yylhs.value.as < Instruction* > () -> set_second_operand(yystack_[0].value.as < string > (), Label_type::SYMBOL);
        yylhs.value.as < Instruction* > () -> set_addressing_type(Addressing_type::PC_RELATIVE);
    }
#line 1103 "./misc/parser/parser.cpp"
    break;

  case 46:
#line 328 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction();
        yylhs.value.as < Instruction* > () -> set_second_operand(yystack_[0].value.as < string > (), Label_type::LITERAL);
        yylhs.value.as < Instruction* > () -> set_addressing_type(Addressing_type::MEMORY);
    }
#line 1113 "./misc/parser/parser.cpp"
    break;

  case 47:
#line 334 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction();
        yylhs.value.as < Instruction* > () -> set_second_operand(yystack_[0].value.as < string > (), Label_type::SYMBOL);
        yylhs.value.as < Instruction* > () -> set_addressing_type(Addressing_type::MEMORY);
    }
#line 1123 "./misc/parser/parser.cpp"
    break;

  case 48:
#line 340 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction();
        yylhs.value.as < Instruction* > () -> set_second_operand(yystack_[0].value.as < string > (), Label_type::REGISTER);
        yylhs.value.as < Instruction* > () -> set_addressing_type(Addressing_type::ABSOLUTE);
    }
#line 1133 "./misc/parser/parser.cpp"
    break;

  case 49:
#line 346 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction();
        yylhs.value.as < Instruction* > () -> set_second_operand(yystack_[1].value.as < string > (), Label_type::REGISTER);
        yylhs.value.as < Instruction* > () -> set_addressing_type(Addressing_type::MEMORY);
    }
#line 1143 "./misc/parser/parser.cpp"
    break;

  case 50:
#line 352 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction();
        yylhs.value.as < Instruction* > () -> set_second_operand(yystack_[3].value.as < string > (), Label_type::REGISTER);
        yylhs.value.as < Instruction* > () -> set_addressing_type(Addressing_type::LITERAL_OFFSET);
        yylhs.value.as < Instruction* > () -> set_offset(yystack_[1].value.as < string > (), Label_type::LITERAL);
    }
#line 1154 "./misc/parser/parser.cpp"
    break;

  case 51:
#line 359 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction();
        yylhs.value.as < Instruction* > () -> set_second_operand(yystack_[3].value.as < string > (), Label_type::REGISTER);
        yylhs.value.as < Instruction* > () -> set_addressing_type(Addressing_type::SYMBOL_OFFSET);
        yylhs.value.as < Instruction* > () -> set_offset(yystack_[1].value.as < string > (), Label_type::SYMBOL);
    }
#line 1165 "./misc/parser/parser.cpp"
    break;

  case 52:
#line 368 "./misc/parser.yy"
        { yylhs.value.as < Instruction_type > () = Instruction_type::LDR; }
#line 1171 "./misc/parser/parser.cpp"
    break;

  case 53:
#line 369 "./misc/parser.yy"
          { yylhs.value.as < Instruction_type > () = Instruction_type::STR; }
#line 1177 "./misc/parser/parser.cpp"
    break;

  case 54:
#line 373 "./misc/parser.yy"
        { yylhs.value.as < Instruction_type > () = Instruction_type::SHL; }
#line 1183 "./misc/parser/parser.cpp"
    break;

  case 55:
#line 374 "./misc/parser.yy"
          { yylhs.value.as < Instruction_type > () = Instruction_type::SHR; }
#line 1189 "./misc/parser/parser.cpp"
    break;

  case 56:
#line 375 "./misc/parser.yy"
           { yylhs.value.as < Instruction_type > () = Instruction_type::TEST; }
#line 1195 "./misc/parser/parser.cpp"
    break;

  case 57:
#line 376 "./misc/parser.yy"
          { yylhs.value.as < Instruction_type > () = Instruction_type::XOR; }
#line 1201 "./misc/parser/parser.cpp"
    break;

  case 58:
#line 377 "./misc/parser.yy"
         { yylhs.value.as < Instruction_type > () = Instruction_type::OR; }
#line 1207 "./misc/parser/parser.cpp"
    break;

  case 59:
#line 378 "./misc/parser.yy"
          { yylhs.value.as < Instruction_type > () = Instruction_type::AND; }
#line 1213 "./misc/parser/parser.cpp"
    break;

  case 60:
#line 379 "./misc/parser.yy"
          { yylhs.value.as < Instruction_type > () = Instruction_type::CMP; }
#line 1219 "./misc/parser/parser.cpp"
    break;

  case 61:
#line 380 "./misc/parser.yy"
          { yylhs.value.as < Instruction_type > () = Instruction_type::DIV; }
#line 1225 "./misc/parser/parser.cpp"
    break;

  case 62:
#line 381 "./misc/parser.yy"
          { yylhs.value.as < Instruction_type > () = Instruction_type::MUL; }
#line 1231 "./misc/parser/parser.cpp"
    break;

  case 63:
#line 382 "./misc/parser.yy"
          { yylhs.value.as < Instruction_type > () = Instruction_type::SUB; }
#line 1237 "./misc/parser/parser.cpp"
    break;

  case 64:
#line 383 "./misc/parser.yy"
          { yylhs.value.as < Instruction_type > () = Instruction_type::ADD; }
#line 1243 "./misc/parser/parser.cpp"
    break;

  case 65:
#line 384 "./misc/parser.yy"
           { yylhs.value.as < Instruction_type > () = Instruction_type::XCHG; }
#line 1249 "./misc/parser/parser.cpp"
    break;

  case 66:
#line 389 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction(Instruction_type::POP, yystack_[0].value.as < string > ()); 
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1258 "./misc/parser/parser.cpp"
    break;

  case 67:
#line 394 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction(Instruction_type::PUSH, yystack_[0].value.as < string > ()); 
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1267 "./misc/parser/parser.cpp"
    break;

  case 68:
#line 399 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction(Instruction_type::NOT, yystack_[0].value.as < string > ()); 
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1276 "./misc/parser/parser.cpp"
    break;

  case 69:
#line 404 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction(Instruction_type::INT, yystack_[0].value.as < string > ()); 
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1285 "./misc/parser/parser.cpp"
    break;

  case 70:
#line 409 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = yystack_[0].value.as < Instruction* > ();
        yylhs.value.as < Instruction* > () -> set_instruction_type(Instruction_type::CALL);
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1295 "./misc/parser/parser.cpp"
    break;

  case 71:
#line 418 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction(Instruction_type::RET);
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1304 "./misc/parser/parser.cpp"
    break;

  case 72:
#line 423 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction(Instruction_type::IRET);
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1313 "./misc/parser/parser.cpp"
    break;

  case 73:
#line 428 "./misc/parser.yy"
    {
        yylhs.value.as < Instruction* > () = new Instruction(Instruction_type::HALT);
        assembly.add_new_line(yylhs.value.as < Instruction* > ());
    }
#line 1322 "./misc/parser/parser.cpp"
    break;


#line 1326 "./misc/parser/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      yy_lac_discard_ ("error recovery");
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  bool
  parser::yy_lac_check_ (int yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if YYDEBUG
    YYCDEBUG << "LAC: checking lookahead " << yytname_[yytoken] << ':';
#endif
    std::ptrdiff_t lac_top = 0;
    while (true)
      {
        state_type top_state = (yylac_stack_.empty ()
                                ? yystack_[lac_top].state
                                : yylac_stack_.back ());
        int yyrule = yypact_[+top_state];
        if (yy_pact_value_is_default_ (yyrule)
            || (yyrule += yytoken) < 0 || yylast_ < yyrule
            || yycheck_[yyrule] != yytoken)
          {
            // Use the default action.
            yyrule = yydefact_[+top_state];
            if (yyrule == 0)
              {
                YYCDEBUG << " Err\n";
                return false;
              }
          }
        else
          {
            // Use the action from yytable.
            yyrule = yytable_[yyrule];
            if (yy_table_value_is_error_ (yyrule))
              {
                YYCDEBUG << " Err\n";
                return false;
              }
            if (0 < yyrule)
              {
                YYCDEBUG << " S" << yyrule << '\n';
                return true;
              }
            yyrule = -yyrule;
          }
        // By now we know we have to simulate a reduce.
        YYCDEBUG << " R" << yyrule - 1;
        // Pop the corresponding number of values from the stack.
        {
          std::ptrdiff_t yylen = yyr2_[yyrule];
          // First pop from the LAC stack as many tokens as possible.
          std::ptrdiff_t lac_size = std::ptrdiff_t (yylac_stack_.size ());
          if (yylen < lac_size)
            {
              yylac_stack_.resize (std::size_t (lac_size - yylen));
              yylen = 0;
            }
          else if (lac_size)
            {
              yylac_stack_.clear ();
              yylen -= lac_size;
            }
          // Only afterwards look at the main stack.
          // We simulate popping elements by incrementing lac_top.
          lac_top += yylen;
        }
        // Keep top_state in sync with the updated stack.
        top_state = (yylac_stack_.empty ()
                     ? yystack_[lac_top].state
                     : yylac_stack_.back ());
        // Push the resulting state of the reduction.
        state_type state = yy_lr_goto_state_ (top_state, yyr1_[yyrule]);
        YYCDEBUG << " G" << state;
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  parser::yy_lac_establish_ (int yytoken)
  {
    /* Establish the initial context for the current lookahead if no initial
       context is currently established.

       We define a context as a snapshot of the parser stacks.  We define
       the initial context for a lookahead as the context in which the
       parser initially examines that lookahead in order to select a
       syntactic action.  Thus, if the lookahead eventually proves
       syntactically unacceptable (possibly in a later context reached via a
       series of reductions), the initial context can be used to determine
       the exact set of tokens that would be syntactically acceptable in the
       lookahead's place.  Moreover, it is the context after which any
       further semantic actions would be erroneous because they would be
       determined by a syntactically unacceptable token.

       yy_lac_establish_ should be invoked when a reduction is about to be
       performed in an inconsistent state (which, for the purposes of LAC,
       includes consistent states that don't know they're consistent because
       their default reductions have been disabled).

       For parse.lac=full, the implementation of yy_lac_establish_ is as
       follows.  If no initial context is currently established for the
       current lookahead, then check if that lookahead can eventually be
       shifted if syntactic actions continue from the current context.  */
    if (!yy_lac_established_)
      {
#if YYDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << yytname_[yytoken] << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
    return true;
  }

  // Discard any previous initial lookahead context.
  void
  parser::yy_lac_discard_ (const char* evt)
  {
   /* Discard any previous initial lookahead context because of Event,
      which may be a lookahead change or an invalidation of the currently
      established initial context for the current lookahead.

      The most common example of a lookahead change is a shift.  An example
      of both cases is syntax error recovery.  That is, a syntax error
      occurs when the lookahead is syntactically erroneous for the
      currently established initial context, so error recovery manipulates
      the parser stacks to try to find a new initial context in which the
      current lookahead is syntactically acceptable.  If it fails to find
      such a context, it discards the lookahead.  */
    if (yy_lac_established_)
      {
        YYCDEBUG << "LAC: initial context discarded due to "
                 << evt << '\n';
        yy_lac_established_ = false;
      }
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
         In the first two cases, it might appear that the current syntax
         error should have been detected in the previous state when
         yy_lac_check was invoked.  However, at that time, there might
         have been a different syntax error that discarded a different
         initial context during error recovery, leaving behind the
         current lookahead.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

#if YYDEBUG
        // Execute LAC once. We don't care if it is succesful, we
        // only do it for the sake of debugging output.
        if (!yy_lac_established_)
          yy_lac_check_ (yytoken);
#endif

        int yyn = yypact_[+yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            for (int yyx = 0; yyx < yyntokens_; ++yyx)
              if (yyx != yy_error_token_ && yyx != yy_undef_token_
                  && yy_lac_check_ (yyx))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -19;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
       0,   -19,   -10,   -10,    -1,   -18,     2,   -19,    36,   -19,
      48,   -19,    37,   -19,   -19,   100,   104,   104,   104,   104,
      38,    39,   -19,   -19,   -19,   -19,   -19,   -19,    40,   -19,
     -19,   -19,   -19,   -19,   -19,   -19,   -19,   -19,    55,   -19,
     -19,   -19,   -19,   -19,   -19,   -19,   -19,    41,    42,   -19,
      60,    60,   -19,   -19,   -19,    61,   -19,    70,   -19,   -19,
     -16,    56,    87,   -19,   -19,   -19,   -19,   -12,    89,   -19,
     -19,   -19,   -19,   -19,   -19,   -19,   -19,   -19,   -19,   -19,
     107,   108,    93,    31,    95,   -19,   -19,   -19,    -3,    94,
     -19,   -19,   -19,   -19,   100,    96,   -19,   -19,   -19,   -19,
      33,   -19,    -2,   -19,   -19,   110,   114,    35,   -19,   -19,
     -19,   117,   118,   -19,   -19
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     4,     0,     0,     0,     0,     0,    17,     0,    14,
       0,    73,     0,    72,    71,     0,     0,     0,     0,     0,
       0,     0,    65,    64,    63,    61,    62,    60,     0,    59,
      58,    57,    56,    54,    55,    52,    53,     8,     0,     3,
       5,     6,    40,    37,    38,    39,     7,     0,     0,    19,
       9,    10,    11,    22,    23,    12,    13,     0,    15,    69,
       0,     0,     0,    46,    47,    48,    70,     0,     0,    28,
      29,    24,    25,    26,    27,    67,    66,    68,     1,     2,
       0,     0,     0,     0,     0,    43,    44,    45,     0,     0,
      31,    32,    33,    30,     0,     0,    18,    21,    20,    16,
       0,    49,     0,    42,    41,     0,     0,     0,    34,    50,
      51,     0,     0,    35,    36
  };

  const short
  parser::yypgoto_[] =
  {
     -19,   -19,    84,   -19,   -19,   -19,   -19,   -19,   -19,   -13,
     -19,   130,   -19,   -19,   -19,    43
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    71,
      46,    50,    55,    47,    48,    66
  };

  const signed char
  parser::yytable_[] =
  {
       1,    89,   100,   107,    72,    73,    74,    53,    54,    85,
      86,   101,   108,    90,    91,    92,    49,     2,     3,     4,
       5,     6,     7,     8,     9,    52,    10,    56,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    78,    97,    98,   105,   106,
     111,   112,    57,    58,    59,    75,    76,    77,    80,    81,
      82,    83,     2,     3,     4,     5,     6,     7,     8,     9,
      84,    10,    87,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      67,    60,    61,    62,    88,    93,    68,    94,    95,    96,
      99,   102,    79,   104,   109,    63,    64,    65,   110,    69,
      70,   113,   114,    51,     0,     0,     0,   103
  };

  const signed char
  parser::yycheck_[] =
  {
       0,    13,     5,     5,    17,    18,    19,    25,    26,    25,
      26,    14,    14,    25,    26,    27,    26,    17,    18,    19,
      20,    21,    22,    23,    24,    26,    26,    25,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,    25,    26,    25,    26,
      25,    26,    26,    15,    27,    27,    27,    27,    27,    27,
      10,    10,    17,    18,    19,    20,    21,    22,    23,    24,
      10,    26,    26,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
       6,    11,    12,    13,    27,    26,    12,    10,    10,    26,
      25,    27,    38,    27,    14,    25,    26,    27,    14,    25,
      26,    14,    14,     3,    -1,    -1,    -1,    94
  };

  const signed char
  parser::yystos_[] =
  {
       0,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      26,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    56,    57,
      58,    59,    60,    61,    62,    63,    65,    68,    69,    26,
      66,    66,    26,    25,    26,    67,    25,    26,    15,    27,
      11,    12,    13,    25,    26,    27,    70,     6,    12,    25,
      26,    64,    64,    64,    64,    27,    27,    27,     0,    57,
      27,    27,    10,    10,    10,    25,    26,    26,    27,    13,
      25,    26,    27,    26,    10,    10,    26,    25,    26,    25,
       5,    14,    27,    70,    27,    25,    26,     5,    14,    14,
      14,    25,    26,    14,    14
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    55,    56,    56,    56,    57,    57,    57,    57,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    66,    66,
      67,    67,    67,    67,    65,    65,    65,    65,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    59,    59,    59,
      61,    61,    60,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    68,    68,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    62,    62,    62,    62,
      62,    63,    63,    63
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     1,     2,     4,     1,     3,     1,
       3,     3,     1,     1,     2,     2,     2,     2,     1,     1,
       2,     2,     2,     2,     4,     6,     6,     1,     1,     1,
       1,     4,     4,     2,     2,     2,     1,     1,     1,     3,
       5,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\":=\"", "\"-\"", "\"+\"",
  "\"*\"", "\"/\"", "\"(\"", "\")\"", "\",\"", "\"$\"", "\"%\"", "\"[\"",
  "\"]\"", "\":\"", "\"#\"", "GLOBAL", "EXTERN", "SECTION", "WORD", "SKIP",
  "ASCII", "EQU", "END", "LITERAL", "SYMBOL", "REGISTER", "HALT", "INT",
  "IRET", "RET", "CALL", "JMP", "JEQ", "JNE", "JGT", "PUSH", "POP", "XCHG",
  "ADD", "SUB", "DIV", "MUL", "CMP", "NOT", "AND", "OR", "XOR", "TEST",
  "SHL", "SHR", "LDR", "STR", "COMMENT", "$accept", "program", "line",
  "directive", "instruction", "load_store_instructions",
  "two_operand_instructions", "one_operand_instructions",
  "zero_operand_instructions", "jump_operand", "jump", "symbol_list",
  "label_list", "load_store", "instruction_type", "operand", YY_NULLPTR
  };

#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   105,   105,   106,   107,   111,   112,   113,   114,   118,
     125,   132,   139,   145,   152,   159,   166,   170,   177,   178,
     182,   187,   192,   197,   205,   210,   215,   220,   228,   233,
     238,   243,   248,   253,   258,   263,   271,   282,   283,   284,
     288,   289,   298,   309,   315,   321,   327,   333,   339,   345,
     351,   358,   368,   369,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   388,   393,   398,   403,
     408,   417,   422,   427
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1933 "./misc/parser/parser.cpp"

#line 434 "./misc/parser.yy"


void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
