#include<fstream>
#include<iostream>
#include<vector>
#include<unistd.h> 
#include<stdlib.h>
#include"Map.h"
#include"DistanceMap.h"

using namespace std;

bool checkMove(int *,int *);
bool checkMove(int *,int *, Map&);
int checkLanding(int *, Map&);
void playMoves(ifstream&,Map&);
void randomStep(int*);
void teleport(int&,int&,Map&);
void teleport(int*,Map&);
void findPathRandom(ostream&, Map&, int *,int *);
void findPathDA(ostream&, Map&, DistanceMap&, int *, int *);
bool deadEnd(int*, Map&);
ostream& operator<<(ostream&, Map&);
ostream& operator<<(ostream&, DistanceMap&);

int main(int argc, char** argv){
  char mapFileName[]="data/mapFile.txt";
  Map map(mapFileName);
  Map mapClean(mapFileName);
  int sizeX=map.getXsize(), sizeY=map.getYsize();
  DistanceMap dmap(sizeX,sizeY);
  const char *filename=(argc==2)?argv[1]:"data/move.txt";
  ofstream guess(filename);
  int start[]={21,21};
  int end[]={0,31};
  mapClean.flip(start,'S');
  mapClean.flip(end,'E');
//  findPathDA(guess, map, dmap, start, end);
  findPathRandom(guess, map, start, end);
  guess.close(); 
    
  ifstream move(filename);
  playMoves(move,mapClean);
  move.close();

  return(0);
}

void findPathDA(ostream &out, Map &map, DistanceMap &dmap, int* start, int* end, bool longest){
  map.flip(start,'S');
  map.flip(end,'E');
  int const dx[]={-2,-2,-1,-1,1,1,2,2};
  int const dy[]={1,-1,2,-2,2,-2,1,-1};
  vector<int> checkedX, nextCheckX; 
  vector<int> checkedY, nextCheckY; 

  checkedX.push_back(start[0]);
  checkedY.push_back(start[1]);

  int **startPath;
  startPath=&start;
  dmap.flip(start,0);
  dmap.flipN(start,0);
  dmap.flipPath(start,1,startPath);

  while(map.get(start)!='E'){
    nextCheckX.clear();
    nextCheckY.clear();

    for(int i=0; i<checkedX.size();i++){
      for(int j=0; j<8; j++){
        int xtmp=checkedX[i]+dx[j], ytmp=checkedY[i]+dy[j];
        int startTmp[]={checkedX[i],checkedY[i]}, endTmp[]={xtmp,ytmp};
        if( checkMove(startTmp, endTmp, map) ){
          if(dmap.get(xtmp,ytmp)==-1){
            nextCheckX.push_back(xtmp);
            nextCheckY.push_back(ytmp);
            dmap.flip(xtmp,ytmp,-2);
          }
        }
      }
    }

    for(int i=0; i<nextCheckX.size(); i++){
      int xToCheck=nextCheckX[i], yToCheck=nextCheckY[i];
      int bestx=-1, besty=-1;
      int startTmp[]={xToCheck,yToCheck};
      int weight=checkLanding(startTmp,map);
      for(int j=0; j<8; j++){
        int xtmp=xToCheck+dx[j], ytmp=yToCheck+dy[j];
        int endTmp[]={xtmp,ytmp};
        if(checkMove(startTmp,endTmp,map)){
          teleport(xtmp,ytmp,map);
          int testD=dmap.get(xtmp,ytmp);
          if(testD>=0){
            testD+=weight;
            int testN=dmap.getN(xtmp,ytmp)+1;
            int currentD = dmap.get(startTmp);
            if(currentD < 0 || (testD < currentD)){
              dmap.flip(xToCheck,yToCheck,testD);
              dmap.flipN(xToCheck,yToCheck,testN);
              bestx=xtmp;
              besty=ytmp;
            }
          }
        }
      }

      int nMoves=dmap.getN(xToCheck,yToCheck);
      int **pathOld=dmap.getPath(bestx,besty);
      int **pathNew=new int*[nMoves+1];
      
      for(int i=0; i<nMoves; i++){
        pathNew[i]=new int[2];
        pathNew[i][0]=pathOld[i][0];
        pathNew[i][1]=pathOld[i][1];
      }

      pathNew[nMoves]=new int[2];
      pathNew[nMoves][0]=xToCheck;
      pathNew[nMoves][1]=yToCheck;

      dmap.flipPath(xToCheck,yToCheck,nMoves+1,pathNew);
    }

    checkedX.clear();
    checkedY.clear();

    for(int i=0; i<nextCheckX.size();i++){
      teleport(nextCheckX[i],nextCheckY[i],map);
      checkedX.push_back(nextCheckX[i]);
      checkedY.push_back(nextCheckY[i]);
      if(map.get(nextCheckX[i],nextCheckY[i])=='E'){
        start[0]=nextCheckX[i];
        start[1]=nextCheckY[i];

        int **bestpath=dmap.getPath(start);
        for(int j=0; j<(dmap.getN(start)+1);j++){
          out << bestpath[j][0] << " " << bestpath[j][1] << endl;
        }
      }
    }
  }
}

void findPathRandom(ostream& out, Map &map, int* start, int* end){
  map.flip(start,'S');
  map.flip(end,'E');

  out << start[0] << " " << start[1] << endl;
  while(map.get(start)!='E'&&!deadEnd(start,map)){
    int tmp[]={0,0};
    randomStep(tmp);
    int newx=start[0]+tmp[0];
    int newy=start[1]+tmp[1];
    int end[]={newx,newy};
    if(checkMove(start,end,map)){
      char old=map.get(start);
      map.flip(start,'R');
      start[0]=newx;
      start[1]=newy;
      cout << map << endl;

      out << start[0] << " " << start[1] << endl;
    }
  }
}

bool deadEnd(int *point,Map& map){
  int waysOut=0;
  int const dx[]={-2,-2,-1,-1,1,1,2,2};
  int const dy[]={1,-1,2,-2,2,-2,1,-1};
  for(int k=0; k<8;k++){
    int xtmp=point[0]+dx[k], ytmp=point[1]+dy[k];
    int end[]={xtmp,ytmp};
    waysOut+=checkMove(point,end,map);
  }
  return(waysOut==0);
}

void randomStep(int* step){
  int dx=2*(rand()%2)-1;
  int dy=2*(rand()%2)-1;
  if(rand()%2) dx*=2;
  else dy*=2;
  step[0]=dx;
  step[1]=dy;
}
  
bool checkMove(int *start,int *end, Map &map){
  if(!checkMove(start,end)) return(false);

  int dx=end[0]-start[0], dy=end[1]-start[1];
  int dxSign=dx/abs(dx), dySign=dy/abs(dy);
  bool xy=true, yx=true;

  for(int i=1;i<=abs(dx);i++){
    if(map.get(start[0]+i*dxSign,start[1])=='B') xy=false;
  }
  for(int i=0;i<abs(dy);i++){
    if(map.get(start[0]+dx,start[1]+i*dySign)=='B') xy=false;
  }

  for(int i=1;i<=abs(dy);i++){
    if(map.get(start[0],start[1]+i*dySign)=='B') yx=false;
  }
  for(int i=0;i<abs(dx);i++){
    if(map.get(start[0]+i*dxSign,start[1]+dy)=='B') yx=false;
  }

  return((yx||xy)&&map.get(end)!='R'&&map.get(start)!='R'&&map.get(end)!='B'&&map.get(start)!='B');
}

int checkLanding(int *end, Map &map){
  char tmp=map.get(end);
  int weight=1;

  switch(tmp){
    case 'W': weight=2;
      break;
    case 'L': weight=5;
      break;
  }

  return(weight);
}
 
bool checkMove(int *start,int *end){
  int dx=end[0]-start[0];
  int dy=end[1]-start[1];

  if(dx==0||dy==0) return(false);
  else if((dx==1 || dx==-1)&&(dy==2||dy==-2)) return(true);
  else if((dx==2 || dx==-2)&&(dy==1||dy==-1)) return(true);

  return(false);
}

void playMoves(ifstream &move, Map &map){
  int start[]={0,0}, end[]={0,0};
  move >> start[0] >> start[1];
  int n=0;
  int dtime=5e5;
  bool tpStep=false;

  while(!move.eof()&&move.is_open()){
    move >> end[0] >> end[1];

    if(!move.eof()){
      if(n==0){
        map.flip(start,'S');
      }
      usleep(dtime);
      if(!system("clear")){
        cout << map;
      }
    }

    n++;

    if(n>1&&!tpStep) map.flip(start,char(int('O')+((n-2)%2)*32));
    if(tpStep){ 
      map.flip(start,'T');
      tpStep=false; 
    }
    if(map.get(end)=='T') tpStep=true;

    map.flip(end,'K');
    start[0]=end[0];
    start[1]=end[1];
  }
 
  usleep(dtime);
  map.flip(end,'K');
  if(!system("clear")){
    cout << map << endl;
    usleep(dtime);
  }
  map.flip(end,'E');
  if(!system("clear"));
  cout << map << endl;
}

void teleport(int &xtmp, int &ytmp,  Map &map){
  if(map.get(xtmp,ytmp)=='T'){
    int iPoint=0;
    int **tpPoints=map.getTPpoints();
    int ntpp=map.getNTPpoints();
    int xToCheck=xtmp, yToCheck=ytmp;

    for(int j=0;j<ntpp;j++){
      iPoint++;
      if(tpPoints[j][0]==xToCheck&&tpPoints[j][1]==yToCheck){
        xtmp=tpPoints[iPoint%ntpp][0];
        ytmp=tpPoints[iPoint%ntpp][1];
      }
    }
  }
}

void teleport(int* point, Map &map){
  teleport(point[0],point[1],map);
}
