
void volume::neighbour(int type, int x, int y, int z, unsigned short min, unsigned short max)
{
  //boundary check
  if (x < 0 || y < 0 || z < 0 ) return;
  if (x >= xdim || y>= ydim || z >= zdim) return;
  
  if (type >= 6) {
    
    if ( matrix[z][y][x].i >= min && matrix[z][y][z].i <= max && matrix[z][y][x].t != 1) {

      matrix[z][y][x].t = 1;

      if ( z < this->min.z && y < this->min.y && x < this->min.x ) {
	this->min.z = z;
	this->min.y = y;
	this->min.x = x;
      }

      if ( z > this->max.z && y > this->max.y && x > this->max.x ) {
	this->max.z = z;
	this->max.y = y;
	this->max.x = x;
      }

      neighbour(type,x+1,y,z,min,max);
      neighbour(type,x-1,y,z,min,max);
      neighbour(type,x,y+1,z,min,max);
      neighbour(type,x,y-1,z,min,max);
      neighbour(type,x,y,z+1,min,max);
      neighbour(type,x,y,z-1,min,max);
      
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
      }
    }
  }
}

bool volume::validate()
{
  cout << xdim << " points on x & " ;
  cout << ydim << " points on y" << endl;
  cout << zdim << " slides" << endl;
  
  // find a place with proper instensity
  neighbour(6,xdim/2,ydim/2,zdim/2, LOWEST_INT, BIGEST_INT);

  cerr << "box:[" << min.x << ", " << min.y << ", " << min.z << "] to [" << max.x << ", " << max.y << ", " << max.z << "]" << endl;

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

void aniTool::loadVolume()
{
  v.load();
}

void aniTool::write(int fmt, int alg)
{
  v.write(fmt, alg);
}

void aniTool::setOutFilename(string Filename)
{
  this->OutFilename = Filename;
}

bool aniTool::validate()
{
  return v.validate();
}

bool aniTool::infer(int x, int y, int z, unsigned short min, unsigned short max)
{
  v.raytrace(x,y,z,min,max);
  v.infer(x,y,z,min,max);
  return true;
}
void aniTool::setBox(location& min, location& max)
{
  v.setBox( min, max);
}

aniTool::aniTool()
{    
}

aniTool::~aniTool()
{
}
