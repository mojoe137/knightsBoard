class Map{
  public:
    Map(int,int);
    Map(char *);
    ~Map();
    void clear();
    void flip(int *,char);
    char get(int *);
    char get(int ,int);
    int getXsize();
    int getYsize();
    int getNTPpoints();
    int** getTPpoints();
  private:
    char **_map;
    int **_tpPoints;
    int _nx, _ny;
    int _nTpPoints;
};

