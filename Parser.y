%{
  #include <math.h>
  #include <stdbool.h>

  // bridges
  #include <parser.h>
  #include <lexer.h>


  // #include <deque.h>
  // #include <stack.h>
  
  #include <sym.h>
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
  #include <sym.h>

}

%union {
  int ival;
  double fval;
  char sval[100];

  symrec* sympointer;
  node* exprpointer;
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



%type <exprpointer> expr
%type <fval> number

%type <none> def
%type <none> call

%type <sympointer> params

%left "+"
%left "*"

%%

start:
  input                     {
                              if (yychar == YYEOF) {
                                //printf("input parsed\n"); 
                                //printf("defs: \n");
                                //print_list(defs);
                              }
                            }

input:
  | input line

line:
  call                      { }
  | def                     { }


call:
  "ID" "(" params ")"         {
                                // printf("call parsed, args:\n");
                                // print_list($3);

                                symrec *def = getfromlist(defs, $1);
                                if (def) {
                                  printf("function exists\n\n");
                                  print_node(def->value.exprpointer);
                                  printf("value: \t%lf\n", evaluate_node(def->value.exprpointer));
                                  
                                }
                              }

def:
  "fn" "ID" "(" params ")" expr "end"     {

                                            defs = addtolist(defs, $2, SYM_DEF);
                                            defs->value.exprpointer = $6;
                                          }

params:
                            { $$ = NULL;                   }
  | "ID"                    { $$ = addtolist(NULL, $1, SYM_PARAM); }
  | params "," "ID"         { $$ = addtolist($1, $3, SYM_PARAM);   }

expr:
  number                    {
                              double n = $1;
                              $$ = newnode((node){kNum,  {n}}); 
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

number:
  "INTEGER"                 { $$ = (double) $1; }
  | "DECIMAL"               { $$ = $1;          }

