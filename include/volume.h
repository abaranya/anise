#ifndef __VOLUME_ANI_H
#define __VOLUME_ANI_H

#include <string>
#include <iostream>
#include "location.h"

namespace ani
{

#define LOWEST_INT 3000
#define BIGEST_INT 4200
#define MIN_PROB 0.15

  typedef enum { V_BINARY, V_RAW_BINARY, V_CSV_TEXT, V_TAB_TEXT, V_ANI_BINARY, V_ANI_BINARY_OLD , V_FAKE_BINARY } vformat;
  typedef enum { A_NO_RULE=0, A_TISSUE_MAP=1, A_BOUNDING_BOX=2, A_SIMILARITY=4, A_TISSUE_JAW=8} ruleType;
  typedef enum { I_INTENSITY, I_OPACITY } infoType;

#define N_ALGS 4

  class volume
  {
  private:
    int xdim;
    int ydim;
    int zdim;
    int hsize;
    short* header;
    ppoint*** matrix;

    location min;
    location max;

    ruleType rules;
    int nRules;

  public:

    volume();
    volume(int h, int x=184,int y=256,int z=170);
    ~volume();

    bool load();
    bool load(std::istream& input);
    bool writeHeader(std::ostream& output, ani::vformat fmt);
    void write(std::ostream& output=std::cout, vformat fmt=V_BINARY, int alg=1);
    bool write(std::string filename, vformat fmt=V_BINARY);
    bool writeBox(std::ostream& output, ani::location& min, ani::location& max, float low = 0.0, infoType info=I_INTENSITY);
    void check(ani::location& min, ani::location& max);

    unsigned short calculateValue(int x, int y, int z);
    float calculateProb(int x, int y, int z);
    float getProb(int x, int y, int z);
    bool calculateRules();
    void calcBox(ani::location& min, ani::location& max, int value);
    void calcBox(ani::location& min, ani::location& max, float p);
    bool convolve(ani::location& min, ani::location& max, float low, int shift);
    bool resetT(ani::location& min, ani::location& max);

    void set(int h,int x, int y, int z, unsigned char value='\0');
    void setRule(ruleType rule);
    void setRules(ruleType rule, int nrules);

    unsigned char value(int x, int y, int z);
    short*** clone();
    float pdist(int x, int y, int z);
    void raytrace(int x, int y, int z, unsigned short min, unsigned short max);

    void infer(int x, int y, int z, unsigned short min = LOWEST_INT, unsigned short max=BIGEST_INT);

    float neighbour(int type, int d, int x, int y, int z, unsigned short min = LOWEST_INT, unsigned short max=BIGEST_INT);
    void setBox(ani::location& min, ani::location& max);
    ani::volume& cut(int x, int y, int parts);
    bool validate();

  };
}
#endif
