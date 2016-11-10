# -*- coding: utf-8 -*-
"""
Created on Mon Nov  7 15:55:31 2016

@author: anatole parre
"""
import random
import functools
import copy
import sys
from PyQt4 import QtGui




class Frame(QtGui.QPushButton):
    #position, pièce ?
    def __init__(self,x,y):
        self.__x = x
        self.__y = y
        self.__pieceIn = False
        #si la somme est paire, la bouton est blanc
        if (x+y)%2 == 0:
            self.pushButton.setStyleSheet("background-color: white")
        else:
            self.pushButton.setStyleSheet("background-color: black")
        self.setFixedSize(100,100)
        
    
    def deleteChessMan(self):
        if self.__pieceIn == True :        
            self.__pieceIn = False
            if (self.__x+self.__y)%2 == 0:
                self.pushButton.setStyleSheet("background-color: white")
            else:
                self.pushButton.setStyleSheet("background-color: black")
            
    def addChessMan(self):
        
        


class Grid():
    #tableau de frames, quelle pièce est dessus, menacée?

class Display():
    #affichage + interactions