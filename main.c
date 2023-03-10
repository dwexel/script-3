#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// generated files
#include "parser.h"
#include "lexer.h"

//symbol table
#include "sym.h"
symrec *globals;

#include "stack.h"
stack *call_stack;

int yyparse(yyscan_t scanner);
int compile(const char *source) {
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (yylex_init(&scanner)) {
        return 1; /* could not initialize */
    }

    state = yy_scan_string(source, scanner);
    if (yyparse(scanner)) {
        return 2; /* error parsing */
    }
    
    yy_delete_buffer(state, scanner);
    yylex_destroy(scanner);
    return 0;
}

void catfile(char* destination, int size, char *filename) {
	FILE *f = fopen(filename, "r");
    if (f == NULL) perror("file not found");
	char line[100];
	while (fgets(line, 100, f)) {
        strcat(destination, line);
    }
    fclose(f);
}

int main(int argc, char** argv) {
    // ++argv;
    // --argc;

    call_stack = new_stack();

    char file[800];
    catfile(file, 800, "./test/test.txt");
    int code = compile(file);

    // while (!empty(call_stack)) {
    //     T call = pop(call_stack);
    //     printf("call name: %s\n", call.name);
    // }


    return code;
}
