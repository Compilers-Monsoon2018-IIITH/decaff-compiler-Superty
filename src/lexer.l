%option noyywrap nounput noinput batch debug
%pointer

%{
#include "parser.hh"
#include <stdio.h>
#include <stdlib.h>
%}

char [ !#-&(-[\]-~]|(\\\')|(\\\")|(\\\\)|(\\\t)|(\\\n)
%%

"if" { return yy::parser::make_IF(); }
"else" { return yy::parser::make_ELSE(); }
"for" { return yy::parser::make_FOR(); }
"return" { return yy::parser::make_RETURN(); }
"break" { return yy::parser::make_BREAK(); }
"continue" { return yy::parser::make_CONTINUE(); }
"int" { return yy::parser::make_INT_TYPE(); }
"boolean" { return yy::parser::make_BOOLEAN_TYPE(); }
"void" { return yy::parser::make_VOID_TYPE(); }
"class" { return yy::parser::make_CLASS(); }
"callout" { return yy::parser::make_CALLOUT(); }
"=" { return yy::parser::make_ASSIGN(); }
"+=" { return yy::parser::make_PLUS_ASSIGN(); }
"-=" { return yy::parser::make_MINUS_ASSIGN(); }
"," { return yy::parser::make_COMMA(); }
";" { return yy::parser::make_SEMICOLON(); }
"(" { return yy::parser::make_OPEN_PAREN(); }
")" { return yy::parser::make_CLOSE_PAREN(); }
"[" { return yy::parser::make_OPEN_BRACKET(); }
"]" { return yy::parser::make_CLOSE_BRACKET(); }
"{" { return yy::parser::make_OPEN_BRACE(); }
"}" { return yy::parser::make_CLOSE_BRACE(); }
"+" { return yy::parser::make_PLUS(); }
"-" { return yy::parser::make_MINUS(); }
"*" { return yy::parser::make_MULT(); }
"/" { return yy::parser::make_DIV(); }
"%" { return yy::parser::make_MOD(); }
"<" { return yy::parser::make_LT(); }
">" { return yy::parser::make_GT(); }
"<=" { return yy::parser::make_LE(); }
">=" { return yy::parser::make_GE(); }
"==" { return yy::parser::make_EQ(); }
"!=" { return yy::parser::make_NE(); }
"&&" { return yy::parser::make_AND(); }
"||" { return yy::parser::make_OR(); }
"!" { return yy::parser::make_NOT(); }
(true)|(false) { /*yylval.bool_val = yytext[0] == 't'; */return yy::parser::make_BOOL_LITERAL(); }
[a-zA-Z][a-zA-Z0-9]* { /*yylval.string_val = yytext;*/ return yy::parser::make_ID(); }
(0x[0-9a-fA-F]+)|([0-9]+) { /*int base = (yytext[1] == 'x') ? 16 : 10; yylval.int_val = strtol(yytext, NULL, base);*/ return yy::parser::make_INT_LITERAL(); }
"\'"{char}"\'" { /*yylval.char_val = yytext[1];*/ return yy::parser::make_CHAR_LITERAL(); }
"\""{char}*"\"" { /*yylval.string_val = yytext;*/ return yy::parser::make_STRING_LITERAL(); }
[ \t\v\r\n]+ { /* ignore whitespace */ }
. { printf("Mystery character %c\n", *yytext); }
%%