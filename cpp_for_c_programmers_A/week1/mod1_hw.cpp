//  Convert this program to C++
//
// change to C++ io
//
// change to one line comments
//
// change defines of constants to const
//
// change array to vector<>
//
// inline any short function

#include <iostream>
using namespace std;

const int N=40;

inline void sum(int& p, const int& n, int d[]){

  for(int i = 0; i < n; ++i)
    p= p +d[i];

}

int main(){

   int accum = 0;

   int *data = new int[N];

   for(int i = 0; i < N; ++i)
      data[i] = i;

    sum(accum, N, data);

    printf("sum is %d\n", accum); 
    cout << "sum is "<<accum;
    return 0;

}
