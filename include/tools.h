#ifndef __TOOLS_ANI_H
#define __TOOLS_ANI_H

#include <string>
#include <iostream>
using namespace std;

#define LOWEST_INT 3000
#define BIGEST_INT 4200


class point
{
 public:
  short i;
  short o;
  float p;
  int   t;
  float pb;
  float pt;

  point(){i=0;o=0;p=pb=pt=0.0;t=0;};
  ~point(){;};
};

class location
{
 public:
  
  int x;
  int y;
  int z;

  location(){x=y=z=0;};
  ~location(){;};
};

class volume
{
 private:
  int xdim;
  int ydim;
  int zdim;
  int hsize;
  short* header;
  point*** matrix;

  location min;
  location max;

 public:

  //  volume(int h = 0, int x=255,int y=255,int z=128);
  volume(int h = 6, int x=184,int y=256,int z=170);
  ~volume();

  void load();
  void load(istream& input);
  void write(int fmt=1, int alg=1);
  bool write(string filename);
  void set(int h,int x, int y, int z, unsigned char value);
  unsigned char value(int x, int y, int z);
  short*** clone();
  float pdist(int x, int y, int z);
  void raytrace(int x, int y, int z, unsigned short min, unsigned short max);
  void infer(int x, int y, int z, unsigned short min = LOWEST_INT, unsigned short max=BIGEST_INT);
  void neighbour(int type, int x, int y, int z, unsigned short min = LOWEST_INT, unsigned short max=BIGEST_INT);
  void calcBox(location& min, location& max, int value);
  void setBox(location& min, location& max);
  volume cut(int x, int y, int parts);
  bool validate();

};

class aniTool 
{

 private:
  string OutFilename;
  volume v;

 public:

  aniTool();
  ~aniTool();
  
  void setOutFilename(string Filename);
  void loadVolume();
  void write(int fmt = 1, int alg = 1);
  bool validate();
  bool infer(int x, int y, int z, unsigned short min = LOWEST_INT, unsigned short max=BIGEST_INT);
  void calcBox(location& min, location& max, int value) {v.calcBox(min,max,value);};
  void setBox(location& min, location& max);
};

#endif
