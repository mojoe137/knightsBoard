class DistanceMap{
  public:
    DistanceMap(int,int);
    ~DistanceMap();
    void clear();
    void flip(int *,int);
    void flip(int, int, int);
    void flipN(int *,int);
    void flipN(int, int, int);
    void flipPath(int *,int, int**);
    void flipPath(int, int, int, int**);
    int get(int *);
    int get(int ,int);
    int getN(int *);
    int getN(int ,int);
    int** getPath(int *);
    int** getPath(int ,int);
    float getDistance(int *);
    float getDistance(int ,int);
    int getXsize();
    int getYsize();
  private:
    int **_map;
    int **_nMap;
    int ****_pathMap;
    int _nx, _ny;
};
