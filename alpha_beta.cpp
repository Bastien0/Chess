#include "Chess_Grid.h"
#include <iostream>
#include <set>
#include <map>
#include <limits.h>
#include "point.h"
using namespace std;

int alpha_beta(Grid& G, int depth, int depthmax, bool isMax, bool color, int alpha, int beta, map<string,Point>& memory){
    if (depth <= 0)
        return (2*color-1)*G.getScore();
    int eval = 0;
    vector<Point> possibleMoves;
    // On enregistre la prise en passant
    Point Enpassant = G.en_passant();

    map<string,Point>::iterator it = memory.find(G.fen(true));
    // Si on a deja teste la grille avec une profondeur au moins egale
    // On retourne la valeur deja calculee
    if (it != memory.end() && (*it).second.getx()>= depth)
        return (*it).second.gety();


    else if (isMax){
        int M = (INT_MIN+depthmax)-depth;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (!G.isVoid(i,j) && G(i,j)->getIsWhite() == G.getWhiteIsPlaying()){
                    possibleMoves = G(i,j)->allowed_moves(G);
                    for (vector<Point>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it){
                        Chessman* startingFrame = G(i,j)->clone();
                        Chessman* arrivingFrame = G(it->getx(),it->gety())->clone();
                        // On teste si un pion arrive sur la case de promotion
                        // On ne teste que pour la reine pour l'instant
                        if (G(i,j)->getName() == "Pawn" && (*it).getx() == (!G(i,j)->getIsWhite())*7)
                            G.move((*it), G(i,j), "Queen");
                        else
                            G.move((*it), G(i,j));
                        // On evalue la grille
                        eval = alpha_beta(G, depth-1, depthmax, !isMax, color, alpha, beta, memory);
                        // Si l'evaluation est meilleure
                        if (eval > M)
                            M = eval;
                        if (alpha < M)
                            alpha = M;
                        G.unmove(startingFrame, arrivingFrame, *it, Enpassant);
                        if (beta < alpha)
                            return M;
                    }
                }
            }
        }
        memory.insert(pair<string,Point>(G.fen(true), Point(depth, M)));
        return M;
    }
    else{
        int M = (INT_MAX-depthmax)+depth;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (!G.isVoid(i,j) && G(i,j)->getIsWhite() == G.getWhiteIsPlaying()){
                    possibleMoves = G(i,j)->allowed_moves(G);
                    for (vector<Point>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it){
                        Chessman* startingFrame = G(i,j)->clone();
                        Chessman* arrivingFrame = G(it->getx(),it->gety())->clone();
                        // On teste si un pion arrive sur la case de promotion
                        // On ne teste que pour la reine pour l'instant
                        if (G(i,j)->getName() == "Pawn" && (*it).getx() == (!G(i,j)->getIsWhite())*7)
                            G.move(*it, G(i,j), "Queen");
                        else
                            G.move(*it, G(i,j));
                        // On evalue la grille
                        eval = alpha_beta(G, depth-1, depthmax, !isMax, color, alpha, beta, memory);
                        // Si l'evaluation est meilleure
                        if (eval < M)
                            M = eval;
                        if (beta > M)
                            beta = M;
                        G.unmove(startingFrame, arrivingFrame, *it, Enpassant);
                        if (beta < alpha)
                            return M;
                    }
                }
            }
        }
        memory.insert(pair<string,Point>(G.fen(true), Point(depth, M)));
        return M;
    }
}

int best_move(int depth, string fen){
    // Creation de la grille
    Grid G(fen);

    // Vecteur des positions accessibles et map de stockage
    vector<Point> possibleMoves;
    map<string, Point> memory;
    vector<Point> Start;
    vector<Point> Final;
    vector<int> Value;
    int best[5] = {-1,-1,-1,-1,-1};

    // On enregistre la prise en passant
    Point Enpassant(0,0);
    for (int i = 0; i < 8; i++)
        if (G(4-G.getWhiteIsPlaying(), i)->getName() == "Pawn" && G(4-G.getWhiteIsPlaying(), i)->isDouble_done()){
            Enpassant.setx(4-G.getWhiteIsPlaying());
            Enpassant.sety(i);
        }
    int M = INT_MIN;
    int eval;
    int move = -1;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (!G.isVoid(i,j) && G(i,j)->getIsWhite() == G.getWhiteIsPlaying()){
                possibleMoves = G(i,j)->allowed_moves(G);
                for (vector<Point>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it){
                    if (move == -1)
                        move = i*1000+j*100+(*it).getx()*10+(*it).gety();
                    Chessman* startingFrame = G(i,j)->clone();
                    Chessman* arrivingFrame = G(it->getx(),it->gety())->clone();
                    // On teste si un pion arrive sur la case de promotion
                    // On ne teste que pour la reine pour l'instant
                    if (G(i,j)->getName() == "Pawn" && (*it).getx() == !(G(i,j)->getIsWhite())*7)
                        G.move(*it, G(i,j), "Queen");
                    else
                        G.move(*it, G(i,j));
                    // On evalue la grille (la couleur d'evaluation est celle precedent le move
                    eval = alpha_beta(G, depth-1, depth, false, !G.getWhiteIsPlaying(), INT_MIN, INT_MAX, memory);
                    Start.push_back(Point(i,j));
                    Final.push_back(*it);
                    Value.push_back(eval);
                    bool done = false;
                    for (int n = 0; n < 5; n++){
                        if (!done && (best[i] == -1 || eval > Value[best[i]])){
                            for (int j = 4; j > n; j--){
                                best[j] = best[j-1];
                            }
                            best[n] = Value.size()-1;
                            done =true;
                        }
                    }
                    // Si l'evaluation est meilleure
                    if (eval > M){
                        M = eval;
                        move = i*1000+j*100+(*it).getx()*10+(*it).gety();
                    }
                    G.unmove(startingFrame, arrivingFrame, *it, Enpassant);
                }
            }
        }
    }
    /*M = INT_MIN;
    for (int n = 0; n < 5; n++){
        if (best[n] != -1){
            Chessman* startingFrame = G(Start[best[n]])->clone();
            Chessman* arrivingFrame = G(Final[best[n]])->clone();
            if (G(Start[best[n]])->getName() == "Pawn" && Start[best[n]].getx() == (!G(Start[best[n]])->getIsWhite())*7)
                G.move(Final[best[n]], G(Start[best[n]]), "Queen");
            else
                G.move(Final[best[n]], G(Start[best[n]]));
            // On evalue la grille
            eval = alpha_beta(G, depth-1, depth, false, !G.getWhiteIsPlaying(), INT_MIN, INT_MAX, memory);
            if (eval > M){
                M = eval;
                move = Start[best[n]].getx()*1000+Start[best[n]].gety()*100+Final[best[n]].getx()*10+Final[best[n]].gety();
            }
            G.unmove(startingFrame, arrivingFrame, Final[best[n]], Enpassant);
        }
    }*/
    return move;
}

/*************************************************************************************************************/
/*************************************************************************************************************/

/*int f(int dep){
    cout<<"feoifh"<<endl;
    fstream file("C:/Users/anatole parre/Desktop/ENPC/2A/TD Log/Chess/Chess/Chess/data.docx",ios::in);
    if(file)  // si l'ouverture a réussi
    {    string fen;
         getline(file,fen);

         cout<<fen<<endl;
         file.close();  // on ferme le fichier
    }
    else  // sinon
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    fstream file2("C:/Users/anatole parre/Desktop/ENPC/2A/TD Log/Chess/Chess/Chess/data.docx",ios::in | ios::out | ios::trunc);
    if (file2){    file2 << best_move(dep,fen) <<" "<<4<<endl;
         file2.close();  // on ferme le fichier
    }
    else  // sinon
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    return 0;
}*/
