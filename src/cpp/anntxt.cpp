#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "anntxt.h"
#include "location.h"

using std::cerr;
using std::cout;
using std::cin;
using std::endl;

using namespace ani;

// /////////////////////////////////////////////////////////////////////
//
// Method: Constructor : Initializes annotation structure for given volume size.
//
// Parameters:  
//            none
//
// Returns :
//
// Exceptions:
//
// /////////////////////////////////////////////////////////////////////



bool annotation::parseRead(std::iostream& fs) {

  string line;

  // read line

  while( !fs.eof() )
  {
    // Parse the line

  }
}



bool annotation::parseLine(string& line) {

  // Read token
  std::istringstream stok(line);
  token_process& tp = ta.tp[0];
  string tok;

  stok >> tok;
  int index = ta.find(tok);

  tp = ta.tp[index];
  tp.func(stok); //TODO acomodar a la variable correcta

  return false;
}


tokenp_array::tokenp_array(int nro) {
 
  tp = new token_process[nro];
   
}


int tokenp_array::find(string tok) {

  if ( tok[0] != '-' )
    return -1;

  if ( tok[1] > 'a' && tok[1] < 'z')
    return (int)(tok[1] - 'a');
   
  if ( tok[1] > 'A' && tok[1] < 'Z' )
    return (int)(tok[1] - 'A');
   
  return -1;
}


token_process::token_process(std::string token, bool (*func)(std::istringstream&))
{

  this->func = func;
  this->token = token;

}
