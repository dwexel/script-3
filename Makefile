FILES = lexer.c parser.c main.c sym.c stack.c
OBJS = $(FILES:%.c=obj/%.o)

FLEX = win_flex
BISON = win_bison

CC = gcc
CFLAGS = -I"C:\Users\dexte\Documents\GitHub\c-notes\script 3\include"
HEADERS = include

build/main: $(OBJS)
	$(CC) $^ -o build/main

obj/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

lexer.c lexer.h: lexer.l
	$(FLEX) --outfile=lexer.c --header-file=$(HEADERS)/lexer.h --nodefault \
		--reentrant --bison-bridge --nounistd --noyywrap --never-interactive --warn Lexer.l

parser.c parser.h: parser.y lexer.c
	$(BISON) --output=parser.c --defines=$(HEADERS)/parser.h parser.y

clean:
# rm -f *.o *~ lexer.c lexer.h parser.c parser.h main
	del lexer.c parser.c
