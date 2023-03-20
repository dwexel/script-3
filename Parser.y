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



  symrec *symptr;
  node* nodeptr;
  void* none;
}

%token TOKEN_LPAREN      "("
%token TOKEN_RPAREN      ")"
%token TOKEN_PLUS        "+"
%token TOKEN_STAR        "*"
%token TOKEN_HYPH        "-"
%token TOKEN_FSLASH      "/"
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

// list
%type <symptr> defs
%type <symptr> def

// expression trees
// %type <nodeptr> call
%type <nodeptr> expr

// lists
%type <symptr> statement
%type <symptr> args

// literals
%type <fval> number

%left "+"
%left "*"

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
                              // print_list($1);
                              // evaluate with symbols
                              // for (symrec *p = $1; p; p = p->next)
                              // {
                              //   print_node(p->value.expr);
                              // }

                              // print_node_wsym($2, $1);
                              nVal n = evaluate($2, $1);
                              printf("%g\n", n);
                            }

defs:
  %empty
  | defs def                { $$ = putsym($1, $2); }

def:
  "ID" "{" expr "}"                       { 
                                            $$ = newsym((symrec) {kDef, strdup($1), {.expr = $3}, NULL});
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
  | "(" expr ")"            { 
                              $$ = $2; 
                            }
  | "ID"                    { 
                              $$ = newnode((node){kVar, {.var = {strdup($1)}}}); 
                            }
  | "ID" "(" args ")"       {
                              $$ = newnode((node){kCall, {.call = {strdup($1), $3}}});
                            }

args:
  %empty                      { $$ = NULL;           }
  | statement                 { $$ = $1;             }
  | args "," statement        { $$ = putsym($1, $3); }

statement:
  "ID" "=" number             { $$ = newsym((symrec) {kNum, strdup($1), {.number = $3}, NULL}); }
  

number:
  "INTEGER"                   { $$ = (double) $1; }
  | "DECIMAL"                 { $$ = $1;          }

