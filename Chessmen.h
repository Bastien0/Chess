#pragma once
#include<vector>
#include "Chess_Grid.h"
#include <iostream>
#include "point.h"
using namespace std;


class Grid;

class Chessman{
    int x,y;
    string name;
    bool isWhite;
public:
    //Constructeur
    Chessman(){}
    Chessman(int a, int b, string n, bool iW);
    Chessman(int a, int b, string n);
    //Accesseurs et setter
    int getx(){ return x;}
    int gety(){ return y;}
    bool getIsWhite(){return isWhite;}
    string getName(){ return name; }
    void setx(int i) { x = i; }
    void sety(int i) { y = i; }
    void setName(string n) { name = n; }
    virtual void setdouble_done(bool value);
    virtual bool getHasMoved();
    virtual bool isDouble_done();
    Chessman (const Chessman& C) {x=C.x ; y=C.y ; name=C.name; isWhite=C.isWhite;}

    // Fonctions de jeu
    vector<Point> move_straight(Grid grid, int a, int b);
    vector<Point> testedTuples(Grid& grid, vector<Point> tab);
    vector<Point> allowed_moves(Grid grid);
    virtual vector<Point> moves(Grid& grid);

    // Fonction virtuelle sethasMoved
    virtual void sethasMoved(bool b);
};

class Empty_Chessman : Chessman{
public:
    //Constructeur
    Empty_Chessman(int a, int b) : Chessman(a, b, "Empty"){}
};


class Rook: Chessman{
    bool hasMoved;
public:
    //Constructeur
    Rook(int a, int b, bool iW): Chessman(a, b, "Rook", iW){hasMoved = false;}

    //Accesseurs
    bool getHasMoved(){ return hasMoved; }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};

class Bishop: Chessman{
public:
    //Constructeur
    Bishop(int a, int b, bool iW): Chessman(a, b, "Bishop", iW){}

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};


class Queen: Chessman{
public:
    //Constructeur
    Queen(int a, int b, bool iW): Chessman(a, b, "Queen", iW){}

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};

class Knight: Chessman{
public:
    //Constructeur
    Knight(int a, int b, bool iW): Chessman(a, b, "Knight", iW){}

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};


class King: Chessman{
    bool hasMoved;
public:
    //Constructeur
    King(int a, int b, bool iW): Chessman(a, b, "King", iW){hasMoved = false;}

    //Accesseurs et setter
    bool getHasMoved(){ return hasMoved; }
    void sethasMoved(bool value){ hasMoved = value; }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};


class Pawn: Chessman{
    bool double_done;
public:
    //Constructeur
    Pawn(int a, int b, bool iW): Chessman(a, b, "Pawn", iW){}

    //Accesseurs et setter
    bool isDouble_done(){ return double_done; }
    void setdouble_done(bool value){ double_done = value; }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};
