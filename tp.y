/* attention: NEW est defini dans tp.h Utilisez un autre token */
%token IS CLASS VAR EXTENDS DEF OVERRIDE RETURN AS IF THEN ELSE AFF ADD SUB MUL DIV THIS SUPER RESULT NeW
%token<S> Id Str
%token<I> Cste
%token<C> RelOp

%right ELSE
%left RelOp
%left ADD SUB
%left MUL DIV
%nonassoc unary

%{
#include "tp.h"
#include "tp_y.h"

extern int yylex();
extern void yyerror(char *);
%}

%%
Prog : classLOpt block
;

classLOpt:
| class classLOpt 
;

class: declClass block IS '{' corps '}'
;

declClass: CLASS Id '(' paramLOpt ')' extendsOpt
;

extendsOpt:
| EXTENDS Id '(' exprLOpt ')'
;

paramLOpt:
| paramL
;

paramL: param
| param ',' paramL
;

param: Id ':' Id
;

corps: varLOpt methodeLOpt
;

varLOpt:
| varDecl varLOpt
;

varDecl: VAR Id ':' Id affOpt ';'
;

affOpt:
| AFF expr
;

methodeLOpt:
| methodeDecl methodeLOpt
;

methodeDecl: redifOpt DEF Id '('paramLOpt')' bodyAlt
;

bodyAlt: ':' Id AFF expr
| returnOpt IS block
;

returnOpt:
| ':' Id
;

redifOpt:
| OVERRIDE
;

block: '{' blockOpt '}'
;

blockOpt: instrLOpt
| varDecl varLOpt IS instr instrLOpt
;

instrLOpt:
| instr instrLOpt
;

instr: block
| expr ';'
| RETURN ';'
| Id AFF expr ';'
| IF expr THEN instr ELSE instr
;

expr: expr RelOp expr 
| expr ADD expr
| expr SUB expr 
| expr MUL expr         
| expr DIV expr	      
| ADD expr %prec unary  
| SUB expr %prec unary
| NeW Id '(' paramLOpt ')'  
| '(' AS Id ':' expr ')'
| Cste		       
| Id
| Str
| THIS
| SUPER
| RESULT	
| '(' expr ')'		
;

exprLOpt:
|exprL
;

exprL: expr
| expr ',' exprL
;




















