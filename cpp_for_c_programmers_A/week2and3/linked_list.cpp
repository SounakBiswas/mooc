#include <iostream>
using namespace std;
template <class T>
class listelem
{ 
  public:
  T data;
  listelem * next;

  listelem(T x, listelem* nx=0): data(x), next(nx){}
  ~listelem(){
    cout<<"destructor called for element,"<<"data="<<this->data<<endl;
    delete next;
  };

};

template <class T>
class slist{
  public :
    listelem<T>* head;
    slist(T init){
      this->head = new listelem<T>(init,0);
    }
    void append(T x);
    void prepend(T x);
    ~slist();
};

template <class T>
slist<T>::~slist(){
  cout<<"list destructor called"<<endl;
  listelem<T>* it=this->head;
  listelem<T>* temp;
  while (it!=NULL) {
    temp=it;
    it=it->next;
  }
  
}




template <class T>
void  slist<T>::append(T x) {
    listelem<T>* ptr= head;
    while( ptr->next != NULL)
      ptr = ptr->next;
    ptr->next=new listelem<T>(x,0);
  };

template <class T>
  void slist<T>:: prepend(T x) {
    listelem<T> * temp = new listelem<T>(x,this->head);
    this->head = temp;
  };
template <class T>
  ostream& operator<<(ostream& os,slist<T>& list) {
    cout<<"print list"<<endl;
    listelem<T>* ptr= list.head;
    while( ptr->next != NULL){
      os<<ptr->data<<" ";
      ptr = ptr->next;
    }
    return os;
  };
int main(){
  slist<int> arr(1);
  for (int i=1; i<= 10; i++)
    arr.append(i);
  arr.prepend(-1);
  cout<<arr<<endl;
}



