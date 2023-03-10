%{
  #include <stdbool.h>

  // bridges
  #include <parser.h>
  #include <lexer.h>
  #include <math.h>

  void yyerror(yyscan_t scanner, const char *msg) 
  {
    fprintf(stderr, "Error: %s\n", msg);
  }

  // #define error
%}

%define api.pure
%lex-param   { void* scanner }
%parse-param { void* scanner }

%code requires {
  #include <sym.h>
  #include <stack.h>
}

%union {
  int ival;
  double fval;
  char sval[100];
  
  symrec* sympointer;
  node* exprpointer;
  void* none;
}

%token TOKEN_LPAREN   "("
%token TOKEN_RPAREN   ")"
%token TOKEN_PLUS     "+"
%token TOKEN_STAR     "*"
%token TOKEN_HYPH     "-"
%token TOKEN_FSLASH   "/"
%token TOKEN_LBRACE   "{"
%token TOKEN_RBRACE   "}"
%token TOKEN_COMMA    ","
%token TOKEN_KEYWORD_FN "fn"
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
                              if (yychar == YYEOF) printf("input parsed\n"); 
                            }

input:
  | input line

line:
  call                      {
                              $$ = NULL;
                            }
  | def                     { 
                              $$ = NULL; 
                            }


call:
  "ID" "(" params ")"         { 
                                printf("call parsed, args:\n");
                                print_list($3);
                                
                                if (strlen($1) >= 100)
                                  perror("name too long");
                                
                                T call;
                                strcpy(call.name, $1);
                                push(call_stack, call);
                              }

def:
  "fn" "ID" "(" params ")" expr "end"     {
                                            // printf("def parsed, params:\n");
                                            // print_list($4);
                                            // putsymlist(globals, $2, 0);
                                            // print_node($6);

                                            printf("test evaluation:\n");
                                            printf("%lf\n", evaluate_node($6));

                                            $$ = (symrec*)0;
                                          }

params:
                            { $$ = NULL;                   }
  | "ID"                    { $$ = addtolist(NULL, $1, 0); }
  | params "," "ID"         { $$ = addtolist($1, $3, 0);   }

expr:
  number                    {
                              double n = $1;
                              $$ = newnode((node){kNum,  {n}}); 
                            }
  | expr "-" expr           {
                              node *left = $1;
                              node *right = $3;

                              //printf("left: %lf\n", left->e.number.value);
                              //printf("right: %lf\n", right->e.number.value);

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

