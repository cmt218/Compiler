# CSE109
# Cole Tomlinson
# cmt218
# Lexer program
# Program 4

OBJ = lextest.o token.o lexer.o
OPTS = -g -c -Wall -Werror

lextest: $(OBJ)
	g++ -o lextest $(OBJ)

lextest.o: lextest.cc
	g++ $(OPTS) lextest.cc

token.o: token.cc
	g++ $(OPTS) token.cc

lexer.o: lexer.cc
	g++ $(OPTS) lexer.cc

clean:
	rm lextest *.o