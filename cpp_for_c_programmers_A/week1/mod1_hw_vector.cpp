//  Homework 1: convert c to c++, use vector

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
#include <vector>
using namespace std;

const int N=40;

//sum function, vector input
inline void sum(int& p, const int& n, vector<int> d){

  for(int i = 0; i < n; ++i)
    p= p +d[i];

}

int main(){

   int accum = 0;

   vector<int> data(N);

   for(int i = 0; i < N; ++i)
      data[i] = i;

    sum(accum, N, data);

    //output sum, c++ io
    cout << "sum is "<<accum;
    return 0;

}
