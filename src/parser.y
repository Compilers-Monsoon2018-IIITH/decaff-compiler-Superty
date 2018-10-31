%{
#include "AstNode.h"
#include "ExprNode.h"
#include "BlockNode.h"
#include "ListNode.h"
#include "RootNode.h"
#include "driver.h"
#define YYSTYPE AstNode*
#include <stdio.h>
#include <iostream>
#define YYDEBUG 1

void yyerror(char *s);
int yylex(void);

AstNode *ast;
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
program: CLASS ID OPEN_BRACE body CLOSE_BRACE {$$ = $4; ((RootNode*)$$)->id = ReduceToString((StringLitNode*)$2); ast = $$;}
;

body: method_decl_list {$$ = new RootNode; ReduceToVector((ListNode<MethodNode*>*)$1, &(((RootNode*)$$)->method_decls));}
| value_type nonempty_field_list body {$$ = $3; ((RootNode*)$$)->AppendFields((TypeNode*)$1, (FieldList*)$2);}

method_decl_list: /* nothing */ {$$ = nullptr;}
| method_decl method_decl_list {$$ = new ListNode<MethodNode*>((MethodNode*)$1, (ListNode<MethodNode*>*)$2);}
;

field: ID {$$ = new FieldList(Field{Type::VOID_TYPE, ReduceToString((StringLitNode*)$1), -1}, nullptr);}
| ID OPEN_BRACKET INT_LITERAL CLOSE_BRACKET {$$ = new FieldList(Field{Type::VOID_TYPE, ReduceToString((StringLitNode*)$1), ReduceToInt((IntLitNode*)$3)}, nullptr);}
;

nonempty_field_list: field SEMICOLON {$$ = $1;}
| field COMMA nonempty_field_list {$$ = $1; ((FieldList*)$$)->next = (FieldList*)$3;}
;

method_decl: VOID_TYPE ID OPEN_PAREN formal_parameter_list CLOSE_PAREN block {$$ = ConstructMethodNode(new TypeNode(Type::VOID_TYPE), $2, $4, $6);}
| value_type ID OPEN_PAREN formal_parameter_list CLOSE_PAREN block { /*std::cerr << "MethodNode " << ((StringLitNode*)$2)->value <<  '\n'; */$$ = ConstructMethodNode($1, $2, $4, $6); }
;

formal_parameter_list: /* nothing */ {$$ = nullptr;}
| nonempty_formal_parameter_list {$$ = $1;}
;

nonempty_formal_parameter_list: value_type ID {$$ = new VarList(ReduceToVar($1, $2), nullptr);}
| value_type ID COMMA nonempty_formal_parameter_list {$$ = new VarList(ReduceToVar($1, $2), (VarList*)$4); }
;

block: OPEN_BRACE var_decl_list statement_list CLOSE_BRACE {$$ = $2; ((BlockNode*)$$)->SetStatements((ListNode<AstNode*>*)$3);}
;

statement_list: /* nothing */ {$$ = nullptr;}
| statement statement_list {$$ = new ListNode<AstNode*>($1, (ListNode<AstNode*>*)$2);}
;

var_decl_list: /* nothing */ {$$ = new BlockNode;}
| var_decl_list value_type nonempty_id_list SEMICOLON {$$ = $1; ((BlockNode*)$$)->AppendVars((TypeNode*)$2, (IdList*)$3);}
;

nonempty_id_list: ID {$$ = new IdList((StringLitNode*)$1, nullptr);}
| ID COMMA nonempty_id_list {$$ = new IdList((StringLitNode*)$1, (IdList*)$3);}
;

value_type: INT_TYPE {$$ = new TypeNode(Type::INT_TYPE);}
| BOOLEAN_TYPE {$$ = new TypeNode(Type::BOOLEAN_TYPE);}
;

statement: location assign_op expr SEMICOLON {$$ = $2; ((AssignNode*)$$)->location = (LocationNode*)$1; ((AssignNode*)$$)->value = $3;}
| method_call SEMICOLON {$$ = $1;}
| IF OPEN_PAREN expr CLOSE_PAREN block {$$ = new IfNode($3, (BlockNode*)$5, nullptr);}
| IF OPEN_PAREN expr CLOSE_PAREN block ELSE block {$$ = new IfNode($3, (BlockNode*)$5, (BlockNode*)$7);}
| FOR ID ASSIGN expr COMMA expr block {/*std::cerr << "ForNode " << ((StringLitNode*)$2)->value << '\n'; */$$ = new ForNode(ReduceToString((StringLitNode*)$2), $4, $6, (BlockNode*)$7);}
| RETURN SEMICOLON {$$ = new ReturnNode(nullptr);}
| RETURN expr SEMICOLON {$$ = new ReturnNode($2);}
| BREAK SEMICOLON {$$ = new LoopControlNode(LoopControlType::BREAK);}
| CONTINUE SEMICOLON {$$ = new LoopControlNode(LoopControlType::CONTINUE);}
| block {$$ = $1;}
;

assign_op: ASSIGN {$$ = new AssignNode(nullptr, AssignOp::ASSIGN, nullptr);}
| PLUS_ASSIGN {$$ = new AssignNode(nullptr, AssignOp::PLUS_ASSIGN, nullptr);}
| MINUS_ASSIGN {$$ = new AssignNode(nullptr, AssignOp::MINUS_ASSIGN, nullptr);}
;

expr: location {$$ = $1;}
| method_call {$$ = $1;}
| INT_LITERAL {$$ = $1;}
| CHAR_LITERAL {$$ = $1;}
| BOOL_LITERAL {$$ = $1;}
| expr OR expr {$$ = new BinopNode($1, Op::OR, $3);}
| expr AND expr {$$ = new BinopNode($1, Op::AND, $3);}
| expr EQ expr {$$ = new BinopNode($1, Op::EQ, $3);}
| expr NE expr {$$ = new BinopNode($1, Op::NE, $3);}
| expr LT expr {$$ = new BinopNode($1, Op::LT, $3);}
| expr LE expr {$$ = new BinopNode($1, Op::LE, $3);}
| expr GE expr {$$ = new BinopNode($1, Op::GE, $3);}
| expr GT expr {$$ = new BinopNode($1, Op::GT, $3);}
| expr PLUS expr {$$ = new BinopNode($1, Op::PLUS, $3);}
| expr MINUS expr {$$ = new BinopNode($1, Op::MINUS, $3);}
| expr MULT expr {$$ = new BinopNode($1, Op::MULT, $3);}
| expr DIV expr {$$ = new BinopNode($1, Op::DIV, $3);}
| expr MOD expr {$$ = new BinopNode($1, Op::MOD, $3);}
| NOT expr {$$ = new UnopNode(Op::NOT, $2);}
| MINUS expr {$$ = new UnopNode(Op::MINUS, $2);}
| OPEN_PAREN expr CLOSE_PAREN {$$ = $2;}
;

location: ID {$$ = new LocationNode((StringLitNode*)$1);}
| ID OPEN_BRACKET expr CLOSE_BRACKET {$$ = new LocationNode((StringLitNode*)$1, $3);}
;

method_call: ID OPEN_PAREN expr_list CLOSE_PAREN {$$ = new MethodCallNode((StringLitNode*)$1, (ArgList*)$3);}
| CALLOUT OPEN_PAREN STRING_LITERAL callout_arg_list CLOSE_PAREN {$$ = new MethodCallNode((StringLitNode*)$3, (CalloutArgList*)$4);}
;

expr_list: /* nothing */ {$$ = nullptr;}
| nonempty_expr_list {$$ = $1;}
;

nonempty_expr_list: expr {$$ = new ArgList($1, nullptr);}
| expr COMMA nonempty_expr_list {$$ = new ArgList($1, (ArgList*)$3);}
;

callout_arg_list: /* nothing */ {$$ = nullptr;}
| COMMA nonempty_callout_arg_list {$$ = $2;}

nonempty_callout_arg_list: callout_arg {$$ = $1;}
| callout_arg COMMA nonempty_callout_arg_list {$$ = $1; ((CalloutArgList*)$$)->next = (CalloutArgList*)$3;}
;

callout_arg: expr {$$ = new ListNode<CalloutArg>($1, nullptr);}
| STRING_LITERAL {$$ = new ListNode<CalloutArg>(ReduceToString((StringLitNode*)$1), nullptr);}
;

%%
int main(int argc, char **argv)
{
 yyparse();
 drive(ast);
}
void yyerror(char *s)
{
 fprintf(stderr, "error: %s\n", s);
}
