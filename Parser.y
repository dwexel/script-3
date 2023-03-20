%{
  #include <math.h>
  #include <stdbool.h>

  // bridges
  #include <parser.h>
  #include <lexer.h>


  void yyerror(yyscan_t scanner, const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
  }
%}

%define api.pure
%lex-param   { void* scanner }
%parse-param { void* scanner }

%code requires {
  #include <symstack.h>
}

%union {
  int ival;
  double fval;
  char sval[100];



  node* nodeptr;
  void* none;
}

%token TOKEN_LPAREN      "("
%token TOKEN_RPAREN      ")"
%token TOKEN_PLUS        "+"
%token TOKEN_STAR        "*"
%token TOKEN_HYPH        "-"
%token TOKEN_FSLASH      "/"
%token TOKEN_CARET       "^"

%token TOKEN_LBRACE      "{"
%token TOKEN_RBRACE      "}"
%token TOKEN_COMMA       ","

%token TOKEN_EQUALS      "="


/* %token TOKEN_KEYWORD_FN  "fn"
%token TOKEN_KEYWORD_END "end" */

%token <ival> TOKEN_INTEGER "INTEGER"
%token <fval> TOKEN_DECIMAL "DECIMAL"
%token <sval> TOKEN_ID "ID"

%type <none> start
%type <none> input


%type <nodeptr> defs
%type <nodeptr> def
%type <nodeptr> expr
%type <nodeptr> statement
%type <nodeptr> args

// literals
%type <fval> number

%left "+"
%left "*"
%left "-"
%left "/"
%left "^"

%%

start:
  input                     {
                              if (yychar == YYEOF)
                              {
                                printf("input parsed\n");
                              }
                            }

input:
  %empty
  | defs expr               { 
                              // print_node($2);
                              double n = evaluate($2, $1);
                              printf("result: %g\n", n);
                            }

defs:
  %empty
  | defs def                { 
                              node *top = $2;
                              node *prev = $1;
                              top->e.li.next = prev;
                              $$ = top;
                            }

def:
  "ID" "{" expr "}"         { 
                              $$ = newnode((node) {kDef, {.li = {strdup($1), $3, NULL}}});
                            }

expr:
  number                    {
                              nVal n = $1;
                              $$ = newnode((node) {kNum,  {n}}); 
                            }
  | expr "-" expr           {
                              node *left = $1;
                              node *right = $3;
                              $$ = newnode((node) {kDiff, {.binary = {left, right}}});  
                            }
  | expr "+" expr           { 
                              node *left = $1; 
                              node *right = $3;
                              $$ = newnode((node) {kSum, {.binary = {left, right}}});  
                            } 
  | expr "/" expr           { 
                              node *left = $1;
                              node *right = $3;
                              $$ = newnode((node) {kDiv, {.binary = {left, right}}});  
                            }
  | expr "*" expr           {
                              node *left = $1;
                              node *right = $3;
                              $$ = newnode((node) {kMult, {.binary = {left, right}}});  
                            }
  | expr "^" expr           {
                              node *left = $1;
                              node *right = $3;
                              $$ = newnode((node) {kPow, {.binary = {left, right}}});  
                            }
  | "(" expr ")"            {
                              $$ = $2;
                            }
  | "ID"                    {
                              $$ = newnode((node) {kVar, {.id = {strdup($1)}}});
                            }
  | "ID" "(" args ")"       {
                              $$ = newnode((node) {kCall, {.link = {strdup($1), $3}}});
                            }

args:
  %empty                      { 
                                $$ = NULL;       
                              }
  | statement                 { 
                                $$ = $1;
                              }
  | args "," statement        {
                                node *top = $3;
                                node *prev = $1;
                                top->e.li.next = prev;
                                $$ = top;
                              }

statement:
  "ID" "=" expr               {
                                // $$ = newnode((node) {kLink, {.link = {strdup($1), $3}}});
                                $$ = newnode((node) {kArg, {.li = {strdup($1), $3, NULL}}});
                              }
  

number:
  "INTEGER"                   { $$ = (double) $1; }
  | "DECIMAL"                 { $$ = $1;          }

