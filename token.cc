/*Prog 4
  Mohammad Hossain (Rafsan)
  mmh318
  This is the token.cc file.The definition of the lexer class is in
  this file*/














#include "token.h"
#include <iostream>


using namespace std;

  Token::Token(){ type =0; pos=0;line =1; lexme ="";}
  Token::Token(int tp,int ps, int ln,string lex):type(tp),pos(ps),line(ln),lexme(lex){}
  
