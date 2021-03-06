
# -*- coding: utf-8 -*-
"""
Created on Mon Nov  7 15:55:31 2016

@author: anatole parre
"""
from PIL import Image
import sys
from PyQt4 import QtGui, QtCore
import os
from Chess_Grid import Grid

import sip

#dictionnaire des pièces

dicWhitePict = {
    "Rook" : "white_rook.png",
    "Knight" : "white_knight.png",
    "Bishop" : "white_bishop.png",
    "Queen" : "white_queen.png" ,
    "King" : "white_king.png",
    "Pawn" : "white_pawn.png"
}

dicBlackPict = {
    "Rook" : "black_rook.png",
    "Knight" : "black_knight.png",
    "Bishop" : "black_bishop.png",
    "Queen" : "black_queen.png" ,
    "King" : "black_king.png",
    "Pawn" : "black_pawn.png"
}                

#classe des cases
class Frame(QtGui.QPushButton):
    #initialisation
    def __init__(self, x, y):
        super(Frame,self).__init__()       
        self.__x = x
        self.__y = y
        #si la somme est paire, la bouton est blanc
        if (x+y)%2 == 0:
            self.setStyleSheet("background-color: rgb(225, 206, 154);")
        else: #sinon, il est noir
            self.setStyleSheet("background-color: rgb(139, 69, 19);")
        self.setFixedSize(80,80)
        
    #accesseurs
    @property
    def x(self):
        return self.__x
        
    @property
    def y(self):
        return self.__y    
    
    def deleteChessMan(self):
        self.setIcon(QtGui.QIcon())

            
    def addChessMan(self, name, isWhite):      
        icon = QtGui.QIcon()
        if isWhite:
            image = dicWhitePict[name]
        else :
            image = dicBlackPict[name]
        icon.addPixmap(QtGui.QPixmap(image),QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setIcon(icon)
        self.setIconSize(QtCore.QSize(100, 100))
        
#classe affichage
class Disp(QtGui.QWidget):
    def __init__(self):
        super(Disp,self).__init__()
        # tableau de boutons
        self.__chessboard= [[Frame(i,j) for j in range(8)] for i in range(8)]
        self.mainLayout = QtGui.QGridLayout()
        self.setLayout(self.mainLayout)
        
        #affichage en grille de ce tableau
        gridLayout=QtGui.QGridLayout()
        gridLayout.setSpacing(0)

        for i in range(8):
            for j in range(8):
                gridLayout.addWidget(self.__chessboard[i][j],i,j)
        self.mainLayout.addLayout(gridLayout,1,0)
        
        # creation de la grille theorique utilisee pour les deplacements 
        self.__grid = Grid()
        
        # remplissage des frames de l'echiquier
        self.evolve_chessboard() 
        
        # affichage des pieces
        computerPlays = False
        self.choose_chessman(True,computerPlays)
        
        self.setWindowTitle('XXXX Chess Master Game XXXX')
        self.showMaximized()
    
    # mise a jour de l'affichage des cases de l'echiquier en fonction du
    # contenu de self.__grid
    def evolve_chessboard(self):
        #if self.__grid[(1,1)] == None:
        #    return True
        for i in range(8):
            for j in range(8):
                if (self.__grid[(i,j)] != None):
                    self.__chessboard[i][j].addChessMan( \
                                  self.__grid[(i,j)].name,\
                                  self.__grid[(i,j)].isWhite)
                else :
                    self.__chessboard[i][j].deleteChessMan()
        #force PyQt à mettre à jour l'affichage même si on est dans un bouton
        self.repaint()
                    
                        
    
    # fonction qui desalloue toutes les frames, on ne peut donc pas
    # cliquer dessus
    def unallow_all_frame(self):
        for i in range(8):
            for b in self.__chessboard[i] :
                try:
                    b.clicked.disconnect()
                except:
                    pass
                b.setEnabled(False)
        
    #quelle pièce va-t'on jouer ?
    def choose_chessman(self, whiteIsPlaying,computerPlays,tabAccess=None):
        self.unallow_all_frame()        
        if computerPlays:
            #C'est à l'ordinateur de jouer
            with open ("data.txt", "w") as fichier:
                fichier.write(self.__grid.grid_to_ascii())
            os.system(r'"C:/Users/bri/Documents/ENPC/IMI/TDlog/Chess-master-build/Echecs.exe"')
            with open("data.txt", "r") as fichier2:
                contenu = fichier2.read()
            fichier.close()
            promotion = None
            (initx,inity,arrivx,arrivy)=(int(contenu[0])-1,int(contenu[1])-1, \
                    int(contenu[2])-1,int(contenu[3])-1)
            if (self.__grid[(initx,inity)].name == "Pawn"):
                if (arrivx == 0):
                    promotion = "Queen"
                if (arrivx == 7):
                    promotion = "Queen"
            
            #s'il n'y a pas de promotion
            if promotion == None :
                self.__grid.move((arrivx,arrivy), self.__grid[(initx,inity)])
            else :
                self.__grid.move((arrivx, arrivy), self.__grid[(initx,inity)], \
                                                promotion)
            # on met a jour l'affichage                                                
            self.__chessboard[initx][inity].deleteChessMan()
            self.evolve_chessboard()
            (i,j)=self.__grid.king_position(whiteIsPlaying)
            #50 coups
            if self.__grid.halfMove() == 50:
                print("pat")
                return (0)
            if (self.chessMat(whiteIsPlaying)):
                (x,y)=self.__grid.king_position(1-whiteIsPlaying)
                if (self.__grid.isChessed(self.__grid[self.__grid.\
                    king_position(1-whiteIsPlaying)],x,y)):            
                    print ("Echec et mat !")
                    return (0)
                else:
                    print("pat")
                    return (0)
            #on passe le tour à l'humain
            self.choose_chessman(not whiteIsPlaying,not computerPlays)
        #Tour de l'humain
        else:
            #On remet l'affichage des cases accessibles au début.
            if tabAccess != None:        
                for (i,j) in tabAccess:
                    if (i+j)%2 == 0:
                        self.__chessboard[i][j].setStyleSheet("background-color:\
                                                            rgb(225, 206, 154);")
                    else:
                        self.__chessboard[i][j].setStyleSheet("background-color:\
                                                            rgb(139, 69, 19);")
            #on connecte les boutons
            for (i, j) in self.__grid.list_chessman_col(whiteIsPlaying):
                self.__chessboard[i][j].setEnabled(True)
                self.__chessboard[i][j].clicked.connect(lambda : self.allow_moves(\
                                whiteIsPlaying,computerPlays,tabAccess))
    
    #quelles cases sont accessibles ?
    def allow_moves(self, whiteIsPlaying,computerPlays,tabAccess=None):
        #on désalloue les boutons
        self.unallow_all_frame()
        fr = self.sender()
        chessman = self.__grid[(fr.x,fr.y)] 
        #On colore les cases accessibles (selon qu'elles sont noires ou blanches)
        tabAccess=chessman.allowed_moves(self.__grid)
        for (i,j) in tabAccess:
            if (i+j)%2 == 0:
                self.__chessboard[i][j].setStyleSheet("background-color: \
                                                    rgb(119, 181, 254,90);")
            else:
                self.__chessboard[i][j].setStyleSheet("background-color: \
                                                    rgb(29, 72, 81,90);")
        
        # si on reclique sur la case où on est,
        # on revient a l'étape de selection d'une piece a jouer
        self.__chessboard[fr.x][fr.y].setEnabled(True)
        
        self.__chessboard[fr.x][fr.y].clicked.connect(\
            lambda : self.choose_chessman(whiteIsPlaying,computerPlays,tabAccess))

        # si on clique sur une case on effectue donc un coup
        moves = self.__grid[(fr.x,fr.y)].allowed_moves(self.__grid)
        for (i, j) in moves:
            # s'il y a une promotion
            if i == 0 and chessman.name == "Pawn":
                self.__chessboard[i][j].setEnabled(True)
                self.__chessboard[i][j].clicked.connect(lambda : \
                    self.promo(whiteIsPlaying,fr,computerPlays,tabAccess))
            elif i == 7 and chessman.name == "Pawn":
                self.__chessboard[i][j].setEnabled(True)
                self.__chessboard[i][j].clicked.connect(lambda : \
                    self.promo(whiteIsPlaying,fr,computerPlays,tabAccess))
            else :
                self.__chessboard[i][j].setEnabled(True)
                self.__chessboard[i][j].clicked.connect(lambda : \
                    self.play(whiteIsPlaying,fr,computerPlays,tabAccess))                                                             
    
    #Dans le cas d'une promotion, on crée une fenêtre qui permet de choisir 
    #la pièce à ajouter.
    def promo(self, whiteIsPlaying, chessmanFrame, computerPlays, tabAccess):
        self.choices = QtGui.QVBoxLayout()
        
        aim = self.sender()
        
        self.Knight = Frame(aim.x, aim.y)
        self.Knight.addChessMan("Knight", whiteIsPlaying)
        self.Knight.clicked.connect(lambda : \
            self.play(whiteIsPlaying, chessmanFrame, computerPlays,tabAccess,\
                                            "Knight"))
        
        self.Queen = Frame(aim.x, aim.y)
        self.Queen.addChessMan("Queen", whiteIsPlaying)
        self.Queen.clicked.connect(lambda : \
                self.play(whiteIsPlaying, chessmanFrame, computerPlays,tabAccess,\
                                                "Queen"))
        self.Rook = Frame(aim.x, aim.y)
        self.Rook.addChessMan("Rook", whiteIsPlaying)
        self.Rook.clicked.connect(lambda : \
                self.play(whiteIsPlaying, chessmanFrame, computerPlays,tabAccess,\
                                                      "Rook"))
        self.Bishop = Frame(aim.x, aim.y)
        self.Bishop.addChessMan("Bishop", whiteIsPlaying)
        self.Bishop.clicked.connect(lambda : \
                self.play(whiteIsPlaying, chessmanFrame,computerPlays, tabAccess, \
                                            "Bishop"))
        
        self.choices.addWidget(self.Knight)
        self.choices.addWidget(self.Queen)
        self.choices.addWidget(self.Rook)
        self.choices.addWidget(self.Bishop)
        
        
        self.mainLayout.addLayout(self.choices,1,2)
        # on doit supprimmer choices après le choix de la promotion.
    
    #fonction de jeu
    def play(self, whiteIsPlaying, chessmanFrame,computerPlays,tabAccess,\
                                                promotion = None):
        # le joueur vient de cliquer sur la case où il veut aller        
        aim = self.sender()
        
        # on recupere la piece qui va bouger
        chessman = self.__grid[(chessmanFrame.x,chessmanFrame.y)] 
       
        #On réinitinalise les cases accessibles
        for (i,j) in tabAccess:
            if (i+j)%2 == 0:
                self.__chessboard[i][j].setStyleSheet("background-color:\
                                                        rgb(225, 206, 154);")
            else:
                self.__chessboard[i][j].setStyleSheet("background-color: \
                                                rgb(139, 69, 19);")        

        # on effectue le "vrai" deplacement dans self.__grid
        self.__chessboard[chessmanFrame.x][chessmanFrame.y].deleteChessMan()
        if promotion == None :
            self.__grid.move((aim.x, aim.y), chessman)
        else :
            aim.clicked.disconnect()
            deleteLayout(self.choices)
            self.__grid.move((aim.x, aim.y), chessman, promotion)
        
        # on met a jour l'affichage
        self.evolve_chessboard()
        (i,j)=self.__grid.king_position(whiteIsPlaying)
        #50 coups
        if self.__grid.halfMove() == 50:
                print("pat")
                return (0)
        if (self.chessMat(whiteIsPlaying)==True):
            (x,y)=self.__grid.king_position(1-whiteIsPlaying)
            if (self.__grid.isChessed(self.__grid[(self.__grid.\
                    king_position(1-whiteIsPlaying))],x,y)):            
                print ("Echec et mat !")
                return (0)
            else:
                print("pat")
                return (0)
        # c'est au joueur suivant de jouer (couleur différente et autre joueur)
        if not computerPlays:
            self.choose_chessman(not whiteIsPlaying,not computerPlays)
            
        
    #fonction qui gère l'echec et mat    
    def chessMat(self,whiteIsPlaying):
        """renvoie false s'il n'y pas echec et mat, et true sinon"""
        for i in range(8):
            for j in range(8):
                Chessman = self.__grid[(i,j)]
                if not self.__grid.isVoid(i,j):
                    if (Chessman.isWhite != whiteIsPlaying):
                        
                        if (len(Chessman.allowed_moves(self.__grid))!=0):
                            
                            
                            return (False)
        return (True)
        
    def disp_taken_chessmen():
        pass
    
#permet de supprimer un Layout    
def deleteLayout(layout):
    if layout is not None:
        while layout.count():
            item = layout.takeAt(0)
            widget = item.widget()
            if widget is not None:
                widget.deleteLater()
            else:
                deleteLayout(item.layout())
        sip.delete(layout)


#Affichage
def disp():
    app = QtGui.QApplication(sys.argv)
    ex = Disp()
    sys.exit(app.exec_())
        
#fonction d'affichage à executer
disp()

#os.system('"C:/Users/anatole parre/Desktop/ENPC/2A/TD Log/Chess-build/Imp.exe"')

#os.startfile(r'C:/Users/anatole parre/Desktop/ENPC/2A/TD Log/Proj/Chess/Chess.exe')