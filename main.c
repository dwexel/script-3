#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// generated files
#include "parser.h"
#include "lexer.h"


// my types
#include "symstack.h"


// symrec *globals;

// #include "stack.h"
// #include "deque.h"


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

    // printf("running...\n\n");
    // call_stack = new_stack();


    // symrec test;
    // test.name = "start";
    // test.value.f = 0.0;
    // test.next = NULL;
    // addtolist_end(&test, "heelo");
    // addtolist_end(&test, "hi");
    // print_list(&test);

    // symrec *test = addtolist(NULL, "start", 0);
    // addtolist_end(test, "heelo");
    // addtolist_end(test, "hi");
    // print_list(test);



    char file[800];
    catfile(file, 800, "./test/test.txt");
    int code = compile(file);


    // print_stack();


    return code;
}
