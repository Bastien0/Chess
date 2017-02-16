#include "Chess_Grid.h"
#include <iostream>
#include <set>
#include <map>
#include <limits.h>
#include "point.h"
using namespace std;


//Fonction alpha-beta
int alpha_beta(Grid& G, int depth, int depthmax, bool isMax, bool color, int alpha, int beta, map<string,Point>& memory){
    //Condition d'arrêt
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

    //Max
    else if (isMax){
        //Initialisation
        int M = (INT_MIN+depthmax)-depth;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (!G.isVoid(i,j) && G(i,j)->getIsWhite() == G.getWhiteIsPlaying()){
                    possibleMoves = G(i,j)->allowed_moves(G);
                    //On parcourt tous les coups possibles
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
    //Min
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

//Meilleur coup : fonction appelée ppar Python pour connaître le meilleur coup à jouer
int best_move(int depth, string fen){
    // Creation de la grille
    Grid G(fen);

    // Vecteur des positions accessibles et map de stockage
    vector<Point> possibleMoves;
    map<string, Point> memory;

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
                        move = (i+1)*1000+(j+1)*100+((*it).getx()+1)*10+(*it).gety()+1;
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

                    // Si l'evaluation est meilleure, on retient le coup à jouer
                    if (eval > M){
                        M = eval;
                        move = (i+1)*1000+(j+1)*100+((*it).getx()+1)*10+(*it).gety()+1;
                    }
                    G.unmove(startingFrame, arrivingFrame, *it, Enpassant);
                }
            }
        }
    }
    return move;
}
