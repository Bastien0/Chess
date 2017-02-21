#include <iostream>
#include <fstream>
#include "Chess_Grid.h"
using namespace std;

int best_move(int depth, string fen);

int f(int dep){
    string fen;
    std::fstream file("C:/Users/bri/Documents/ENPC/IMI/TDlog/Chess-master/data.txt", std::fstream::in);
    if(file)  // si l'ouverture a réussi
    {
         getline(file,fen);

         file.close();  // on ferme le fichier
    }
    else  // sinon
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    std::fstream file2("C:/Users/bri/Documents/ENPC/IMI/TDlog/Chess-master/data.txt",std::fstream::in | std::fstream::out | std::fstream::trunc);
    file2 << best_move(dep, fen) << endl;
    file2.close();  // on ferme le fichier
    return 0;
}

int main(){
    //string s = "r1bq1rk1/p3bppp/p1np1n2/2p3Q1/8/8/1PP1PPPP/RNB1KBNR w KQ - 0 18";
    //cout << "creation de G" << endl;
    //Grid G(s);
    //cout << "G.fen : " << G.fen() << endl;
    f(4);
    return 0;
}
