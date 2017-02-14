#include <iostream>
#include <fstream>
#include "Chess_Grid.h"
using namespace std;

int evaluation(Grid& G, bool color);
int best_move(int depth, string fen);


bool test_move_unmove(Grid& G){
    string fen_ini = G.fen();
    string posdep;
    cout << "case de depart svp en mode e5"<<endl;
    cin >> posdep;
    int dep[2];
    dep[0] = '8'- posdep[1];
    dep[1] = posdep[0]-'a';

    string posarr;
    cout <<" case d'arrivee svp en mode e5" << endl;
    cin >> posarr;
    int arr[2];
    arr[0] = '8' - posarr[1];
    arr[1] = posarr[0] -'a';
    cout << arr[0] << " " << arr[1] << endl;
    Point arriv(arr[0],arr[1]);

    cout << "construction du point ok" << endl ;
    Chessman* c1 = G(dep[0],dep[1])->clone();
    Chessman* c2 = G(arr[0],arr[1])->clone();
    G.move(arriv, G(dep[0],dep[1]), " ");

    cout << "deplacement effectue" << endl;

    cout << "ancien fen : " << fen_ini << endl;
    cout << "fen apres deplacement : "<< G.fen() << endl;
    cout << G.getScore() << endl;
    Point final(arr[0], arr[1]);
    Point Enpassant(0,0);

    G.unmove(c1, c2, final, Enpassant);
    cout << "annulation effectuee" << endl;
    cout << "fen apres annulation : " << G.fen() << endl;

    if (G.fen() == fen_ini){
        cout << "on annule bien le mouvement"<< endl;
    }
    return G.fen() == fen_ini;
}

int f(int dep){
    cout<<"feoifh"<<endl;
    string fen;
    std::fstream file("C:/Users/bri/Documents/ENPC/IMI/TDlog/Chess-master/data.txt", std::fstream::in);
    if(file)  // si l'ouverture a réussi
    {
         getline(file,fen);

         cout<<fen<<endl;
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
    std::cout << "test de jeu"<< std::endl;
    //while(true){

        string s = "r1bq2kr/pppp1ppp/2nb1n2/4p1B1/4P3/3P1N2/PPP1BPPP/RN1QK2R w KQ - 5 10";
        //string s = "r3k3/8/8/4pP2/8/8/8/4K3 b q - 0 0";
        //string s = "6k1/pnpqr2p/5rp1/3PN3/2P5/1P5P/P4PP1/R4RK1 b - - 0 47";
        /*cout << "fen svp" << endl;
        getline(cin,s);*/
        cout << "creation de G" << endl;
        Grid G(s);
        cout << "G.fen : " << G.fen() << endl;
        f(4);
        //while(test_move_unmove(G)){cout << G.getScore() << endl;}
        /*Point P(0,5);
        Chessman* d = G(0,4);
        Chessman* d1 = d->clone();
        Chessman* a = G(0,5);
        G.move(P, G(0,4));
        G.unmove(d1,a,P);
        cout << "G.fen : " << G.fen() << endl;*/
        //cout << G.isChessed(G(0,0), 7, 0) << endl;
        //cout << G.getScore() << endl;
        /*vector<Point> all = G.operator ()(6,6)->allowed_moves(G);
        for (vector<Point>::iterator it = all.begin(); it != all.end(); ++it){
            cout << it->getx() << " " << it-> gety() << endl;
        }*/
        //cout << best_move(5, s) << endl;
        /*Point p(2,4);
        Chessman* c1 = G(3,5)->clone();
        Chessman* c2 = G(2,4)->clone();
        G.move(p, G(3,5));
        cout << G.fen() << endl;
        G.unmove(c1, c2);
        cout << G.fen() << endl;*/


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
