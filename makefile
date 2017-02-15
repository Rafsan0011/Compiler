OPTS= -g -c -Wall -Werror
efile = myprog
OBJ= token.o lexer.o parser.o lextest.o

myprog: $(OBJ)
	 g++ -o $(efile) $(OBJ)



lextest.o:lextest.cc
	g++ $(OPTS) lextest.cc

token.o:token.cc token.h 
	g++ $(OPTS) token.cc
  
lexer.o: lexer.cc lexer.h token.h 
	g++ $(OPTS) lexer.cc




parser.o:parser.cc lexer.h token.h parser.h
	g++ $(OPTS) parser.cc


clean:
	rm -f $(OBJ) $(efile)
