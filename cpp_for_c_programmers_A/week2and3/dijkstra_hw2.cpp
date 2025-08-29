#include <iostream>
#include <stack>
#include <vector>
#include <limits>
using namespace std;

class graph {
    int n_verts;
    bool **admat;
    int **edge_list;
    int *n_edges;
    double **cost;

    public:
    graph(int n_verts=10){
        this->n_verts=n_verts;
         
        n_edges= new int[n_verts]();
        edge_list=new int *[n_verts];
        for(int i=0; i <n_verts; i++) {
            edge_list[i]= new int[n_verts]();
        }
    }
    void add_edge(int v1,int v2, double cost=1.0){
        edge_list[v1][n_edges[v1]]=v2;
        edge_list[v2][n_edges[v2]]=v1;
        n_edges[v1]++;
        n_edges[v2]++;
        this->cost[v1][v2]=this->cost[v2][v1]=cost;
    }

    ~graph(){
        for(int i = 0; i < this->n_verts; i++){
            delete[] edge_list[i];
            delete[] cost[i];
        }
        delete[] edge_list;
        delete[] cost;
    }
    void shortest_path_dijkstra(int v1,double[] );

};
//Naive Priority Queue
class priority_queue{
    int top;
    int* elements;
    const int nmax;
    const double *priority;
    public:
       priority_queue(int nmax,double* priority):nmax(nmax), priority(priority){
           elements=new int[nmax];

       }
       ~priority_queue(){
           delete[] elements;
       }
       int pop(){
           double min=static_cast<double>(nmax);
           int min_elem=0;
           for(int i=0; i<top; i++){
               if(priority[elements[i]]<min)
                   min_elem=i;
           }
           //pop the minimum element from the priority_queue
           swap(elements[top],elements[min_elem]);
           return elements[top--];
       }
       void add_element(int vert){
           elements[top++]=vert;
       }
       bool if_empty(){
           return (top==0);
       }
       
};

void graph::shortest_path_dijkstra(int src, double dist[] ){
    double present_dist;
    int current_node;
    for(int vert=0; vert<n_verts; vert++)
        dist[vert]=numeric_limits<double>::infinity();


    priority_queue open_set(n_verts,dist);

    open_set.add_element(src);

    while(not open_set.if_empty()){
        current_node=open_set.pop();
        for(int edge=0; edge<n_edges[current_node]; edge++){
            int nbr=edge_list[current_node][edge];
            double newdist=dist[current_node]+cost[current_node][nbr];
            if(newdist<dist[nbr]){
                dist[nbr]=newdist;
            }
        }

    }


}
