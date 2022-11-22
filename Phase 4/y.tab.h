/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    CLASS = 258,
    OBRACE = 259,
    SEMI = 260,
    CBRACE = 261,
    STATIC = 262,
    BOOL = 263,
    BOOLLIT = 264,
    DO = 265,
    DOTLENGTH = 266,
    DOUBLE = 267,
    ELSE = 268,
    IF = 269,
    INT = 270,
    PARSEINT = 271,
    PRINT = 272,
    PUBLIC = 273,
    RETURN = 274,
    STRING = 275,
    VOID = 276,
    WHILE = 277,
    OCURV = 278,
    CCURV = 279,
    AND = 280,
    OR = 281,
    LT = 282,
    GT = 283,
    ID = 284,
    EQ = 285,
    NEQ = 286,
    LEQ = 287,
    GEQ = 288,
    PLUS = 289,
    MINUS = 290,
    STAR = 291,
    DIV = 292,
    MOD = 293,
    NOT = 294,
    ASSIGN = 295,
    COMMA = 296,
    RESERVED = 297,
    OSQUARE = 298,
    CSQUARE = 299,
    DECLIT = 300,
    REALLIT = 301,
    STRLIT = 302,
    IFX = 303
  };
#endif
/* Tokens.  */
#define CLASS 258
#define OBRACE 259
#define SEMI 260
#define CBRACE 261
#define STATIC 262
#define BOOL 263
#define BOOLLIT 264
#define DO 265
#define DOTLENGTH 266
#define DOUBLE 267
#define ELSE 268
#define IF 269
#define INT 270
#define PARSEINT 271
#define PRINT 272
#define PUBLIC 273
#define RETURN 274
#define STRING 275
#define VOID 276
#define WHILE 277
#define OCURV 278
#define CCURV 279
#define AND 280
#define OR 281
#define LT 282
#define GT 283
#define ID 284
#define EQ 285
#define NEQ 286
#define LEQ 287
#define GEQ 288
#define PLUS 289
#define MINUS 290
#define STAR 291
#define DIV 292
#define MOD 293
#define NOT 294
#define ASSIGN 295
#define COMMA 296
#define RESERVED 297
#define OSQUARE 298
#define CSQUARE 299
#define DECLIT 300
#define REALLIT 301
#define STRLIT 302
#define IFX 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "jac.y"

    struct token* value;
    struct node* node;

#line 158 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
