#include <iostream>
using namespace std;
template <class T>
class listelem
{ 
  T data;
  listelem * next;

  listelem(T x, listelem* nx=0): data(x), next(nx){}
};

template <class T>
class slist{
  T* head;
  slist(T init){
    this->head = listelem<T>(init,0);
  }
  public :
    void append(T x);
    void prepend(T x);
    ~slist();
};
template <class T>
void  slist<T>::append(T x) {
    T* ptr= head;
    while( ptr->next != NULL)
      ptr = ptr->next;
    ptr->next=listelem<T>(x,0);
  };

template <class T>
  void slist<T>:: prepend(T x) {
    listelem<T> * temp = listelem<T>(x,this->head);
    this->head = temp;
  };
template <class T>
slist<T>::~slist(){
  listelem<T>* it=this->head;
  listelem<T>* temp;
  while (it!=NULL) 
    it=it->next;
  
}



