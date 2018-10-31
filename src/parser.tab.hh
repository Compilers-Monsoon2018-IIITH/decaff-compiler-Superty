/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_TAB_HH_INCLUDED
# define YY_YY_PARSER_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    INT_LITERAL = 259,
    STRING_LITERAL = 260,
    BOOL_LITERAL = 261,
    CHAR_LITERAL = 262,
    CALLOUT = 263,
    COMMA = 264,
    SEMICOLON = 265,
    OPEN_PAREN = 266,
    CLOSE_PAREN = 267,
    OPEN_BRACKET = 268,
    CLOSE_BRACKET = 269,
    OPEN_BRACE = 270,
    CLOSE_BRACE = 271,
    ASSIGN = 272,
    PLUS_ASSIGN = 273,
    MINUS_ASSIGN = 274,
    IF = 275,
    ELSE = 276,
    FOR = 277,
    RETURN = 278,
    BREAK = 279,
    CONTINUE = 280,
    INT_TYPE = 281,
    BOOLEAN_TYPE = 282,
    VOID_TYPE = 283,
    CLASS = 284,
    OR = 285,
    AND = 286,
    EQ = 287,
    NE = 288,
    LT = 289,
    LE = 290,
    GE = 291,
    GT = 292,
    PLUS = 293,
    MINUS = 294,
    MULT = 295,
    DIV = 296,
    MOD = 297,
    NOT = 298
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_HH_INCLUDED  */
