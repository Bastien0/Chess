# -*- coding: utf-8 -*-
"""
Created on Fri Nov 18 14:47:52 2016

@author: bastien
"""

from Chessmen import Rook, Knight, Bishop, Queen, King, Pawn

class Grid:
    def __init__(self):
        self.__whiteLostChessmen = []
        self.__blackLostChessmen = []
        self.__grid = [[None for j in range(8)] for i in range(8)]
        # pieces blanches
        self.__whiteRooks = [Rook(True, 0, 0), Rook(True, 0, 7)]        
        self.__whiteKnights = [Knight(True, 0, 1), Knight(True, 0, 6)]
        self.__whiteBishops = [Bishop(True, 0, 2), Bishop(True, 0, 5)]
        self.__whiteQueen = Queen(True, 0, 3)
        self.__whiteKing = King(True, 0, 4)
        # pions blancs
        self.__whitePawns = []
        for i in range(8):
            self.__whitePawns.append(Pawn(True, 1, i))
        
        
        self.__grid[0][0] = self.__whiteRooks[0]
        self.__grid[0][1] = self.__whiteKnights[0]
        self.__grid[0][2] = self.__whiteBishops[0]
        self.__grid[0][3] = self.__whiteQueen
        self.__grid[0][4] = self.__whiteKing
        self.__grid[0][5] = self.__whiteBishops[1]
        self.__grid[0][6] = self.__whiteKnights[1]
        self.__grid[0][7] = self.__whiteRooks[1]
        # pions blancs
        for i in range(8):
            self.__grid[1][i] = self.__whitePawns[i]
        
              
        # pieces noires
        self.__blackRooks = [Rook(False, 7, 0), Rook(False, 7, 7)]        
        self.__blackKnights = [Knight(False, 7, 1), Knight(False, 7, 6)]
        self.__blackBishops = [Bishop(False, 7, 2), Bishop(False, 7, 5)]
        self.__blackQueen = Queen(False, 7, 3)
        self.__blackKing = King(False, 7, 4)
        # pions blancs
        self.__blackPawns = []
        for i in range(8):
            self.__blackPawns.append(Pawn(False, 6, i))
        
        self.__grid[7][0] = self.__blackRooks[0]
        self.__grid[7][1] = self.__blackKnights[0]
        self.__grid[7][2] = self.__blackBishops[0]
        self.__grid[7][3] = self.__blackQueen
        self.__grid[7][4] = self.__blackKing
        self.__grid[7][5] = self.__blackBishops[1]
        self.__grid[7][6] = self.__blackKnights[1]
        self.__grid[7][7] = self.__blackRooks[1]
        # pions blancs
        for i in range(8):
            self.__grid[6][i] = self.__blackPawns[i]

    
    # accesseur d'une piece de coordonnees (i,j)
    def __getattr__(self, coord):
        return self.__grid[coord[0]][coord[1]]
    
    # set d'une piece aux coordonnes (i,j)
    # n'effectue pas la suppression d'une piece prise, 
    def __set__(self, coord, Chessman):
        Chessman.x = coord[0]
        Chessman.y = coord[1]
        self.__grid[coord[0]][coord[1]] = Chessman
    
    # vrai deplacement
    def move(self, coord, Chessman):
        oldCoordChessman = (Chessman.x, Chessman.y)
        # si une piece est prise, on l'ajoute a la liste des pieces prises de
        # sa couleur
        if not self.isVoid(coord[0], coord[1]):
            if self[coord].isWhite:
                self.__whiteLostChessmen.append(self[coord])
            else:
                self.__blackLostChessmen.append(self[coord])
        
        # s'il y a roque
        # a faire
        
        # s'il y a prise d'un pion en passant
        # a faire
                
        self[coord] = Chessman
        self[oldCoordChessman] = None
        
        
      
    
    # La case est-elle vide ?
    def isVoid(self, x, y):
        return (self.__grid[x][y] == None)
    
    # Recherche tous les cavaliers blancs si colorIsWhite==True, noir sinon
    # utile pour le roque
    def listRooks(self, colorIsWhite):
        if colorIsWhite :
            return self.__whiteRooks
        else :
            return self.__blackRooks
    
    def list_chessman_col(self, colorIsWhite):
        l = []
        for i in range(8):
            for j in range(8):
                if type(self.__grid[i][j]):
                    if self.__grid[i][j].isWhite == colorIsWhite:
                        l.append((i, j))
        return l
        
    # s'il y a quelqu'un dans cette case, est-ce qu'il est de la même couleur?
    def sameColor(self, Chessman, x, y):
        assert(self[(x,y)] != None)
        return self[(x,y)].isWhite == Chessman.isWhite
    
    
    # quand on deplace une piece sur la grille, se met on en echec?
    # pour savoir cela, on effectue le deplacement et on regarde.
    # Apres avoir regarde cela, il faut remettre les pieces a leur place :
    # il faut reparer les degats en remettant a leur place les elements qui ont
    # ete deplaces.
    def isChessed(self, Chessman, x, y):
        # memorisation de la piece prise, si le deplacement en prend une       
        if not self.isVoid(x, y):
            someoneTaken = True
            takenChessman = self[(x, y)]
        else:
            someoneTaken = False
        #memoristaion de l'ancienne position
        coordIniChess =(Chessman.x, Chessman.y) 
        self[(x,y)] = Chessman
        self[(Chessman.x, Chessman.y)] = None
        for i in range(8):
            for j in range(8):
                # si on tombe sur une piece de couleur differente,
                # on verifie qu'elle ne met pas le roi en echec
                # ie si la position du roi n'est pas dans la liste des
                # positions accessibles,
                # on repare ce qu'on a bouge dans la grille et on renvoie true
                if not self[(i,j)].isVoid(i, j)\
                    and Chessman.isWhite != self[(i,j)].isWhite\
                    and self.king_position(Chessman. isWhite) \
                                        in self[(i,j)].moves(self):
                        # On remet l'echiquier en place
                        if someoneTaken:
                            self[(x,y)] = takenChessman
                        else:
                            self[(x,y)] = None
                        self[coordIniChess] = Chessman
                        return True
        if someoneTaken:
            self[(x,y)] = takenChessman
        else:
            self[(x,y)] = None
        self[coordIniChess] = Chessman       
        return False
        