#include <iostream>
#include <stack>
#include <vector>
#include <limits>
using namespace std;
/*  minimum binary-heap
 *  integer entries keyed by doubles*/
class priority_queue{
    int* data;
    double* keys;
    int nmax;
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
        while (keys[data[pos]]<keys[parent(pos)] and pos!=0){
            swap(data[pos],data[parent(pos)]);
            pos=parent(pos);
        }

    }

    inline void sink(int pos){
        int swap_pos;
        int branch1, branch2;
        while(pos<size){
            swap_pos=pos;
            branch1=child1(pos);
            branch2=child2(pos);
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
        size--;
        sink(0);
        return data[size];
    }
    int get_size(){
        return size;
    }
    void change_priority(int entry, double new_priority){
        int pos=find_index(entry);
        double old_priority=keys[data[pos]];
        keys[entry]=new_priority;
        if(old_priority<new_priority)
            swim(pos);
        else 
            sink(pos);
    }
};
