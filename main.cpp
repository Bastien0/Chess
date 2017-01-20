#include <iostream>
#include "Chess_Grid.h"
using namespace std;

int main(){
    std::cout << "test de jeu"<< std::endl;
    //while(true){

        // rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e6 64 8
        string s = "4k3/8/8/4pP2/8/8/8/4K3 w - e3 0 0";
        /*cout << "fen svp" << endl;
        getline(cin,s);*/
        cout << "creation de G" << endl;
        Grid G(s);
        cout << "G.fen : " << G.fen() << endl;
        cout << G.operator ()(3,4)->isDouble_done() << endl;
        //cout << G.isChessed(G(4,5),4,6) << endl;
        vector<Point> move = G.operator ()(3,5)->moves(G);
        for (int i = 0; i < move.size(); i++)
            cout << move[i].getx() << " " << move[i].gety() << endl;

        /*
        string posdep;
        cout << "case de depart svp en mode e5"<<endl;
        cin >> posdep;
        int dep[2];
        dep[0] = posdep[0]-'a';
        dep[1] = int(posdep[1]);

        string posarr;
        cout <<" case d'arrivee svp en mode e5" << endl;
        cin >> posarr;
        int arr[2];
        arr[0] = posarr[0]-'a';
        arr[1] = int(posarr[1]);
        cout<< " pas de bug avant move" << endl;
        G.move(arr, G(dep[0],dep[1]));
        cout<< "pas de bug avant G.fen()" << endl;
        s = G.fen();
        cout<< "bug pendant G.fen()" << endl;
        cout << "et voici la fen de la nouvelle config" << s << endl;
        */
    //}

    return 0;
}