#ifndef __ANNTXT_ANI_H
#define __ANNTXT_ANIH_

#include <string>
#include <iostream>
#include "location.h"

using std::string;

namespace ani
{
  
  class token_process {

  public:

    std::string token;
    bool (*func) (std::istringstream&);

    token_process(std::string token = "", bool (*func)(std::istringstream&) = NULL);

  };

  class tokenp_array {

  public:
    
    int no;
    token_process *tp;

    tokenp_array(int nro = 0);
    int find(string tok);
  };


  class annotation {

  private:
  
    point seed; 
    tokenp_array ta;

  public:

    bool parseRead(std::iostream& fs);
    bool parseLine(std::string& Line);
    
    annotation();

  };
  
}
#endif //__ANNTXT_ANI_H
