#include <iostream>
using namespace std;
class point{
  public:
    double x,y;
    double getx(){return x;}
    double gety(){return y;}
    void setx(double v){ x=v;}
    void sety(double v){ y=v;}
    point(){x=y=0.0;}


};

point operator+ (point& p1, point& p2){
  point sum;
  sum.setx( p1.getx()+p2.getx());
  sum.sety( p1.gety()+p2.gety());
  return sum;
}
ostream& operator<< (ostream& out, const point& p){
  out<<"( "<<p.x<<","<<p.y<<" ) ";
  return out;
}

