#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "tissue.h"
#include "tmap.h"

using std::cerr;
using std::cout;
using std::cin;
using std::endl;

using namespace ani;

tissueMap::tissueMap()
{
  tmap = new tissue[T_OTHER+1];

  // Set Each Tissue;
  //tmap[T_BONE].setTissue(1500,3100,2600,250);
  tmap[T_BONE].setTissue(1500,3600,2600,250);
  //tmap[T_DENTITION].setTissue(3800,4300,4000,300);  // skewed_head
  // tmap[T_DENTITION].setTissue(2800,4300,3100,300);  // skewed_head
  // tmap[T_DENTITION].setTissue(2750,4300,3100,300);  // skewed_head close
  tmap[T_DENTITION].setTissue(2400,4300,3000,500); // VIS_HUMAN
  //  tmap[T_DENTITION].setTissue(2600,3400,3000,300); // CT_Head
  tmap[T_SPEC].setTissue(8400,9100,8600,200);
  tmap[T_OTHER].setTissue(0,1000,500,500);
}

tissueMap::~tissueMap()
{
}

tissue& tissueMap::getTissue(tissues t)
{
  return (tissue&) this->tmap[t];
}


