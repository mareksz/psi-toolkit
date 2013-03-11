/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

/* bison -d -p trparser trparser.y */

%{
//#define YYERROR_VERBOSE

#define YYDEBUG 1
#include <iostream>
//static void yyprint (FILE *file, int type, YYSTYPE value);
//#define YYPRINT(file, type, value)   yyprint (file, type, value)

#define XXX {std::cerr << __FILE__ <<  " " << __LINE__ << std::endl; }

#include "trules.hpp"


int trlexlex();
extern int trlexlineno;
extern zsymbol* the_file_name;
extern zsymbolfactory* the_zsymbolfactory;
extern zobjects_holder* the_holder;

int trparserlex()
{
	return trlexlex();
}

void trparsererror(char* s);

tmil::TScript trparser_all = NULL;
char trerror_message[128] = "GRULES UNKNOWN ERROR";
int subrflag = 0;
%}

%union{
    int i;
    char* s;
    tmil::Expression* expression;
    tmil::Instruction* instruction;
    zvector* vector;
    tmil::TScript tscript;
}

%type <expression>  expr
%type <expression>  subrarg
%type <expression>  subrexec
%type <expression>  subrargs
%type <tscript>     script
%type <instruction> instr
%type <instruction> instrs

%type <s> transfer_cat
%type <s> transfer_label
%type <s> atom

%type <s> argn
%type <vector> argns
%type <vector> argnsi
%type <vector> order_sequence

%type <s> order_op

%token <s> TTOKEN_ATOM
%token <s> TTOKEN_ATTRIBUTE
%token <i> TTOKEN_NUMBER
%token <s> TTOKEN_VAR
%token <s> TTOKEN_PLREF
%token <s> TTOKEN_SREF
%token <s> TTOKEN_SSREF
%token <s> TTOKEN_REGEXP

%token <s> TTOKEN_NOT_EQUAL
%token <s> TTOKEN_UEQUAL
%token <s> TTOKEN_ASSIGN
%token <s> TTOKEN_ARROW
%token <s> TTOKEN_GREATERE
%token <s> TTOKEN_QUAL
%token <s> TTOKEN_BIND

%token <s> TTOKEN_LEFT_PAREN
%token <s> TTOKEN_RIGHT_PAREN
%token <s> TTOKEN_LEFT_CURLY
%token <s> TTOKEN_RIGHT_CURLY
%token <s> TTOKEN_LEFT_SQUARE
%token <s> TTOKEN_RIGHT_SQUARE

%token <s> TTOKEN_COMMA
%token <s> TTOKEN_COLON
%token <s> TTOKEN_DOT
%token <s> TTOKEN_CARET
%token <s> TTOKEN_LESS
%token <s> TTOKEN_MINUS
%token <s> TTOKEN_PLUS
%token <s> TTOKEN_STAR
%token <s> TTOKEN_SEMICOLON
%token <s> TTOKEN_EQUAL
%token <s> TTOKEN_ET

%token <s> TTOKEN_IF
%token <s> TTOKEN_AND
%token <s> TTOKEN_OR
%token <s> TTOKEN_NOT
%token <s> TTOKEN_ELSE
%token <s> TTOKEN_SUB
%token <s> TTOKEN_FOREACH
%token <s> TTOKEN_RETURN
%token <s> TTOKEN_AMONG
%token <s> TTOKEN_ORDER

%token <s> TTOKEN_ARG_SEP

%token <i> TTOKEN_ERROR


%right TTOKEN_COMMA
%right TTOKEN_OR
%right TTOKEN_AND
%nonassoc TTOKEN_NOT
%nonassoc TTOKEN_ASSIGN
%nonassoc TTOKEN_EQUAL
%nonassoc TTOKEN_NOT_EQUAL
%nonassoc TTOKEN_UEQUAL
%nonassoc TTOKEN_BIND
%left TTOKEN_CARET
%left TTOKEN_LESS
%left TTOKEN_PLUS
%left TTOKEN_ET
%left TTOKEN_ARROW
%left TTOKEN_DOT

%%

alls:   script
		{
			trparser_all = (tmil::Instruction*)($1 ->getFirstChainElement());
		}
;

script: instr
		{
		    $$ = $1;
		}
	| script instr
		{
		    $$->insertChainAtTheEnd($2);
		}
;

atom:   TTOKEN_ATOM
      | TTOKEN_REGEXP
      | TTOKEN_STAR
;


expr: TTOKEN_NUMBER
		{
		    $$ = new tmil::AtomicExpression(INTEGER_TO_ZVALUE($1));
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | atom
		{
		    $$ = new tmil::AtomicExpression(ZOBJECT_TO_ZVALUE(the_zsymbolfactory->get_symbol($1,true)));
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | TTOKEN_ATTRIBUTE
		{
		    $$ = new tmil::AttributeExpression(the_zsymbolfactory->get_symbol($1,true));
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | TTOKEN_PLREF
		{
		    $$ = new tmil::PlrefExpression(the_zsymbolfactory->get_symbol($1,true));
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | TTOKEN_VAR
		{
		    $$ = new tmil::VariableExpression(the_zsymbolfactory->get_symbol($1,true));
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | TTOKEN_SREF
		{
		    $$ = new tmil::SrefExpression(the_zsymbolfactory->get_symbol($1,true));
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | expr TTOKEN_OR expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | expr TTOKEN_AND expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_ASSIGN expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_EQUAL expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_NOT_EQUAL expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_UEQUAL expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_CARET expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_LESS expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | TTOKEN_MINUS expr
		{
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($1,true), $2);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_PLUS expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_ET expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_BIND expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_ARROW expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
	  | expr TTOKEN_DOT expr
		{
		    $1->insertChainBehind($3);
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($2,true), $1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | TTOKEN_NOT expr
		{
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($1,true), $2);
            $$->setOrigin(the_file_name, trlexlineno);
		}
      | TTOKEN_LEFT_PAREN expr TTOKEN_RIGHT_PAREN
		{
		    $$ = $2;
		}
      | subrexec
		{
		    $$ = $1;
		}
;

subrexec: TTOKEN_ATOM TTOKEN_LEFT_PAREN TTOKEN_RIGHT_PAREN
		{
		    $$ = new tmil::SubrExpression(
		             the_zsymbolfactory->get_symbol($1,true),
		             tmil::Expression::createEmptyExpression()
			 );
            $$->setOrigin(the_file_name, trlexlineno);
		}
;

subrexec: TTOKEN_ATOM TTOKEN_LEFT_PAREN subrargs TTOKEN_RIGHT_PAREN
		{
		    $$ = new tmil::SubrExpression(the_zsymbolfactory->get_symbol($1,true), $3);
            $$->setOrigin(the_file_name, trlexlineno);
		}
;

subrargs: subrarg
		{
		    $$ = $1;
		}
          | subrargs TTOKEN_COMMA subrarg
		{
		    $$->insertChainAtTheEnd($3);
		}
;

subrarg: expr
;

subrarg: /* empty */
		{
           $$ = tmil::Expression::createEmptyExpression();
           $$->setOrigin(the_file_name, trlexlineno);
        }
;


/* ------------------------| Instructions |------------------------- */



instr: expr TTOKEN_SEMICOLON
		{
		    $$ = new tmil::SimpleInstruction($1);
            $$->setOrigin(the_file_name, trlexlineno);
		}
;

instrs: /* empty */
		{
		    $$ = tmil::Instruction::createEmptyInstruction();
		}
        | instrs instr
		{
		    $$->insertChainAtTheEnd($2);
		}
;

instr: TTOKEN_LEFT_CURLY instrs TTOKEN_RIGHT_CURLY
		{
		    $$ = new tmil::InstructionBlock($2);
		}
;

instr: TTOKEN_IF TTOKEN_LEFT_PAREN expr TTOKEN_RIGHT_PAREN instr
		{
		    $$ = new tmil::InstructionIf($3,$5);
		}
;

instr: TTOKEN_IF TTOKEN_LEFT_PAREN expr TTOKEN_RIGHT_PAREN
       instr
       TTOKEN_ELSE instr
		{
		    $$ = new tmil::InstructionIf($3,$5,$7);
		}
;

transfer_cat: TTOKEN_ATTRIBUTE
;

transfer_cat: TTOKEN_ATOM
;

transfer_label: TTOKEN_ATOM
;

instr: transfer_cat TTOKEN_COLON transfer_label TTOKEN_GREATERE transfer_cat TTOKEN_COLON transfer_label instr
		{
		    $$ = new tmil::TransferSpec(the_holder, the_zsymbolfactory->get_symbol($1,true),the_zsymbolfactory->get_symbol($3,true),the_zsymbolfactory->get_symbol($5,true),the_zsymbolfactory->get_symbol($7,true),$8);
		}
;

instr: TTOKEN_COLON transfer_label TTOKEN_GREATERE TTOKEN_COLON transfer_label instr
		{
		    $$ = new tmil::TransferSpec(the_holder, NULL,the_zsymbolfactory->get_symbol($2,true),NULL,the_zsymbolfactory->get_symbol($5,true),$6);
		}
;

instr: transfer_cat TTOKEN_GREATERE transfer_cat instr
		{
		    $$ = new tmil::TransferSpec(the_holder, the_zsymbolfactory->get_symbol($1,true),NULL,the_zsymbolfactory->get_symbol($3,true),NULL,$4);
		}
;

argn: TTOKEN_ATOM
;

argns: argn
		{
		    $$ = zvector::generate(the_holder);
		    $$->add( ZOBJECT_TO_ZVALUE(the_zsymbolfactory->get_symbol($1,true)) );
		}
       | argns TTOKEN_COMMA argn
		{
		    $$->add( ZOBJECT_TO_ZVALUE(the_zsymbolfactory->get_symbol($3,true)) );
		}
;

argnsi: argns
;

argnsi: /* empty */
		{
		    $$ = zvector::generate(the_holder);
		}
;

instr: TTOKEN_SUB TTOKEN_QUAL TTOKEN_ATOM TTOKEN_LEFT_PAREN argnsi TTOKEN_RIGHT_PAREN instr
		{
		    $$ = new tmil::Subroutine(NULL, the_zsymbolfactory->get_symbol($3,true), $5, $7);
		}
;

instr: TTOKEN_SUB transfer_cat TTOKEN_QUAL TTOKEN_ATOM TTOKEN_LEFT_PAREN argnsi TTOKEN_RIGHT_PAREN instr
		{
		    $$ = new tmil::Subroutine(the_zsymbolfactory->get_symbol($2,true), the_zsymbolfactory->get_symbol($4,true), $6, $8);
		}
;

instr: TTOKEN_FOREACH TTOKEN_VAR instr
		{
		    $$ = new tmil::ForeachLoop(the_zsymbolfactory->get_symbol($2,true),NULL,$3);
		}
;

instr: TTOKEN_FOREACH TTOKEN_VAR TTOKEN_AMONG TTOKEN_SREF instr
		{
		    $$ = new tmil::ForeachLoop(the_zsymbolfactory->get_symbol($2,true),the_zsymbolfactory->get_symbol($4,true),$5);
		}
;


instr: TTOKEN_SREF TTOKEN_LEFT_SQUARE expr TTOKEN_RIGHT_SQUARE TTOKEN_SEMICOLON
		{
		    $$ = new tmil::SrefSetting(the_zsymbolfactory->get_symbol($1,true),$3);
		}
;


order_op: TTOKEN_LESS

order_op: TTOKEN_EQUAL

order_sequence: TTOKEN_SREF order_op TTOKEN_SREF
		{
		    $$ = zvector::generate(the_holder);
		    $$->add(the_zsymbolfactory->get_symbol($1,true));
		    $$->add(the_zsymbolfactory->get_symbol($2,false));
		    $$->add(the_zsymbolfactory->get_symbol($3,true));
		}
;

order_sequence: order_sequence order_op TTOKEN_SREF
		{
		    $$->add(the_zsymbolfactory->get_symbol($2,false));
		    $$->add(the_zsymbolfactory->get_symbol($3,true));
		}
;

instr: TTOKEN_ORDER order_sequence TTOKEN_SEMICOLON
		{
		    $$ = new tmil::OrderSetting(the_holder, $2);
		}
;

instr: TTOKEN_RETURN expr TTOKEN_SEMICOLON
		{
		    $$ = new tmil::ReturnInstruction($2);
		}
;

%%
void trparsererror(char* s)
{
	snprintf(trerror_message,127,"%d::%s", trlexlineno,s);
}

/*
static void
yyprint (FILE *file, int type, YYSTYPE value)
{
switch (type) {
	case TTOKEN_ATOM: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_ATTRIBUTE: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_NUMBER: std::cerr << value.i << "\n"; break;
	case TTOKEN_VAR: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_PLREF: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_SREF: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_SSREF: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_REGEXP: std::cerr << *(value.s) << "\n"; break;
 break;
	case TTOKEN_NOT_EQUAL: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_UEQUAL: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_ASSIGN: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_ARROW: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_GREATERE: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_QUAL: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_BIND: std::cerr << *(value.s) << "\n"; break;
 break;
	case TTOKEN_LEFT_PAREN: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_RIGHT_PAREN: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_LEFT_CURLY: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_RIGHT_CURLY: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_LEFT_SQUARE: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_RIGHT_SQUARE: std::cerr << *(value.s) << "\n"; break;
 break;
	case TTOKEN_COMMA: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_COLON: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_DOT: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_CARET: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_LESS: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_MINUS: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_PLUS: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_STAR: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_SEMICOLON: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_EQUAL: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_ET: std::cerr << *(value.s) << "\n"; break;
 break;
	case TTOKEN_IF: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_AND: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_OR: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_NOT: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_ELSE: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_SUB: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_FOREACH: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_RETURN: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_AMONG: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_ORDER: std::cerr << *(value.s) << "\n"; break;
 break;
	case TTOKEN_ARG_SEP: std::cerr << *(value.s) << "\n"; break;
	case TTOKEN_ERROR: std::cerr << value.i << "\n"; break;
	default: std::cerr << "nie wiem co to jest" << "\n";
}
}
*/
