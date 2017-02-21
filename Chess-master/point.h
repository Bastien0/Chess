#pragma once
/*****************************************************************************************************/
/******************************** classe utile pour représenter des points en 2d *********************/
/*****************************************************************************************************/
class Point{
    int x;
    int y;
public:
    setx(int i){x = i;}
    sety(int j){y = j;}
    int getx() const {return x;}
    int gety() const {return y;}
    Point(){}
    Point(int x0, int y0){
        x = x0;
        y = y0;
    }
    Point(const Point& p){
        x = p.getx();
        y = p.gety();
    }
    bool operator ==(const Point& p){
        return (x == p.getx() && y == p.gety()) ;
    }
};
