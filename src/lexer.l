%option noyywrap

%{
#include "parser.tab.h"
#include <stdio.h>
%}

char [ !#-&(-[\]-~]|(\\\')|(\\\")|(\\\\)|(\\\t)|(\\\n)
%%

"callout" { return CALLOUT; }
"," { return COMMA; }
"(" { return OPEN_PAREN; }
")" { return CLOSE_PAREN; }
"[" { return OPEN_BRACKET; }
"]" { return CLOSE_BRACKET; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MULT; }
"/" { return DIV; }
"%" { return MOD; }
"<" { return LT; }
">" { return GT; }
"<=" { return LE; }
">=" { return GE; }
"==" { return EQ; }
"!=" { return NE; }
"&&" { return AND; }
"||" { return OR; }
"!" { return NOT; }
(true)|(false) { return BOOL_LITERAL; }
[a-zA-Z][a-zA-Z0-9]* { return ID; }
(0x[0-9a-fA-F]+)|([0-9]+) { return INT_LITERAL; }
"\'"{char}"\'" { return CHAR_LITERAL; }
"\""{char}*"\"" { return STRING_LITERAL; }
[ \t\v\r\n]+ { /* ignore whitespace */ }
. { printf("Mystery character %c\n", *yytext); }
%%