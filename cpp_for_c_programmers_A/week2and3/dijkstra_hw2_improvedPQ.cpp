/* Dijkstra with Priority queues based on a binary heap.
 * All codes are in the same file, to facilitate homework
 * submission*/
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


/*class priority_queue{
 * member variables:
 *    int* data : stores entries of priortiy queue (graph vertices);
 *    double* keys: keys[v] store present distance(priority) of node v;
 *    int size: total size of queue;
 *
 * private member functions:
 *    inline void swim(int pos): maintain binary heap property by upward
 *    shifting an element from position pos
 *
 *
 *
 *    inline void sink(int pos): maintain binary heap property by downward
 *    shifting an element from position pos
 *
 *    int find_index(int entry): find index of an element(vertex)
 *
 * public member functions:
 *    priority_queue(int size): constructor
 *
 *    inline bool contains_element(int entry): check if entry exists in the PQ
 *
 *    void push(int entry,double key): push a new element into PQ, maintaing the
 *    heap property
 *
 *    double pop(): remove and return the element with minimum priority
 *
 *    int get_size(): get size of the queue
 *
 *    void change_priority(int entry, double new_priority):  change the priority
 *    of an entry, after finding the position of the entry
 */
class priority_queue{
    int* data;
    double* keys;
    int nmax;
    int size;
    inline void swim(int pos){
        while (keys[data[pos]]<keys[data[pos/2]] and pos!=0){
            swap(data[pos],data[pos/2]);
            pos=pos/2;
        }

    }

    inline void sink(int pos){
        int swap_pos;
        int branch1, branch2;
        while(pos<size){
            swap_pos=pos;
            branch1=2*pos;
            branch2=2*pos+1;
            if(branch1<size and keys[data[pos]]>keys[data[branch1]])
                swap_pos=branch1;
            if(branch2<size and keys[data[swap_pos]]>keys[data[branch2]])
                swap_pos=branch2;
            if(swap_pos!=pos){
                swap(data[pos],data[swap_pos]);
                pos=swap_pos;
            }
            else 
                break;

        }


    }
    int find_index(int entry){
        int idx=-1;
        for(idx=0; idx<size; idx++){
            if(data[idx]==entry){
                break;
            }
        }
        return idx;
    }
    public:
    priority_queue(int nmax){
        data= new int[nmax];
        keys= new double[nmax];
        size=0;
    }
    ~priority_queue(){
        delete[] data;
        delete[] keys;
    }
    inline bool contains_element(int entry){
        return (find_index(entry)<size);
    }
    void push(int entry,double priority){
        data[size++]=entry;
        keys[entry]=priority;
        int pos=size-1;
        swim(pos);
    }
    double pop(){
        swap(data[0],data[size-1]);
        int pos=0;
        sink(pos);
        return data[--size];
    }
    int get_size(){
        return size;
    }
    void change_priority(int entry, double new_priority){
        int pos=find_index(entry);
        double old_priority=keys[pos];
        keys[pos]=new_priority;
        if(old_priority<new_priority)
            swim(pos);
        else 
            sink(pos);
    }
};


/*Dijkstra's algorithm
 *
 * Dijkstra's algorithm as in lectures. The open set from lectures is implemented as a 
 * priority queue (PQ). The implementation is using a binary heap. 
 */


void graph::shortest_path_dijkstra(int src, double dist[] ){
    double newdist;
    //closed array keeps track of vertices that have already been removed form the open set
    bool *closed = new bool[n_verts];
    int current_node;
    for(int vert=0; vert<n_verts; vert++){
        dist[vert]=numeric_limits<double>::infinity();
        closed[vert]=false;
    }

    //initialise priority queue with the source node
    priority_queue open_set(n_verts);
    dist[src]=0;
    open_set.push(src,dist[src]);
    while(open_set.get_size()!=0){
        current_node=open_set.pop();
        closed[current_node]=true;
        for(int edge=0; edge<n_edges[current_node]; edge++){
            int nbr=edge_list[current_node][edge];
            newdist=dist[current_node]+cost[current_node][nbr];
            if(not closed[nbr]){
                if(! open_set.contains_element(nbr) ){
                    open_set.push(nbr,newdist);
                    dist[nbr]=newdist;
                    //cout<<"pushed "<<nbr<<endl;
                }
                else{
                    if(newdist<dist[nbr]){
                        dist[nbr]=newdist;
                        open_set.change_priority(nbr,newdist);
                    }
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
    const int ngraphs=1000;
    graph G(n_verts);
    srand(clock());

    for(int density_ix=0; density_ix<2; density_ix++){
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
        cout<<"density"<<density<<", average shortest path, averaged over "<<ngraphs<<" graphs, ="<<avg_path/(ngraphs*(n_verts-1.0))<<endl;
    }
}

