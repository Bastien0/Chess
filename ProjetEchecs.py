
# -*- coding: utf-8 -*-
"""
Created on Mon Nov  7 15:55:31 2016

@author: anatole parre
"""

import sys
from PyQt4 import QtGui, QtCore

from Chess_Grid import Grid

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

class Frame(QtGui.QPushButton):
    #position, pièce ?
    def __init__(self, x, y):
        super(Frame,self).__init__()       
        self.__x = x
        self.__y = y
        #si la somme est paire, la bouton est blanc
        if (x+y)%2 == 0:
            self.setStyleSheet("background-color: white")
        else:
            self.setStyleSheet("background-color: grey")
        self.setFixedSize(80,80)
        
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
        

class Disp(QtGui.QWidget):
    def __init__(self):
        super(Disp,self).__init__()
        # tableau de boutons
        self.__chessboard= [[Frame(i,j) for j in range(8)] for i in range(8)]
        mainLayout = QtGui.QGridLayout()
        self.setLayout(mainLayout)
        
        #affichage en grille de ce tableau
        gridLayout=QtGui.QGridLayout()
        gridLayout.setSpacing(0)

        for i in range(8):
            for j in range(8):
                gridLayout.addWidget(self.__chessboard[i][j],i,j)
        mainLayout.addLayout(gridLayout,1,0)
        
        # creation de la grille theorique utilisee pour les deplacements 
        self.__grid = Grid()
        
        # remplissage des frames de l'echiquier
        self.evolve_chessboard() 
        
        # affichage des pieces
        self.choose_chessman(True)
        
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

    def choose_chessman(self, whiteIsPlaying,tabAccess=None):
        self.unallow_all_frame()
        #On remet l'affichage des cases accessibles au début.
        if tabAccess != None:        
            for (i,j) in tabAccess:
                if (i+j)%2 == 0:
                    self.__chessboard[i][j].setStyleSheet("background-color:\
                                                            white")
                else:
                    self.__chessboard[i][j].setStyleSheet("background-color:\
                                                                grey")
                                                                
        for (i, j) in self.__grid.list_chessman_col(whiteIsPlaying):
            self.__chessboard[i][j].setEnabled(True)
            self.__chessboard[i][j].clicked.connect(lambda : self.allow_moves(\
                                            whiteIsPlaying,tabAccess))
    
    def allow_moves(self, whiteIsPlaying,tabAccess=None):
        
        self.unallow_all_frame()
        fr = self.sender()
        chessman = self.__grid[(fr.x,fr.y)] 
        #On colore les cases accessibles (selon qu'elles sont noires ou blanches)
        tabAccess=chessman.allowed_moves(self.__grid)
        for (i,j) in tabAccess:
            if (i+j)%2 == 0:
                self.__chessboard[i][j].setStyleSheet("background-color: \
                                                    rgb(150, 200, 150,50);")
            else:
                self.__chessboard[i][j].setStyleSheet("background-color: \
                                                    rgb(50, 10, 50,100);")
        
        # si on reclique sur la case où on est,
        # on revient a l'étape de selection d'une piece a jouer
        self.__chessboard[fr.x][fr.y].setEnabled(True)
        
        self.__chessboard[fr.x][fr.y].clicked.connect(\
                      lambda : self.choose_chessman(whiteIsPlaying,tabAccess))

        # si on clique sur une case on effectue donc un coup
        moves = self.__grid[(fr.x,fr.y)].allowed_moves(self.__grid)
        for (i, j) in moves:
            self.__chessboard[i][j].setEnabled(True)
            self.__chessboard[i][j].clicked.connect(lambda : \
                                        self.play(whiteIsPlaying,fr,tabAccess))                                                             
        
    def play(self, whiteIsPlaying, chessmanFrame,tabAccess):
        # le joueur vient de cliquer sur la case où il veut aller        
        aim = self.sender()
        
        # on recupere la piece qui va bouger
        chessman = self.__grid[(chessmanFrame.x,chessmanFrame.y)] 
       
        #On réinitinalise les cases accessibles
        for (i,j) in tabAccess:
            if (i+j)%2 == 0:
                self.__chessboard[i][j].setStyleSheet("background-color:\
                                                        white")
            else:
                self.__chessboard[i][j].setStyleSheet("background-color: grey")        

        # on effectue le "vrai" deplacement dans self.__grid
        self.__chessboard[chessmanFrame.x][chessmanFrame.y].deleteChessMan()
        self.__grid.move((aim.x, aim.y), chessman)
        
        # on met a jour l'affichage
        self.evolve_chessboard()
        # c'est au joueur suivant de jouer
        self.choose_chessman(not whiteIsPlaying)
        
        
    def disp_taken_chessmen():
        pass
    
    
def disp():
    app = QtGui.QApplication(sys.argv)
    ex = Disp()
    sys.exit(app.exec_())
##class Display():
##    def __init__(self):
        
    #affichage + interactions
disp()
