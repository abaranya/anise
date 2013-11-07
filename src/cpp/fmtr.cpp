#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "volume.h"

using std::cerr;
using std::cout;
using std::cin;
using std::endl;

#define SLICES 3

typedef enum {O_BINARY, O_TEXT} otype;
class parameters {

public:
  int x;
  int y;
  int z;
  int h;
  int cuts;
  otype outputT;
  std::string filename;
  std::string prefix;
  std::string ext;

  parameters()
  {
    this->h=6;
    this->x=184;
    this->y=255;
    this->z=170; //default dimmensions
    this->cuts = SLICES;
    this->filename = "";
    this->prefix = "points";
    this->ext = ".txt";
    this->outputT = O_TEXT;
  }
  ~parameters(){;};
};

void printUsage(void)
{
  cerr << "Usage:  fmtr [Options] \n";
  cerr << "Options:\n";
  cerr << "\t -p <prefixname>: writes to prefix_x.ext where x is the cut and ext (txt | dat) \n";
  cerr << "\t -f <filename>: reads raw volume from filename \n";
  cerr << "\t -x <dim>\t: Specify number of elements on x dimesion\n";
  cerr << "\t -y <dim>\t: Specify number of elements on y dimesion\n";
  cerr << "\t -z <dim>\t: Specify number of slides \n";
  cerr << "\t -h <size>\t: Specify number of bytes used as header\n";
  cerr << "\t -c n\t\t: cut the volume in n pieces per dimmension \n";
  cerr << "\t -o n\t\t: output type: 0 for text, 1 for binary \n";  
  cerr << "\n";
}

int doProcess(ani::volume& v, parameters& p)
{
  
  std::string filename;
  std::ostringstream stemp;
  std::ifstream ifile;
  ani::volume c;
  int k = 0;
  int i = 0, j= 0;
  bool bload = false;

  // ///////////// //
  //  Load volume  //
  // ///////////// //

  cerr << "Loading volume..." << endl;

  if (p.filename == "")
    bload = v.load(); //using cin
  else
  {
    ifile.open(p.filename.c_str());
    if(!ifile.is_open())
    {
      cerr << "Unable to open file:" << p.filename << " in " __FILE__ << ":" << __LINE__ << endl;
      return -1;
    }
    
    bload  = v.load(ifile);
  
  }
  
  if (!bload)
  {
    cerr << "Failure to load volume:" << p.filename << " in " __FILE__ << ":" << __LINE__ << endl;
    return -1;
  }

  cerr << "volume from :" << p.filename << " suscessful loaded" << endl;
  
  for ( i = 0 ; i < p.cuts ; i++,k++)
  {
    for ( j = 0 ; j < p.cuts ; j++,k++)
    {
      cerr << "creando cut: " << k << endl;
      c = v.cut(i,j,p.cuts);
      stemp.str("");
      stemp << p.prefix << "_" << k << p.ext;
      filename = (std::string) stemp.str();
      cout << "writing to filename: " << filename << endl;
      if (p.outputT== O_BINARY)
	c.write(filename, ani::V_RAW_BINARY);
      else
	c.write(filename, ani::V_TAB_TEXT);
    }
  }
  return 0;
}

int procParameters(int argc, char* argv[], parameters& p)
{
  int proc=0;

  while ( proc < argc )
  {
      if((std::string)argv[proc] == "--help" || \
	 (std::string)argv[proc] == "-?")
      {
	printUsage();
      }
      else if ((std::string)argv[proc] == "-h") 
      {
	sscanf("%d",argv[++proc],&p.h);
	cerr << "Header:" << p.h << "\n";
      }
      else if ((std::string)argv[proc] == "-x") 
      {
	sscanf("%d",argv[++proc],&p.x);
	cerr << "X dim:" << p.x << "\n";
      }
      else if ((std::string)argv[proc] == "-y") 
      {
	sscanf("%d",argv[++proc],&p.y);
	cerr << "Y dim:" << p.y << "\n";
      }
      else if ((std::string)argv[proc] == "-z") 
      {
	sscanf("%d",argv[++proc],&p.z);
	cerr << "Z dim:" << p.z << "\n";
      }
      else if ((std::string)argv[proc] == "-c") 
      {
	sscanf("%d",argv[++proc],&p.cuts);
	cerr << "Cuts per dimension:" << p.cuts << "\n";
      }
      else if ((std::string)argv[proc] == "-p") 
      {
	p.prefix = argv[++proc];
	cerr << "Output prefix:" << p.prefix << "\n";
      }
      else if ((std::string)argv[proc] == "-f") 
      {
	p.filename = argv[++proc];
	cerr << "Input filename:" << p.filename << "\n";
      }
      else if ((std::string)argv[proc] == "-o") 
      {
	int o = 0;
	sscanf(argv[++proc],"%d",&o);
	if (o == 1)
	{
	  p.outputT = O_BINARY;
	  p.ext = ".dat";
	  cerr << "Generates Binary File \n";
	}
	else
	{
	  p.outputT = O_TEXT;
	  p.ext = ".txt";
	  cerr << "Generates TEXT File " << endl;
	}
      }
      // Read Next Parámeter.
      proc++;
  }  

}

bool setParameters(ani::volume& v, parameters& p)
{
  v.set(p.h,p.x,p.y,p.z,'\0');
  return true;
}

int main(int argc, char* argv[])
{

  parameters p;
  ani::volume v;

  if ( argc < 2 ) 
  {
    cerr << "\nOption required\n\n";
    printUsage();
    return 1;
  }

  // ///////////////////////// //
  //  Do Parameter processing  //
  // ///////////////////////// //

  procParameters(argc,argv,p);

  setParameters(v,p);

  doProcess(v,p);

}

