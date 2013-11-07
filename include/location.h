#ifndef __LOCATION_ANI_H
#define __LOCATION_ANI_H

#include <string>
#include <iostream>

namespace ani
{
  
  class point
  {
    public:
       int x;
       int y;
       int z;

       point(){x=y=z=0;};
       ~point(){;};
    
       inline void set(int x, int y, int z){this->x = x; this->y=y; this->z=z;};
  };

  class ppoint
  {
  public:
    short i;
    int   t;
    float p[5];
    float pt;
    float o;

    ppoint(){i=0;o=0.0;p[0]=p[1]=p[2]=p[3]=p[4]=0.0;t=0;pt=0.0;};
    ~ppoint(){;};
  };

  class location
  {
  public:
  
    point p;

    location(){;};
    ~location(){;};

    inline void setPoint(int x, int y, int z) {p.set(x,y,z);}
  };

}

#endif //_LOCATION_ANI_H
