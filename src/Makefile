parser: parser.y lexer.l
		bison -o parser.tab.cc -d parser.y -v
		flex -o lex.yy.cc lexer.l
		g++ -o parser parser.tab.cc lex.yy.cc -std=c++14

.PHONY: clean
clean:
	rm -f lex.yy.cc parser.tab.cc parser.tab.hh parser parser.output
