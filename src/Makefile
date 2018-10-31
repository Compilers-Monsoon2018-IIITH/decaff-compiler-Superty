%.o: %.cc %.h 
		g++ -g -std=c++17 -c $<

parser: parser.y lexer.l ExprNode.o AstNode.o ListNode.h BlockNode.o RootNode.o driver.o AstVisitor.o TypeCheckVisitor.o
		bison -o parser.tab.cc -d parser.y -v
		flex -o lex.yy.cc lexer.l
		g++ -g -std=c++17 -o parser parser.tab.cc lex.yy.cc ExprNode.o AstNode.o BlockNode.o RootNode.o driver.o AstVisitor.o TypeCheckVisitor.o

.PHONY: clean
clean:
	rm -f lex.yy.cc parser.tab.cc parser.tab.hh parser parser.output *.o

.PHONY: test
test:
