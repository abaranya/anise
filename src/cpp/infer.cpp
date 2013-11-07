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

typedef enum {O_BINARY_DAT=2, O_TEXT=0, O_BINARY_ANI=1, O_BINARY_FAKE=3} otype;

class parameters {

public:
  int x;
  int y;
  int z;
  int h;
  int nrules;
  ani::ruleType rule;
  ani::vformat oType;
  std::string filename;
  std::string prefix;
  std::string ofilename;
  std::string ext;

  parameters()
  {
    this->h=6;
    this->x=184;
    this->y=255;
    this->z=170; //default dimmensions
    this->rule = ani::A_NO_RULE;
    this->filename = "";
    this->ofilename = "";
    this->prefix = "points";
    this->ext = ".txt";
    this->oType = ani::V_TAB_TEXT;
    this->nrules = 0;
  }
  ~parameters(){;};
};

void setRule(parameters& p, int rule)
{
  switch(rule)
  { 
  case ani::A_NO_RULE:
    p.rule = ani::A_NO_RULE;
    cerr << "no_rule setting"<< endl;
    break;
  case ani::A_TISSUE_MAP:
  case ani::A_BOUNDING_BOX:
  case ani::A_SIMILARITY:
  case ani::A_TISSUE_JAW:
    p.rule = (ani::ruleType)((int) p.rule | rule);
    break;
  default:
    p.rule = ani::A_NO_RULE;
    cerr << "INVALID ALGORITHM SPECIFIED" << endl;
  }
  p.nrules++;
}

void printUsage(void)
{
  cerr << "Usage:  infer [Options] \n";
  cerr << "Options:\n";
  cerr << "\t -f <filename>: reads raw volume from filename \n";
  cerr << "\t -o <filename>: file to store results \n";
  cerr << "\t -x <dim>\t: Specify number of elements on x dimesion\n";
  cerr << "\t -y <dim>\t: Specify number of elements on y dimesion\n";
  cerr << "\t -z <dim>\t: Specify number of slides \n";
  cerr << "\t -h <size>\t: Specify number of bytes used as header\n";
  cerr << "\t -a n\t\t: Bitmap for algorithm to apply: 1 for tissues, 2 regions, 4 similaity\n";
  cerr << "\t -t n\t\t: output type: 0 for text (psl format), 1 for binary (ani format), 2 binary (dat format) 3 binary (dat convoluted)\n";  
  cerr << "\n";
}

int doProcess(ani::volume& v, parameters& p)
{
  
  std::string filename;
  std::ostringstream stemp;
  std::ifstream ifile;
  std::ofstream ofile;
  int k = 0;
  int i = 0, j= 0;
  bool bload = false;

  // ///////////// //
  //  Load volume  //
  // ///////////// //

  cerr << "Loading volume..." << p.filename << "!" << endl;

  if (p.filename == "")
    bload = v.load(); //using cin
  else
  {
    ifile.open(p.filename.c_str(), std::ios::in | std::ios::binary);
    if(!ifile.is_open())
    {
      cerr << "Unable to open file:" << p.filename << " in " __FILE__ << ":" << __LINE__ << endl;
      return -1;
    }
    bload  = v.load(ifile);
  }
  
  if (!bload)
  {
    cerr << "Failure loading volume:" << p.filename << " in " __FILE__ << ":" << __LINE__ << endl;
    return -1;
  }

  cerr << "volume from :" << p.filename << " suscessful loaded" << endl;

  // ///////////////////// //
  //  open resulting file  //
  // ///////////////////// //

  ofile.open(p.ofilename.c_str(), std::ios::out | std::ios::binary);

  if(!ofile.is_open())
  {
    cerr << "Unable to open file:" << p.ofilename << " in " __FILE__ << ":" << __LINE__ << endl;
    return -1;
  }

  // //////////////////////////// //
  //  Calculates & write results  //
  // //////////////////////////// //
 
  // HERE SHOULD GO THE CICLE BASED.
 
  v.setRules(p.rule, p.nrules);

  v.write(ofile, p.oType, p.rule);

  //v.write(cout, ani::V_TAB_TEXT, p.rule);

  /*  
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
  */
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
	sscanf(argv[++proc],"%d",&p.h);
	cerr << "Header:" << p.h << "\n";
      }
      else if ((std::string)argv[proc] == "-x") 
      {
	sscanf(argv[++proc],"%d",&p.x);
	cerr << "X dim:" << p.x << "\n";
      }
      else if ((std::string)argv[proc] == "-y") 
      {
	sscanf(argv[++proc],"%d",&p.y);
	cerr << "Y dim:" << p.y << "\n";
      }
      else if ((std::string)argv[proc] == "-z") 
      {
	sscanf(argv[++proc],"%d",&p.z);
	cerr << "Z dim:" << p.z << "\n";
      }
      else if ((std::string)argv[proc] == "-a") 
      {
	int rule  = 25;
	sscanf(argv[++proc],"%d",&rule);
	setRule(p,rule);
	cerr << "appling rule:" << rule << "\n";
      }
      else if ((std::string)argv[proc] == "-f") 
      {
	p.filename = argv[++proc];
	cerr << "Input filename:" << p.filename << "\n";
      }
      else if ((std::string)argv[proc] == "-o") 
      {
	p.ofilename = argv[++proc];
	cerr << "Input filename:" << p.ofilename << "\n";
      }
      else if ((std::string)argv[proc] == "-t") 
      {
	int o = 0;
	sscanf(argv[++proc],"%d",&o);
	switch(o)
	{
	case O_BINARY_ANI:
	  p.oType = ani::V_ANI_BINARY;
	  p.ext = ".ani";
	  cerr << "Generates Binary File ANI format \n";
	  break;
	case  O_BINARY_DAT:
	  p.oType = ani::V_RAW_BINARY;
	  p.ext = ".dat";
	  cerr << "Generates Binary File DAT format \n";
	  break;
	case O_BINARY_FAKE:
	  p.oType = ani::V_FAKE_BINARY;
	  p.ext = "F.dat";
	  cerr << "Generates Binary File F.DAT format \n";
	  break;
	default:
	  p.oType = ani::V_TAB_TEXT;
	  p.ext = ".txt";
	  cerr << "Generates TEXT File " << endl;
	}// end switch
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

