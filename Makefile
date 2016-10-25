# CSE109
# Cole Tomlinson
# cmt218
# Parser program
# Program 5

OBJ = token.o lexer.o parser.o microc.o #lextest.o

OPTS = -g -c -Wall -Werror

microc: $(OBJ)
	g++ -o microc $(OBJ)

#lextest: $(OBJ)
#	g++ -o lextest $(OBJ)

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

clean:
	rm microc *.o