# -*- coding: utf-8 -*-
"""
Created on Fri Nov 18 14:47:52 2016

@author: bastien
"""

from Chessmen import Rook, Knight, Bishop, Queen, King, Pawn

#Grille d'echecs
class Grid:
    #initialisation de la grilles
    def __init__(self):
        # pieces noires
        self.__grid = [[None for j in range(8)] for i in range(8)]
        self.__grid[0][0] = Rook(False, 0, 0)
        self.__grid[0][1] = Knight(False, 0, 1)
        self.__grid[0][2] = Bishop(False, 0, 2)
        self.__grid[0][3] = Queen(False, 0, 3)
        self.__grid[0][4] = King(False, 0, 4)
        self.__grid[0][5] = Bishop(False, 0, 5)
        self.__grid[0][6] = Knight(False, 0, 6)
        self.__grid[0][7] = Rook(False, 0, 7)
        # pions noirs
        for i in range(8):
            self.__grid[1][i] = Pawn(False, 1, i)
        
        # pieces blanches
        self.__grid[7][0] = Rook(True, 7, 0)
        self.__grid[7][1] = Knight(True, 7, 1)
        self.__grid[7][2] = Bishop(True, 7, 2)
        self.__grid[7][3] = Queen(True, 7, 3)
        self.__grid[7][4] = King(True, 7, 4)
        self.__grid[7][5] = Bishop(True, 7, 5)
        self.__grid[7][6] = Knight(True, 7, 6)
        self.__grid[7][7] = Rook(True, 7, 7)
        # pions blancs
        for i in range(8):
            self.__grid[6][i] = Pawn(True, 6, i)
        
        # compteurs
        # compteur du nombre total de coups
        self.__countMoves = 0
        # compteur du nombre de coups depuis qu'un pion a bouge ou la derniere
        # prise, "demi-coups"
        self.__countHalfMoves = 0
        
        self.__whiteIsPlaying = True

    
    # accesseur d'une piece de coordonnees (i,j)
    def __getitem__(self, coord):
        return self.__grid[coord[0]][coord[1]]
    
    # set d'une piece aux coordonnes (i,j)
    # n'effectue pas la suppression d'une piece prise, 
    def __setitem__(self, coord, Chessman):
        Chessman.x = coord[0]
        Chessman.y = coord[1]
        self.__grid[coord[0]][coord[1]] = Chessman
    
    # vrai deplacement
    def move(self, coord, Chessman, promotion = ""):
        """effectue le vrai déplacement ; promotion attend la pièce choisie (dame, tour..)"""
        # on incremente les compteurs de 1 
        self.__countMoves += 1
        self.__countHalfMoves += 1
        self.__whiteIsPlaying = not self.__whiteIsPlaying  
        
    
        oldCoordChessman = (Chessman.x, Chessman.y)
        # on regarde si la piece a un argument de mouvement
        if Chessman.name in ["King", "Rook"]:
            Chessman._hasMoved = True

        if Chessman.name == "Pawn" :
            self.__countHalfMoves = 0 
            
            # cas du coup double du pion
            if (Chessman.x-coord[0]) not in [-1,1]:
                Chessman.double_done = True
                
            # cas de la prise en passant
            if Chessman.y != coord[1] and\
               self.isVoid(coord[0], coord[1]):
                if Chessman.isWhite:
                    self.setNone(coord[0]+1, coord[1])
                else:
                    self.setNone(coord[0]-1, coord[1])
            # promotion 
            if promotion != "":
                if promotion == "Queen":
                    Chessman = Queen(Chessman.isWhite, Chessman.x, Chessman.y)
                if promotion == "Bishop":
                    Chessman = Bishop(Chessman.isWhite, Chessman.x, Chessman.y)
                if promotion == "Knight":
                    Chessman = Knight(Chessman.isWhite, Chessman.x, Chessman.y)
                if promotion == "Rook":
                    Chessman = Rook(Chessman.isWhite, Chessman.x, Chessman.y)
              

        # s'il y a roque
        #retourne les coordonnées de la tour roquée
        elif Chessman.name == "King" and abs(Chessman.y-coord[1]) == 2:
            self.__countHalfMoves = 0
            # si on va vers la gauche
            if Chessman.y > coord[1]:
                self[(Chessman.x, 3)] = self[(Chessman.x, 0)]
                self.setNone(Chessman.x, 0)
            # vers la droite
            else:
                self[(Chessman.x, 5)] = self[(Chessman.x, 7)]
                self.setNone(Chessman.x, 7)        
        
        # on passe a false l'avancee de double de la couleur opposee
        for frame in self.list_chessman_col(not Chessman.isWhite):
            if self[frame].name == "Pawn":
                self[frame].double_done = False
        
        self.setNone(oldCoordChessman[0],oldCoordChessman[1])
        # s'il y a prise, on doit ramener countHalfMoves a 0
        if self[coord] != None :
            self.__countHalfMoves = 0
        self[coord] = Chessman
           
    #vide une case
    def setNone(self,x,y):
        self.__grid[x][y] = None
      
      
    #position du roi
    def king_position(self,isWhite):
        for i in range(8):
            for j in range(8):
                if not self.isVoid(i,j) and \
                   self.__grid[i][j].isWhite==isWhite and \
                   self.__grid[i][j].name == "King":
                        return (i,j)
    
    # La case est-elle vide ?
    def isVoid(self, x, y):
        return (self.__grid[x][y] == None)
    
    #ensemble des pièces de couleur
    def list_chessman_col(self, colorIsWhite):
        l = []
        for i in range(8):
            for j in range(8):
                if self.__grid[i][j] != None:
                    if self.__grid[i][j].isWhite == colorIsWhite:
                        l.append((i, j))
        return l
        
    # s'il y a quelqu'un dans cette case, est-ce qu'il est de la même couleur?
    def sameColor(self, Chessman, x, y):
        """S'il y a quelqu'un dans cette case, est-il de la même couleur ?"""
        assert(self[(x,y)] != None)
        return self[(x,y)].isWhite == Chessman.isWhite
    
    
    # quand on deplace une piece sur la grille, se met on en echec?
    # pour savoir cela, on effectue le deplacement et on regarde.
    # Apres avoir regarde cela, il faut remettre les pieces a leur place :
    # il faut reparer les degats en remettant a leur place les elements qui ont
    # ete deplaces.
    def isChessed(self, Chessman, x, y):
        """quand on déplace une pièce, se met-on en echecs ?"""
        # memorisation de la piece prise, si le deplacement en prend une
        someoneTaken = False
        takenChessman = None
        if not self.isVoid(x, y):
            someoneTaken = True
            takenChessman = self[(x, y)]
        #memoristaion de l'ancienne position 
        coordIniChess =(Chessman.x, Chessman.y) 
        self[(x,y)] = Chessman
        Chessman.x = x
        Chessman.y = y
        self.setNone(coordIniChess[0], coordIniChess[1])
        for i in range(8):
            for j in range(8):
                # si on tombe sur une piece de couleur differente,
                # on verifie qu'elle ne met pas le roi en echec
                # ie si la position du roi n'est pas dans la liste des
                # positions accessibles,
                # on repare ce qu'on a bouge dans la grille et on renvoie true
                if not self.isVoid(i, j)\
                    and Chessman.isWhite != self[(i,j)].isWhite\
                    and self.king_position(Chessman.isWhite) \
                                        in self[(i,j)].moves(self, True):
                        # On remet l'echiquier en place
                        if someoneTaken:
                            self[(x,y)] = takenChessman
                        else:
                            self.setNone(x,y)
                        Chessman.x = coordIniChess[0]
                        Chessman.y = coordIniChess[1]
                        self[coordIniChess] = Chessman
                        return True
        if someoneTaken:
            self[(x,y)] = takenChessman
        else:
            self.setNone(x,y)
        Chessman.x = coordIniChess[0]
        Chessman.y = coordIniChess[1]
        self[coordIniChess] = Chessman
        return False
        
    
    def grid_to_ascii(self):
        """renvoie la fen de la grille"""
        fen = ""
        
        # etat de l'echiquier
        for x in range(8):
            y = 0
            while (y < 8) :
                if self[(x,y)] == None :
                    compt = 1
                    y += 1
                    while (y < 8 and self[(x,y)] == None):
                        compt += 1
                        y += 1
                    fen += str(compt)
                else :
                    if self[(x,y)].name == "Knight" :
                        letter = 'N'
                    else :
                        letter = self[(x,y)].name[0]
                    # les pieces noires sont ecrites en minuscules
                    if not self[(x,y)].isWhite :
                        letter = letter.lower()
                    fen += letter
                    y += 1
            if x < 7 :
                fen+="/"
        fen += " "
        # couleur du joueur dont c'est le tour
        if self.__whiteIsPlaying :
            fen += "w"
        else :
            fen += "b"
        fen+=" "
        
        # roques possibles
        rockPossible = False
        # pour les blancs
        if self[(7,4)] != None and self[(7,4)].name == "King" \
            and not (self[(7,4)].hasMoved) :
                if self[(7,7)] != None and self[(7,7)].name == "Rook" \
                    and not (self[(7,7)].hasMoved):
                        fen += "K"
                        rockPossible = True
                if self[(7,0)] != None and self[(7,0)].name == "Rook" \
                    and not (self[(7,0)].hasMoved):
                        fen += "Q" 
                        rockPossible = True
        
        # pour les noirs
        if self[(0,4)] != None and self[(0,4)].name == "King" \
            and not (self[(0,4)].hasMoved) :
                if self[(0,7)] != None and self[(0,7)].name == "Rook" \
                    and not (self[(0,7)].hasMoved):
                        fen += "k"
                        rockPossible = True
                if self[(0,0)] != None and self[(0,0)].name == "Rook" \
                    and not (self[(0,0)].hasMoved):
                        fen += "q"
                        rockPossible = True

        if not rockPossible :
            fen += "-"
        fen +=" "
        
        # position de prise en passant
        
        # si c'est au blanc de jouer, les noirs ont pu faire un coup double
        # sur la ligne 3
        # sinon, les blancs ont pu faire un coup double sur la ligne 4
        passing = False
        lign = 4 - self.__whiteIsPlaying
        for y in range(8):
            if self[(lign,y)] != None and self[(lign,y)].name == "Pawn" \
                and self[(lign,y)].double_done:
                    passing = True
                    fen += chr(y + ord('a')) #a pour 0, b pour 1, c pour 2 etc.
                    fen += str(lign -  2*self.__whiteIsPlaying + 1)
        if not passing :
            fen += "-"
        fen += " "
        
        # nombres de demi coups
        fen += str(self.__countHalfMoves) 
        fen += " "
        # nombre de coups
        fen += str(self.__countMoves)
        return fen
