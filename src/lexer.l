%option noyywrap
%pointer

%{
#include "AstNode.h"
#define YYSTYPE AstNode*
#include "parser.tab.hh"
#include "ExprNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

bool isValidChar(char c) {
  return 32 <= c && c <= 126 && c != '\"' && c != '\'' && c != '\\';
}

std::string parseStringLiteral(char* text) {
  std::string s;
  int i = 1;
  while (text[i + 1] != '\0') {
    char c = text[i];
    if (!isValidChar(c)) {
      if (c == '\\') {
        i++;
        // std::cerr << "val = " << text[i] << '\n';
        if (text[i] == 't') {
          c = '\t';
        } else if (text[i] == 'n') {
          c = '\n';
        } else if (text[i] == '\"' || text[i] == '\'' | text[i] == '\\') {
          c = text[i];
        } else {
          std::cerr << "syntax error\n";
          exit(1);
        }
      } else {
        std::cerr << "syntax error\n";
        exit(1);
      }
    }
    s.push_back(c);
    i++;
  }
  return s;
}
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
(true)|(false) { yylval = new BoolLitNode(yytext[0] == 't'); return BOOL_LITERAL; }
[a-zA-Z][a-zA-Z0-9]* { yylval = new StringLitNode(std::string(yytext)); return ID; }
(0x[0-9a-fA-F]+)|([0-9]+) { int base = (yytext[1] == 'x') ? 16 : 10; yylval = new IntLitNode(strtol(yytext, NULL, base)); return INT_LITERAL; }
"\'"{char}"\'" { yylval = new CharLitNode(yytext[1]); return CHAR_LITERAL; }
"\""[^\"]*"\"" { std::string s = parseStringLiteral(yytext); yylval = new StringLitNode(s); return STRING_LITERAL; }
[ \t\v\r\n]+ { /* ignore whitespace */ }
"//".* { /* ignore comments */ }
. { printf("Mystery character %c\n", *yytext); }
%%
