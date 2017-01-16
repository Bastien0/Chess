#pragma once
#include<vector>
#include "Chess_Grid.h"
#include <iostream>
#include "point.h"
using namespace std;


class Grid;

class Chessman{
protected:
    int x,y;
    string name;
    bool isWhite;
public:
    //Constructeur
    Chessman(){}
    Chessman(int a, int b, string n, bool iW);
    Chessman(int a, int b, string n);
    virtual Chessman* clone(){}

    //Accesseurs et setter
    int getx(){ return x;}
    int gety(){ return y;}
    bool getIsWhite(){return isWhite;}
    string getName(){ return name; }
    void setx(int i) { x = i; }
    void sety(int i) { y = i; }
    void setName(string n) { name = n; }
    virtual void setdouble_done(bool value){}
    virtual bool getHasMoved(){}
    virtual bool isDouble_done(){}
    virtual void sethasMoved(bool b){}

    Chessman (const Chessman& C) {x=C.x ; y=C.y ; name=C.name; isWhite=C.isWhite;}

    // Fonctions de jeu
    vector<Point> move_straight(Grid grid, int a, int b);
    vector<Point> testedTuples(Grid& grid, vector<Point> tab);
    vector<Point> allowed_moves(Grid grid);
    virtual vector<Point> moves(Grid& grid){}
};

class Empty_Chessman : Chessman{
public:
    //Constructeur
    Empty_Chessman(int a, int b) : Chessman(a, b, "Empty"){}
    Chessman* clone(){ return new Empty_Chessman(x, y); }
};


class Rook: Chessman{
    bool hasMoved;
public:
    //Constructeur
    Rook(int a, int b, bool iW): Chessman(a, b, "Rook", iW){hasMoved = false;}
    Chessman* clone(){return new Rook(x, y, isWhite); }

    //Accesseurs
    bool getHasMoved(){ return hasMoved; }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};

class Bishop: Chessman{
public:
    //Constructeur
    Bishop(int a, int b, bool iW): Chessman(a, b, "Bishop", iW){}
    Chessman* clone(){ return new Bishop(x, y, isWhite); }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};


class Queen: Chessman{
public:
    //Constructeur
    Queen(int a, int b, bool iW): Chessman(a, b, "Queen", iW){}
    Chessman* clone(){ return new Queen(x, y, isWhite); }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};

class Knight: Chessman{
public:
    //Constructeur
    Knight(int a, int b, bool iW): Chessman(a, b, "Knight", iW){}
    Chessman* clone(){ return new Knight(x, y, isWhite); }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};


class King: Chessman{
    bool hasMoved;
public:
    //Constructeur
    King(int a, int b, bool iW): Chessman(a, b, "King", iW){hasMoved = false;}
    Chessman* clone(){ return new King(x, y, isWhite); }

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
    Chessman* clone(){ return new Pawn(x, y, isWhite); }

    //Accesseurs et setter
    bool isDouble_done(){ return double_done; }
    void setdouble_done(bool value){ double_done = value; }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};
