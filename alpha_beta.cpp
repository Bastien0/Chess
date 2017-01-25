#include "Chess_Grid.h"
#include <iostream>
#include <limits.h>
#include "point.h"
using namespace std;

int evaluation(Grid& G){
    int eval=0;
    int PawnOnColumn[7];
    for (int i=0;i<7;i++){
        PawnOnColumn[i]=0;
    }
    int sameCol;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (!G.isVoid(i,j)){
                sameCol = 2*(G.getWhiteIsPlaying() != G(i,j)->getIsWhite()) - 1;
                //vaut 1 si le joueur qui a joue est de la meme couleur que la piece
                //vaut -1 s'ils sont differents
                vector<Point> allow=G(i,j)->allowed_moves(G);
                eval += allow.size();
                if (G(i,j)->getName() == "Pawn"){
                    eval += sameCol*100; //valeur du pion est 100
                    /*
                    if (PawnOnColumn[j]==1){
                        eval -= 50;
                    }*/

                }
                if (G(i,j)->getName() == "Knight"){
                    eval += sameCol*300; //valeur du cavalier est 300
                }
                if (G(i,j)->getName() == "Bishop"){
                    eval += sameCol*300; //valeur du fou est 300
                }
                if (G(i,j)->getName() == "Rook"){
                    eval += sameCol*500; //valeur de la tour est 500
                }
                if (G(i,j)->getName() == "Queen"){
                    eval += sameCol*900; //valeur de la reine est 900
                }
            }
        }
    }
    cout << eval << endl;
    cout << G.fen() << endl;
    return eval;
}

int alpha_beta(Grid& G, int depth, bool isMax){
    int eval = 0;
    if (depth == 0)
        return evaluation(G);
    else if (isMax){
        int M = INT_MIN;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (!G.isVoid(i,j) && G(i,j)->getIsWhite() == G.getWhiteIsPlaying()){
                    vector<Point> possibleMoves = G(i,j)->allowed_moves(G);
                    for (vector<Point>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it){
                        Chessman* startingFrame = G(i,j)->clone();
                        Chessman* arrivingFrame = G(it->getx(),it->gety())->clone();
                        // On teste si un pion arrive sur la case de promotion
                        // On ne teste que pour la reine pour l'instant
                        if (G(i,j)->getName() == "Pawn" && (*it).getx() == (!G(i,j)->getIsWhite())*7)
                            G.move((*it), G(i,j));
                        else
                            G.move((*it), G(i,j));
                        // On evalue la grille
                        eval = alpha_beta(G, depth-1, !isMax);
                        // Si l'evaluation est meilleure
                        if (eval > M)
                            M = eval;
                        G.unmove(startingFrame, arrivingFrame);
                    }
                }
            }
        }
        return M;
    }
    else{
        int M = INT_MAX;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (!G.isVoid(i,j) && G(i,j)->getIsWhite() == G.getWhiteIsPlaying()){
                    vector<Point> possibleMoves = G(i,j)->allowed_moves(G);
                    for (vector<Point>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it){
                        Chessman* startingFrame = G(i,j)->clone();
                        Chessman* arrivingFrame = G(it->getx(),it->gety())->clone();
                        // On teste si un pion arrive sur la case de promotion
                        // On ne teste que pour la reine pour l'instant
                        if (G(i,j)->getName() == "Pawn" && (*it).getx() == (!G(i,j)->getIsWhite())*7)
                            G.move(*it, G(i,j));
                        else
                            G.move(*it, G(i,j));
                        // On evalue la grille
                        eval = alpha_beta(G, depth-1, !isMax);
                        // Si l'evaluation est meilleure
                        if (eval < M)
                            M = eval;
                        G.unmove(startingFrame, arrivingFrame);
                    }
                }
            }
        }
        return M;
    }
}

int best_move(int depth, string fen){
    Grid G(fen);
    int M = INT_MIN;
    int move = 0;
    int eval;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (!G.isVoid(i,j) && G(i,j)->getIsWhite() == G.getWhiteIsPlaying()){
                vector<Point> possibleMoves = G(i,j)->allowed_moves(G);
                for (vector<Point>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it){
                    Chessman* startingFrame = G(i,j)->clone();
                    Chessman* arrivingFrame = G(it->getx(),it->gety())->clone();
                    // On teste si un pion arrive sur la case de promotion
                    // On ne teste que pour la reine pour l'instant
                    if (G(i,j)->getName() == "Pawn" && (*it).getx() == !(G(i,j)->getIsWhite())*7)
                        G.move(*it, G(i,j));
                    else
                        G.move(*it, G(i,j));
                    // On evalue la grille
                    eval = alpha_beta(G, depth-1, false);
                    // Si l'evaluation est meilleure
                    if (eval > M){
                        M = eval;
                        move = 1000*i+100*j+10*(*it).getx()+(*it).gety();
                    }
                    G.unmove(startingFrame, arrivingFrame);
                }
            }
        }
    }
    return move;
}
