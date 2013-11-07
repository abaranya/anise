#ifndef __TISSUE_ANI_H
#define __TISSUE_ANI_H

#include <string>
#include <iostream>

namespace ani
{

  class tissue 
  {
    private:
    
    int min;
    int max;
    int mean;
    int stddev;

    public:

    tissue();
    tissue(int m, int M, int acg, int std);
    ~tissue();

    void setTissue(int m, int M, int acg, int std);
    float prob(int i);
  };

}

#endif // TISSUE_ANI_H
