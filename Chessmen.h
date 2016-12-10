#pragma once

#include "ChessGrid.h"
#include <iostream>
using namespace std;

class Chessman{
    int x,y;
    string name;
    bool isWhite;
public:
    //Constructeur
    Chessman(int a, int b, string n, bool iW);

    //Accesseurs et setter
    int x(){ return x;}
    int y(){ return y;}
    bool isWhite(){return isWhite;}
    string name(){ return name; }
    void setx(int i) { x = i; }
    void sety(int i) { y = i; }
    void setName(string n) { name = n; }

    // Fonctions de jeu
    vector<vector<2,int>> move_straight(Grid grid, int a, int b);
    vector<vector<2,int>> testedTuples(Grid grid, vector<vector<2,int>> tab);
    vector<vector<2,int>> allowed_moves(Grid grid);
};


class Rook: Chessman{
    bool hasMoved;
public:
    //Constructeur
    Rook(int a, int b, bool iW): Chessman(a, b, "Rook", iW){hasMoved = false;}

    //Accesseurs
    bool hasMoved(){ return hasMoved; }

    //Fonction de jeu
    vector<vector<2,int>> moves(Grid grid);
};

class Bishop: Chessman{
public:
    //Constructeur
    Bishop(int a, int b, bool iW): Chessman(a, b, "Bishop", iW){}

    //Fonction de jeu
    vector<vector<2,int>> moves(Grid grid);
};


class Queen: Chessman{
public:
    //Constructeur
    Queen(int a, int b, bool iW): Chessman(a, b, "Queen", iW){}

    //Fonction de jeu
    vector<vector<2,int>> moves(Grid grid);
}

class Knight: Chessman{
public:
    //Constructeur
    Knight(int a, int b, bool iW): Chessman(a, b, "Knight", iW){}

    //Fonction de jeu
    vector<vector<2,int>> moves(Grid grid);
};


class King: Chessman{
    bool hasMoved;
public:
    //Constructeur
    King(int a, int b, bool iW): Chessman(a, b, "King", iW){hasMoved = false;}

    //Accesseurs et setter
    bool hasMoved(){ return hasMoved; }
    void sethasMoved(bool value){ hasMoved = value; }

    //Fonction de jeu
    vector<vector<2,int>> moves(Grid grid);
};


class Pawn: Chessman{
    bool double_done;
public:
    //Constructeur
    Pawn(int a, int b, bool iW): Chessman(a, b, "Pawn", iW){}

    //Accesseurs et setter
    bool double_done(){ return double_done; }
    void setdouble_done(bool value){ double_done = value; }

    //Fonction de jeu
    vector<2,int>* moves(Grid grid);
};
