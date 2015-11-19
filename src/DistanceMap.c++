#include"DistanceMap.h"
#include<ostream>
#include<cmath>

using namespace std;

DistanceMap::DistanceMap(int nx,int ny){
  _nx=nx;
  _ny=ny;
  _map=new int*[nx];
  _nMap=new int*[nx];
  _pathMap=new int***[nx];
  for(int i=0;i<nx;i++){
    _map[i]=new int[ny];
    _nMap[i]=new int[ny];
    _pathMap[i]=new int**[ny];
    for(int j=0; j<ny; j++){
      _map[i][j]=-1;
      _nMap[i][j]=0;
      _pathMap[i][j]=0;
    }
  }
}

DistanceMap::~DistanceMap(){
  for(int i=0; i<_nx; i++){
    for(int j=0; j<_ny; j++){
      for(int k=0; k<(_nMap[i][j]+1);k++){
        delete[] _pathMap[i][j][k];
      }
      delete[] _pathMap[i][j];
    }
    delete[] _map[i];
    delete[] _nMap[i];
    delete[] _pathMap[i];
  }

  delete[] _map;
  delete[] _nMap;
  delete[] _pathMap;
}

void DistanceMap::clear(){
  for(int i=0;i<_nx;i++){
    for(int j=0; j<_ny; j++){
      _map[i][j]=-1;
      _nMap[i][j]=0;
      _pathMap[i][j]=0;
    }
  }
}

void DistanceMap::flip(int *point, int state){
  if(point[0]<_nx&&point[0]>=0&&point[1]<_ny&&point[1]>=0){ 
    _map[point[0]][point[1]] = state;
  }
}

void DistanceMap::flip(int x, int y, int state){
  int point[]={x,y};
  flip(point, state);
}

void DistanceMap::flipN(int *point, int state){
  if(point[0]<_nx&&point[0]>=0&&point[1]<_ny&&point[1]>=0){ 
    _nMap[point[0]][point[1]] = state;
  }
}

void DistanceMap::flipN(int x, int y, int state){
  int point[]={x,y};
  flipN(point, state);
}

void DistanceMap::flipPath(int *point, int nNodes, int** path){
  if(point[0]<_nx&&point[0]>=0&&point[1]<_ny&&point[1]>=0){
    _pathMap[point[0]][point[1]]=new int*[nNodes];
    for(int i=0; i<nNodes; i++){
      _pathMap[point[0]][point[1]][i]=new int[2];
      _pathMap[point[0]][point[1]][i][0]=path[i][0];
      _pathMap[point[0]][point[1]][i][1]=path[i][1];
    }
  }
}

void DistanceMap::flipPath(int x, int y, int nNodes, int** path){
  int point[]={x,y};
  flipPath(point,nNodes,path);
}

int DistanceMap::get(int *point){
  if(point[0]<_nx&&point[0]>=0&&point[1]<_ny&&point[1]>=0){ 
    return(_map[point[0]][point[1]]);
  }else {
    return(-1);
  }
}

int DistanceMap::get(int x, int y){
  int point[]={x,y};
  return(get(point));
}

int** DistanceMap::getPath(int *point){
  if(point[0]<_nx&&point[0]>=0&&point[1]<_ny&&point[1]>=0){ 
    return(_pathMap[point[0]][point[1]]);
  }else {
    return(0);
  }
}

int** DistanceMap::getPath(int x, int y){
  int point[]={x,y};
  return(getPath(point));
}

float DistanceMap::getDistance(int *point){
  return(sqrt(_map[point[0]][point[1]]));
}

float DistanceMap::getDistance(int x, int y){
  return(sqrt(_map[x][y]));
}

int DistanceMap::getXsize(){
  return(_nx);
}

int DistanceMap::getYsize(){
  return(_ny);
}

int DistanceMap::getN(int x, int y){
  return(_nMap[x][y]);
}

int DistanceMap::getN(int *point){
  return(getN(point[0],point[1]));
}

ostream& operator<<(ostream& os, DistanceMap& m){
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

