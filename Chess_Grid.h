#pragma once
#include"Chessmen.h"
#include <vector>
#include <iostream>
using namespace std;

class Grid{
    vector<Chessman> whiteLostChessmen;
    vector<Chessman> blackLostChessmen;
    Chessman* grid;
public:
    Grid();
    Chessman* operator()(int coord0, int coord1);
    void operator()(int coord0, int coord1, Chessman& chessman);
    void move(int coord[2], Chessman& chessman, string promotion = "");
    void setNone(int x, int y);
    int* king_position(bool isWhite);
    bool isVoid(int x, int y);
    vector<Chessman> list_chessman_col(bool colorIsWhite);
    bool sameColor(Chessman& chessman, int x, int y);
    bool isChessed(Chessman& chessman, int x, int y);
};
