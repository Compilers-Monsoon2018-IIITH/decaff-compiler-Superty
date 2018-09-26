%{
#include <stdio.h>
%}
/* declare tokens */
%token ID
%token INT_LITERAL
%token STRING_LITERAL
%token BOOL_LITERAL
%token CHAR_LITERAL
%token CALLOUT
%token COMMA
%token OPEN_PAREN CLOSE_PAREN
%token OPEN_BRACKET CLOSE_BRACKET

%left OR
%left AND
%left EQ NE
%left LT LE GE GT
%left PLUS MINUS
%left MULT DIV MOD
%nonassoc NOT

%%
expr: location
| method_call
| INT_LITERAL
| CHAR_LITERAL
| BOOL_LITERAL
| expr OR expr
| expr AND expr
| expr EQ expr
| expr NE expr
| expr LT expr
| expr LE expr
| expr GE expr
| expr GT expr
| expr PLUS expr
| expr MINUS expr
| expr MULT expr
| expr DIV expr
| expr MOD expr
| NOT expr
| MINUS expr
| OPEN_PAREN expr CLOSE_PAREN
;

location: ID
| ID OPEN_BRACKET expr CLOSE_BRACKET
;

method_call: ID OPEN_PAREN maybe_expr_list CLOSE_PAREN
| CALLOUT OPEN_PAREN STRING_LITERAL maybe_callout_arg_list CLOSE_PAREN
;

maybe_expr_list: /* nothing */
| expr_list
;

expr_list: expr
| expr COMMA expr_list
;

maybe_callout_arg_list: /* nothing */
| COMMA callout_arg maybe_callout_arg_list

callout_arg: expr
| STRING_LITERAL
;

%%
int main(int argc, char **argv)
{
 yyparse();
}
void yyerror(char *s)
{
 fprintf(stderr, "error: %s\n", s);
}