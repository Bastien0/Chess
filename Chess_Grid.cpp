#include "Chess_Grid.h"
Grid::Grid(string s){
    grid = new Chessman*[64];
    countMove = 0;
    countHalfMove = 0;
    int ligne = 0;
    int colonne = 0;
    int caractere = 0;
    cout << "constructeur de Grid" << endl;
    while (s[caractere] != ' '){
        if (s[caractere] == '/'){
            ligne += 1;
            colonne = -1;
        }
        // Cas des pieces noires
        else if (s[caractere] == 'r'){
            Rook r(ligne, colonne, false);
            (*this)(ligne, colonne, r.clone());
        }
        else if (s[caractere] == 'n'){
            Knight n(ligne, colonne, false);
            (*this)(ligne, colonne, n.clone());
        }
        else if (s[caractere] == 'b'){
            Bishop b(ligne, colonne, false);
            (*this)(ligne, colonne, b.clone());
        }
        else if (s[caractere] == 'q'){
            Queen q(ligne, colonne, false);
            (*this)(ligne, colonne, q.clone());
        }
        else if (s[caractere] == 'k'){
            King k(ligne, colonne, false);
            (*this)(ligne, colonne, k.clone());
        }
        else if (s[caractere] == 'p'){
            Pawn p(ligne, colonne, false);
            (*this)(ligne, colonne, p.clone());
        }

        //Cas des pieces blanches
        else if (s[caractere] == 'R'){
            Rook r(ligne, colonne, true);
            (*this)(ligne, colonne, r.clone());
        }
        else if (s[caractere] == 'N'){
            Knight n(ligne, colonne, true);
            (*this)(ligne, colonne, n.clone());
        }
        else if (s[caractere] == 'B'){
            Bishop b(ligne, colonne, true);
            (*this)(ligne, colonne, b.clone());
        }
        else if (s[caractere] == 'Q'){
            Queen q(ligne, colonne, true);
            (*this)(ligne, colonne, q.clone());
        }
        else if (s[caractere] == 'K'){
            King k(ligne, colonne, true);
            (*this)(ligne, colonne, k.clone());
        }
        else if (s[caractere] == 'P'){
            Pawn p(ligne, colonne, true);
            (*this)(ligne, colonne, p.clone());
        }

        // Cas des chiffres
        else{
            int n = s[caractere]-'0';
            cout << "on a "<< n << "cases vides a la suite"<<endl;
            for (int i = 0; i < n; i++){
                Empty_Chessman e(ligne, colonne);
                (*this)(ligne, colonne, e.clone());
                if (i < n-1)
                    colonne += 1; // on augmente la colonne de n-1 au total
            }
        }
        cout << "on a lu la case " << ligne <<" "<< colonne<< endl;
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
        ch[i]=(s[caractere]);
        caractere +=1;
    }
        //S'il n 'y a pas de roque possible
    if (s[0] == '-'){
        Point Break = this->king_position(whiteIsPlaying);
        (*this)(Break.getx(), Break.gety())->sethasMoved(true);
    }
        //Sinon

    else{
        bool PresenceK=false;
        bool Presencek=false;
        bool PresenceQ=false;
        bool Presenceq=false;
        for (int ind=0;ind<ch.size();ind++){
            if(ch[ind]== 'K'){
                PresenceK = true;
            }
            else if(ch[ind]== 'q'){
                Presenceq = true;
            }
            else if(ch[ind]== 'Q'){
                PresenceQ = true;
            }
            else if(ch[ind]== 'k'){
                Presencek = true;
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
    cout<<"TEST SUR LA PRISE EN PASSANT"<<endl;
    if (s[caractere] != '-'){
        if (s[caractere+1]=='6'){
            (*this)(4,int(s[caractere]-'a'))->setdouble_done(true);
        }
        if (s[caractere+1]=='3'){
            (*this)(3,int(s[caractere]-'a'))->setdouble_done(true);
        }
        int direc=2*whiteIsPlaying-1; //selon si les blancs jouent ou non, on considere la case au dessus/dessous
        cout<<int(s[caractere]-'a')<<endl;
        caractere +=1;
    }
    cout<<"test prise en passant"<<endl;
    cout<<(*this)(4,4)->isDouble_done()<<endl;
    caractere += 2;

    //Compte des coups
    countHalfMove = 0;
    while (s[caractere] != ' '){
        countHalfMove = countHalfMove*10+int(s[caractere]-'0');
        caractere +=1;
    }
    cout<<"demis coups : "<<countHalfMove<<endl;
    caractere += 1;

    countMove = 0;
    while (caractere != s.size()){
        countMove = countMove*10+int(s[caractere]-'0');
        caractere +=1;
    }
    cout<<"coups : "<<countMove<<endl;


    //Test pour le constructeur
    for (int i=0;i<8;i++){
        for(int j=0;j<8;j++){
        cout<<(*this)(i,j)->getName()<<endl;
        }
    }
}

Chessman* Grid::operator()(int coord0, int coord1){
    return grid[coord0+8*coord1];
}

void Grid::operator()(int coord0, int coord1, Chessman* chessman){
    chessman->setx(coord0);
    chessman->sety(coord1);
    grid[coord0+8*coord1] = chessman;
}

// il y avait un probleme avec string promotion = "" j'essaie d ne rien mettre
void Grid::move(int coord[2], Chessman* chessman, string promotion){
    int oldCoordChessman[2];
    oldCoordChessman[0] = coord[0]; oldCoordChessman[1] = coord[1];
    countHalfMove += 1;
    countMove += 1;
    cout << "on entre bien dans la fonction move" << endl;
    // on regarde si la piece a un argument de mouvement
    if ((*chessman).getName() == "King" || (*chessman).getName() == "Rook")
        chessman->sethasMoved(true);

    if ((*chessman).getName() == "Pawn"){
        countHalfMove = 0;
        cout<< "on a bien teste que c'est un pion"<< endl;
        //cas de la prise en passant
        if ((*chessman).gety() != coord[1] && this->isVoid(coord[0], coord[1])){
            if ((*chessman).getIsWhite()){
                this->setNone(coord[0]+1, coord[1]);
            }
            else{
                this->setNone(coord[0]-1, coord[1]);
            }
        }

        //promotion
        if (promotion != ""){
            if (promotion == "Queen"){
                delete chessman;
                chessman = chessman->clone();
            }
            if (promotion == "Bishop"){
                delete chessman;
                chessman = chessman->clone();
            }
            if (promotion == "Knight"){
                delete chessman;
                chessman = chessman->clone();
            }
            if (promotion == "Rook"){
                delete chessman;
                chessman = chessman->clone();
            }
        }

        // cas du coup double du pion
        if ((*chessman).gety() != coord[1] && this->isVoid(coord[0], coord[1]))
            chessman->setdouble_done(true);
    }


    // s'il y a roque
    else if (chessman->getName() == "King" && (chessman->gety() != coord[1])-1 && (chessman->gety()-coord[1]) != 0 && (chessman->gety()-coord[1]) != 1){
        // si on va vers la gauche
        if (chessman->gety() > coord[1]){
            (*this)((*chessman).getx(), 3, (*this)((*chessman).getx(), 0));
            this->setNone((*chessman).getx(), 0);
        }
        // vers la gauche
        else{
            (*this)((*chessman).getx(), 5, (*this)((*chessman).getx(), 7));
            this->setNone((*chessman).getx(), 7);
        }
    }

    if (!this->isVoid(coord[0], coord[1]))
        countHalfMove = 0;

    vector<Chessman*> l = this->list_chessman_col(!(*chessman).getIsWhite());
    vector<Chessman*>::iterator it = l.begin();
    for (; it != l.end(); ++it)
        if ((*this)((*it)->getx(), (*it)->gety())->getName() == "Pawn"){
            (*this)((*it)->getx(), (*it)->gety())->setdouble_done(false);
         }

    this->setNone(oldCoordChessman[0],oldCoordChessman[1]);
    (*this)(coord[0], coord[1], chessman);
}


//Attention à la construction par copie !
void Grid::setNone(int x, int y){
    Empty_Chessman E(x,y);
    grid[x+8*y] = E.clone();
}

Point Grid::king_position(bool isWhite){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (!this->isVoid(i,j) && grid[i+8*j]->getIsWhite() == isWhite && grid[i+8*j]->getName() == "King"){
                return Point(i,j);
            }
        }
    }
}

bool Grid::isVoid(int x, int y){
    return (grid[x+8*y]->getName() == "Empty");
}

vector<Chessman*> Grid::list_chessman_col(bool colorIsWhite){
    vector<Chessman*> l;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (!(grid[i+8*j]->getName() == "Empty") && grid[i+8*j]->getIsWhite() == colorIsWhite)
                    l.push_back(grid[i+8*j]);
        }
    }
    return l;
}

bool Grid::sameColor(Chessman* chessman, int x, int y){
    return ((*(*this)(x,y)).getIsWhite() == chessman->getIsWhite());
}

// quand on deplace une piece sur la grille, se met on en echec?
// pour savoir cela, on effectue le deplacement et on regarde.
// Apres avoir regarde cela, il faut remettre les pieces a leur place :
// il faut reparer les degats en remettant a leur place les elements qui ont
// ete deplaces.
bool Grid::isChessed(Chessman* chessman, int x, int y){
    // memorisation de la piece prise, si le deplacement en prend une
    bool someoneTaken = false;
    Chessman* takenChessman;
    Empty_Chessman E(-1,-1);
    takenChessman = E.clone();
    if (!this->isVoid(x, y)){
        someoneTaken = true;
        //Attention à la construction par copie
        takenChessman = (*this)(x, y);
    }
    // memoristaion de l'ancienne position
    int  coordIniChess[2];
    coordIniChess[0] = chessman->getx();
    coordIniChess[1] = chessman->gety();
    (*this)(x,y,chessman);
    // utile ?
    chessman->setx(x);
    chessman->sety(y);
    this->setNone(coordIniChess[0], coordIniChess[1]);
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            // si on tombe sur une piece de couleur differente,
            // on verifie qu'elle ne met pas le roi en echec
            // ie si la position du roi n'est pas dans la liste des
            // positions accessibles,
            // on repare ce qu'on a bouge dans la grille et on renvoie true
            if(!this->isVoid(i,j)){
                vector<Point> l = (*(*this)(i,j)).moves((*this));
                if (chessman->getIsWhite() != (*this)(i,j)->getIsWhite() \
                        && std::find(l.begin(),l.end(), this->king_position(chessman->getIsWhite())) != l.end()){
                    // On remet l'echiquier en place
                    if (someoneTaken)
                        (*this)(x,y,takenChessman);
                    else
                        this->setNone(x,y);
                    chessman->setx(coordIniChess[0]);
                    chessman->sety(coordIniChess[1]);
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
    chessman->setx(coordIniChess[0]);
    chessman->sety(coordIniChess[1]);
    (*this)(coordIniChess[0], coordIniChess[1], chessman);
    return false;
}

string Grid::fen(){
    vector<char> f;
    char letter;
    for (int ligne=0;ligne<=7;ligne++){
        int colonne=0;
        while (colonne <= 7){
            int compt = 0;
            cout<<"Test0"<<endl;
            if ((*this).isVoid(ligne,colonne)){
                cout<<"Test 1"<<endl;
                compt += 1;
                colonne += 1;
                while (colonne<8 && (*this).isVoid(ligne,colonne)){
                    cout<<"Test2 "<<compt<<endl;
                    compt += 1;
                    colonne += 1;
                }
                f.push_back(char(compt + '0'));
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
                f.push_back(letter);
                colonne += 1;
            }
        }
        if (ligne < 7){
            f.push_back('/');
        }

    }
    f.push_back(' ');

    //couleur du joueur
    if (whiteIsPlaying){
        f.push_back('w');
    }
    else{
        f.push_back('b');
    }
    f.push_back(' ');

    //roques possibles
    //pour les blancs
    bool Rockpossible = false;
    if (((*this)(0,4)->getName() != "Empty_Chessman") && ((*this)(0,4)->getName() == "King") && (!(*this)(0,4)->getHasMoved())){
        if (((*this)(0,7)->getName() != "Empty_Chessman") && ((*this)(0,7)->getName() == "Rook") && (!(*this)(0,7)->getHasMoved())){
            f.push_back('k');
            Rockpossible=true;
        }
        if (((*this)(0,0)->getName() != "Empty_Chessman") && ((*this)(0,0)->getName() == "Rook") && (!(*this)(0,0)->getHasMoved())){
            f.push_back('q');
            Rockpossible=true;
        }
    }
    //pour les noirs
    if (((*this)(7,4)->getName() != "Empty_Chessman") && ((*this)(7,4)->getName() == "King") && (!(*this)(7,4)->getHasMoved())){
        if (((*this)(7,7)->getName() != "Empty_Chessman") && ((*this)(7,7)->getName() == "Rook") && (!(*this)(7,7)->getHasMoved())){
            f.push_back('k');
            Rockpossible=true;
        }
        if (((*this)(7,0)->getName() != "Empty_Chessman") && ((*this)(7,0)->getName() == "Rook") && (!(*this)(7,0)->getHasMoved())){
            f.push_back('q');
            Rockpossible=true;
        }
    }
    if (!Rockpossible){
        f.push_back('-');
    }
    f.push_back(' ');

    //position de la prise en passant

    bool passing = false;
    int li = 4 - whiteIsPlaying;
    for (int col=0;col<8;col++){
        if (!(*this).isVoid(li,col)){
            if (((*this)(li,col)->getName() == "Pawn")){
                cout<<"ligne "<<li<<"colonne "<<col<<endl;
                if ((*this)(li,col)->isDouble_done()){
                    passing = true;
                    f.push_back(char(col + 'a')); //caractere associé au code ASCII
                    if (li==4){
                        f.push_back(char(6+'0'));
                    }
                    if (li==3){
                        f.push_back(char(3+'0'));
                    }
                }
            }
        }
    }
    if(!passing){
        f.push_back('-');
    }
    f.push_back(' ');

    //Nombre de demi-coups
    int n = 0;
    int count = countHalfMove;
    if (count<10){
        f.push_back(char(count+'0'));
    }
    else {
        n = count/10;
        count = count%10;
        cout<<"n "<<n<<"count vaut "<<count<<endl;
        f.push_back(char(n+'0'));
        f.push_back(char(count+'0'));
    }

    f.push_back(' ');

    // Nombre de coups
    n = 0;
    count = countMove;
    if (count<10){
        f.push_back(char(count+'0'));
    }
    else {
        n = count/10;
        count = count%10;
        f.push_back(char(n+'0'));
        f.push_back(char(count+'0'));
    }

    string resultat;
    cout << "la taille de f est : " << f.size() << endl;
    for (int i = 0; i<f.size(); i++)
        resultat += f[i];

    return resultat;
}
