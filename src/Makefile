parser: parser.y lexer.l
		bison -d parser.y -v 
		flex lexer.l
		gcc -o parser parser.tab.c lex.yy.c 

.PHONY: clean
clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h parser parser.output
