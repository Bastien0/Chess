# -*- coding: utf-8 -*-
"""
Created on Mon Nov  7 15:55:31 2016

@author: anatole parre
"""
import random
import functools
import copy
import sys
from PyQt4 import QtGui, QtCore

chess = "Chess_rdt60.png"



class Frame(QtGui.QPushButton):
    #position, pièce ?
    def __init__(self,x,y):
        super(Frame,self).__init__()       
        self.__x = x
        self.__y = y
        self.__pieceIn = False
        #si la somme est paire, la bouton est blanc
        if (x+y)%2 == 0:
            self.setStyleSheet("background-color: white")
        else:
            self.setStyleSheet("background-color: grey")
        self.setFixedSize(100,100)
        
    
    def deleteChessMan(self):
        if self.__pieceIn:        
            self.setIcon(QtGui.Icon())
            self.__pieceIn = False
            
    def addChessMan(self, image):      
        self.__pieceIn = True
        self.setIcon(QtGui.QIcon(image))
        self.setIconSize(QtCore.QSize(10,10))
            
        """
            if (self.__x+self.__y)%2 == 0:
                self.setIcon(icon2)
            else:
                self.setStyleSheet("background-image:url( "+ image + ")")
        """

class Grid(QtGui.QWidget):
    def __init__(self):
        super(Grid,self).__init__()        
        self.__grid = [[Frame(i,j) for j in range(8)] for i in range(8)]
        mainLayout=QtGui.QGridLayout()
        self.setLayout(mainLayout)
        
        
        #grille
        gridLayout=QtGui.QGridLayout()
        for i in range(8):
            for j in range(8):
                gridLayout.addWidget(self.__grid[i][j],i,j)
        mainLayout.addLayout(gridLayout,1,0)
        self.__grid[1][0].addChessMan(chess)
        self.setWindowTitle('XXXX Chess Master Game XXXX')
        self.showMaximized()
    #tableau de frames, quelle pièce est dessus, menacée?

def disp():
    app = QtGui.QApplication(sys.argv)
    ex = Grid()
    sys.exit(app.exec_())
##class Display():
##    def __init__(self):
        
    #affichage + interactions