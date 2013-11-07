#include <string>
#include <iostream>
#include <stdio.h>
#include "tools.h"


void printUsage(void)
{
  cerr << "Usage:  arm [Options] \n";
  cerr << "Options:\n";
  cerr << "\t -o:\t\t writes to standard output the inference results of the standard input as given volume)\n";
  cerr << "\t -v:\t\t validates standard input given volume for description\n";
  cerr << "\t -x\t<dim>:\t Specify number of elements on x dimesion\n";
  cerr << "\t -y\t<dim>:\t Specify number of elements on y dimesion\n";
  cerr << "\t -z\t<dim>:\t Specify number of slides \n";
  cerr << "\t -h\t<size>:\t Specify number of bytes used as header\n";
  cerr << "\t -p\t<x> <y> <z>:\t Specify seed point\n";
  cerr << "\t -t\t<lrange> <urange>:\t Tissue range\n";
  cerr << "\t -f\t<fmt>:\t if fmt is 1 produces comma separated output, fmt 2 creates a dat volume, value of 3 creates .ani file\n";
  cerr << "\t\t\t with 0 intensity on points with lower than 0.25 probability";
  cerr << "\t -a\t<alg>:\t Specify point selection method [1] by Region, [2] by Similarity, [3] weighted (both methods) \n";
  cerr << "\t -n\t\t: Don't process inference, use incoming set as output";
  cerr << "\t -c:\t\t cut the volume in 9 pieces";
  cerr << "\n";
}

int main(int argc, char* argv[])
{

  aniTool t;
  int proc = 1;
  int limit = LOWEST_INT;
  int alg = 1;
  int h=3,x=255,y=255,z=128; //default dimmensions
  location seed;
  bool val = false;
  bool ext = false;
  bool calc = false;
  bool box = false;
  bool cut = false;
  int fmt = 0;
  int lInt = LOWEST_INT, uInt = BIGEST_INT;
  location m, M;
  int bsx = 0, bsy = 0, bsz = 0, bex = 255, bey = 255, bez = 255;

  if ( argc < 2 ) {
    cerr << "\nOption required\n\n";
    printUsage();
    return 1;
  }

  seed.x = 64;
  seed.y = 70;
  seed.z = 69;

  while ( proc < argc ) {
    
    if((string)argv[proc] == "-h")
      cerr << "El header es de: " << argv[++proc] << "\n";
    else if ((string)argv[proc] == "-x") 
    {
      cerr << "X dim:" << argv[++proc] << "\n";
    }
    else if ((string)argv[proc] == "-y") 
    {
      cerr << "Y dim" << argv[++proc] << "\n";
    }
    else if ((string)argv[proc] == "-z") 
    {
      cerr << "Z dim" << argv[proc] << "\n";
    }
    else if ((string)argv[proc] == "-c") 
    {
      cerr << "Cut" << "\n";
      cut = true;
    }
    else if ((string)argv[proc] == "-o") 
    {
      if(val){
	cerr << "Conflictive options -o -v" << endl;
	return 1;
      }
      ext = true;
    }
    else if ((string)argv[proc] == "-v") 
    {
      if(ext){
	cerr << "Conflictive options -o -v" << endl;
	return 1;
      }
      val = true;
    }
    else if ((string)argv[proc] == "-f") 
    {
      sscanf(argv[++proc],"%d",&fmt);
      cerr << "Format Selected : " << fmt << endl;
    }
    else if ((string)argv[proc] == "-a") 
    {
      if (alg > 0)
      { 
	// ignore when no inference is set
	sscanf(argv[++proc],"%d",&alg);
	cerr << "Alg Selected : " << alg << endl;
      }
    }
    else if ((string)argv[proc] == "-p") 
    {
      sscanf(argv[++proc],"%d",&seed.x);
      sscanf(argv[++proc],"%d",&seed.y);
      sscanf(argv[++proc],"%d",&seed.z);
      cerr << "seed defined: [" << seed.x << "," << seed.y << "," << seed.z << "]" << endl;
    }
    else if ((string)argv[proc] == "-b") 
    {
      sscanf(argv[++proc],"%d",&m.x);
      sscanf(argv[++proc],"%d",&m.y);
      sscanf(argv[++proc],"%d",&m.z);
      sscanf(argv[++proc],"%d",&M.x);
      sscanf(argv[++proc],"%d",&M.y);
      sscanf(argv[++proc],"%d",&M.z);
      cerr << "box defined: [" << m.x << "," << m.y << "," << m.z << "].[" <<  M.x << "," << M.y << "," << M.z << "]" << endl;
      box = true;
    }
    else if ((string)argv[proc] == "-c") 
    {
      sscanf(argv[++proc],"%d",&limit);
      cerr << "calc point over: " << limit << endl;
    }
    else if ((string)argv[proc] == "-t") 
    {
      sscanf(argv[++proc],"%d",&lInt);
      sscanf(argv[++proc],"%d",&uInt);
      cerr << "Intensity Range:[ " << lInt <<","<< uInt << "]" << endl;
    }
    else {
      cerr << "\nInvalid Option: " << argv[proc] << "\n\n";
      printUsage();
      return 1;
    }    
    
    proc++;
  }

  t.loadVolume();

  if ( alg > 0 )
  {
    if (box) {
      t.setBox(m,M);
    }
    else {
      m.x = m.y = m.z = 255;
      M.x = M.y = M.z = 0;

      t.calcBox(m,M,lInt);
    }

    if (val) {
      val = t.validate();
      if (val)
	cout << "Status: OK " << endl;
      else
	cout << "Status: ERROR " << endl;
    }

    //t.infer(57,67,42,lInt,uInt);
    //t.infer(68,76,70,lInt,uInt);

    //  t.infer(m.x,m.y,m.z);
    //  t.infer(M.x,M.y,M.z);
    t.infer(seed.x,seed.y,seed.z, lInt, uInt);
  }

  t.cut = cut;

  if (ext)
    if(fmt>=0)
      t.write(fmt,alg);

  return 0;
}
