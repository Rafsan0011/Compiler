/*Prog 4
  Mohammad Hossain (Rafsan)
  mmh318
  This is the lexer.cc file.
*/







#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include "token.h"
#include "lexer.h"
#include <map>


using namespace std;


Lexer::Lexer(istream& in):input(in){
	ch = ' ';
	lineLexer =0;
	posLexer =0;

}
char Lexer::nextChar(){

char recordChar = input.get();

 if(input.eof()){
      recordChar = '#';
    }
else if(recordChar == '\n'){
	lineLexer++;
	posLexer=0;
	recordChar =' ';
}
else {
	posLexer++;
}


return recordChar;
}

Token Lexer::nextToken(){

	int type =0;
	string lexme ="";
	int line =1;
	int pos =0;

//This while loop skips the white space.
	while(isspace(ch)){
		ch = nextChar();
		}
//This if clause will look for end of file.
    if(ch == '#'){
			type = Token::ENDOFFILE;
			lexme = "End of file token";
			line = Lexer::lineLexer;
			pos = Lexer::posLexer;
	return  Token(type,pos,line,lexme);
	}

//This else if looks for alpha first and then alpha numerics.
	else if(isalpha(ch)){
		line = Lexer::lineLexer;
    	pos = Lexer::posLexer;
    	lexme += ch;
    	ch = nextChar();
	

	while(isalnum(ch) || ch == '_'){
      line = Lexer::lineLexer;
      pos = Lexer::posLexer;
      lexme += ch;
      ch = nextChar();
    }

   
  
	map <string ,int> lex; 

    lex ["if"] = Token::IF;
    lex ["else"] = Token::ELSE;
    lex ["while"] = Token::WHILE;
    lex ["function"] = Token::FUNCTION;
    lex ["var"] = Token::VAR;
    lex ["printf"] = Token::PRINTF;
    lex ["return"] = Token::RETURN;
   



 	if(lexme == "if"){
  		type = lex ["if"];
 	} 

	else if(lexme == "else"){
		type = lex ["else"];
	}
	else if(lexme == "while"){
		type = lex ["while"];
	}
	else if(lexme == "function"){
		type = lex ["function"];
	}

	else if(lexme == "var"){
		type = lex["var"];
	}
	else if(lexme == "printf"){
		type = lex["printf"];
	}

	else if(lexme == "return"){
		type = lex ["return"];
	}
    else{
        type = Token::IDENT;

    }


	
 
    return  Token(type,pos,line,lexme);
}

	else if(isdigit(ch)){

		while(isdigit(ch)){
			type = Token::INTLIT;
			line = Lexer::lineLexer;
			pos = Lexer::posLexer;
			lexme +=ch;
			ch = nextChar();
    	}
	return Token(type,pos,line,lexme);
   }

	else if (ch == '"'){

		ch = nextChar();
		while(ch != '"'  ){
			lexme = +ch;
			type = Token::STRINGLIT;
			line = Lexer::lineLexer;
			pos = Lexer::posLexer;
			ch = nextChar();
			return  Token(type,pos,line,lexme);
		}

	}


   else	if(ch == '+'){
		type = Token::PLUS;
		lexme += ch;
		line = Lexer::lineLexer;
		pos = Lexer::posLexer;
		ch = nextChar();
		return  Token(type,pos,line,lexme);
	}
	else if(ch == '-'){
		type = Token::MINUS;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();
		return Token(type,pos,line,lexme);
	}

	else if(ch == '*'){
		type = Token::TIMES;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();
		return Token(type,pos,line,lexme);
	}


	else if(ch == '/'){
		type = Token::DIVIDE;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();
		return Token(type,pos,line,lexme);
	}


	else if(ch == '='){
		char tmp = ch;
		type = Token::ASSIGN;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();
		if(ch == tmp){
		type = Token::EQ;
		lexme += ch;
		ch = nextChar();
		}
		return Token(type,pos,line,lexme);
	}


	else if(ch == '!'){
		char tmp = ch;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();
		if(ch == '=' && tmp == '!'){
		type = Token::NE;
		lexme += ch;
		ch = nextChar();
		return  Token(type,pos,line,lexme);
		}

	}




	else if(ch == '<'){
		char tmp  = ch;
		type = Token::LT;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();

		if(ch == '=' && tmp == '<'){
		type = Token::LE;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();
	}
		return Token(type,pos,line,lexme);
	}





	else if(ch == '>'){

		char tmp = ch;
		type = Token::GT;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;

		ch = nextChar();



	if(ch == '=' && tmp == '>'){
		type = Token::GE;
		lexme += ch;
	line = Lexer::lineLexer;
			pos = Lexer::posLexer;

		ch = nextChar();
	}
		return Token(type,pos,line,lexme);

	}



	else if(ch == '&'){
		char tmp = ch;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();


	if(ch == '&' && tmp =='&'){
		type = Token::AND;
		lexme += ch;
		ch = nextChar();
	}

	return Token(type,pos,line,lexme);
	}




	else if(ch == '|'){
		char tmp = ch;
		//type = Token::LT;
		lexme += ch;
	line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();


	if(ch == '|'   && tmp =='|' ){
		type = Token::OR;
		lexme += ch;
		ch = nextChar();
	}




		return Token(type,pos,line,lexme);


	}




	else if(ch == '('){
		type = Token::LPAREN;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();
			return  Token(type,pos,line,lexme);
	}




	else if(ch == ')'){
		type = Token::RPAREN;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;

		ch = nextChar();
			return Token(type,pos,line,lexme);
	}


	else if(ch == '{'){
		type = Token::LBRACE;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;

		ch = nextChar();
			return Token(type,pos,line,lexme);
	}



	else if(ch == '}'){
		type = Token::RBRACE;
		lexme += ch;
	line = Lexer::lineLexer;
			pos = Lexer::posLexer;

		ch = nextChar();
			return Token(type,pos,line,lexme);
	}



	else if(ch == ','){
		type = Token::COMMA;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;

		ch = nextChar();
			return Token(type,pos,line,lexme);
	}




	else if(ch == ';'){
		type = Token::SEMICOLON;
		lexme += ch;
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;

		ch = nextChar();
			return Token(type,pos,line,lexme);
	}

	else{

		type = Token::ERROR;

		lexme ="ERROR";
		line = Lexer::lineLexer;
			pos = Lexer::posLexer;
		ch = nextChar();
	}
		return Token(type,pos,line,lexme);


}

