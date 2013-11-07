#ifndef __TMAP_ANI_H
#define __TMAP_ANI_H

#include <string>
#include <iostream>
#include "tissue.h"

namespace ani
{

#define X 3000

  typedef enum { T_BONE=0, T_DENTITION, T_SPEC, T_OTHER } tissues;

  class tissueMap 
  {
    private:
    
    tissue* tmap;
    int nmaps;

    public:

    tissueMap();
    ~tissueMap();

    tissue& getTissue(tissues t);

  };

}

#endif // TISSUE_ANI_H
