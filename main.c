#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// generated files
#include "parser.h"
#include "lexer.h"

// my types
#include "symstack.h"

// #define BUFSIZE 800


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

    if (!f) {
        perror("file not found");
        exit(1);
    }

    int read = fread(destination, 1, size, f);
    printf("file size: %i\n", read);
    fclose(f);
}

int main(int argc, char** argv) {
    ++argv;
    --argc;

    // for (int i = 0; i < argc; i++) {
    //     printf("%s\n", argv[i]);
    // }

    // printf("running...\n\n");
    // call_stack = new_stack();

    char file[800];
    
    // catfile(file, 800, "./test/test.txt");
    catfile(file, 800, argv[0]);


    int code = compile(file);
    // print_stack();
    return code;
}
