#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "location.h"
#include "tmap.h"
#include "volume.h"
#include "bbox.h"

using std::cerr;
using std::cout;
using std::cin;
using std::endl;

using namespace ani;

// /////////////////////////////////////////////////////////////////////
//
// Method: Constructor : Initializes object structure for given volume size.
//
// Parameters:  
//            none
//
// Returns :
//
// Exceptions:
//
// /////////////////////////////////////////////////////////////////////

bbox::bbox()
{
  ;
};

bbox::~bbox()
{
  ;
};


bool bbox::setStartPoint(location& l)
{
  this->spoint = l;
  return true;
};

bool bbox::setEndPoint(location& l)
{
  this->epoint =l;
  return true;
};

/*bool setTissue(tissue& t)
{
  
};
*/

float calcProbDist(int w1, int w2, int value)
{
  float m;
  float d; 
  float t;

  if ( value <= w2 && value >= w1 ) return 1.0;
  if ( value <= w1 || value >= w2 ) return 0.0;

  d = std::abs((float) w1 + w2)/2;
  m = std::abs(d - (float)value);

  t = 1 - (m / d)/2;

  t = ( t >= 0.45) ? t : 0.0; //t/2;

  //cerr << "prob using d:" << d << " m:" << m << "for :"<< value << "in [" << w1 << "," << w2 << "] is:" << t <<  endl;

  return (t >= 0.0)? t : 0.0;

}

static int t = 0;

float bbox::getProb(point& p)
{
  float px, py, pz;
  float m;

  //  if (t == 0)
  //    {
      px = getAxisProb(X_AXIS,p.x);
      py = getAxisProb(Y_AXIS,p.y);
      pz = getAxisProb(Z_AXIS,p.z);  
      //    t++;

      // cerr << "Box prob for: [" << p.x << "," << p.y << "," << p.z << "]:" << px << endl; 
      //      cerr << "Getting probs:[" << px << "," << py << "," << pz << "] Totalling: " << 1.0/3.0 * (px + py + pz) << endl;

      //    }

      m = px + py + pz - 2;

      return (m > 0)? m : 0.0 ;
};
 
float bbox::getAxisProb(axis dim, int value)
{
  float p = 0.0;
  /*  
  cerr << "p1:" << calcProbDist(2,5,3) << endl;
  cerr << "p2:" << calcProbDist(2,5,4) << endl;
  cerr << "p3:" << calcProbDist(2,6,3) << endl;
  cerr << "p3.5:" << calcProbDist(2,6,4) << endl;
  cerr << "p3.75:" << calcProbDist(2,6,5) << endl;
  cerr << "p4:" << calcProbDist(2,6,2) << endl;
  cerr << "p5:" << calcProbDist(2,6,6) << endl;
  cerr << "p6:" << calcProbDist(2,6,1) << endl;
  cerr << "p7:" << calcProbDist(2,6,7) << endl;
  cerr << "p8:" << calcProbDist(3,7,1) << endl;
  cerr << "p9:" << calcProbDist(3,7,10) << endl;
  cerr << "p10:" << calcProbDist(3,7,26) << endl;
  */  
  switch(dim)
  {
    case X_AXIS:
      p = calcProbDist(spoint.p.x, epoint.p.x, value);
      break;
    case Y_AXIS:
      p = calcProbDist(spoint.p.y, epoint.p.y, value);
      break;
    case Z_AXIS:
      p = calcProbDist(spoint.p.z, epoint.p.z, value);
      break;
    default:
      return 0.0;
  }

  return p;

};



