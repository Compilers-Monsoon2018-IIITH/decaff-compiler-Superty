%option noyywrap
%pointer

%{
#include "parser.tab.hh"
#include <stdio.h>
#include <stdlib.h>
%}

char [ !#-&(-[\]-~]|(\\\')|(\\\")|(\\\\)|(\\\t)|(\\\n)
%%

"if" { return IF; }
"else" { return ELSE; }
"for" { return FOR; }
"return" { return RETURN; }
"break" { return BREAK; }
"continue" { return CONTINUE; }
"int" { return INT_TYPE; }
"boolean" { return BOOLEAN_TYPE; }
"void" { return VOID_TYPE; }
"class" { return CLASS; }
"callout" { return CALLOUT; }
"=" { return ASSIGN; }
"+=" { return PLUS_ASSIGN; }
"-=" { return MINUS_ASSIGN; }
"," { return COMMA; }
";" { return SEMICOLON; }
"(" { return OPEN_PAREN; }
")" { return CLOSE_PAREN; }
"[" { return OPEN_BRACKET; }
"]" { return CLOSE_BRACKET; }
"{" { return OPEN_BRACE; }
"}" { return CLOSE_BRACE; }
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
(true)|(false) { /*yylval.bool_val = yytext[0] == 't'; */return BOOL_LITERAL; }
[a-zA-Z][a-zA-Z0-9]* { /*yylval.string_val = yytext;*/ return ID; }
(0x[0-9a-fA-F]+)|([0-9]+) { /*int base = (yytext[1] == 'x') ? 16 : 10; yylval.int_val = strtol(yytext, NULL, base);*/ return INT_LITERAL; }
"\'"{char}"\'" { /*yylval.char_val = yytext[1];*/ return CHAR_LITERAL; }
"\""{char}*"\"" { /*yylval.string_val = yytext;*/ return STRING_LITERAL; }
[ \t\v\r\n]+ { /* ignore whitespace */ }
. { printf("Mystery character %c\n", *yytext); }
%%
