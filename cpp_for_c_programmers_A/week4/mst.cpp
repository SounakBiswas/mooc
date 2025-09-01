/* Dijkstra with Priority queues based on a binary heap.
 * All codes are in the same file, to facilitate homework
 * submission*/
#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <fstream>
#include <stack>
#include <iterator>
#include <vector>
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
    void add_edge(int v1,int v2, double cost=1.0,bool symm=true){
        edge_list[v1][n_edges[v1]]=v2;
        n_edges[v1]++;
        this->cost[v1][v2]=cost;

        if(symm){
            edge_list[v2][n_edges[v2]]=v1;
            n_edges[v2]++;
            this->cost[v2][v1]=cost;
        }
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
    graph(string filename){
        ifstream data_file(filename);
        istream_iterator<int> start(data_file),end;
        n_verts=*(start++);

        n_edges= new int[n_verts]();
        edge_list=new int *[n_verts];
        cost=new double *[n_verts];
        for(int i=0; i <n_verts; i++) {
            edge_list[i]= new int[n_verts]();
            cost[i]= new double[n_verts]();
        }

        cout<<n_verts<<endl;
        int v1,v2,cost;
        while(start!=end){
            v1=*(start++);
            v2=*(start++);
            cost=*(start++);
            add_edge(v1,v2,cost,false);
        }
    };
    int get_tot_edges(){
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
    void mst_prim();
    void mst_kruskal();
};
pair<int,int> make_edge(int x, int y){
    return x<y?make_pair(x,y):make_pair(y,x);
}
template <class T>
class priority_queue{
    vector<pair<T,double>> data;
    int size;
    inline void swim(int pos){
        while (data[pos].second<data[pos/2].second and pos!=0){
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
            if(branch1<size and data[pos].second>data[branch1].second)
                swap_pos=branch1;
            if(branch2<size and data[swap_pos].second>data[branch2].second)
                swap_pos=branch2;
            if(swap_pos!=pos){
                swap(data[pos],data[swap_pos]);
                pos=swap_pos;
            }
            else 
                break;

        }


    }
    int find_index(T entry){
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
        data.resize(nmax);
        size=0;
    }
    inline bool contains_element(T entry){
        return (find_index(entry)<size);
    }
    void push(T entry,double priority){
        data[size++]=make_pair(entry,priority);
        int pos=size-1;
        swim(pos);
    }
    T pop(){
        swap(data[0],data[size-1]);
        int pos=0;
        sink(pos);
        return data[--size].first;
    }
    int get_size(){
        return size;
    }
    void change_priority(T entry, double new_priority){
        int pos=find_index(entry);
        double old_priority=data[pos].second;
        data[pos].second=new_priority;
        if(old_priority<new_priority)
            swim(pos);
        else 
            sink(pos);
    }
    void display(){
        int row=0;
        int rowend=1;
        int elem=0;
        cout<<setprecision(1)<<std::fixed;
        while(elem<size){
            for(; elem <rowend && elem<size; elem++ ){
                cout<<data[elem].first<<"("<<data[elem].second<<") ";
            }
            cout<<endl;
            rowend+=2*rowend;
            cout<<endl;
        }
    }
};

void graph::mst_prim(){
    int src=0;
    int nbr;
    int tot_edges=get_tot_edges();
    stack<int> open_verts;
    priority_queue<pair<int,int>> open_edges(tot_edges);
    vector<bool> in_mst(n_verts,false);

    auto check_loop=[in_mst](pair<int,int> p ){
      return in_mst[p.first] && in_mst[p.second];
    };
    auto get_open_vert=[in_mst](pair<int,int> p ){
      if(in_mst[p.first])
        return p.second;
      else 
        return p.first;
    };

    open_verts.push(src);

    while(! open_verts.empty()){
        src=open_verts.top();
        open_verts.pop();
        in_mst[src]=true;
        for(int edge=0; edge<n_edges[src]; edge++){
            nbr=edge_list[src][edge];
            if(not in_mst[nbr])
                open_edges.push(make_edge(src,nbr),cost[src][nbr]);

        }
        auto edge=open_edges.pop();
        while( check_loop(edge)){
          edge= open_edges.pop();
        }
        int new_vert=get_open_vert(edge);
        open_verts.push(get_open_vert(edge));
        in_mst[new_vert]=true;
    }

}



int main(){
    graph G("mst_data.dat");


}

