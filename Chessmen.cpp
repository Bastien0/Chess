#include "Chessmen.h"
#include "Chess_Grid.h"
#include <cmath>
using namespace std;
#include<iostream>

Chessman::Chessman(int a, int b, string n, bool iW){
    x = a; y = b; name = n; isWhite = iW;
}

Chessman::Chessman(int a, int b, string n){
    x = a; y = b; n = name;
}

// deplacement dans la direction (a,b) sur
// une ligne ou une diagonale entieres.
vector<Point> Chessman::move_straight(Grid grid, int a, int b){
    vector<Point> tabAccess;
    int x0 = x + a;
    int y0 = y + b;
    while ((0 <= x0 < 8) && (0 <= y0 < 8) && grid.isVoid(x0, y0)){
        Point point(x0, y0);
        tabAccess.push_back(point);
        x0 = x0 + a;
        y0 = y0 + b;
    }
    // prise
    if (0 <=x < 8 && 0 <=y < 8 && !grid.sameColor(this, x0, y0)){
        Point point(x0, y0);
        tabAccess.push_back(point);
    }
    return tabAccess;
}




// Cette fonction renvoie une liste de positions accessibles parmi celles
// proposées (ne marche pas pour le fou, la dame, et la tour)
vector<Point> Chessman::testedTuples(Grid &grid, vector<Point> tab){
   vector<Point> tabAccess;
   int s = tab.size();
   for (int i = 0; i < s; i++){
       if (0 <= tab[i].getx() < 8 && 0 <= tab[i].gety() < 8) {
           if (!grid.isVoid(tab[i].getx(), tab[i].gety()))
               tabAccess.push_back(tab[i]);
           else if (!grid.sameColor(this, x, y))
               tabAccess.push_back(tab[i]);
       }
   }
   return tabAccess;
}

vector<Point> Chessman::allowed_moves(Grid grid){
    vector<Point> allowed;
    vector<Point> vecMoves = moves(grid);
    for (int i = 0; i < vecMoves.size(); i++){
        if (!grid.isChessed(this, vecMoves[i].getx(), vecMoves[i].gety()))
            allowed.push_back(vecMoves[i]);
    }
    return allowed;
}

vector<Point> Rook::moves(Grid& grid){
    vector<Point>  m = move_straight(grid, -1,0);
    vector<Point>  m1 = move_straight(grid, 1,0);
    vector<Point>  m2 = move_straight(grid, 0,1);
    vector<Point>  m3 = move_straight(grid, 0,-1);
    m.insert(m.end(), m1.begin(), m1.end());
    m.insert(m.end(), m2.begin(), m2.end());
    m.insert(m.end(), m3.begin(), m3.end());
    return m;
}

vector<Point> Bishop::moves(Grid& grid){
    vector<Point>  m = move_straight(grid, -1,-1);
    vector<Point>  m1 = move_straight(grid, -1,1);
    vector<Point>  m2 = move_straight(grid, 1,-1);
    vector<Point>  m3 = move_straight(grid, 1,1);
    m.insert(m.end(), m1.begin(), m1.end());
    m.insert(m.end(), m2.begin(), m2.end());
    m.insert(m.end(), m3.begin(), m3.end());
    return m;
}


vector<Point> Queen::moves(Grid& grid){
    vector<Point>  m = move_straight(grid, -1,-1);
    vector<Point>  m1 = move_straight(grid, -1,1);
    vector<Point>  m2 = move_straight(grid, 1,-1);
    vector<Point>  m3 = move_straight(grid, 1,1);
    vector<Point>  m4 = move_straight(grid, -1,0);
    vector<Point>  m5 = move_straight(grid, 1,0);
    vector<Point>  m6 = move_straight(grid, 0,1);
    vector<Point>  m7 = move_straight(grid, 0,-1);
    m.insert(m.end(), m1.begin(), m1.end());
    m.insert(m.end(), m2.begin(), m2.end());
    m.insert(m.end(), m3.begin(), m3.end());
    m.insert(m.end(), m4.begin(), m4.end());
    m.insert(m.end(), m5.begin(), m5.end());
    m.insert(m.end(), m6.begin(), m6.end());
    m.insert(m.end(), m7.begin(), m7.end());
    return m;
}


vector<Point> Knight::moves(Grid& grid){
    vector<Point> m;
    int t[4] = {-1, 1, -2, 2};
    for (int i = 0; i < 3; i++){
        for (int j = 0; j <3; j++){
            if (abs(t[i]) != abs(t[j])){
                Point pos;
                pos.setx(this->getx() + t[i]);
                pos.sety(this->gety() + t[j]);
                m.push_back(pos);
            }
        }
    }
    return testedTuples(grid, m);
}

vector<Point> King::moves(Grid& grid){
    vector<Point> m;
    for (int i = -1; i< 2; i++){
        for (int j = -1; j< 2; j++){
            if ((i != 0) || (j != 0)){
                Point point(this->getx() + i, this->gety() + j);
                m.push_back(point);
            }
        }
    }
    m = testedTuples(grid, m);
    //roque
    if ( !hasMoved ){
        for (int i = 0; i<8; i+=7){
            for (int j = 0; j<8; j+=7){
                // tour de gauche
                if ( (j == 0) && !grid.isVoid(i,j) \
                     && grid(i,0)->getName() == "Rook" \
                     && !grid(i,0)->getHasMoved() && grid.isVoid(i, 1) && grid.isVoid(i, 3) \
                     && !grid.isChessed(this, i, 2) && !grid.isChessed(this, i, 2) \
                     && !grid.isChessed(this, i, 4) ){
                    Point point(i, 2);
                    m.push_back(point);
                }
                // tour de droite
                else if (!grid.isVoid(i,j) && grid(i,0)->getName() == "Rook" \
                        && !grid(i,j)->getHasMoved() && grid.isVoid(i,6)\
                        && grid.isVoid(i,5) && !grid.isChessed(this, i, 4) \
                        && !grid.isChessed(this, i, 5) && !grid.isChessed(this, i, 6)){
                    Point point(i, 6);
                    m.push_back(point);
                }
            }
        }
    }
    return m;
}

vector<Point> Pawn::moves(Grid& grid){
    vector<Point> m;
    int direction = 2*this->getIsWhite()-1;
    // la direction de deplacement differe entre les blancs et
    // les noirs
    if (grid.isVoid(this->getx() + direction, this->gety())){
        Point point(this->getx() + direction,this->gety());
        m.push_back(point);
    }
    m = testedTuples(grid, m);
    // avancee initiales
    // prises normales
    // d'un cote
    if ( this->gety()+ 1 < 8 && !grid.isVoid(this->getx() + direction, this->gety() + 1) \
        && !grid.sameColor(this, this->getx() + direction, this->gety() + 1)){
            Point point(this->getx() + direction, this->gety() + 1);
            m.push_back(point);
    }
    // de l'autre
    if ( this->gety() - 1 >= 0 && !grid.isVoid(this->getx() + direction, this->gety() - 1) \
            && !grid.sameColor(this, this->getx() + direction, this->gety() - 1)){
                Point point(this->getx() + direction, this->gety() - 1);
                m.push_back(point);
    }
    // avancee de deux cases
    if (this->getIsWhite() && this->getx() == 1 && grid.isVoid(2, this->gety()) && grid.isVoid(3, this->gety())){
        Point point(3,this->gety());
        m.push_back(point);
    }
    if (!this->getIsWhite() && this->getx() == 6 && grid.isVoid(5, this->gety()) && grid.isVoid(4, this->gety())){
        Point point(4, this->gety());
        m.push_back(point);
    }
    // prise en passant
    Point P(this->getx(), this->gety()+1);
    Point point;
    //d'un côté
    if (this->gety()+ 1 < 8 && !grid.isVoid(P.getx(),P.gety())
            && grid(P.getx(),P.gety())->getName() == "Pawn" && grid(P.getx(),P.gety())->getIsWhite() != this->getIsWhite()
            && grid(P.getx(),P.gety())->isDouble_done() && grid.isVoid(P.getx()+direction,P.gety())){
        point.setx(this->getx()+direction);
        point.sety(this->gety()+1);
        m.push_back(point);
    }
    //de l'autre
    P.setx(this->getx());
    P.sety(this->gety()-1);
    if (this->gety()- 1 >= 0 && !grid.isVoid(P.getx(),P.gety())
            && grid(P.getx(),P.gety())->getName() == "Pawn" && grid(P.getx(),P.gety())->getIsWhite() != this->getIsWhite()
            && grid(P.getx(),P.gety())->isDouble_done() && grid.isVoid(P.getx()+direction,P.gety())){
        point.setx(this->getx()+direction);
        point.sety(this->gety()-1);
        m.push_back(point);
    }
}


