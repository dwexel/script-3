%{
  #include <math.h>
  #include <stdbool.h>

  // bridges
  #include <parser.h>
  #include <lexer.h>

  // global symbol table
  symrec *defs;

  void yyerror(yyscan_t scanner, const char *msg) 
  {
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

  symrec* symrec_ptr;
  numrec* numrec_ptr;
  node* expr_ptr;
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

%token TOKEN_KEYWORD_FN  "fn"
%token TOKEN_KEYWORD_END "end"

%token <ival> TOKEN_INTEGER "INTEGER"
%token <fval> TOKEN_DECIMAL "DECIMAL"
%token <sval> TOKEN_ID "ID"

%type <none> start
%type <none> input
%type <none> line

%type <none> def
%type <none> call

%type <expr_ptr> expr
%type <symrec_ptr> params
%type <numrec_ptr> args

%type <fval> number

%left "+"
%left "*"

%%

start:
  input                     {
                              if (yychar == YYEOF) 
                              {
                              }
                            }

input:
  | input line

line:
  call                      { }
  | def                     { }

call:
  "ID" "(" args ")"         {
                              symrec *funcsym = getfromlist(defs, $1);                              

                              if (funcsym) {
                                symrec *params = funcsym->value.fn->params;
                                numrec *args = $3;

                                if (!params) {
                                  puts("no params");
                                }
                                if (!args) {
                                  puts("no args");
                                }

                                if (params && args) {
                                  puts("params and args");
                                  fill(params, args);
                                  print_list(params);
                                }
                                node *expr = funcsym->value.fn->expr;
                                print_node(expr);

                                printf("evaluation: \n%lf\n", evaluate_node_wsym(funcsym->value.fn->expr, params));
                              }
                              else {
                                printf("attempt to call nonexistent function: %s", $1);
                              }
                            }

def:
  "fn" "ID" "(" params ")" expr "end"     {
                                            // create
                                            struct def *fn = malloc(sizeof(struct def));
                                            fn->params = $4;
                                            fn->expr = $6;

                                            // add to list
                                            defs = addtolist(defs, $2, 0);
                                            defs->value.fn = fn;
                                            
                                            if (semantic_verify($6, $4)) {
                                              printf("semantic verified: %s\n\n", $2);
                                            }
                                            else {
                                              printf("error, semantic error\n");
                                            }
                                          }

params:
  %empty                      { $$ = NULL; }
  | "ID"                      {
                                $$ = newlist($1);
                              }
  | params "," "ID"           {
                                addtolist_end($1, $3);
                                $$ = $1;
                              }

args:
  %empty                      { $$ = NULL; }
  | number                    { 
                                $$ = newlist_num($1);
                              }
  | args "," number           {
                                addtolist_end_num($1, $3);
                                $$ = $1;
                              }




expr:
  number                    {
                              double n = $1;
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
                              $$ = newnode((node){kSym, {.symbol = {strdup($1)}}});
                            }
  | call                    { 
                              // IMPLEMENT LATER
                              $$ = 0;
                            }

number:
  "INTEGER"                 { $$ = (double) $1; }
  | "DECIMAL"               { $$ = $1;          }

