#include <iostream>
#include <stack>
#include <vector>
#include <limits>
#include <iomanip>
using namespace std;
/*  minimum binary-heap
 *  integer entries keyed by doubles*/
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
    double pop(){
        swap(data[0],data[size-1]);
        int pos=0;
        sink(pos);
        return data[--size];
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
