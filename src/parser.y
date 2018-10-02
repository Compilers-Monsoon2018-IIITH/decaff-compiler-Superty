%{
#include <stdio.h>
#define YYDEBUG 1
%}

/*
%union {
 int int_val;
 bool bool_val;
 char char_val;
 char* string_val;
}*/

/* declare tokens */
%token ID
%token INT_LITERAL
%token STRING_LITERAL
%token BOOL_LITERAL
%token CHAR_LITERAL
%token CALLOUT
%token COMMA SEMICOLON
%token OPEN_PAREN CLOSE_PAREN
%token OPEN_BRACKET CLOSE_BRACKET
%token OPEN_BRACE CLOSE_BRACE
%token ASSIGN PLUS_ASSIGN MINUS_ASSIGN
%token IF ELSE FOR RETURN BREAK CONTINUE
%token INT_TYPE BOOLEAN_TYPE VOID_TYPE
%token CLASS

%left OR
%left AND
%left EQ NE
%left LT LE GE GT
%left PLUS MINUS
%left MULT DIV MOD
%nonassoc NOT

%%
program: CLASS ID OPEN_BRACE body CLOSE_BRACE
;

body: method_decl_list
| value_type nonempty_field_list body

method_decl_list: /* nothing */
| method_decl method_decl_list
;

field: ID
| ID OPEN_BRACKET INT_LITERAL CLOSE_BRACKET
;

nonempty_field_list: field SEMICOLON
| field COMMA nonempty_field_list
;

method_decl: VOID_TYPE ID OPEN_PAREN formal_parameter_list CLOSE_PAREN block
| value_type ID OPEN_PAREN formal_parameter_list CLOSE_PAREN block
;

formal_parameter_list: /* nothing */
| nonempty_formal_parameter_list
;

nonempty_formal_parameter_list: value_type ID
| nonempty_formal_parameter_list COMMA value_type ID
;

block: OPEN_BRACE var_decl_list statement_list CLOSE_BRACE
;

statement_list: /* nothing */
| statement_list statement
;

var_decl_list: /* nothing */
| var_decl_list value_type nonempty_id_list SEMICOLON
;

nonempty_id_list: ID
| nonempty_id_list COMMA ID
;

value_type: INT_TYPE
| BOOLEAN_TYPE
;

statement: location assign_op expr SEMICOLON
| method_call SEMICOLON
| IF OPEN_PAREN expr CLOSE_PAREN block
| IF OPEN_PAREN expr CLOSE_PAREN block ELSE block
| FOR ID ASSIGN expr COMMA expr block
| RETURN SEMICOLON
| RETURN expr SEMICOLON
| BREAK SEMICOLON
| CONTINUE SEMICOLON
| block
;

assign_op: ASSIGN
| PLUS_ASSIGN
| MINUS_ASSIGN
;

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

method_call: ID OPEN_PAREN expr_list CLOSE_PAREN
| CALLOUT OPEN_PAREN STRING_LITERAL callout_arg_list CLOSE_PAREN
;

expr_list: /* nothing */
| nonempty_expr_list
;

nonempty_expr_list: expr
| nonempty_expr_list COMMA expr
;

callout_arg_list: /* nothing */
| nonempty_callout_arg_list

nonempty_callout_arg_list: COMMA callout_arg
| callout_arg_list COMMA callout_arg
;

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