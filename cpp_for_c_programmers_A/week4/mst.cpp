/* Dijkstra with Priority queues based on a binary heap.
 * All codes are in the same file, to facilitate homework
 * submission*/
#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <fstream>
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
    double total_cost(){
        double total=0.0;
        for(int vert =0; vert<n_verts; vert++){
            for(int edge=0; edge<n_edges[vert]; edge++){
                total += cost[vert][edge_list[vert][edge]];
            }
        }
        return total/2;
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
    graph mst_prim();
    graph mst_prim_alt();
    graph mst_kruskal();
};
pair<int,int> make_edge(int x, int y){
    return x<y?make_pair(x,y):make_pair(y,x);
}
template <class T>
class priority_queue{
    vector<pair<T,double>> data;
    int size;
    inline int parent(int pos){
      return (pos+1)/2-1;

    }
    inline int child1(int pos){
      return 2*(pos+1)-1;

    }
    inline int child2(int pos){
      return 2*(pos+1);

    }
    inline void swim(int pos){
        while (data[pos].second<data[parent(pos)].second and pos!=0){
            swap(data[pos],data[parent(pos)]);
            pos=parent(pos);
        }

    }

    inline void sink(int pos){
        int swap_pos;
        int branch1, branch2;
        while(pos<size){
            swap_pos=pos;
            branch1=2*pos+1;
            branch2=2*pos+2;
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
            if(data[idx].first==entry){
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
    void pop(){
        swap(data[0],data[size-1]);
        size--;
        sink(0);
    }
    T top(){
        return data[0].first;
    }
    int get_size(){
        return size;
    }
    void change_priority(T entry, double new_priority){
        int pos=find_index(entry);
        double old_priority=data[pos].second;
        data[pos].second=new_priority;
        if(new_priority<old_priority)
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

graph graph::mst_prim(){
    int nbr;
    int tot_edges=get_tot_edges();
    priority_queue<pair<int,int>> open_edges(tot_edges);
    vector<bool> in_mst(n_verts,false);
    graph mst(n_verts);

    auto check_loop=[&](pair<int,int> p ){
        return (in_mst[p.first] and in_mst[p.second]);
    };

    auto get_open_vert=[&](pair<int,int> p ){
        if(in_mst[p.first])
            return p.second;
        else 
            return p.first;
    };

    auto get_closed_vert=[&](pair<int,int> p ){
        if(in_mst[p.first])
            return p.first;
        else 
            return p.second;
    };


    int src=0;
    while(src>=0){
        in_mst[src]=true;
        for(int edge=0; edge<n_edges[src]; edge++){
            nbr=edge_list[src][edge];
            if(not in_mst[nbr]){
                open_edges.push(make_edge(src,nbr),cost[src][nbr]);
            }

        }
        while(open_edges.get_size()>0 and check_loop(open_edges.top())){
            open_edges.pop();
        }
        if(open_edges.get_size()==0){
            src=-1;
        }
        else{
            src=get_open_vert(open_edges.top());
            nbr=get_closed_vert(open_edges.top());
            open_edges.pop();
            mst.add_edge(src,nbr,cost[src][nbr]);
        }
    }

    return mst;

}

graph graph::mst_prim_alt(){
    int nbr;
    int tot_edges=get_tot_edges();
    priority_queue<int> open_verts(tot_edges);
    vector <double> vcost(n_verts,numeric_limits<double>::infinity());
    vector <int> mst_edge(n_verts,-1);
    vector<bool> in_mst(n_verts,false);
    graph mst(n_verts);

    auto check_loop=[&](pair<int,int> p ){
        return (in_mst[p.first] and in_mst[p.second]);
    };

    auto get_open_vert=[&](pair<int,int> p ){
        if(in_mst[p.first])
            return p.second;
        else 
            return p.first;
    };

    auto get_closed_vert=[&](pair<int,int> p ){
        if(in_mst[p.first])
            return p.first;
        else 
            return p.second;
    };


    int src=0;
    vcost[src]=0;
    for(int vert=0; vert<n_verts; vert++)
        open_verts.push(vert,vcost[vert]);
    while(open_verts.get_size()>0){
        src=open_verts.top();
        open_verts.pop();
        in_mst[src]=true;
        for(int edge=0; edge<n_edges[src]; edge++){
            nbr=edge_list[src][edge];
            if(not in_mst[nbr]){
                if(cost[src][nbr]<vcost[nbr]){
                    vcost[nbr]=cost[src][nbr];
                    open_verts.change_priority(nbr,vcost[nbr]);
                    mst_edge[nbr]=src;
                }
            }

        }
    }
    for(int vert=0; vert<n_verts; vert++){
        if(mst_edge[vert]!=-1){
            mst.add_edge(mst_edge[vert],vert,vcost[vert]);

        }


    }

    return mst;

}



int main(){
    graph G("mst_data.txt");
    graph mst=G.mst_prim();
    cout<<mst.get_tot_edges()<<" "<<mst.total_cost()<<endl;
    graph mst2=G.mst_prim_alt();
    cout<<mst2.get_tot_edges()<<" "<<mst2.total_cost()<<endl;

}

