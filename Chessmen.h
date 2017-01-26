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
    //Constructeur et destructeur
    Chessman(){}
    Chessman(int a, int b, string n, bool iW);
    virtual Chessman* clone(){}
    virtual ~Chessman(){}

    //Accesseurs et setter
    int getx(){ return x;}
    int gety(){ return y;}
    bool getIsWhite(){return isWhite;}
    string getName(){ return name; }
    void setx(int i) { x = i; }
    void sety(int i) { y = i; }
    void setIsWhite(bool value){ isWhite = value; }
    void setName(string n) { name = n; }
    virtual void setdouble_done(bool value){}
    virtual bool getHasMoved(){}
    virtual bool isDouble_done(){}
    virtual void sethasMoved(bool b){}

    Chessman (const Chessman& C) {x=C.x ; y=C.y ; name=C.name; isWhite=C.isWhite;}

    // Fonctions de jeu
    vector<Point> move_straight(Grid& grid, int a, int b);
    vector<Point> testedTuples(Grid& grid, vector<Point> tab);
    vector<Point> allowed_moves(Grid &grid);
    virtual vector<Point> moves(Grid& grid){}
};

//Empty_Chessman est une pièce blanche qui n'existe pas.
//Attention à ne pas prendre en compte sa couleur.
class Empty_Chessman : public Chessman{
public:
    //Constructeur
    Empty_Chessman() : Chessman(0, 0, "Empty", true){}
    Chessman* clone(){ return new Empty_Chessman(); }
    ~Empty_Chessman(){}
};


class Rook: public Chessman{
    bool hasMoved;
public:
    //Constructeur et destructeur
    Rook(int a, int b, bool iW): Chessman(a, b, "Rook", iW){hasMoved = false;}
    Chessman* clone(){return new Rook(x, y, isWhite); }
    ~Rook(){}

    //Accesseurs
    bool getHasMoved(){ return hasMoved; }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};

class Bishop: public Chessman{
public:
    //Constructeur
    Bishop(int a, int b, bool iW): Chessman(a, b, "Bishop", iW){}
    Chessman* clone(){ return new Bishop(x, y, isWhite); }
    ~Bishop(){}

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};


class Queen: public Chessman{
public:
    //Constructeur
    Queen(int a, int b, bool iW): Chessman(a, b, "Queen", iW){}
    Chessman* clone(){ return new Queen(x, y, isWhite); }
    ~Queen(){}

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};

class Knight:  public Chessman{
public:
    //Constructeur
    Knight(int a, int b, bool iW): Chessman(a, b, "Knight", iW){}
    Chessman* clone(){ return new Knight(x, y, isWhite); }
    ~Knight(){}

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};


class King: public Chessman{
    bool hasMoved;
public:
    //Constructeur
    King(int a, int b, bool iW): Chessman(a, b, "King", iW){hasMoved = false;}
    Chessman* clone(){ return new King(x, y, isWhite); }
    ~King(){}

    //Accesseurs et setter
    bool getHasMoved(){ return hasMoved; }
    void sethasMoved(bool value){ hasMoved = value; }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};


class Pawn: public Chessman{
    bool double_done;
public:
    //Constructeur
    Pawn(int a, int b, bool iW): Chessman(a, b, "Pawn", iW){double_done = false;}
    Chessman* clone(){ return new Pawn(x, y, isWhite); }
    ~Pawn(){}

    //Accesseurs et setter
    bool isDouble_done(){ return double_done; }
    void setdouble_done(bool value){ double_done = value; }

    //Fonction de jeu
    vector<Point> moves(Grid& grid);
};
