
#include "parser.h"
#include <cstring>
#include <stack>
#include <iostream>




//parser.cc:16:60: error: conversion from ‘Parser::Operation’ to non-scalar type ‘std::string {aka std::basic_string<char>}’ requested
   //  JUMP, JUMPF, JUMPT, CALL, RET,FUNC, PRINTF, LABEL, SEQ };

const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV", 
    "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE", 
    "AND", "OR", 
    "PUSHL", "PUSHV", "STORE", 
    "JUMP", 
    "JUMPF", "JUMPT", "CALL", "RET","FUNC", "PRINTF", "LABEL", "SEQ" };



  std::vector<string> Parser::process(vector<string> v){

    for(unsigned int i =0; i < v.size()-1; i++){

      for (unsigned int j =1 ; j < v.size(); j++){

         if(v[i] == v[j]){
          v.erase(v.begin() + i);
         vector<string>(v.begin(), v.end()).swap(v);
       }
      }
    }
    
  return v;
  }









void Parser::vardefs(TreeNode *node){


  
string vname;



 
 if(node != NULL){

    if (node->op == STORE || node->op == PUSHV ){ 

          vname = node->val;
          
         // mapping[vname] = vanme;
          vec.push_back(vname);
          process(vec);
    }

} 

else {

vardefs(node->leftChild);

vardefs(node->rightChild);
           
}

          
}




void Parser::geninst(TreeNode *node) {
  string fmt = "";
  int nparams = 0;
  if (node != NULL) {
    geninst(node->leftChild);
    geninst(node->rightChild);
    switch (node->op) {
    case SEQ:
      break;
    case PUSHV:
      emit("  push qword[" + node->val + "]");
      break;
    case PUSHL:
      emit("  mov rax," + node->val);
      emit("  push rax");
      break;
    case ADD:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  add rax,rbx");
      emit("  push rax");
      break;
    case SUB:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  sub rax,rbx");
      emit("  push rax");
      break;
    case MULT:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  imul rbx");
      emit("  push rax");
      break;
    case DIV:
      emit("  mov rdx,0");
      emit("  pop rbx");
      emit("  pop rax");
      emit("  idiv rbx");
      emit("  push rax");
      break;
    case STORE:
      emit("  pop qword[" + node->val + "]");
      break;
    case AND:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  and rax,rbx");
      emit("  push rax");
      break;
    case OR:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  or  rax,rbx");
      emit("  push rax");
      break;
    case LABEL:
      emit(node->val);      
      break;
    case ISEQ:
      cond("je");
      break;
    case ISNE:
      cond("jne");
      break;
    case ISLT:
      cond("jl");
      break;
    case ISLE:
      cond("jle");
      break;
    case ISGT:
      cond("jg");
      break;
    case ISGE:
      cond("jge");
      break;
    case JUMP:
      emit("  jmp " + node->val);
      break;
    case JUMPF:
      emit("  pop rax");
      emit("  cmp rax,0");
      emit("  je " + node->val);
      break;
    case JUMPT:
      emit("  pop rax");
      emit("  cmp rax,0");
      emit("  jne " + node->val);
      break;
    case CALL:
      emit("  call " + node->val);
      emit("  push rax");      
      break;
    case FUNC:
      currentFunc = node->val;
      emit(currentFunc);
      if (currentFunc != "main:")
        emit("  pop r15");      
      break;
    case RET:
      emit("  pop rax");
      if (currentFunc != "main:")
         emit("  push r15");            
      emit("  ret");
      break;
    case PRINTF: 
      fmt = node->val;
      nparams = fmt.at(0) - '0';
      fmt = "`" + fmt.substr(1) + "`";
      fmts[nfmts++] = fmt;
      emit("  mov    rdi,fmt" + itos(nfmts));
      if (nparams == 5) {
  emit("  pop   r9");
  --nparams;
      }
      if (nparams == 4) {
  emit("  pop   r8");
  --nparams;
      }
      if (nparams == 3) {
  emit("  pop   rcx");
  --nparams;
      }
      if (nparams == 2) {
  emit("  pop   rdx");
  --nparams;
      }
      if (nparams == 1) {
  emit("  pop   rsi");
      }
      emit("  mov    rax,0");
      emit("  push   rbp");
      emit("  call   printf");
      emit("  pop    rbp");       
      break;
    default:
      cerr << "In geninst: Unknown operation " << ops[node->op] << endl;
      exit(1);
      break;      
    }
  }
}










      


void Parser::genasm(TreeNode *node) {
  int  n = 1;
  cout << "  global main" << endl;
  cout << "  extern printf" << endl << endl;
  cout << "  segment .bss" << endl;
  vardefs(node);
  for (int i=1; i < n; ++i) {
    cout << "  " << vec[i] << " resq 1" << endl;
  }
  cout << endl << "  section .data" << endl;
  cout << "fmt:    db '%ld ', 0" << endl;
  cout << "endl:    db 10, 0" << endl << endl;
  cout << "  section .text" << endl;

  geninst(node);
}
















Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1) {
  token = lexer.nextToken();
}

Parser::~Parser() {
}

void Parser::error(string message) {
  cerr << message << " Found " << token.getLexme() << " at line " << token.getLine() << " position " << token.getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.getType() != tokenType)
    error(message);
}








// This function is used to call the function, the string param is the function IDENT or name
//<funcall> ::= IDENT LPAREN [<expression> { COMMA <expression> }] RPAREN
Parser::TreeNode* Parser::funcall(string fname) {

    TreeNode* funcallNode = NULL;

    check(Token::LPAREN," '(' expected");

    token = lexer.nextToken();

    if(token.getType() != Token::RPAREN){

      funcallNode = expression();

      while(token.getType() == Token::COMMA){

        token = lexer.nextToken();

        funcallNode = new TreeNode(SEQ,funcallNode, expression());
       // token = lexer.nextToken();
       }
      
     }
    check(Token::RPAREN, "expression");

    funcallNode = (funcallNode == NULL) ? new TreeNode(CALL, fname) :
    
    new TreeNode(SEQ, funcallNode , new TreeNode(CALL,fname));
  
    token = lexer.nextToken();
 
        
    return funcallNode; 
}







//<factor> ::= LPAREN <expression> RPAREN | INTLIT | IDENT | <funcall>

Parser::TreeNode* Parser::factor() {

		TreeNode* node;

 if(token.getType()==Token::LPAREN )
    {
      
       token = lexer.nextToken();

       node = expression();
     
       
       check(Token::RPAREN,"ERROR");

      
       token=lexer.nextToken();
         
    }
        


  else if(token.getType()==Token::IDENT || token.getType()==Token::INTLIT)
    {
            string vname;
            string intlit ;
    	   switch(token.getType()){

         case Token::IDENT :
         
          vname  = token.getLexme();

          token=lexer.nextToken();

         if(token.getType() == Token::LPAREN){

          node = funcall(vname);
         
         }
         else
         
         node = new TreeNode(PUSHV,vname);

         token=lexer.nextToken();

         
         break;
//parser.cc:387:22: error: jump to case label [-fpermissive]
          case Token::INTLIT :
        

          intlit = token.getLexme();
         
         token=lexer.nextToken();

         node = new TreeNode(PUSHL, intlit);

         
         break;
    	   }
    }
    
   
 //  token=lexer.nextToken();
   return node; 

  }





//<term> ::=  <factor> { TIMES <factor> | DIVIDE <factor> }

Parser::TreeNode* Parser::term() {
    
    TreeNode* node= factor();

  while(token.getType()==Token::TIMES || token.getType()==Token::DIVIDE)
    {
       switch(token.getType()){

       case Token::TIMES:
	     node = new TreeNode(MULT,node,factor());
	     
	     break;
       case Token::DIVIDE:
	     node = new TreeNode(DIV,node,factor());
       
	     break;
	  }

    token=lexer.nextToken(); 

    }
   
  return node ; 

}


//<expression> ::= <term> { PLUS <term> | MINUS <term> }

Parser::TreeNode* Parser::expression() 
 {
    
    TreeNode* node= term();

  while(token.getType()==Token::PLUS || token.getType()==Token::MINUS)
    {
       switch(token.getType()){

       case Token::PLUS:

       node = new TreeNode(ADD,node,term());
      
       break;
       case Token::MINUS:

       node = new TreeNode(SUB,node,term());
      
       break;
    }
    token=lexer.nextToken(); 
    }
   
  return node ; 

}


//<relationalexpression> ::= <expression> [ EQ <expression> | LT <expression> | LE <expression> | GT <expression> | 
//GE <expression> | NE <expression> ]


Parser::TreeNode* Parser::relationalExpression() {



  TreeNode* node =  expression();

  if(token.getType() == Token::EQ || token.getType() == Token::LT || token.getType() == Token::LE || token.getType() == Token::GT ||

  token.getType() == Token::GE, token.getType() == Token::NE){

      switch(token.getType()){
      case Token::EQ :

      token=lexer.nextToken();
      
      node = new TreeNode(ISEQ,node,expression());

      break;

      case Token::LT :

      token=lexer.nextToken();
      
      node = new TreeNode(ISLT,node,expression());
      
      break;
    
      case Token::LE :

      token=lexer.nextToken();
      
      node = new TreeNode(ISLE,node,expression());

      break;

      case Token:: GT :
  
      token=lexer.nextToken();
      
      node = new TreeNode(ISGT,node,expression());
      
      break;
    
    
     case Token::GE :
      
     token=lexer.nextToken();
  
    node = new TreeNode(ISGE,node,expression());
    
    break;
    
    case Token::NE :
    
    token=lexer.nextToken();
      
    node = new TreeNode(ISNE,node,expression());
    
    break;
    }
 }    
  
  token=lexer.nextToken();
  return node;
}


//<logicalexpression> ::= <relationalexpression> { AND <relationalexpression> | OR <relationalexpression> }
Parser::TreeNode* Parser::logicalExpression() {

	 TreeNode* node = relationalExpression();

  while(token.getType()==Token::AND || token.getType()==Token::OR)
    {
      
       switch(token.getType()){
        
       case Token::AND :
        
       node = new TreeNode(AND,node,relationalExpression());
	     
       break;
	     
       case Token::OR:
       
       node = new  TreeNode(OR,node,relationalExpression());
	    
      break;
	    }
      
      token=lexer.nextToken();
    
  }

  token = lexer.nextToken();
  return node;
}


//<returnstatement> ::= RETURN <logicalExpression> SEMICOLON
 
  Parser::TreeNode* Parser::returnStatement(){
   
      TreeNode* node;

     if(token.getType()== Token::RETURN){
        
        token = lexer.nextToken();
        TreeNode* rt = new TreeNode(RET);
        node = logicalExpression();

        node = new TreeNode(SEQ,node,rt);

        check(Token::SEMICOLON,"ERROR");

     }


    return node;

    }


//<ifstatement> ::= IF LPAREN <logicalexpression> RPAREN <block> [ELSE <block>]

Parser::TreeNode* Parser::ifStatement(){

  TreeNode* node;

  string L1 = makeLabel();

  string L2 = makeLabel();

  if(token.getType()==Token::IF){

      token = lexer.nextToken();

      check(Token::LPAREN,"ERROR");

      token = lexer.nextToken();

      TreeNode* logic  = logicalExpression();

      TreeNode* jumpf = new TreeNode(JUMPF,L1);
     
   
     
     /*TreeNode* node0 */ node = new TreeNode(SEQ,logic,jumpf);

      check(Token::RPAREN,"ERROR");
          
            token = lexer.nextToken();

            TreeNode* codeBlock = block();

           /*  node1 */node = new TreeNode(SEQ,node,codeBlock);

            TreeNode* jump = new TreeNode(JUMP,L2);

          /* node2 */ node = new TreeNode(SEQ,node,jump); 

            if(token.getType()== Token::ELSE){

                token = lexer.nextToken();

                TreeNode* label1 = new TreeNode(LABEL,L1);

                /* node3 */ node = new TreeNode(SEQ,node,label1);

                 TreeNode* codeBlock2 = block();

                /* node4 */node = new TreeNode(SEQ,node,codeBlock2);

                 TreeNode* label2 = new TreeNode(LABEL,L2);

                 /* node5 */ node = new TreeNode(SEQ,node,label2);

                }  else

                {
                  TreeNode* label2 = new TreeNode(LABEL,L2);

                  node = new TreeNode(SEQ,node,label2);

                 }
          }

        


  return node;
}



//<whilestatement> ::= WHILE LPAREN <logicalexpression> RPAREN <block>

Parser::TreeNode* Parser::whileStatement(){

   TreeNode* node;

  if(token.getType()==Token::WHILE){

     token = lexer.nextToken();

     check(Token::LPAREN,"ERROR");

     token = lexer.nextToken();

     string L1 = makeLabel();

     TreeNode* logic = logicalExpression();

     check(Token::RPAREN,"ERROR");
   
     token = lexer.nextToken();

     string L2 = makeLabel();

     TreeNode* jumpf = new TreeNode(JUMPF,L2);

      node = new TreeNode(SEQ,logic,jumpf);

      node = new TreeNode(SEQ,node,block());

      TreeNode* jump = new TreeNode(JUMP,L1);

      node = new TreeNode(SEQ,node,jump);

  }
      
return node;
}




//<assignstatement> ::= IDENT ASSIGN <logicalexpression> SEMICOLON

Parser::TreeNode* Parser::assignStatement(){
   
    TreeNode* node;

  if (token.getType()== Token::IDENT){

     string vname = token.getLexme();

     token = lexer.nextToken();
     
     node = new TreeNode(STORE,vname);
    
     if(token.getType()== Token::ASSIGN){
     
     token = lexer.nextToken();
     
     node = new TreeNode(SEQ,node,logicalExpression());

     check(Token::SEMICOLON,"ERROR");

    }
  }

return node;


}








//<statement> ::= <printfstatement> | <assignstatement> | <whilestatement> | <ifstatement> | <returnstatement>

Parser::TreeNode* Parser::statement(){

  TreeNode* node;
if(token.getType() == Token::ASSIGN || token.getType() == Token::PRINTF || token.getType() == Token::WHILE ||

token.getType() == Token::IF || token.getType() == Token::RETURN){

  switch(token.getType()){

  case Token::ASSIGN :

  node =  assignStatement();

  break;
  
  case Token::PRINTF:

  node =  printfStatement();

  break;

  case Token::WHILE :

  node = whileStatement();

  break;

  case Token::IF :

  node = ifStatement();

  break;

  case Token::RETURN:

  node = returnStatement();

  break;

  }
}
  return node;

 }



//<parameterdef> ::= IDENT
 Parser::TreeNode* Parser::parameterdef(){
  
  TreeNode* node;
  
  if(token.getType()== Token::IDENT){
    
    string vname = token.getLexme();
    
    token = lexer.nextToken();

    node = new TreeNode(STORE,vname);
  }
  return node;
 }


//<parameterdefs> ::= [ <parameterdef> ] { COMMA <parameterdef> }

Parser::TreeNode* Parser::parameterdefs(){

  TreeNode* node = parameterdef();

   token = lexer.nextToken();

  while(token.getType() == Token::COMMA){

       token = lexer.nextToken();

       node = new TreeNode(SEQ,node,parameterdef());
     
      }
     return node;
 } 

//<function> ::= FUNCTION IDENT LPAREN <parameterdefs> RPAREN <block>

 Parser::TreeNode* Parser::function() {

  TreeNode*functionNode = NULL;

  check(Token::FUNCTION, "Missing function keyword.");

  token = lexer.nextToken();

  check(Token::IDENT, "Missing function name.");

  string fname = token.getLexme();

  token = lexer.nextToken();

  check(Token::LPAREN, "Missing (");

  token = lexer.nextToken();

  std::stack<std::string> params;

  std::string vname;

  if (token.getType() != Token::RPAREN) {

    check(Token::IDENT, "Expecting variable");

    vname = token.getLexme();

    params.push(vname);

    token = lexer.nextToken();

    while (token.getType() == Token::COMMA) {

      token = lexer.nextToken();

      check(Token::IDENT, "Expecting variable");

      vname = token.getLexme();

      params.push(vname);



      token = lexer.nextToken();

    }

  }

  check(Token::RPAREN, ") expected");

  token = lexer.nextToken();

  TreeNode* blockNode = block();

  functionNode = new TreeNode(FUNC, fname + ":");

  while (!params.empty()) {

    vname = params.top();
       params.pop();

    functionNode = new TreeNode(SEQ, functionNode, new TreeNode(STORE, vname));


  }

  functionNode = new TreeNode(SEQ, functionNode, blockNode);

  //functionNode = new TreeNode(SEQ, functionNode, new TreeNode(RET));                

  return functionNode;


}




// LBRACE { <statement> } RBRACE
Parser::TreeNode* Parser::block(){

  TreeNode* node = new TreeNode(SEQ);

 // if(token.getType() == Token::LBRACE){

 check(Token::LBRACE, " '{' expected ");
    
     token = lexer.nextToken();
     
     node = statement();

   while(token.getType() != Token::ENDOFFILE){

      token = lexer.nextToken();
    
      node = new TreeNode(SEQ,node,statement());
  
  }
 
  check(Token::RPAREN,"'}' expected");
  
  return node;
}


//<compilationunit> ::= { <function> }
Parser::TreeNode* Parser::compilationunit(){
    
    TreeNode* node = function();
 
   while(token.getType() == Token::FUNCTION ){
    
     node = new TreeNode(SEQ,node,function());
    
   }

   return node;
 }
 

Parser::TreeNode* Parser::printfStatement() {
  TreeNode* paramList = NULL;
  int nparams = 0;
  check(Token::PRINTF, "Expecting printf");
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();
  check(Token::STRINGLIT, "Expecting string literal");
  string formatString = token.getLexme();
  token = lexer.nextToken();
 
  if (token.getType() == Token::COMMA) {
    token = lexer.nextToken();
    paramList = expression();
    ++nparams;
    while (token.getType() == Token::COMMA) {
      token = lexer.nextToken();
      paramList = new TreeNode(SEQ, paramList, expression());
      ++nparams;
    }
  }
 
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  check(Token::SEMICOLON, "Expecting ;");
  token = lexer.nextToken();
  TreeNode* printStatement = new TreeNode(SEQ, paramList,
            new TreeNode(PRINTF, itos(nparams) + formatString));
  return printStatement;
}
