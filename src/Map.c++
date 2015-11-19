#include"Map.h"
#include<ostream>
#include<fstream>
#include<vector>

using namespace std;

Map::Map(int nx,int ny){
  _nx=nx;
  _ny=ny;
  _nTpPoints=0;
  _map=new char*[nx];
  for(int i=0;i<nx;i++){
    _map[i]=new char[ny];
    for(int j=0; j<ny; j++){
      _map[i][j]='.';
    }
  }
}

Map::Map(char *filename){
  ifstream mapFile(filename);
  _nTpPoints=0;
  int nRows=0;
  char tmp='.';
  vector<char> tmpMap;

  while(mapFile.get(tmp)){
    if(tmp!=' '&&tmp!='\n') tmpMap.push_back(tmp);
    if(tmp=='\n') nRows++;
    if(tmp=='T') _nTpPoints++;
  }
  
  mapFile.close();

  _tpPoints=new int*[_nTpPoints]; 
  
  for(int i=0; i<_nTpPoints; i++){
    _tpPoints[i]=new int[2];
  }

  _nx=nRows;
  _ny=tmpMap.size()/nRows;

  _map=new char*[_nx];
 
  int nTp=0;

  for(int i=0;i<_nx;i++){
    _map[i]=new char[_ny];
    for(int j=0; j<_ny; j++){
      _map[i][j]=tmpMap[i*nRows+j];
      if(_map[i][j]=='T'){
        _tpPoints[nTp][0]=i;
        _tpPoints[nTp][1]=j;
        nTp++;
      }
    }
  }
}

void Map::clear(){
  for(int i=0;i<_nx;i++){
    for(int j=0; j<_ny; j++){
      _map[i][j]='.';
    }
  }
}

Map::~Map(){
  for(int i=0; i<_nx; i++){
    delete[] _map[i];
  }

  delete[] _map;
}

void Map::flip(int *point, char state){
  _map[point[0]][point[1]] = state;
}

char Map::get(int x, int y){
  if(x>=0&&y>=0&&x<_nx&&y<_ny) return(_map[x][y]);
  else return('B');
}

char Map::get(int *point){
  return(get(point[0],point[1]));
}

int Map::getXsize(){
  return(_nx);
}

int Map::getYsize(){
  return(_ny);
}

ostream& operator<<(ostream& os, Map& m){
  int nx=m.getXsize();
  int ny=m.getYsize();
  for(int i=0; i<nx; i++){
    for(int j=0; j<ny; j++){
      os << m.get(i,j) << " ";
    }
    os << '\n';
  }
  return(os);
}

int Map::getNTPpoints(){
  return(_nTpPoints);
}

int** Map::getTPpoints(){
  return(_tpPoints);
}
