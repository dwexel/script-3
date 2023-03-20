
FILES = lexer.c parser.c main.c symstack.c

OBJS = $(FILES:%.c=obj/%.o)

FLEX = flex
BISON = bison

CC = gcc
#CFLAGS = -I"C:\Users\dexte\Documents\GitHub\script-3\include"
CFLAGS = -I"include"
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
	rm -f *~ lexer.c parser.c
#	del lexer.c parser.c

cleanobj:
	rmdir /s /q ".\obj"
	mkdir ".\obj"

run:
	build/main

.PHONY = clean cleanobj run