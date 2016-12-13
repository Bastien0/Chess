#include "Chess_Grid.h"
#include "Chessmen.h"
Grid::Grid(){
    grid = new Chessman[64];
}

Chessman* Grid::operator()(int coord0, int coord1){
    return &grid[coord0+8*coord1];
}

void Grid::operator()(int coord0, int coord1, Chessman& chessman){
    chessman.setx(coord0);
    chessman.sety(coord1);
    grid[coord0+8*coord1] = chessman;
}

// il y avait un probleme avec string promotion = "" j'essaie d ne rien mettre
void Grid::move(int coord[2], Chessman& chessman, string promotion){
    int oldCoordChessman[2];
    oldCoordChessman[0] = coord[0]; oldCoordChessman[1] = coord[1];

    // on regarde si la piece a un argument de mouvement
    if (chessman.getName() == "King" || chessman.getName() == "Rook")
        chessman.sethasMoved(true);

    //promotion
    if (chessman.getName() == "Pawn" && promotion != ""){
        if (promotion == "Queen")
            chessman = Queen(chessman.getIsWhite(), chessman.getx(), chessman.gety());
        if (promotion == "Bishop")
            chessman = Bishop(chessman.getIsWhite(), chessman.getx(), chessman.gety());
        if (promotion == "Knight")
            chessman = Knight(chessman.getIsWhite(), chessman.getx(), chessman.gety());
        if (promotion == "Rook")
            chessman = Rook(chessman.getIsWhite(), chessman.getx(), chessman.gety());
    }
    //cas de la prise en passant
    if (chessman.getName() == "Pawn" && chessman.gety() != coord[1] && this->isVoid(coord[0], coord[1])){
        if (!chessman.getIsWhite()){
            whiteLostChessmen.push_back(*((*this)(coord[0]+1, coord[1])));
            this->setNone(coord[0]+1, coord[1]);
        }
        else{
            blackLostChessmen.push_back(*((*this)(coord[0]-1, coord[1])));
            this->setNone(coord[0]-1, coord[1]);
        }
    }

    // cas du coup double du pion
    else if (chessman.getName() == "Pawn" && chessman.gety() != coord[1] && this->isVoid(coord[0], coord[1]))
        chessman.setdouble_done(true);

    // s'il y a roque
    else if (chessman.getName() == "King" && (chessman.gety()-coord[1]) != -1 && (chessman.gety()-coord[1]) != 0 && (chessman.gety()-coord[1]) != 1){
        // si on va vers la gauche
        if (chessman.gety() > coord[1]){
            (*this)(chessman.getx(), 3, *(*this)(chessman.getx(), 0));
            this->setNone(chessman.getx(), 0);
        }
        // vers la gauche
        else{
            (*this)(chessman.getx(), 5, *(*this)(chessman.getx(), 7));
            this->setNone(chessman.getx(), 7);
        }
    }

    // cas general
    else{
        // si une piece est prise, on l'ajoute a la liste des pieces prises de sa couleur
        if (!this->isVoid(coord[0], coord[1])){
            if ((*(*this)(coord[0],coord[1])).getIsWhite())
                whiteLostChessmen.push_back(*((*this)(coord[0], coord[1])));
            else
                blackLostChessmen.push_back(*((*this)(coord[0], coord[1])));
        }
    }

    vector<Chessman> l = this->list_chessman_col(!chessman.getIsWhite());
    vector<Chessman>::iterator it = l.begin();
    for (; it != l.end(); ++it)
        if ((*this)(it->getx(), it->gety())->getName() == "Pawn"){
            (*this)(it->getx(), it->gety())->setdouble_done(false);
         }

    this->setNone(oldCoordChessman[0],oldCoordChessman[1]);
    (*this)(coord[0], coord[1], chessman);
}

void Grid::setNone(int x, int y){
    grid[x+8*y] = Empty_Chessman(x, y);
}

int* Grid::king_position(bool isWhite){
    int v[2];
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (!this->isVoid(i,j) && grid[i+8*j].getIsWhite() == isWhite && grid[i+8*j].getName() == "King"){
                v[0] = i;
                v[1] = j;
                return v;
            }
        }
    }
}

bool Grid::isVoid(int x, int y){
    return (grid[x+8*y].getName() == "Empty");
}

vector<Chessman> Grid::list_chessman_col(bool colorIsWhite){
    vector<Chessman> l;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (!(grid[i+8*j].getName() == "Empty") && grid[i+8*j].getIsWhite() == colorIsWhite)
                    l.push_back(grid[i+8*j]);
        }
    }
    return l;
}

bool Grid::sameColor(Chessman& chessman, int x, int y){
    return ((*(*this)(x,y)).getIsWhite() == chessman.getIsWhite());
}

// quand on deplace une piece sur la grille, se met on en echec?
// pour savoir cela, on effectue le deplacement et on regarde.
// Apres avoir regarde cela, il faut remettre les pieces a leur place :
// il faut reparer les degats en remettant a leur place les elements qui ont
// ete deplaces.
bool Grid::isChessed(Chessman& chessman, int x, int y){
    // memorisation de la piece prise, si le deplacement en prend une
    bool someoneTaken = false;
    Chessman takenChessman = Empty_Chessman(-1,-1);
    if (!this->isVoid(x, y))
        someoneTaken = true;
        takenChessman = (*this)(x, y);
    // memoristaion de l'ancienne position
    int  coordIniChess[2];
    coordIniChess[0] = chessman.getx();
    coordIniChess[1] = chessman.gety();
    (*this)(x,y,chessman);
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
            if(!this->isVoid(i,j)){
                vector<int[2]> l = (*(*this)(i,j)).moves(this);
                if (chessman.getIsWhite() != (*this)(i,j)->getIsWhite() \
                        && l.find(l.begin(),l.end(),this->king_position(chessman.getIsWhite())) != l.end()){
                    // On remet l'echiquier en place
                    if (someoneTaken)
                        (*this)(x,y,takenChessman);
                    else
                        this->setNone(x,y);
                    chessman.setx(coordIniChess[0]);
                    chessman.sety(coordIniChess[1]);
                    (*this)(coordIniChess[0], coordIniChess[1], chessman);
                    return true;
                }
            }
        }
    }
    if (someoneTaken)
        (*this)(x,y,takenChessman);
    else
        this->setNone(x,y);
    chessman.setx(coordIniChess[0]);
    chessman.sety(coordIniChess[1]);
    (*this)(coordIniChess[0], coordIniChess[1], chessman);
    return false;
}
