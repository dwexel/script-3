%{
  #include <stdio.h>
  #include <math.h>
  #include <string.h>

  // bridge headers
  #include <parser.h>
%}

whitespace [ \r\n\t]
digit [0-9]
alpha [A-Za-z_]
id    {alpha}*


%%

{digit}+              { 
                        yylval->ival = atoi(yytext);
                        return TOKEN_INTEGER;
                      }

{digit}+"."{digit}*   {
                        yylval->fval = atof(yytext);
                        return TOKEN_DECIMAL;
                      }

{id}        {
              strcpy(yylval->sval, yytext);
              return TOKEN_ID;
            }

"+"         { return TOKEN_PLUS;   }
"*"         { return TOKEN_STAR;   }
"-"         { return TOKEN_HYPH;   }
"/"         { return TOKEN_FSLASH; }

"^"         { return TOKEN_CARET;  }

"("         { return TOKEN_LPAREN; }
")"         { return TOKEN_RPAREN; }
"{"         { return TOKEN_LBRACE; }
"}"         { return TOKEN_RBRACE; }

","         { return TOKEN_COMMA;  }
"="         { return TOKEN_EQUALS; }



"//"[^\n]*            { 
                        // printf("comment\n");
                        continue;
                      }
{whitespace}*         { continue; }



.           {
              // printf("unrecognized character: %s\n", yytext);
            }
