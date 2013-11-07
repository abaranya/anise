#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "tmap.h"
#include "volume.h"
#include "bbox.h"

using std::cerr;
using std::cout;
using std::cin;
using std::endl;

using namespace ani;

tissueMap tmap;

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

volume::volume()
{
  this->matrix = NULL;
  setRule(ani::A_NO_RULE);
}
// /////////////////////////////////////////////////////////////////////
//
// Method: Constructor : Initializes object structure for given volume size.
//
// Parameters:  int h  : header size
//              int x  : size in x dim
//              int y  :  size in y dim 
//              int z  :  size in y dim 
// Returns :
//
// Exceptions:
//
// /////////////////////////////////////////////////////////////////////

volume::volume(int h, int x,int y,int z)
{
  this->matrix = NULL;
  set(h,x,y,z);
  setRule(ani::A_NO_RULE);
}

// /////////////////////////////////////////////////////////////////////
//
// Method: set : sets object structure for given volume size, 
//               asigns value parameter as default value.
//
// Parameters:  
//     int h               : header size
//     int x               : size in x dim
//     int y               : size in y dim 
//     int z               : size in y dim 
//     unsigned char value : default initializing value
//
// Returns : none
//
// Exceptions: none
//
// /////////////////////////////////////////////////////////////////////

void volume::set(int h, int x,int y,int z,unsigned char value)
{
  // free previous
  if (this->matrix != NULL)
  {
    delete this-> matrix;
    this -> matrix = NULL;
  }

  this->hsize = h;
  this->xdim = x;
  this->ydim = y;
  this->zdim = z;

  this->matrix = new ppoint**[z+1];

  for(int k = 0; k <= z; k++){
    //    cerr << "here:"  << __FILE__ << ":" << __LINE__ << " & k:" << k << endl;
    matrix[k] = new ppoint*[y+1];
    for(int j=0; j <= y; j++){
      matrix[k][j]= new ppoint[x+1];
    }
  }
  this->header = new short[h];
  
}

void volume::setRule(ruleType rule)
{
  if ( rule == A_NO_RULE )
  {
    this-> nRules = 0;
    this-> rules = A_NO_RULE;
  }
  else
  {
    this-> nRules ++;
    this-> rules = (ani::ruleType)((int) this-> rules | (int) rule);
  }
}

void volume::setRules(ruleType rule, int nrules)
{
  if ( rule == A_NO_RULE )
  {
    this-> nRules = 0;
    this-> rules = A_NO_RULE;
  }
  else
  {
    this-> nRules = nrules;
    this-> rules = rule;
  }
}

// /////////////////////////////////////////////////////////////////////
//
// Method: destructor : frees object memory
//
// Parameters: none 
//
// Returns : none
//
// Exceptions: none
//
// /////////////////////////////////////////////////////////////////////

volume::~volume()
{
  if (this->matrix != NULL)
  {
    delete this-> matrix;
    this -> matrix = NULL;
  }
}


// /////////////////////////////////////////////////////////////////////
//
// Method: load : reads from standard input volume content considering 
//               initialization values.
//
// Parameters: none
//
// Returns : bool indicating suscess filling volume
//
// Exceptions: none
//
// /////////////////////////////////////////////////////////////////////


bool volume::load()
{
  return load(cin);
}

// /////////////////////////////////////////////////////////////////////
//
// Method: load : reads from file volume content considering 
 //               initialization values.
//
// Parameters:  
//     istream& input      : reference to input file containing volume data
//
// Preconditions:
//     input file should be already open & file descriptor positioned at
//     file start position
//
// Returns : bool indicating suscess filling volume
//
// Exceptions: none
//
// /////////////////////////////////////////////////////////////////////

bool volume::load(std::istream& input)
{
  int i, j, k;
  unsigned short dummy;
  char c;

  cerr << "Loading header:" << this->hsize << endl;
  for(int i=0; i < hsize;i++)
    input.read((char*)&dummy,sizeof(unsigned char));

  cerr << "Loading data x:"<< xdim << " y:" << ydim << " z:" << zdim << endl;
  cerr << "btw the size of unsigned short is:" << sizeof(unsigned short)<< endl;

  for(k=0 ; k < zdim ; k++) {    
    for(j=0 ; j < ydim ; j++) {
      for(i=0; i < xdim; i++) {
	//input >> dummy;
	if (input.good()) { 
	  input.read((char*)&dummy,sizeof(unsigned short));
	  matrix[k][j][i].i = (unsigned short) dummy;
	}
	else
	{
	  cerr << "unexpected eof found at[" << i << "," << j << "," << k << "]" << endl;
	  return false;
	}
      }
    }
  }
  if (!input.eof()) {
    // cerr << "Warning: End of file should be reached, possible loss of data" << endl;
    // cerr << input;
  }

  return true;
}

// /////////////////////////////////////////////////////////////////////
//
// Method: writeHeader
//
//
// Parameters:  
//     istream& input      : reference to input file containing volume data
//
// Preconditions:
//     input file should be already open & file descriptor positioned at
//     file start position. Probability calculations are considered for 
//     some formats, they should be updated before.
//
// Returns : bool indicating suscess filling volume
//
// Exceptions: none
//
// /////////////////////////////////////////////////////////////////////

bool volume::writeHeader(std::ostream& output, vformat fmt)
{
  unsigned short d;
  location min;
  location max;

  switch(fmt)
  {
  case V_ANI_BINARY:
  case V_BINARY:
    // Write subvolume representation
    // Recalulates Box
    // Write full volume dimension
    d = this->xdim;
    output.write((char*)&d,sizeof(unsigned short));
    d = this->ydim;
    output.write((char*)&d,sizeof(unsigned short));
    d = this->zdim;
    output.write((char*)&d,sizeof(unsigned short));

    this->calcBox(min,max,(float)MIN_PROB);

    d = min.p.x;
    output.write((char*)&d,sizeof(unsigned short));
    d = max.p.x;
    output.write((char*)&d,sizeof(unsigned short));
    d = min.p.y;
    output.write((char*)&d,sizeof(unsigned short));
    d = max.p.y;
    output.write((char*)&d,sizeof(unsigned short));
    d = min.p.z;
    output.write((char*)&d,sizeof(unsigned short));
    d = max.p.z;
    output.write((char*)&d,sizeof(unsigned short));
    return true;    
    break;

  case V_FAKE_BINARY:
  case V_RAW_BINARY:
    // Write full volume dimension
    d = this->xdim;
    output.write((char*)&d,sizeof(unsigned short));
    d = this->ydim;
    output.write((char*)&d,sizeof(unsigned short));
    d = this->zdim;
    output.write((char*)&d,sizeof(unsigned short));
    return true;
  case V_CSV_TEXT:
  default:
    ;
  }

  return true;
}

// /////////////////////////////////////////////////////////////////////
//
// Method: writeHeader
//
//
// Parameters:  
//     istream& input      : reference to input file containing volume data
//
// Preconditions:
//     input file should be already open & file descriptor positioned at
//     file start position
//
// Returns : bool indicating suscess filling volume
//
// Exceptions: none
//
// /////////////////////////////////////////////////////////////////////

bool volume::write(std::string filename, vformat fmt)
{
  std::ofstream fout;

  fout.open(filename.c_str());

  if(fout.is_open())
  {
    this->write(fout,fmt,0);
  }
  else
    return false;

  fout.close();

  return true;
}

unsigned short volume::calculateValue(int x, int y, int z)
{
  return 0;
}

float volume::calculateProb(int x, int y, int z)
{
  float acum = 1.0;
  float prob = 0.0;
  ani::ruleType c_alg;
  point p;

  p.set(x,y,z);

  for(int d = 0; d < N_ALGS; d++)
  {
    prob = 0.0;
    // cerr << "doing the " << d << "th rule for " << this->rules << endl;
    
    if( (int)this->rules & (int)pow(2,d) )
    {
      // This bit was set
      switch((int)pow(2,d))
      {
	case A_TISSUE_MAP:
	  //prob = (float) tmap.getTissue(ani::T_DENTITION).prob(this->matrix[z][y][x].i);
	  prob = (float) tmap.getTissue(ani::T_BONE).prob(this->matrix[z][y][x].i);
	  /*
	  if ( prob > 0.0 )
	  {
	    cerr << "caculating prob using tissue for[" << x << "," << y << "," << z \
		 << "]:" << this->matrix[z][y][x].i << " en:" << prob << endl;
	  }*/
	  this->matrix[z][y][x].p[d] = prob;
	  break;
	case A_BOUNDING_BOX:
	  {
	    bbox box;
	    location b,e; // just a test;
	    /* skewed_head 
	    b.p.x = 46;b.p.y = 10; b.p.z = 18; 
	    e.p.x = 138;e.p.y = 96; e.p.z = 92;
	    */
	    
	    /* skewed_head - extended 9 */
	    b.p.x = 21;b.p.y = 10; b.p.z = 2; 
	    e.p.x = 158;e.p.y = 126; e.p.z = 102;

	    /*vis_head
	    b.p.x = 138;b.p.y = 10; b.p.z = 15; 
	    e.p.x = 294;e.p.y = 226; e.p.z = 114;
	    */	    

	    //ct_head
	    //b.p.x = 64;b.p.y = 20; b.p.z = 65; 
	    //e.p.x = 192;e.p.y = 118; e.p.z = 102;
	    	    

 	    box.setStartPoint(b);
	    box.setEndPoint(e);
	    // Where is the bounding box?
	    prob = (float) box.getProb(p);
	    /*	    if (prob < 0.15)
	      cerr << "[" << x << "," << y << "," << z << "] is :" << prob << endl;
	    */
	    this->matrix[z][y][x].p[d] = prob;
	    break;
	  }
	case A_SIMILARITY:
	  {
	  // TODO: Needs to be implemented again

	  // skewed_head - dentition
	  // 51 87 60
	  //int x1 = 115; int y1 = 41; int z1 = 50;

	  //neighbour(6,d,x1,y1,z1,2800,4300);	  

	  // skewed_head - jaw
	  // 51 87 60
	  //int x1 = 51; int y1 = 87; int z1 = 60;
	  int x1 = 141; int y1 = 86; int z1 = 42;

	  neighbour(6,d,x1,y1,z1,4300,8000);	  

	  prob = this->matrix[z][y][x].p[d];

	  //	  if (prob > 0 ) cerr << " some non 0 value " << endl;

	  }
	  break;
	case A_TISSUE_JAW:
	  //prob = (float) tmap.getTissue(ani::T_BONE).prob(this->matrix[z][y][x].i);
	  prob = (float) tmap.getTissue(ani::T_DENTITION).prob(this->matrix[z][y][x].i);
	  /*
	  if ( prob > 0.0 )
	  {
	    cerr << "caculating prob using tissue for[" << x << "," << y << "," << z \
		 << "]:" << this->matrix[z][y][x].i << " en:" << prob << endl;
	  }*/
	  if ( acum > 0 /*MIN_PROB*/ )
	  {
	    this->matrix[z][y][x].p[d] = 0;
	    acum = 0;
	  }	  
	  else
	  {
	    this->matrix[z][y][x].p[d] = prob;
	    acum = prob;
	  }
	  break;
	default:
	  break;
      } 
    }
    else
    {
      this->matrix[z][y][x].p[d] = 0.0;
      prob = 1.0; // trick.
    }
    
    // acum += (1.0/this->nRules)*prob;
    if (acum + prob - 1 <= 0.0)
      acum = 0.0;
    else
      acum += prob - 1;
    
  /* witness */

  /*  Skewed Head */
  if ( x == 58 && y == 68 && z == 40)
    cerr << "Witness 1= "  << acum << endl; 

  if ( x == 68 && y == 75 && z == 70)
    cerr << "Witness 2= "  << acum << endl; 

  if ( x == 89 && y == 19 && z == 60)
    cerr << "Witness 3= "  << acum << endl; 

  }
  return acum;
  //return (acum>0)?acum/(this->nRules+1):0;
}


bool volume::calculateRules()
{
  int i,j,k;
  unsigned short value = 0;
  float prob = 0.01;

  for(k = 0 ; k < zdim ; k++) 
  {    
    for(j=0 ; j < ydim ; j++) 
    {
      for(i=0; i < xdim; i++) 
      {
	this->calculateValue(i,j,k); // it shouldn't be necesary, but to produce copies
	this->matrix[k][j][i].o = this->calculateProb(i,j,k);
      }
    }
  }  
  return true;
}

float volume::getProb(int x, int y, int z)
{
  float p = 0;
  for(int d = 0 ; d < this->nRules ;d++)
  {
    // asumes evenly weighted probs
    p += (1.0 /(float) this->nRules) * this->matrix[z][y][x].p[d];
  }

  return p;
}

bool volume::convolve( ani::location& min, ani::location& max, float low, int shift)
{
  float p;
  unsigned long howmany = 0;

  for(int k = min.p.z; k <= max.p.z; k++)
  {
    for( int j = min.p.y; j <= max.p.y; j++)
    {
      for(int i = min.p.x; i <= max.p.x; i++) 
      {	
	p = calculateProb(i,j,k);
	if ( p >= low)
	{
	  howmany++;
	  matrix[k][j][i].i += shift;  
	}
      }
    }
  }

  cerr << "there were " << howmany << " values shifted " << endl;
}


bool volume::writeBox(std::ostream& output, ani::location& min, ani::location& max, float low, infoType info)
{
  float p;
  unsigned short d;
  bool r = true;
  
  cerr << "writing box:[" << min.p.x << "," << min.p.y << "," << min.p.z << "] to [" \
       << max.p.x << "," << max.p.y << "," << max.p.z << "]" << endl;
  for(int k = min.p.z; k <= max.p.z; k++)
  {
    for( int j = min.p.y; j <= max.p.y; j++)
    {
      for(int i = min.p.x; i <= max.p.x && r; i++) 
      {	
	p = (float) calculateProb(i,j,k);

	if (info == I_OPACITY)
	{
	  d = (unsigned short) p * 100;
	  if ( p < low ) d = 0.0;
	}
	else
	{
	  if ( p >= low )
	    d = (unsigned short) matrix[k][j][i].i;
	  else d = 0;
	}
        r = r && output.write ((char*) &d, sizeof(unsigned short));
      }
      
      if ( !r ) return r;
    }
  }

  return r;

  cerr << "Box wrote" << endl;
}

void volume::write(std::ostream& output, vformat fmt, int alg)
{
  unsigned short i,j,k;
  ani::location min;
  ani::location max;
  float mprob = MIN_PROB;
  int tp = 0;

  // Doing Statefull calculations because efficiency
  cerr << "Calculations for algorithm : " << alg << endl;
  this -> calculateRules(); // TODO:Check alg parameter, should be part of volume on statefull  
  

  cerr << "Writing output file" << endl;
  // BODY SECTION
  if (!this -> writeHeader(output,fmt))
  {
    cerr << "Error: unable to write header:" << __FILE__ << ":" << __LINE__ << endl;
    return;
  }

  // Finally write results deppending on fmt
  switch(fmt)
  {
    case V_ANI_BINARY:
      // write only > MIN_PROB 
      calcBox(min,max,(float)mprob);
      writeBox(output,min,max,0.0,ani::I_OPACITY);
      break;
    default:
      //calcBox(min,max,(float)mprob);
      mprob = 0.0;
    case V_RAW_BINARY:
      min.setPoint(0,0,0);
      max.setPoint(xdim-1,ydim-1,zdim-1);
      cerr << "wrinting box using prob: "<< mprob << endl;
      writeBox(output,min,max,mprob,ani::I_INTENSITY);
      break;
  case V_FAKE_BINARY:
      min.setPoint(0,0,0);
      max.setPoint(xdim-1,ydim-1,zdim-1);
      cerr << "wrinting fake box using prob: "<< mprob << endl;
      // this->convolve(min, max, mprob, 4300); //skewed head
      this->convolve(min, max, mprob, 12000); //skewed jaw
      writeBox(output,min,max,0.0,ani::I_INTENSITY);
      break;
  }

  return;
  /*
  // Finally write results deppending on fmt
  for(k = 0 ; k < zdim ; k++) 
  {    
    for(j=0 ; j < ydim ; j++) 
    {
      for(i=0; i < xdim; i++) 
      {
	unsigned short value = this->matrix[k][j][i].i;
	float o = this->matrix[k][j][i].o;
	float prob = this->getProb(i,j,k);
	unsigned short d;

	switch (fmt) 
	{
	  case V_TAB_TEXT:
	    cerr << "Writing text point" << endl;
	    output << i << "\t" << j << "\t" << k << "\t" << value;
	    output << endl;
	    break;
	  case V_CSV_TEXT:
	    // WIRED temporary until box adjustment made
	    output << i << "," << j << "," << k << "," << value << "," << prob;
	    output << endl;
	    break;
	  case V_BINARY:
	    if (prob > 0.25){
	      output.write( (char*)&value,sizeof(unsigned short));
	    }	  
	    else {
	      d = 1;
	      output.write((char*)&d, sizeof(unsigned short));
	    }
	    break;
	  case V_ANI_BINARY_OLD:
	    d = ( (prob < 0.5) ? 0 : 1);
	    output.write ((char*) &d, sizeof(unsigned short));
	    break;
	  case V_ANI_BINARY:
	    // write only > MIN_PROB 
	    if ( prob > MIN_PROB  )
	    {
	      tp++;
	      d = (unsigned short) prob*100;
	      output.write ((char*) &d, sizeof(unsigned short));
	    }
	    break;
	  case V_RAW_BINARY:
	    d = 0;
	    if (prob > 0.0)
	    { 
	      tp++;
	      output.write( (char*)&value,sizeof(unsigned short));
	    }
	    else
	      output.write( (char*)&d,sizeof(unsigned short));

	    break;
	  default:
	    tp++;
	    output.write( (char*)&value,sizeof(unsigned short));
	    //	  cerr << "invalid format \n" ;
	}
      }
    }
  }

  cerr << "Positive prob writen : " << tp << endl;

  /*
  // BODY SECTION
  for(k = 0 ; k < zdim ; k++) {    
    for(j=0 ; j < ydim ; j++) {
      for(i=0; i < xdim; i++) {
	unsigned short value = 0;
	float prob = 0.01;
	switch(alg) {
	case 1:
	  if ( matrix[k][j][i].p > 0.25 )
	    value = matrix[k][j][i].i;
	  prob = matrix[k][j][i].p;
	  break;
	case 2:
	  if ( matrix[k][j][i].t == 1 ) {
	    value = matrix[k][j][i].i;
	    prob =  matrix[k][j][i].p;
	    /*
	    cerr << " Points & Prob: [" << i << "," << j << "," << k << "] :" << value ;
	    cerr << " intens & " << prob << " prob" << endl; 
	    
	  }
	  break;
	case 3:
	  if ( matrix[k][j][i].p > 0.25 && matrix[k][j][i].t == 1 ) {
	    value = matrix[k][j][i].i;
	    prob =  matrix[k][j][i].p;
	  }
	  break;
	case 4:
	  if( matrix[k][j][i].pb > 1.0/2.0 ) {
	    value = matrix[k][j][i].i;
	    prob =  matrix[k][j][i].pb;
	    tp++;
	  }	
	  break;
	case 5:
	  if( matrix[k][j][i].pb > 0.25 && matrix[k][j][i].p > 0.25) {
	    value = matrix[k][j][i].i;
	    prob =  (matrix[k][j][i].p >  matrix[k][j][i].pb )? matrix[k][j][i].p :  matrix[k][j][i].pb;
	  }
	  break;
	case 6:
	  if( matrix[k][j][i].pb > 0.25 && matrix[k][j][i].p > 0.25 && matrix[k][j][i].t == 1 ) {
	    value = matrix[k][j][i].i;
	    prob =  (matrix[k][j][i].p >  matrix[k][j][i].pb )? matrix[k][j][i].p :  matrix[k][j][i].pb;
	  }
	  break;
	default:
	  //	  cerr << "output without inference" << endl;
	  value = matrix[k][j][i].i;
	  prob =  1.0;
	}

	//	cerr << "Selecting format for file" << endl;

	switch (fmt) {
	case V_TAB_TEXT:
	  cerr << "Writing text point" << endl;
	  output << i << "\t" << j << "\t" << k << "\t" << value;
	  output << endl;
	  break;
	case V_CSV_TEXT:
	  // WIRED temporary until box adjustment made
	  output << i << "," << j << "," << k << "," << value << "," << prob;
	  output << endl;
	  break;
	case V_BINARY:
	  if (prob > 0.25){
	    output.write( (char*)&value,sizeof(unsigned short));
	  }	  
	  else {
	    unsigned short dummy = 1;
	    output.write((char*)&dummy, sizeof(unsigned short));
	  }
	  break;
	case V_ANI_BINARY:
	  {
	    unsigned short d = ( (prob < 0.5) ? 0 : 1);
	    output.write ((char*) &d, sizeof(unsigned short));
	  }
	  break;
	case V_RAW_BINARY:
	default:
	  tp++;
	  output.write( (char*)&value,sizeof(unsigned short));
	  //	  cerr << "invalid format \n" ;
	}
      }
    }
  }
  */
}

void volume::calcBox(location& min, location& max, int value) 
{
  int i,j,k;
  
  cerr << "The Box value is: "<< value << endl;
  for(k = 0 ; k < zdim ; k++) {    
    for(j=0 ; j < ydim ; j++) {
      for(i=0; i < xdim; i++) {
	if(matrix[k][j][i].i >= value) {
	  if ( k < min.p.z && j < min.p.y && i < min.p.x ) {
	    min.setPoint(i,j,k);
	  }
	  if ( k > max.p.z && j > max.p.y && i > max.p.x ) {
	    max.setPoint(i,j,k);
	  }
	}  
      }
    }
  }

  cerr << "Min: [" << min.p.x << "," << min.p.y << "," << min.p.z <<"]";
  cerr << "Max: [" << max.p.x << "," << max.p.y << "," << max.p.z <<"]" << endl;
}

void volume::calcBox(location& min, location& max, float p) 
{
  int i,j,k;
  
  min.setPoint(xdim,ydim,zdim);
  max.setPoint(0,0,0);

  cerr << "The Box value is: "<< p << endl;
  for(k = 0 ; k < zdim ; k++) 
  {    
    for(j=0 ; j < ydim ; j++) 
    {
      for(i=0; i < xdim; i++) 
      {
	if(matrix[k][j][i].o >= p) {
	  if ( k < min.p.z )
	  {
	    min.p.z = k;
	  }
	  if (j < min.p.y) 
	  {
	    min.p.y = j;
	  }
	  if (i < min.p.x ) 
	  {
	    min.p.x = i;
	  }
	  if ( k > max.p.z )
	  {
	    max.p.z = k;
	  }
	  if(j > max.p.y )
	  {
	    max.p.y = j;
	  }
	  if (i > max.p.x ) 
	  {
	    max.p.x = i;
	  }
	}  
      }
    }
  }

  cerr << "Min: [" << min.p.x << "," << min.p.y << "," << min.p.z <<"]";
  cerr << "Max: [" << max.p.x << "," << max.p.y << "," << max.p.z <<"]" << endl;
}

float volume::pdist(int x, int y, int z)
{

  float px=1.0,py=1.0,pz=1.0;

  // Use an standar box
  if ( x <= this->max.p.x && x >= this -> min.p.x && y <= this->max.p.y && y >= this -> min.p.y && z <= this->max.p.z && z >= this -> min.p.z )
    return 1.0;

  return 0.0;


  // max distance allowed on X axis
  if ( ( (this->min.p.x - x) > (this->max.p.x - this->min.p.x) ) || (( x - this->max.p.x ) > ( this->max.p.x - this->min.p.x)) )
    px = 0.0;
  else {
    float delta,norm;
    float pm;

    norm = (max.p.x - min.p.x);
    pm = norm / 2;
    delta = std::abs(pm-x)/norm;

    px = ((delta <= 1.0) ? 1.0-delta : 0.0);
  }    

  // max distance allowed on Y axis
  if ( ( (this->min.p.y - y) > (this->max.p.y - this->min.p.y) ) || (( y - this->max.p.y ) > ( this->max.p.y - this->min.p.y)) )
    py = 0.0;
  else {
    float delta,norm;
    float pm;

    norm = (max.p.y - min.p.y);
    pm = norm / 2;
    delta = std::abs(pm-y)/norm;

    py = ((delta <= 1.0) ? 1.0-delta : 0.0);
  }    

  // max distance allowed on Y axis
  if ( ( (this->min.p.z - z) > (this->max.p.z - this->min.p.z) ) || (( z - this->max.p.z ) > ( this->max.p.z - this->min.p.z)) )
    pz = 0.0;
  else {
    float delta,norm;
    float pm;

    norm = (max.p.z - min.p.z);
    pm = (max.p.z + min.p.z)/ 2;
    delta = std::abs(pm-x)/std::abs(norm);

    pz = ((delta <= 1.0) ? 1.0-delta : 0.0);
  }    

  return 3*pz/3 + 0*py/3 + 0*pz/3; 

}

void volume::raytrace(int x, int y, int z, unsigned short min, unsigned short max)
{
  int px1=0, px2=0, py1=0, py2=0, pz1=0, pz2=0;

  // on x
  for(int i = x; i < this -> xdim; i++){
    if(matrix[z][y][i].i > min && matrix[z][y][i].i < max){
      px1 = i;
      break;
    }
  }

  for(int i = x; i >= 0 ; i--){
    if(matrix[z][y][i].i > min && matrix[z][y][i].i < max){
      px2 = i;
      break;
    }
  }

  // on y
  for(int i = y; i < this -> ydim; i++){
    if(matrix[z][i][x].i > min && matrix[z][i][x].i < max){
      py1 = i;
      break;
    }
  }

  for(int i = y; i >= 0 ; i--){
    if(matrix[z][i][x].i > min && matrix[z][i][x].i < max){
      py2 = i;
      break;
    }
  }

  // on z
  for(int i = z; i < this -> zdim; i++){
    if(matrix[i][y][x].i > min && matrix[i][y][x].i < max){
      pz1 = i;
      break;
    }
  }

  for(int i = x; i >= 0 ; i--){
    if(matrix[i][y][x].i > min && matrix[i][y][x].i < max){
      pz2 = i;
      break;
    }
  }

  cerr << "Raytrace 1: [" << px1 << "," << py1 << "," << pz1 << "]" << endl;
  cerr << "Raytrace 2: [" << px2 << "," << py2 << "," << pz2 << "]" << endl;
}

void volume::infer(int x, int y, int z, unsigned short min, unsigned short max)
{
  /*
  int i,j,k;
  int t = 0;
  // precalculate
  for(k=0; k< zdim;k++) {
    for(j=0; j< ydim;j++) {
      for(i=0; i< xdim;i++) {
	matrix[k][j][i].pb = this->pdist(i,j,k);
	if(matrix[k][j][i].i < min) {
	  matrix[k][j][i].o = 0;
	  matrix[k][j][i].p = 0.0;
	}
	else {
	  matrix[k][j][i].p = (matrix[k][j][i].i > max) ? 1.0 : (0.25+ 0.75*( (float)matrix[k][j][i].i / (float)max) );
	  matrix[k][j][i].o = matrix[k][j][i].p;
	  t++;
	}
      }
    }
  }  
  // seed point prob should be 1
  matrix[z][y][x].p = 1.0;

  cerr << t << " Total Positive Points" << endl;

  this->min.x = this->max.x =x;
  this->min.y = this->max.y =y;
  this->min.z = this->max.z =z;

  cerr << "Doing [" << x << "," << y << "," << z << "]" << endl;
  neighbour(6,x,y,z,min,max);
  //neighbour(24,x,y,z,min,max);
  */

}

float volume::neighbour(int type, int d, int x, int y, int z, unsigned short min, unsigned short max)
{

  float prob=1.0;
  //boundary check
  if (x < 0 || y < 0 || z < 0 ) return 0.0;
  if (x >= xdim || y>= ydim || z >= zdim) return 0.0;
  
  if (type >= 6) {
    
    if ( matrix[z][y][x].i >= min && matrix[z][y][z].i <= max && matrix[z][y][x].t != 1) {

      cerr << "positive for [" << x << "," << y << "," << z << "]" << matrix[z][y][x].i << endl; 
      matrix[z][y][x].t = 1;

      if ( z < this->min.p.z && y < this->min.p.y && x < this->min.p.x ) {
	this->min.setPoint(x,y,z);
      }

      if ( z > this->max.p.z && y > this->max.p.y && x > this->max.p.x ) {
	this->max.setPoint(x,y,z);
      }

      prob += neighbour(type,d,x+1,y,z,min,max);
      prob += neighbour(type,d,x-1,y,z,min,max);
      prob += neighbour(type,d,x,y+1,z,min,max);
      prob += neighbour(type,d,x,y-1,z,min,max);
      prob += neighbour(type,d,x,y,z+1,min,max);
      prob += neighbour(type,d,x,y,z-1,min,max) - 6;

      return (matrix[z][y][x].p[d] = 1.0);

      /*
      if (type >= 12) {

	neighbour(type,x+1,y+1,z,min,max);
	neighbour(type,x+1,y+1,z+1,min,max);
	neighbour(type,x+1,y+1,z-1,min,max);
	neighbour(type,x+1,y-1,z,min,max);
	neighbour(type,x+1,y-1,z+1,min,max);
	neighbour(type,x+1,y-1,z-1,min,max);
	
	neighbour(type,x-1,y+1,z,min,max);
	neighbour(type,x-1,y+1,z+1,min,max);
	neighbour(type,x-1,y+1,z-1,min,max);
	neighbour(type,x-1,y-1,z,min,max);
	neighbour(type,x-1,y-1,z+1,min,max);
	neighbour(type,x-1,y-1,z-1,min,max);
	
	neighbour(type,x,y+1,z+1,min,max);
	neighbour(type,x,y+1,z-1,min,max);
	neighbour(type,x,y-1,z+1,min,max);
	neighbour(type,x,y-1,z-1,min,max);
	}*/
    }
    
    // cerr << " not in range " << matrix[z][y][x].i << endl;
    return 0.0;
  
  }
  return 0.0;
}

bool volume::validate()
{
  cerr << xdim << " points on x & " ;
  cerr << ydim << " points on y" << endl;
  cerr << zdim << " slides" << endl;
  
  // find a place with proper instensity
  neighbour(6,xdim/2,ydim/2,zdim/2, LOWEST_INT, BIGEST_INT);

  cerr << "box:[" << min.p.x << ", " << min.p.y << ", " << min.p.z << "] to [" << max.p.x << ", " << max.p.y << ", " << max.p.z << "]" << endl;

  return true;
}

void volume::setBox(location& min, location& max)
{
  cerr << "SettingBox " << endl;
  this-> min = min;
  this-> max = max;
}

short*** volume::clone()
{
  short*** d; 
  return d;
}


volume& volume::cut(int x, int y, int parts)
{
  cerr << "here:"  << __FILE__ << ":" << __LINE__ << endl;
  volume* ref = new volume(this->hsize, this->xdim/parts, this->ydim/parts, this->zdim);
  
  cerr << "creando cut:" << x << "," << y << endl;

  cerr << "from x: " <<  xdim*x/parts << " to " << (x+1)*xdim/parts << endl; 

  for ( int i = 0, m=xdim*x/parts ; m < (x+1)*xdim/parts ; i++,m++) // TODO: check when x%parts > 0
  {
    cerr << "\tfrom y: " <<  ydim*y/parts << " to " << (y+1)*ydim/parts << endl;
 
    for( int j = 0, n=ydim*y/parts; n < (y+1)*ydim/parts ; j++,n++)
    {
      for(int k = 0; k < this->zdim ; k++) // use whole dimmension
      {
	// cerr << " element [" << k << "," << j << "," << i << "]=" << this->matrix[k][n][m].i << endl;
	ref->matrix[k][j][i].i = this->matrix[k][n][m].i;
      }
    }
  }

  return *ref;
}
