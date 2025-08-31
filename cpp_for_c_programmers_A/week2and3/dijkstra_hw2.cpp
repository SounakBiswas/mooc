/* Dijkstra with naive implementation of PQ */
#include <iostream>
#include <limits>
#include <ctime>
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
 * remove_edge(int v1,int v2)
 * removes edge (v1,v2) if it exists
 *
 *
 * clear_edges()
 * clear all edges of graph
 *
 * add_random_edges, described later
 *
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
    void remove_edge(int v1,int v2){
        for(int edge=0; edge<n_edges[v1]; edge++){
            if (edge_list[v1][edge]==v2){
                swap(edge_list[v1][edge],edge_list[v1][n_edges[v1]-1]);
                n_edges[v1]--;
            }

        }
    }
    void clear_edges(){
        for(int vert=0; vert<n_verts; vert++){
            n_edges[vert]=0;
        }
    }

    graph(int n_verts=10){
        this->n_verts=n_verts;

        n_edges= new int[n_verts]();
        edge_list=new int *[n_verts];
        cost=new double *[n_verts];
        for(int i=0; i <n_verts; i++) {
            edge_list[i]= new int[n_verts]();
            cost[i]= new double[n_verts]();
        }
    }
    int tot_edges(){
        int tot_edges=0;
        for(int i =0; i<n_verts; i++){
            tot_edges+=n_edges[i];
        }
        return tot_edges/2;
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
    void add_random_edges(double density,double , double);
};

/*add_random_edges(double density,double mindist=1.0, double maxdist=1.0)  
 *  fills edges randomly with density given by density
 *  randomly assigns a distance uniformly between mindist and maxdist*/
void graph::add_random_edges(double density,double mindist=1.0, double maxdist=1.0){
    double rand_dist;
    for(int vert1=0; vert1<n_verts; vert1++){
        for(int vert2=vert1+1; vert2<n_verts; vert2++){
            rand_dist=mindist+(rand()/(RAND_MAX+1.0))*(maxdist-mindist);
            //cout<<rand_dist<<endl;
            if (rand()/(RAND_MAX+1.0)<density)
                this->add_edge(vert1,vert2);
        }
    }
}

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
    public:
    int top;
    int* elements;
    const double *priority;
    const int nmax;
    priority_queue(int nmax,double* priority):nmax(nmax), priority(priority){
        top=0;
        elements=new int[nmax];

    }
    ~priority_queue(){
        delete[] elements;
    }
    int pop(){
        double min= numeric_limits<double>::infinity();
        int min_elem=0;
        for(int i=0; i<top; i++){
          if(priority[elements[i]]<min){
                min_elem=i;
                min=priority[elements[i]];

          }
        }
        //pop the minimum element from the priority_queue
        swap(elements[top-1],elements[min_elem]);
        return elements[--top];
    }
    void add_element(int vert){
        //cout<<"add elem, top:"<<top<<" vert:"<<vert<<" prior:"<<priority[vert]<<endl;
        //getchar();
        elements[top++]=vert;
    }
    bool if_empty(){
        return (top==0);
    }
    bool contains_element(int v){
        for(int i=0; i<top; i++)
            if(elements[i]==v)
                return true;
        return false;
    }

};


/*dijkstra's algorithm
 *
 * Dijkstra's algorithm as in lectures. The open set from lectures is implemented as a 
 * priority queue (PQ). The PQ is starts off with all vertices reachable from the source node. 
 * The nearest node is removed from 
 */


void graph::shortest_path_dijkstra(int src, double dist[] ){
    double present_dist;
    bool *closed = new bool[n_verts];
    int current_node;
    for(int vert=0; vert<n_verts; vert++){
        dist[vert]=numeric_limits<double>::infinity();
        closed[vert]=false;
    }

    priority_queue open_set(n_verts,dist);
    //cout<<open_set.nmax<<endl;
    dist[src]=0;

    open_set.add_element(src);

    while(not open_set.if_empty()){
        //cout<<"bef nelem:"<<open_set.top<<endl;
        current_node=open_set.pop();
        //cout<<"popped:"<<current_node<<endl;
        //cout<<"aft nelem:"<<open_set.top<<endl;
        closed[current_node]=true;
        for(int edge=0; edge<n_edges[current_node]; edge++){
            int nbr=edge_list[current_node][edge];
            double newdist=dist[current_node]+cost[current_node][nbr];
            //cout<<"nbr: "<<nbr<<" basedist:"<<dist[current_node]<<" new dis:"<<newdist<<" allowed:"<<closed[nbr]<<" old dist: "<<cost[current_node][nbr]<<endl;
            //getchar();
            if(! closed[nbr]){
                if(! open_set.contains_element(nbr)) 
                    open_set.add_element(nbr);
                //cout<<"dist change:"<<dist[nbr]<<" "<<newdist<<endl;
                if(newdist<dist[nbr]){
                    dist[nbr]=newdist;

                }
            }
        }
    }
}

/*main function
 * loops over the two densities 20 % and 50%, creates random graphs and
 * calculates the average shortest distance betwen vertices
 *
 * To obtain a better answe, I have averaged over ngraph different graphs.
 *
 * variables:
 * dist: dist[v] has the shortest distance from 0 to v after the algorithm
 * pair_count: number of reachable vertices
 * ngraphs: number of independent graphs to average over
 * avg_path: stores the shortest average path length
 */

int main(){
    int n_verts=50;
    double densities[]={0.2,0.4};
    double density;
    double *dist=new double[50];
    const double min_dist=1.0;
    const double max_dist=10.0;
    const int ngraphs=1;
    graph G(n_verts);
    srand(10);

    for(int density_ix=0; density_ix<1; density_ix++){
        double avg_path=0;
        int pair_count=0;
        density=densities[density_ix];
        for(int ngr=0; ngr<ngraphs; ngr++){
           G.clear_edges();
           G.add_random_edges(density,1.0,10.0);
           G.shortest_path_dijkstra(0,dist);
           for(int i=1; i <n_verts; i++){
               if(dist[i]!=numeric_limits<double>::infinity()){
                   pair_count+=1;
                   avg_path+=dist[i];
               }
           }

        }
        cout<<"density"<<density<<", average shortest path="<<avg_path/(ngraphs*(n_verts-1.0))<<endl;
    }
}

