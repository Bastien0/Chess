#pragma once
#include "Chessmen.h"
#include "point.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
using namespace std;

class Chessman;

class Grid{
    Chessman** grid;
    int countMove;
    int countHalfMove;
    bool whiteIsPlaying;
public:
    // get and set
    bool getWhiteIsPlaying(){ return whiteIsPlaying; }

    // Constructeur et desctructeur
    Grid(string s);
    ~Grid();

    Chessman* operator()(int coord0, int coord1);
    void operator()(int coord0, int coord1, Chessman* chessman);
    void move(Point point, Chessman *chessman);
    void unmove(Chessman *departure, Chessman *arrival, Point final, Point Enpassant);
    void setNone(int x, int y);
    Point king_position(bool isWhite);
    bool isVoid(int x, int y);
    vector<Chessman *> list_chessman_col(bool colorIsWhite);
    bool sameColor(Chessman *chessman, int x, int y);
    bool isChessed(Chessman *chessman, int x, int y);
    string fen();
};
