# CSE109
# Cole Tomlinson
# cmt218
# Parser program
# Program 5

OBJ = token.o lexer.o parser.o symbolTable.o node.o link.o tri.o microc.o #tTest.o #lextest.o

OPTS = -g -c -Wall -Werror -Wno-non-template-friend



microc: $(OBJ)
	g++ -o microc $(OBJ)

#lextest: $(OBJ)
#	g++ -o lextest $(OBJ)

#tTest: $(OBJ)
#	g++ -o tTest $(OBJ)

#tTest.o: tTest.cc
#	g++ $(OPTS) tTest.cc

microc.o: microc.cc
	g++ $(OPTS) microc.cc

parser.o: parser.cc
	g++ $(OPTS) parser.cc


#lextest.o: lextest.cc
#	g++ $(OPTS) lextest.cc


token.o: token.cc
	g++ $(OPTS) token.cc

lexer.o: lexer.cc
	g++ $(OPTS) lexer.cc

symbolTable.o: symbolTable.cc symbolTable.h
	g++ $(OPTS) symbolTable.cc

node.o: node.cc node.h
	g++ $(OPTS) node.cc

link.o: link.cc link.h
	g++ $(OPTS) link.cc

tri.o: tri.cc tri.h
	g++ $(OPTS) tri.cc 


clean:
	rm microc *.o