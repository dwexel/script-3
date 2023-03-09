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
%}

%define api.pure
%lex-param   { void* scanner }
%parse-param { void* scanner }

%code requires {
  #include <stdio.h>
  #include <sym.h>
}

%union {
  int ival;
  double fval;
  char sval[100];
  
  symrec*   sympointer;
  void*     none;
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

%type <fval> expr
%type <fval> number

%type <sympointer> def
%type <sympointer> params

%left "+"
%left "*"

%%

start:
  input                     {
                              // if (yychar == YYEOF) printf("input parsed\n"); 
                            }

input:
  | input line

line:
  call                      {
                              $$ = NULL;
                            }
  | def                     { $$ = NULL; }


call:
  "ID" "(" params ")"         { 
                                printf("call parsed, args:\n");
                                print_list($3);
                              }





def:
  "fn" "ID" "(" params ")" expr "end"     {
                                            printf("def parsed, params:\n");
                                            print_list($4);

                                            symrec *new = (symrec *) malloc (sizeof (symrec));
                                            new->name = strdup($2);
                                            new->type = SYM_FUNCTION;
                                            new->value.f = 0;
                                            new->next = globals;
                                            globals = new;

                                            // don't return
                                            $$ = (symrec *)0;
                                          }

params:
                            {
                              $$ = NULL; 
                            }
  | "ID"                    {
                              symrec *new = (symrec *) malloc (sizeof (symrec));
                              new->name = strdup($1);
                              new->next = NULL;
                              $$ = new;
                            }
  | params "," "ID"         {
                              symrec *new = (symrec *) malloc (sizeof (symrec));
                              new->name = strdup($3);
                              new->next = $1;
                              $$ = new;
                            }

expr:
  number                    { $$ = $1;      }
  | expr "-" expr           { $$ = $1 - $3; }
  | expr "+" expr           { $$ = $1 + $3; } 
  | expr "/" expr           { $$ = $1 / $3; }
  | expr "*" expr           { $$ = $1 * $3; }
  | "(" expr ")"            { $$ = $2;      }
  | "ID"                    { 
                              printf("identifier: %s\n", $1);
                            }

number:
  "INTEGER"                 { $$ = (double) $1; }
  | "DECIMAL"               { $$ = $1;          }

