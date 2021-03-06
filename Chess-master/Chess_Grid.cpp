#include "Chess_Grid.h"

// Un pointeur sur une pièce vide qui sera commun a toutes les cases vides

Chessman* Empty = new Empty_Chessman();

/***************************************************************************************************/
/***********Constructeur d'une grille a partir d'une chaine de caractere (standard fen)**************/
/***************************************************************************************************/

Grid::Grid(string s){
    grid = new Chessman*[64];
    score = 0;

    // On initialise une grille vide
    for (int i = 0; i < 64; i++)
        grid[i] = Empty;

    countMove = 0;
    countHalfMove = 0;
    int ligne = 0;
    int colonne = 0;
    int caractere = 0;
    while (s[caractere] != ' '){
        if (s[caractere] == '/'){
            ligne += 1;
            colonne = -1;
        }
        // Cas des pieces noires
        else if (s[caractere] == 'r'){
            Rook r(ligne, colonne, false);
            (*this)(ligne, colonne, r.clone());
            score -= (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'n'){
            Knight n(ligne, colonne, false);
            (*this)(ligne, colonne, n.clone());
            score -= (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'b'){
            Bishop b(ligne, colonne, false);
            (*this)(ligne, colonne, b.clone());
            score -= (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'q'){
            Queen q(ligne, colonne, false);
            (*this)(ligne, colonne, q.clone());
            score -= (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'k'){
            King k(ligne, colonne, false);
            (*this)(ligne, colonne, k.clone());
            kingPosBlack.setx(ligne); kingPosBlack.sety(colonne);
            score -= (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'p'){
            Pawn p(ligne, colonne, false);
            (*this)(ligne, colonne, p.clone());
            score -= (*this)(ligne, colonne)->getValue();
        }

        //Cas des pieces blanches
        else if (s[caractere] == 'R'){
            Rook r(ligne, colonne, true);
            (*this)(ligne, colonne, r.clone());
            score += (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'N'){
            Knight n(ligne, colonne, true);
            (*this)(ligne, colonne, n.clone());
            score += (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'B'){
            Bishop b(ligne, colonne, true);
            (*this)(ligne, colonne, b.clone());
            score += (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'Q'){
            Queen q(ligne, colonne, true);
            (*this)(ligne, colonne, q.clone());
            score += (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'K'){
            King k(ligne, colonne, true);
            (*this)(ligne, colonne, k.clone());
            kingPosWhite.setx(ligne); kingPosWhite.sety(colonne);
            score += (*this)(ligne, colonne)->getValue();
        }
        else if (s[caractere] == 'P'){
            Pawn p(ligne, colonne, true);
            (*this)(ligne, colonne, p.clone());
            score += (*this)(ligne, colonne)->getValue();
        }

        // Cas des chiffres
        else{
            int n = s[caractere]-'0';
            for (int i = 0; i < n; i++){
                (*this)(ligne, colonne, Empty);
                if (i < n-1)
                    colonne += 1; // on augmente la colonne de n-1 au total
            }
        }
        caractere += 1;
        colonne += 1;
    }

    // Codage du joueur en cours
    caractere += 1;
    if (s[caractere] == 'w')
        whiteIsPlaying = true;
    else
        whiteIsPlaying = false;
    caractere += 2;

    // Roque possible
    string ch="    ";
    for (int i=0;i<4;i++){
        ch[i]=(s[caractere+i]);
    }
        //S'il n 'y a pas de roque possible
    if (ch[0] == '-'){
        Point Break = this->king_position(whiteIsPlaying);
        (*this)(Break.getx(), Break.gety())->sethasMoved(true);
        caractere += 1;
    }
        //Sinon on effectue un roque

    else{
        bool PresenceK=false;
        bool Presencek=false;
        bool PresenceQ=false;
        bool Presenceq=false;
        for (int ind=0;ind<ch.size();ind++){
            if(ch[ind]== 'K'){
                PresenceK = true;
                caractere += 1;
            }
            else if(ch[ind]== 'q'){
                Presenceq = true;
                caractere += 1;
            }
            else if(ch[ind]== 'Q'){
                PresenceQ = true;
                caractere += 1;
            }
            else if(ch[ind]== 'k'){
                Presencek = true;
                caractere += 1;
            }

        }
        if(!PresenceK){
            if ((*this)(7,7)->getName()=="Rook"){
                (*this)(7,7)->sethasMoved(true);
            }
        }
        if(!PresenceQ){
            if ((*this)(7,0)->getName()=="Rook"){
                (*this)(7,0)->sethasMoved(true);
            }
        }
        if(!Presenceq){
            if ((*this)(0,0)->getName()=="Rook"){
                (*this)(0,0)->sethasMoved(true);
            }
        }
        if(!Presencek){
            if ((*this)(0,7)->getName()=="Rook"){
                (*this)(0,7)->sethasMoved(true);
            }
        }
    }
    caractere +=1;

    //Prise en passant
    if (s[caractere] != '-'){
        if (s[caractere+1]=='6'){
            enPassant.setx(4);
            (*this)(4,int(s[caractere]-'a'))->setdouble_done(true);
        }
        if (s[caractere+1]=='3'){
            enPassant.setx(3);
            (*this)(3,int(s[caractere]-'a'))->setdouble_done(true);
        }
        enPassant.sety(s[caractere]-'a');
        caractere +=1;
    }
    else{
        enPassant.setx(-1); enPassant.sety(-1);
    }

    caractere += 2;

    //Compte des coups
    countHalfMove = 0;

    while (s[caractere] != ' '){
        countHalfMove = countHalfMove*10+int(s[caractere]-'0');
        caractere +=1;
    }
    caractere += 1;

    countMove = 0;
    while (caractere != s.size()){
        countMove = countMove*10+int(s[caractere]-'0');
        caractere +=1;
    }
}


/************************************************************************************************/
/***********************************Destructeur de la grille*************************************/
/************************************************************************************************/

Grid::~Grid(){
    for(int i = 0; i < 64; i++){
        if (grid[i]->getName() != "Empty")
            delete (grid[i]);
    }
    delete[] grid;
}


/************************************************************************************************/
/********************************Operateurs affectation et accessibilite**************************/
/************************************************************************************************/

Chessman* Grid::operator()(int coord0, int coord1){
    return grid[coord0+8*coord1];
}

Chessman* Grid::operator()(Point p){
    return this->operator ()(p.getx(), p.gety());
}

void Grid::operator()(int coord0, int coord1, Chessman* chessman){
    chessman->setx(coord0);
    chessman->sety(coord1);
    grid[coord0+8*coord1] = chessman;
}


/************************************************************************************************/
/*********************************Fonctions de mouvement sur la grille***************************/
/************************************************************************************************/

// Fonction de mouvement: la fonction deplace chessman ves le point donne
void Grid::move(Point point, Chessman* chessman, string promotion){
    int sign = 2*(chessman->getIsWhite())-1;
    Point oldCoordChessman(chessman->getx(), chessman->gety());

    // On augmente le score de la valeur de la piece prise
    score += sign*((*this)(point)->getValue());
    // On le diminue de la valeur de la piece deplacee
    score -= sign*(chessman->getValue());

    countHalfMove += 1;
    countMove += 1;
    enPassant.setx(-1); enPassant.sety(-1);

    // on regarde si la piece a un argument de mouvement
    if ((*chessman).getName() == "King" || (*chessman).getName() == "Rook")
        chessman->sethasMoved(true);

    // On modifie la position du roi
    if ((*chessman).getName() == "King"){
        if (chessman->getIsWhite()){
            kingPosWhite.setx(point.getx()); kingPosWhite.sety(point.gety());
        }
        else{
            kingPosBlack.setx(point.getx()); kingPosBlack.sety(point.gety());
        }
    }

    if ((*chessman).getName() == "Pawn"){
        countHalfMove = 0;
        //cas de la prise en passant
        if ((*chessman).gety() != point.gety() && this->isVoid(point.getx(), point.gety())){
            if ((*chessman).getIsWhite()){
                score += sign*((*this)(point.getx()+1, point.gety())->getValue());
                this->setNone(point.getx()+1, point.gety());
            }
            else{
                score += sign*((*this)(point.getx()-1, point.gety())->getValue());
                this->setNone(point.getx()-1, point.gety());
            }
        }

        // cas du coup double du pion
        if (abs((*chessman).getx()-point.getx()) > 1){
            if ((point.gety()+1 < 8 && (*this)(point.getx(), point.gety()+1)->getName() == "Pawn") || (point.gety()-1 >= 0 && (*this)(point.getx(), point.gety()-1)->getName() == "Pawn"))
                enPassant.setx(point.getx()+2*chessman->getIsWhite()-1); enPassant.sety(point.gety());
        }

        //promotion
        if (point.getx() == (!chessman->getIsWhite())*7){
            if (promotion == "Queen"){
                score -= sign*(chessman->getValue());
                delete chessman;
                chessman = new Queen(point.getx(), point.gety(), point.getx() == 0);
                score += sign*(chessman->getValue());
            }
            else if (promotion == "Bishop"){
                score -= sign*(chessman->getValue());
                delete chessman;
                chessman = new Bishop(point.getx(), point.gety(), point.getx() == 0);
                score += sign*(chessman->getValue());
            }
            else if (promotion == "Knight"){
                score -= sign*(chessman->getValue());
                delete chessman;
                chessman = new Knight(point.getx(), point.gety(), point.getx() == 0);
                score += sign*(chessman->getValue());
            }
            else if (promotion == "Rook"){
                score -= sign*(chessman->getValue());
                delete chessman;
                chessman = new Rook(point.getx(), point.gety(), point.getx() == 0);
                score += sign*(chessman->getValue());
            }
        }
    }


    // s'il y a roque
    else if (chessman->getName() == "King" && chessman->gety()-point.gety() != -1 && (chessman->gety()-point.gety()) != 0 && (chessman->gety()-point.gety()) != 1){
        // si on va vers la gauche
        if (chessman->gety() > point.gety()){
            // On deplace la tour
            score -= sign*(*this)((*chessman).getx(), 0)->getValue();
            (*this)((*chessman).getx(), 3, (*this)((*chessman).getx(), 0));
            score += sign*(*this)((*chessman).getx(), 3)->getValue();
            this->setNone((*chessman).getx(), 0);
        }
        // vers la droite
        else{
            score -= sign*(*this)((*chessman).getx(), 7)->getValue();
            (*this)((*chessman).getx(), 5, (*this)((*chessman).getx(), 7));
            score += sign*(*this)((*chessman).getx(), 5)->getValue();
            this->setNone((*chessman).getx(), 7);
        }
    }

    if (!this->isVoid(point.getx(), point.gety())){
        delete (*this)(point.getx(), point.gety());
        countHalfMove = 0;
    }

    this->setNone(oldCoordChessman.getx(),oldCoordChessman.gety());
    (*this)(point.getx(), point.gety(), chessman);
    score += sign*(chessman->getValue());

    whiteIsPlaying = !whiteIsPlaying;
}

// Annulation des coups
void Grid::unmove(Chessman *departure, Chessman *arrival, Point final, Point Enpassant, int halfmove){
    // On retire le score de la piece prise et on retire le score de la piece deplacee
    int sign = 2*(departure->getIsWhite())-1;
    score -= sign*(arrival->getValue());
    score -= sign*((*this)(final)->getValue());
    countHalfMove = halfmove;
    countMove -= 1;

    if (departure->getName() == "King"){
        if (departure->getIsWhite()){
            kingPosWhite.setx(departure->getx()); kingPosWhite.sety(departure->gety());
        }
        else{
            kingPosBlack.setx(departure->getx()); kingPosBlack.sety(departure->gety());
        }
    }

    if (departure->getName() == "King" && abs(departure->gety() - final.gety()) >= 2){
        if (final.gety() == 2){
            score -= sign*(*this)(departure->getx(), 3)->getValue();
            Chessman* rook = (*this)(departure->getx(), 3);
            rook->sethasMoved(false);
            (*this).setNone(departure->getx(), 3);
            (*this)(departure->getx(), 0, rook);
            score += sign*(*this)(departure->getx(), 0)->getValue();
        }
        else{
            score -= sign*(*this)(departure->getx(), 5)->getValue();
            Chessman* rook = this->operator ()(departure->getx(), 5);
            rook->sethasMoved(false);
            (*this).setNone(departure->getx(), 5);
            (*this)(departure->getx(), 7, rook);
            score += sign*(*this)(departure->getx(), 7)->getValue();
        }
    }
    //prise en passant
    else if (departure->getName() == "Pawn" && arrival->getName() == "Empty" \
             && departure->gety()!= final.gety()){
        if (departure->getIsWhite()){
            Chessman* pawn = departure->clone();
            pawn->setIsWhite(false);
            pawn->setdouble_done(true);
            (*this)(3, final.gety(), pawn);
            score -= sign*((*this)(3, final.gety())->getValue());
        }
        else{
            Chessman* pawn = departure->clone();
            pawn->setIsWhite(true);
            pawn->setdouble_done(true);
            (*this)(4, final.gety(), pawn);
            score -= sign*((*this)(4, final.gety())->getValue());
        }
    }
    if (!this->isVoid(final.getx(), final.gety()))
        delete (*this)(final.getx(), final.gety());
    (*this)(departure->getx(), departure->gety(), departure);
    //on arrive sur une case vide
    if (arrival->getName() == "Empty"){
        (*this)(final.getx(), final.gety(), Empty);
    }
    else
        (*this)(final.getx(), final.gety(), arrival);

    score += sign*(departure->getValue());
    whiteIsPlaying = !whiteIsPlaying;

    // On reinitialise la prise en passant
    enPassant = Enpassant;

}

/************************************************************************************************************/
/*************************************Diverses fonctions utiles**********************************************/
/************************************************************************************************************/

// Rend une case vide
void Grid::setNone(int x, int y){
    (*this)(x, y, Empty);
}

// Renvoie la position du roi de la couleur donnee
Point Grid::king_position(bool isWhite){
    if (isWhite)
        return kingPosWhite;
    else
        return kingPosBlack;
}

// Verifie si une case est vide
bool Grid::isVoid(int x, int y){
    return ((*this)(x, y)->getName() == "Empty");
}

// Vecteur des pieces d'une couleur donnee
vector<Chessman*> Grid::list_chessman_col(bool colorIsWhite){
    vector<Chessman*> l;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (!((*this)(i, j)->getName() == "Empty") \
                    && (*this)(i, j)->getIsWhite() == colorIsWhite)
                    l.push_back((*this)(i, j));
        }
    }
    return l;
}

// Indique si la couleur de chessman est la meme que celle de la piece en x,y
bool Grid::sameColor(Chessman* chessman, int x, int y){
    return ((*(*this)(x,y)).getIsWhite() == chessman->getIsWhite());
}


/*************************************************************************************************/
/************************************Fonction test de la mise en echec****************************/
/*************************************************************************************************/

// quand on deplace une piece sur la grille, se met on en echec?
// pour savoir cela, on effectue le deplacement et on regarde.
// Apres avoir regarde cela, il faut remettre les pieces a leur place :
// il faut reparer les degats en remettant a leur place les elements qui ont
// ete deplaces.
bool Grid::isChessed(Chessman* chessman, int x, int y){
    // memorisation de la piece prise, si le deplacement en prend une
    bool someoneTaken = false;

    Chessman* takenChessman;
    takenChessman = Empty;
    if (!this->isVoid(x, y)){
        someoneTaken = true;
        //Attention à la construction par copie
        takenChessman = (*this)(x, y);
    }

    // memoristaion de l'ancienne position
    int  coordIniChess[2];
    coordIniChess[0] = chessman->getx();
    coordIniChess[1] = chessman->gety();
    this->setNone(coordIniChess[0], coordIniChess[1]);
    (*this)(x,y,chessman);

    Point Kpos;
    if (chessman->getName() == "King"){
        Kpos.setx(chessman->getx()); Kpos.sety(chessman->gety());
    }
    else
        Kpos = king_position(chessman->getIsWhite());

    bool chess = false;

    // On regarde si un cavalier met le roi en echec
    int t[4] = {-1, 1, -2, 2};
    for (int i = 0; i < 4; i++){
        for (int j = 0; j <4; j++){
            if (abs(t[i]) != abs(t[j]) && 0 <= t[i]+Kpos.getx() && \
                    t[i]+Kpos.getx() < 8 && 0 <= t[j]+Kpos.gety() &&  t[j]+Kpos.gety() <8){
                if ((*this)(t[i]+Kpos.getx(), t[j]+Kpos.gety())->getName() == "Knight"
                        && (*this)(t[i]+Kpos.getx(), t[j]+Kpos.gety())->getIsWhite() != chessman->getIsWhite())
                    chess = true;
            }
        }
    }

    // On regarde si un pion met en echec le roi
    if (!chess && 0 <= Kpos.getx()+2*(!chessman->getIsWhite())-1 && Kpos.getx()+2*(!chessman->getIsWhite())-1 <8){
        if (0 <= Kpos.gety()-1 && (*this)(Kpos.getx()+2*(!chessman->getIsWhite())-1, Kpos.gety()-1)->getName() == "Pawn"
                && (*this)(Kpos.getx()+2*(!chessman->getIsWhite())-1, Kpos.gety()-1)->getIsWhite() != chessman->getIsWhite()){
            chess = true;
        }
        if (Kpos.gety()+1 < 8 && (*this)(Kpos.getx()+2*(!chessman->getIsWhite())-1, Kpos.gety()+1)->getName() == "Pawn"
                && (*this)(Kpos.getx()+2*(!chessman->getIsWhite())-1, Kpos.gety()+1)->getIsWhite() != chessman->getIsWhite()){
            chess = true;
        }
    }

    // On regarde les pieces sur les lignes droites et diagonales
    int incr = 1;

    /*******************Lignes droites****************************************/
    // Ligne de gauche
    if (!chess){
        while (Kpos.gety()-incr >= 0 && this->isVoid(Kpos.getx(), Kpos.gety()-incr))
            incr += 1;
        if (Kpos.gety()-incr >= 0){
            // Le roi adverse est a cote
            if (incr ==1 && (*this)(Kpos.getx(), Kpos.gety()-incr)->getName() == "King"){
                chess = true;
            }
            else if ((*this)(Kpos.getx(), Kpos.gety()-incr)->getIsWhite() != chessman->getIsWhite()
                     && ((*this)(Kpos.getx(), Kpos.gety()-incr)->getName() == "Rook" || (*this)(Kpos.getx(), Kpos.gety()-incr)->getName() == "Queen"))
                chess = true;
        }
    }

    // Ligne de droite
    incr = 1;

    if (!chess){
        while (Kpos.gety()+incr < 8 && this->isVoid(Kpos.getx(), Kpos.gety()+incr))
            incr += 1;
        if (Kpos.gety()+incr < 8){
            // Le roi adverse est a cote
            if (incr ==1 && (*this)(Kpos.getx(), Kpos.gety()+incr)->getName() == "King"){
                chess = true;
            }
            else if ((*this)(Kpos.getx(), Kpos.gety()+incr)->getIsWhite() != chessman->getIsWhite()
                     && ((*this)(Kpos.getx(), Kpos.gety()+incr)->getName() == "Rook" || \
                         (*this)(Kpos.getx(), Kpos.gety()+incr)->getName() == "Queen"))
                chess = true;
        }
    }

    // Ligne du bas
    incr = 1;

    if (!chess){
        while (Kpos.getx()+incr < 8 && this->isVoid(Kpos.getx()+incr, Kpos.gety()))
            incr += 1;
        if (Kpos.getx()+incr < 8){
            // Le roi adverse est a cote
            if (incr ==1 && (*this)(Kpos.getx()+incr, Kpos.gety())->getName() == "King"){
                chess = true;
            }
            else if ((*this)(Kpos.getx()+incr, Kpos.gety())->getIsWhite() != chessman->getIsWhite()
                     && ((*this)(Kpos.getx()+incr, Kpos.gety())->getName() == "Rook" || \
                         (*this)(Kpos.getx()+incr, Kpos.gety())->getName() == "Queen"))
                chess = true;
        }
    }

    // Ligne du haut
    incr = 1;

    if (!chess){
        while (Kpos.getx()-incr >=0 && this->isVoid(Kpos.getx()-incr, Kpos.gety()))
            incr += 1;
        if (Kpos.getx()-incr >= 0){
            // Le roi adverse est a cote
            if (incr ==1 && (*this)(Kpos.getx()-incr, Kpos.gety())->getName() == "King"){
                chess = true;
            }
            else if ((*this)(Kpos.getx()-incr, Kpos.gety())->getIsWhite() != chessman->getIsWhite()
                     && ((*this)(Kpos.getx()-incr, Kpos.gety())->getName() == "Rook" || \
                         (*this)(Kpos.getx()-incr, Kpos.gety())->getName() == "Queen"))
                chess = true;
        }
    }

    /************************Diagonales*************************************/
    // Sud-Est
    incr = 1;

    if (!chess){
        while (Kpos.getx()+incr < 8 && Kpos.gety()+incr < 8 && this->isVoid(Kpos.getx()+incr, Kpos.gety()+incr))
            incr += 1;
        if (Kpos.getx()+incr < 8 && Kpos.gety()+incr < 8){
            // Le roi adverse est a cote
            if (incr ==1 && (*this)(Kpos.getx()+incr, Kpos.gety()+incr)->getName() == "King"){
                chess = true;
            }
            else if ((*this)(Kpos.getx()+incr, Kpos.gety()+incr)->getIsWhite() != chessman->getIsWhite()
                     && ((*this)(Kpos.getx()+incr, Kpos.gety()+incr)->getName() == "Bishop" || \
                         (*this)(Kpos.getx()+incr, Kpos.gety()+incr)->getName() == "Queen"))
                chess = true;
        }
    }

    // Nord-Est
    incr = 1;

    if (!chess){
        while (Kpos.getx()-incr >=0 && Kpos.gety()+incr < 8 && this->isVoid(Kpos.getx()-incr, Kpos.gety()+incr))
            incr += 1;
        if (!chess && Kpos.getx()-incr >= 0 && Kpos.gety()+incr < 8){
            // Le roi adverse est a cote
            if (incr ==1 && (*this)(Kpos.getx()-incr, Kpos.gety()+incr)->getName() == "King"){
                chess = true;
            }
            else if ((*this)(Kpos.getx()-incr, Kpos.gety()+incr)->getIsWhite() != chessman->getIsWhite()
                     && ((*this)(Kpos.getx()-incr, Kpos.gety()+incr)->getName() == "Bishop" || \
                         (*this)(Kpos.getx()-incr, Kpos.gety()+incr)->getName() == "Queen"))
                chess = true;
        }
    }

    // Nord-Ouest
    incr = 1;

    if (!chess){
        while (Kpos.getx()-incr >=0 && Kpos.gety()-incr >= 0 && this->isVoid(Kpos.getx()-incr, Kpos.gety()-incr))
            incr += 1;
        if (Kpos.getx()-incr >= 0 && Kpos.gety()-incr >= 0){
            // Le roi adverse est a cote
            if (incr ==1 && (*this)(Kpos.getx()-incr, Kpos.gety()-incr)->getName() == "King"){
                chess = true;
            }
            else if ((*this)(Kpos.getx()-incr, Kpos.gety()-incr)->getIsWhite() != chessman->getIsWhite()
                     && ((*this)(Kpos.getx()-incr, Kpos.gety()-incr)->getName() == "Bishop" || \
                         (*this)(Kpos.getx()-incr, Kpos.gety()-incr)->getName() == "Queen"))
                chess = true;
        }
    }

    // Sud-Ouest
    incr = 1;

    if (!chess){
        while (Kpos.getx()+incr < 8 && Kpos.gety()-incr >= 0 && this->isVoid(Kpos.getx()+incr, Kpos.gety()-incr))
            incr += 1;
        if (Kpos.getx()+incr < 8 && Kpos.gety()-incr >= 0){
            // Le roi adverse est a cote
            if (incr ==1 && (*this)(Kpos.getx()+incr, Kpos.gety()-incr)->getName() == "King"){
                chess = true;
            }
            else if ((*this)(Kpos.getx()+incr, Kpos.gety()-incr)->getIsWhite() != chessman->getIsWhite()
                     && ((*this)(Kpos.getx()+incr, Kpos.gety()-incr)->getName() == "Bishop" || \
                         (*this)(Kpos.getx()+incr, Kpos.gety()-incr)->getName() == "Queen"))
                chess = true;
        }
    }

    /*******************************Fin des tests********************************************/

    if (someoneTaken)
        (*this)(x,y,takenChessman);
    else
        this->setNone(x,y);
    (*this)(coordIniChess[0], coordIniChess[1], chessman);
    return chess;
}



/***********************************************************************************************************/
/******************************************Conversion de la grille en notation fen**************************/
/***********************************************************************************************************/

string Grid::fen(bool just_grid){ // code la grille en standard fen
    char letter;
    string resultat;
    for (int ligne=0;ligne<=7;ligne++){
        int colonne=0;
        while (colonne <= 7){
            int compt = 0;
            if ((*this).isVoid(ligne,colonne)){
                compt += 1;
                colonne += 1;
                while (colonne<8 && (*this).isVoid(ligne,colonne)){
                    compt += 1;
                    colonne += 1;
                }
                resultat += char(compt + '0');
                compt = 0;
            }
            else{
                if((*this)(ligne,colonne)->getName() == "Knight"){
                    letter = 'N';
                }
                else{
                    letter = ((*this)(ligne,colonne)->getName())[0];
                }
                //Les pieces noires sont écrites en minuscule
                if (! (*this)(ligne,colonne)->getIsWhite()){
                    letter = letter - 'A' + 'a';
                }
                resultat += letter;
                colonne += 1;
            }
        }
        if (ligne < 7){
            resultat += '/';
        }

    }
    resultat += ' ';

    //couleur du joueur
    if (whiteIsPlaying){
        resultat += 'w';
    }
    else{
        resultat += 'b';
    }
    resultat += ' ';

    //roques possibles
    bool Rockpossible = false;

    //pour les blancs
    if (((*this)(7,4)->getName() != "Empty_Chessman") && ((*this)(7,4)->getName() == "King") && (!(*this)(7,4)->getHasMoved())){
        if (((*this)(7,7)->getName() != "Empty_Chessman") && ((*this)(7,7)->getName() == "Rook") && (!(*this)(7,7)->getHasMoved())){
            resultat += 'K';
            Rockpossible=true;
        }
        if (((*this)(7,0)->getName() != "Empty_Chessman") && ((*this)(7,0)->getName() == "Rook") && (!(*this)(7,0)->getHasMoved())){
            resultat += 'Q';
            Rockpossible=true;
        }
    }
    //pour les noirs
    if (((*this)(0,4)->getName() != "Empty_Chessman") && ((*this)(0,4)->getName() == "King") && (!(*this)(0,4)->getHasMoved())){
        if (((*this)(0,7)->getName() != "Empty_Chessman") && ((*this)(0,7)->getName() == "Rook") && (!(*this)(0,7)->getHasMoved())){
            resultat += 'k';
            Rockpossible=true;
        }
        if (((*this)(0,0)->getName() != "Empty_Chessman") && ((*this)(0,0)->getName() == "Rook") && (!(*this)(0,0)->getHasMoved())){
            resultat += 'q';
            Rockpossible=true;
        }
    }

    if (!Rockpossible){
        resultat += '-';
    }
    resultat += ' ';

    //position de la prise en passant

    bool passing = false;
    int li = 4 - whiteIsPlaying;
    for (int col=0;col<8;col++){
        if (!(*this).isVoid(li,col)){
            if (((*this)(li,col)->getName() == "Pawn")){
                if ((*this)(li,col)->isDouble_done()){
                    passing = true;
                    resultat += char(col + 'a'); //caractere associé au code ASCII
                    if (li==4){
                        resultat += char(6+'0');
                    }
                    if (li==3){
                        resultat += char(3+'0');
                    }
                }
            }
        }
    }
    if(!passing){
        resultat += '-';
    }

    // Si on ne s'interesse pas au nombre de coups
    if (just_grid){
        return resultat;
    }

    resultat += ' ';

    //Nombre de demi-coups
    int n = 0;
    int count = countHalfMove;
    if (count<10){
        resultat += char(count+'0');
    }
    else {
        n = count/10;
        count = count%10;
        resultat += char(n+'0');
        resultat += char(count+'0');
    }

    resultat += ' ';

    // Nombre de coups
    n = 0;
    count = countMove;
    if (count<10){
        resultat += char(count+'0');
    }
    else {
        n = count/10;
        count = count%10;
        resultat += char(n+'0');
        resultat += char(count+'0');
    }

    return resultat;
}
