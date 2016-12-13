#include "Chessmen.h"
#include "Chess_Grid.h"
#include <cmath>
using namespace std;
#include<iostream>

Chessman::Chessman(int a, int b, string n, bool iW){
    x = a; y = b; n = name; isWhite = iW;
}

Chessman::Chessman(int a, int b, string n){
    x = a; y = b; n = name;
}

// deplacement dans la direction (a,b) sur
// une ligne ou une diagonale entieres.
vector<int[2]> Chessman::move_straight(Grid grid, int a, int b){
    vector<int[2]> tabAccess;
    int x0 = x + a;
    int y0 = y + b;
    while ((0 <= x0 < 8) && (0 <= y0 < 8) && grid.isVoid(x0, y0)){
        int point[2];
        point[0] = x0;
        point[1] = y0;
        tabAccess.push_back(point);
        x0 = x0 + a;
        y0 = y0 + b;
    }
    // prise
    if (0 <=x < 8 && 0 <=y < 8 && !grid.sameColor(this, x0, y0)){
        vector<int[2]> point;
        point[0] = x0;
        point[1] = y0;
        tabAcces.push_back(point);
    }
    return tabAccess;
}




// Cette fonction renvoie une liste de positions accessibles parmi celles
// proposées (ne marche pas pour le fou, la dame, et la tour)
vector<int[2]> Chessman::testedTuples(Grid grid, vector<int[2]> tab){
   vector<int[2]> tabAccess;
   int s = tab.size();
   for (int i = 0; i < s; i++){
       int point[2] = tab[i];
       if (0 <= point[0] < 8 && 0 <= point[1] < 8) {
           if (!grid.isVoid(point[0], point[1]))
               tabAccess.push_back(point);
           else if (!grid.sameColor(this, x, y))
               tabAccess.push_back(point);
       }
   }
   return tabAccess;
}

vector<int[2]> Chessman::allowed_moves(Grid grid){
    vector<int[2]> allowed;
    vector<int[2]> vecMoves = moves(grid);
    for (int i = 0; i < vecMoves.size(); i++){
        if (!grid.isChessed(this, vecMoves[i][0], vecMoves[i][1]))
            allowed.push_back(vecMoves[i]);
    }
    return allowed;
}

vector<vector<2,int>> Rook::moves(Grid grid){
    vector<int[2]>  m = move_straight(grid, -1,0);
    vector<int[2]>  m1 = move_straight(grid, 1,0);
    vector<int[2]>  m2 = move_straight(grid, 0,1);
    vector<int[2]>  m3 = move_straight(grid, 0,-1);
    m.insert(m.end(), m1.begin(), m1.end());
    m.insert(m.end(), m2.begin(), m2.end());
    m.insert(m.end(), m3.begin(), m3.end());
    return m;
}

vector<int[2]> Bishop::moves(Grid grid){
    vector<int[2]>  m = move_straight(grid, -1,-1);
    vector<int[2]>  m1 = move_straight(grid, -1,1);
    vector<int[2]>  m2 = move_straight(grid, 1,-1);
    vector<int[2]>  m3 = move_straight(grid, 1,1);
    m.insert(m.end(), m1.begin(), m1.end());
    m.insert(m.end(), m2.begin(), m2.end());
    m.insert(m.end(), m3.begin(), m3.end());
    return m;
}


vector<int[2]> Queen::moves(Grid grid){
    vector<int[2]>  m = move_straight(grid, -1,-1);
    vector<int[2]>  m1 = move_straight(grid, -1,1);
    vector<int[2]>  m2 = move_straight(grid, 1,-1);
    vector<int[2]>  m3 = move_straight(grid, 1,1);
    vector<int[2]>  m4 = move_straight(grid, -1,0);
    vector<int[2]>  m5 = move_straight(grid, 1,0);
    vector<int[2]>  m6 = move_straight(grid, 0,1);
    vector<int[2]>  m7 = move_straight(grid, 0,-1);
    m.insert(m.end(), m1.begin(), m1.end());
    m.insert(m.end(), m2.begin(), m2.end());
    m.insert(m.end(), m3.begin(), m3.end());
    m.insert(m.end(), m4.begin(), m4.end());
    m.insert(m.end(), m5.begin(), m5.end());
    m.insert(m.end(), m6.begin(), m6.end());
    m.insert(m.end(), m7.begin(), m7.end());
    return m;
}


vector<int[2]> Knight::moves(Grid grid){
    vector<int[2]> m;
    int t[4] = {-1, 1, -2, 2};
    for (int i = 0; i < 3; i++){
        for (int j = 0; j <3; j++){
            if (abs(t[i]) != abs(t[j])){
                int[2] pos;
                pos[0] = x + t[i];
                pos[1] = y + t[j];
                m.push_back(pos);
            }
        }
    }
    return testedTuples(grid, m);
}

vector<int[2]> King::moves(Grid grid){
    vector<int[2]> m;
    for (int i = -1; i< 2; i++){
        for (int j = -1; j< 2; j++){
            if ((i != 0) || (j != 0)){
                int[2] point;
                point[0] = x + i;
                point[1] = y + j;
                m.push_back(point);
            }
        }
    }
    m = testedTuples(grid, m);
    //roque
    if ( !hasMoved() ){
        for (int i = 0; i<8; i+=7){
            for (int j = 0; j<8; j+=7){
                // tour de gauche
                if ( (j == 0) && !grid.isVoid(i,j) \
                     && grid(i,0).name() == "Rook" \
                     && !grid(i,0).getHasMoved() && grid.isVoid(i, 1) && grid.isVoid(i, 3) \
                     && !grid.isChessed(this, i, 2) && !grid.isChessed(this, i, 2) \
                     && !grid.isChessed(this, i, 4) ){
                    int[2] point;
                    point[0] = i;
                    point[1] = 2;
                    m.push_back(point);
                }
                // tour de droite
                else if (!grid.isVoid(i,j) && grid(i,0).name() == "Rook" \
                        && !grid(i,j).hasMoved() && grid.isVoid(i,6)\
                        && grid.isVoid(i,5) && !grid.isChessed(this, i, 4) \
                        && !grid.isChessed(this, i, 5) && !grid.isChessed(this, i, 6)){
                    vector<2,int> point;
                    point[0] = i;
                    point[1] = 6;
                    m.push_back(point);
                }
            }
        }
    }
    return m;
}

vector<vector<2,int>> Pawn::moves(Grid grid){
    vector<vector<2,int> m;
    int direction = 2*isWhite()-1;
    // la direction de deplacement differe entre les blancs et
    // les noirs
    if (grid.isVoid(x() + direction, y())){
        vector<2,int> point;
        point[0] = x() + direction;
        point[1] = y();
        m.push_back(point);
    }
    m = testedTuples(grid, m);
    vector<2,int> point;
    point[0] = x() + direction;
    point[1] = y() + 1;
    // avancee initaila
    // prises normales
    // d'un cote
    if ( y()+ 1 < 8 && !grid.isVoid(x() + direction, y() + 1) \
        && !grid.sameColor(this, x() + direction, y() + 1)){
            int[2] point;
            point[0] = x() + direction;
            point[1] = y() + 1;
            m.push_back(point);
    }
    // de l'autre
    if ( y() - 1 >= 0 && !grid.isVoid(x() + direction, y() - 1) \
            && !grid.sameColor(this, x() + direction, y() - 1)){
                int[2] point;
                point[0] = x() + direction;
                point[1] = y() - 1;
                m.push_back(point);
    }
    // avancee de deux cases
    if (isWhite() && x() == 1 && grid.isVoid(2, y()) && grid.isVoid(3, y())){
        int[2] point;
        point[0] = 3;
        point[1] = y();
        m.push_back(point);
    }
    if (!isWhite() && x() == 6 && grid.isVoid(5, y()) && grid.isVoid(4, y())){
        int[2] point;
        point[0] = 4;
        point[1] = y();
        m.push_back(point);
    }
    // prise en passant
    int P[2];
        int Point[2];
        P[0]=x;
        P[1]=y+1;
        //d'un côté
        if (y()+ 1 < 8 && !grid.isVoid(P[0],P[1])
                && grid(P)->name() == "Pawn" && grid(P)->isWhite() != isWhite
                && grid(P)->double_done && grid.isVoid(P[0]+direction,P[1])){
            Point[0]=x+direction;
            Point[1]=y+1;
            m.push_back(Point);
        }
        //de l'autre
        P[0]=x;
        P[1]=y-1;
        if (y()- 1 >= 0 && !grid.isVoid(P[0],P[1])
                && grid(P)->name() == "Pawn" && grid(P)->isWhite() != isWhite
                && grid(P)->double_done && grid.isVoid(P[0]+direction,P[1])){
            Point[0]=x+direction;
            Point[1]=y-1;
            m.push_back(Point);
        }
}
