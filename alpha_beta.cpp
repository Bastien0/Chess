#include "Chess_Grid.h"
#include <iostream>
#include <set>
#include <map>
#include <limits.h>
#include "point.h"
using namespace std;

int evaluation(Grid& G, bool color){
    int eval=0;
    /*int PawnOnColumn[7];
    for (int i=0;i<7;i++){
        PawnOnColumn[i]=0;
    }*/
    int sameCol;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (!G.isVoid(i,j)){
                sameCol = 2*(color == G(i,j)->getIsWhite()) - 1;
                //vaut 1 si le joueur qui a joue est de la meme couleur que la piece
                //vaut -1 s'ils sont differents
                /*vector<Point> allow=G(i,j)->allowed_moves(G);
                eval += allow.size();*/
                if (G(i,j)->getName() == "Pawn"){
                    eval += sameCol*100; //valeur du pion est 100
                    /*
                    if (PawnOnColumn[j]==1){
                        eval -= 50;
                    }*/

                }
                else if (G(i,j)->getName() == "Knight"){
                    eval += sameCol*300; //valeur du cavalier est 300
                }
                else if (G(i,j)->getName() == "Bishop"){
                    eval += sameCol*300; //valeur du fou est 300
                }
                else if (G(i,j)->getName() == "Rook"){
                    eval += sameCol*500; //valeur de la tour est 500
                }
                else if (G(i,j)->getName() == "Queen"){
                    eval += sameCol*900; //valeur de la reine est 900
                }
            }
        }
    }
    return eval;
}

int alpha_beta(Grid& G, int depth, bool isMax, bool color, int alpha, int beta, map<string,Point>& memory){
    int eval = 0;
    vector<Point> possibleMoves;
    // On enregistre la prise en passant
    Point Enpassant(0,0);
    for (int i = 0; i < 8; i++)
        if (G(4-G.getWhiteIsPlaying(), i)->getName() == "Pawn" && G(4-G.getWhiteIsPlaying(), i)->isDouble_done()){
            Enpassant.setx(4-G.getWhiteIsPlaying());
            Enpassant.sety(i);
        }

    if (depth <= 0)
        return (2*color-1)*G.getScore();

    map<string,Point>::iterator it = memory.find(G.fen(true));
    // Si on a deja teste la grille avec une profondeur au moins egale
    // On retourne la valeur deja calculee
    if (it != memory.end() && (*it).second.getx()>= depth)
        return (*it).second.gety();

    else if (isMax){
        int M = INT_MIN;
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
                        eval = alpha_beta(G, depth-1, !isMax, color, alpha, beta, memory);
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
        int M = INT_MAX;
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
                        eval = alpha_beta(G, depth-1, !isMax, color, alpha, beta, memory);
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
    Grid G(fen);
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
    int move = 0;
    int eval;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (!G.isVoid(i,j) && G(i,j)->getIsWhite() == G.getWhiteIsPlaying()){
                possibleMoves = G(i,j)->allowed_moves(G);
                for (vector<Point>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it){
                    Chessman* startingFrame = G(i,j)->clone();
                    Chessman* arrivingFrame = G(it->getx(),it->gety())->clone();
                    // On teste si un pion arrive sur la case de promotion
                    // On ne teste que pour la reine pour l'instant
                    if (G(i,j)->getName() == "Pawn" && (*it).getx() == !(G(i,j)->getIsWhite())*7)
                        G.move(*it, G(i,j), "Queen");
                    else
                        G.move(*it, G(i,j));
                    // On evalue la grille (la couleur d'evaluation est celle precedent le move
                    eval = alpha_beta(G, depth-1, false, !G.getWhiteIsPlaying(), INT_MIN, INT_MAX, memory);
                    // Si l'evaluation est meilleure
                    if (eval > M){
                        M = eval;
                        move = 1000*i+100*j+10*(*it).getx()+(*it).gety();
                    }
                    G.unmove(startingFrame, arrivingFrame, *it, Enpassant);
                }
            }
        }
    }
    return move;
}
