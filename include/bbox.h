#ifndef __BBOX_ANI_H
#define __BBOX_ANI_H

#include <string>
#include <iostream>
#include "location.h"
#include "tmap.h"

namespace ani
{

  typedef enum { X_AXIS, Y_AXIS, Z_AXIS } axis;

  class bbox
  {
    
  private:
    location spoint;
    location epoint;
    //1tissue   tiss;

  public:

    bbox();
    ~bbox();

    bool setStartPoint(location& l);
    bool setEndPoint(location& l);

    //bool setTissue(tissue& t);
    //tissue& getTissue();

    float getProb(point& p);

    float getAxisProb(axis dim, int value);


  };

}

#endif //_BBOX_ANI_H
