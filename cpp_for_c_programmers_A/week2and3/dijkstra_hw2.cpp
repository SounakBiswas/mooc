#include <iostream>
using namespace std;

class graph {
  int n_verts;
  bool **admat;
  double **cost;

  public:
    graph(int n_verts=10){
      this->n_verts=n_verts;
      admat = new bool*[n_verts];
      for(int i=0; i <n_verts; i++) {
        admat[i]= new bool[n_verts]();
      }
    }
    void add_edge(int i,int j, double cost=1.0){
      admat[i][j]=admat[j][i]=true;
      this->cost[i][j]=this->cost[j][i]=cost;
    }
    ~graph(){
      for(int i = 0; i < this->n_verts; i++){
        delete[] admat[i];
        delete[] cost[i];
      }
      delete[] admat;
      delete[] cost;
    }
    double shortest_path_dijkstra(int v1,int v2);

};

double graph::shortest_path_dijkstra(int v1, int v2){
  bool* closed = new bool[n_verts]();
  bool* open = new bool[n_verts]();
  double* dist = new double[n_verts]();
  open[v1]=0;
  dist[v1]=0.0;
  while 

}
