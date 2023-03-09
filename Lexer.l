%{
  #include <stdio.h>
  #include <math.h>
  #include <string.h>
  // headers
  #include <parser.h>
%}

whitespace [ \r\n\t]
digit [0-9]
alpha [A-Za-z_]
id    {alpha}*

%%

{whitespace}*         { continue; }
{digit}+              { 
                        yylval->ival = atoi(yytext);
                        return TOKEN_INTEGER;
                      }

{digit}+"."{digit}*   {
                        yylval->fval = atof(yytext);
                        return TOKEN_DECIMAL;
                      }

fn                    { return TOKEN_KEYWORD_FN;  }
end                   { return TOKEN_KEYWORD_END; }

{id}        {
              // yylval->sval = yytext;
              strcpy(yylval->sval, yytext);
              // printf("identifier: %s\n", yylval->sval);
              return TOKEN_ID;
            }

"+"         { return TOKEN_PLUS;   }
"*"         { return TOKEN_STAR;   }
"-"         { return TOKEN_HYPH;   }
"/"         { return TOKEN_FSLASH; }

"("         { return TOKEN_LPAREN; }
")"         { return TOKEN_RPAREN; }
"{"         { return TOKEN_LBRACE; }
"}"         { return TOKEN_RBRACE; }

","         { return TOKEN_COMMA;  }



.           {
              printf("unrecognized character: %s\n", yytext);
            }
