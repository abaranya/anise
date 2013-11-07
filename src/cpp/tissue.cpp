#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "tissue.h"

using std::cerr;
using std::cout;
using std::cin;
using std::endl;

using namespace ani;

tissue::tissue()
{
  this->setTissue(0,5000,2500,1000);
}

tissue::tissue(int m, int M, int mean, int std)
{
  this -> setTissue(m,M,mean,std);
}

void tissue::setTissue(int m, int M, int mean, int std)
{
  this -> mean = mean;
  this -> stddev = std;
  this -> min = m;
  this -> max = M;
}

tissue::~tissue()
{;
}


float tissue::prob(int i)
{
  if (i < min || i > max)
  {
    return 0.0;
  }

  if ((float)std::abs((float)i-mean)/(float)stddev <= 1.0) // z-test
    return 0.95;

  if ((float)std::abs((float)i-mean)/(float)stddev <= 2.0)
    return 2.0 - ((float)std::abs((float)i-mean)/(float)stddev);

  return 0.20;

}
