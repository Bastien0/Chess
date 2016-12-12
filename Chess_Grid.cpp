#include "Chess_Grid.h"

Grid(){
    grid = new Chessman[64];
}

Chessman* operator()(int coord0, int coord1){
    return grid[coord0+8*coord1];
}

void operator()(int coord0, int coord1, Chessman& chessman){
    chessman.setx(coord0);
    chessman.sety(coord1);
    grid[coord[0]+8*coord[1]] = Chessman;
}

int move(int coord[2], Chessman& chessman, string promotion = ""){
    int[2] oldCoordChessman;
    oldCoordChessman[0] = coord[0]; oldCoordChessman[1] = coord[1];

    // on regarde si la piece a un argument de mouvement
    if (chessman.name() == "King" || chessman.name() == "Rook")
        chessman.sethasMoved(true);

    //promotion
    if (chessman.name() == "Pawn" && promotion != ""){
        if (promotion == "Queen")
            chessman = Queen(chessman.isWhite(), chessman.x(), chessman.y());
        if (promotion == "Bishop")
            chessman = Bishop(chessman.isWhite(), chessman.x(), chessman.y());
        if (promotion == "Knight")
            chessman = Knight(chessman.isWhite(), chessman.x(), chessman.y());
        if (promotion == "Rook")
            chessman = Rook(chessman.isWhite(), chessman.x(), chessman.y());
    }
    //cas de la prise en passant
    if (chessman.name() == "Pawn" && Chessman.y() != coord[1] && this->isVoid(coord[0], coord[1])){
        if (!chessman.isWhite()){
            whiteLostChessmen.push_back(this->(coord[0]+1, coord[1]));
            this->setNone(coord[0]+1, coord[1]);
        }
        else{
            blackLostChessmen.push_back(this->(coord[0]-1, coord[1]));
            this->setNone(coord[0]-1, coord[1]);
        }
    }

    // cas du coup double du pion
    else if (chessman.name() == "Pawn" && chessman.y() != coord[1] && this->isVoid(coord[0], coord[1]))
        chessman.setdouble_done(true);

    // s'il y a roque
    else if (chessman.name() == "King" && (chessman.y()-coord[1]) != -1 && (chessman.y()-coord[1]) != 0 (chessman.y()-coord[1]) != 1){
        // si on va vers la gauche
        if (chessman.y() > coord[1]){
            this(chessman.x(), 3, this(Chessman.x(), 0));
            this->setNone(chessman.x(), 0);
        }
        // vers la gauche
        else{
            this(Chessman.x, 5, this(Chessman.x, 7));
            this->setNone(Chessman.x, 7);
        }
    }

    // cas general
    else{
        // si une piece est prise, on l'ajoute a la liste des pieces prises de sa couleur
        if (!this->isVoid(coord[0], coord[1])){
            if (this(coord[0],coord[1]).isWhite())
                whiteLostChessmen.push_back(this(coord[0], coord[1]));
            else
                blackLostChessmen.push_back(this(coord[0], coord[1]));
        }
    }

    vector<Chessman> l = this->list_chessman_col(!chessman.isWhite());
    vectot<Chessman>::const_iterator it = V.begin();
    for (; it != l.end(); ++it)
        if (this(*it[0], *it[1]).name() == "Pawn")
            this(*it[0], *it[1]).setdouble_done(false);

    this->setNone(oldCoordChessman[0],oldCoordChessman[1]);
    this(coord[0], coord[1], chessman);
}

void setNone(int x, int y){
    grid[x+8*y] = Empty_Chessman(x, y);
}

int* king_position(bool isWhite){
    int v[2];
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (!this->isVoid(i,j) && grid[i+8*j].isWhite() == isWhite && grid[i+8*j].name() == "King"){
                v[0] = i;
                v[1] = j;
                return v;
            }
        }
    }
}

bool isVoid(int x, int y){
    return (grid[x+8*y].name() == "Empty");
}

vector<Chessman> list_chessman_col(bool colorIsWhite){
    vector<int[2]> l;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (grid[i+8*j] != none){
                if (grid[i+8*j].isWhite() == colorIsWhite){
                    int v[2];
                    v[0] = i;
                    v[1] = j;
                    l.push_back(v);
                }
            }
        }
    }
    return l;
}

bool sameColor(Chessman& chessman, x, y){
    return (this(x,y).isWhite() == chessman.isWhite());
}

// quand on deplace une piece sur la grille, se met on en echec?
// pour savoir cela, on effectue le deplacement et on regarde.
// Apres avoir regarde cela, il faut remettre les pieces a leur place :
// il faut reparer les degats en remettant a leur place les elements qui ont
// ete deplaces.
bool isChessed(Chessman& chessman, int x, int y){
    // memorisation de la piece prise, si le deplacement en prend une
    bool someoneTaken = False;
    Chessman takenChessman = Empty_Chessman(-1,-1);
    if (!this->isVoid(x, y))
        someoneTaken = True;
        takenChessman = this(x, y);
    // memoristaion de l'ancienne position
    vector<int[2]> coordIniChess;
    coordIniChess[0] = chessman.x();
    coordIniChess[1] = chessman.y();
    this(x,y,chessman);
    chessman.setx(x);
    chessman.sety(y);
    this->setNone(coordIniChess[0], coordIniChess[1]);
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            // si on tombe sur une piece de couleur differente,
            // on verifie qu'elle ne met pas le roi en echec
            // ie si la position du roi n'est pas dans la liste des
            // positions accessibles,
            // on repare ce qu'on a bouge dans la grille et on renvoie true
            vector<int[2]> l = this(i,j).moves(this);
            if (!this->isVoid(i,j) && chessman.isWhite() != this(i,j).isWhite() && l.find(this->king_position(chessman.isWhite())) != l.end()){
                // On remet l'echiquier en place
                if (someoneTaken)
                    this(x,y,takenChessman);
                else
                    this->setNone(x,y);
                chessman.setx(coordIniChess[0]);
                chessman.sety(coordIniChess[1]);
                this(coordIniChess[0], coordIniChess[1], chessman);
                return true;
            }
        }
    }
    if (someoneTaken)
        this(x,y,takenChessman);
    else
        this->setNone(x,y);
    chessman.setx(coordIniChess[0]);
    chessman.sety(coordIniChess[1]);
    this(coordIniChess[0], coordIniChess[1], chessman);
    return false;
}
