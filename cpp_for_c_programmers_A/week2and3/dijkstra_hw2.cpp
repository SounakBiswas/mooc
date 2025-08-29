#include <iostream>
#include <stack>
#include <vector>
#include <limits>
using namespace std;
/* Class graph. I maintain a edgelist instead of 
 * adjacency matrix, hoping it would be faster for not-too-dense graphs.
 *
 * member variables:
 * n_verts: number of vertices of graph
 * n_edges: n_edges[v] gives the number of edges of node v
 * edgelist: edgelist[v][k], 0<=k<=n_edges[v] gives the kth neighbour of vertex v
 * cost: cost[u][v] gives the weight/cost of edge (u,v)
 *
 * member functions:
 * void add_edge(int v1,int v2, double cost=1.0)
 * adds an edge (v1,v2) between v1 and v2
 *
 * shortest_path_dijstra, described later
 */

class graph {
    int n_verts;
    int **edge_list;
    int *n_edges;
    double **cost;

    public:
    void add_edge(int v1,int v2, double cost=1.0){
        edge_list[v1][n_edges[v1]]=v2;
        edge_list[v2][n_edges[v2]]=v1;
        n_edges[v1]++;
        n_edges[v2]++;
        this->cost[v1][v2]=this->cost[v2][v1]=cost;
    }

    graph(int n_verts=10){
        this->n_verts=n_verts;
         
        n_edges= new int[n_verts]();
        edge_list=new int *[n_verts];
        for(int i=0; i <n_verts; i++) {
            edge_list[i]= new int[n_verts]();
        }
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
/* Class priority_queue
 * This implements the Open set from the lectures in a stack-like structure called 
 * priority queue, of which I have made a naive implementation.
 *
 * member variables:
 * int * elements : elements[i] gives the ith element of the open set.
 * int top: index of the top element. This helps not scanning over all vertices to look for            the minimum distance. elements[0] to elements[top-1] are the elements of 
 *          the open set
 * int nmax: maximum number of elements, used to allocate space.
 * int priority: used to store distance of elements, to return the shortest distance
 *
 * member functions:
 *
 * add elements(int vert) 
 * adds the vertex vert into the open set.
 *
 *
 * pop():
 * removes the element with lowest priority (distance) from the queue (open set). I swap
 * the minimum element with element[top-1] so that element[0] to element[top-1] continues
 * to describe the open set.
 * 
 *
 */
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
           swap(elements[top-1],elements[min_elem]);
           return elements[top--];
       }
       void add_element(int vert){
           elements[top++]=vert;
       }
       bool if_empty(){
           return (top==0);
       }
       
};


/*dijkstra's algorithm
 *
 * Dijkstra's algorithm as in lectures. Each time the vertex with minimum distance 
 * is removed from the open set.
 */

void graph::shortest_path_dijkstra(int src, double dist[] ){
    double present_dist;
    int current_node;
    for(int vert=0; vert<n_verts; vert++)
        dist[vert]=numeric_limits<double>::infinity();

    priority_queue open_set(n_verts,dist);
    dist[src]=0;

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



int main(){}
