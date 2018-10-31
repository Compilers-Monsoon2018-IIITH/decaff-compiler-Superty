parser: parser.y lexer.l
		bison -o parser.tab.cc -d parser.y -v
		flex -o lex.yy.cc lexer.l
		gcc -o parser parser.tab.c lex.yy.c 

.PHONY: clean
clean:
	rm -f lex.yy.cc parser.tab.cc parser.tab.hh parser parser.output
