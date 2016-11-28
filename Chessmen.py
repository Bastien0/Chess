# -*- coding: utf-8 -*-
"""
Created on Wed Nov 16 16:16:08 2016

@author: bastien
"""

class Chessman():
    def __init__(self, isWhite, x, y, name):
        self.__isWhite = isWhite
        self.__x = x
        self.__y = y
        self.__name = name
    
    @property
    def x(self):
        return self.__x
    @property
    def y(self):
        return self.__y
    @property
    def isWhite(self):
        return self.__isWhite
    @property
    def name(self):
        return self.__name
        
    @x.setter
    def x(self, i):
        self.__x = i
    @y.setter
    def y(self, j):
        self.__y = j
    @name.setter
    def name(self, word):
        self.__name = word
        
    # On a besoin de ce deplacer tout droit dans 8 directions pour
    # le fou et la tour.
    # Cette fonction permet d'eviter de copier 8 boucles tres similaires
    # a et b valent chacun -1, 0 ou 1
    def move_straight(self, grid, a, b):
        tabAccess = []
        x = self.__x + a
        y = self.__y + b 
        while (0 <= x < 8) and (0 <= y < 8) and grid.isVoid(x, y):
            tabAccess.append((x, y))
            x += a
            y += b
        if (0 <= x < 8) and (0 <= y < 8) and \
            not grid.sameColor(self, x, y):
                tabAccess.append((x, y))
        return tabAccess
        
    
    # cette fonction renvoie une liste de positions accessibles parmi celles
    # proposées (ne marche pas pour le fou, la dame, et la tour)
    def testedTuples(self, grid, tab):
        tabAccess = []
        for move in tab:
            x = move[0]
            y = move[1]
            if (0 <= x < 8) and (0 <= y < 8) :
                if grid.isVoid(x,y):
                    tabAccess.append((x,y))
                elif not grid.sameColor(self, x, y) :
                    tabAccess.append((x,y))
        return tabAccess

        
class Rook(Chessman):
    def __init__(self, isWhite, x,y):
        super(Rook, self).__init__(isWhite, x, y, "Rook")
        self.__hasMoved = False # utile pour le roque
    
        
    # Fonction de déplacement
    def moves(self, grid, test = False):
        return self.move_straight(grid, -1, 0) + \
               self.move_straight(grid, 1, 0)  + \
               self.move_straight(grid, 0, -1) + \
               self.move_straight(grid, 0, 1)
    
    
        
    @property
    def hasMoved(self):
        return self.__hasMoved
        
    def allowed_moves(self, grid):
        tabAccess = []
        for coord in self.moves(grid):
            if not grid.isChessed(self,coord[0],coord[1]):
                tabAccess.append(coord)
        return tabAccess    


class Bishop(Chessman):
    def __init__(self, isWhite, x,y):
        super(Bishop,self).__init__(isWhite, x, y, "Bishop")
    
    # Fonction de déplacement
    def moves(self, grid, test = False):
        return self.move_straight(grid, -1, -1) + \
               self.move_straight(grid, -1, 1)  + \
               self.move_straight(grid, 1, -1)  + \
               self.move_straight(grid, 1, 1)
    def allowed_moves(self, grid):
        tabAccess = []    
        for coord in self.moves(grid):
            if not grid.isChessed(self,coord[0],coord[1]):
                tabAccess.append(coord)
        return tabAccess           

class Queen(Chessman):
    def __init__(self, isWhite, x, y):
        super(Queen, self).__init__(isWhite, x, y, "Queen")
    
    # Fonction de deplacement
    def moves(self, grid, test = False):
        return self.move_straight(grid, -1, 0) + \
               self.move_straight(grid, 1, 0)  + \
               self.move_straight(grid, 0, -1) + \
               self.move_straight(grid, 0, 1)  + \
               self.move_straight(grid, -1, -1)+ \
               self.move_straight(grid, -1, 1) + \
               self.move_straight(grid, 1, -1) + \
               self.move_straight(grid, 1, 1)
               
    def allowed_moves(self, grid):
        tabAccess = []
        for coord in self.moves(grid):
            if not grid.isChessed(self,coord[0],coord[1]):
                tabAccess.append(coord)
        return tabAccess          

class Knight(Chessman):
    def __init__(self, isWhite, x, y):
        super(Knight, self).__init__(isWhite, x, y, "Knight")
    
    # Fonction de deplacement
    def moves(self, grid, test = False):
        tab = [(self.x+i,self.y+j) for i in [-1,1,-2,2] for j in [-1,1,-2,2] \
                    if abs(i)!=abs(j) ]
        return self.testedTuples(grid, tab)
        
    def allowed_moves(self, grid):
        tabAccess = []
        for coord in self.moves(grid):
            if not grid.isChessed(self,coord[0],coord[1]):
                tabAccess.append(coord)
        return tabAccess   
        

class King(Chessman):
    def __init__(self, isWhite, x,y):
        super(King, self).__init__(isWhite, x, y, "King")
        self.__hasMoved = False # utile pour le roque
    
    @property
    def hasMoved(self):
        return self.__hasMoved
    
    @hasMoved.setter
    def hasMoved(self, value):
        self.__hasMoved = value
    
    def moves(self, grid, test = False):
        tab = [(self.x+i,self.y+j) for i in [-1, 0, 1] \
               for j in [-1, 0, 1] if (i,j) != 0 ]
        tabAccess = self.testedTuples(grid, tab) # position accessibles
        # hors roque
        
        # roque
        if not self.__hasMoved and not test:
            for (i,j) in [(0,0),(0,7),(7,7),(7,0)]:
                #tour de gauche (quelque soit la couleur)
                if j == 0:
                    # la tour ne doit pas avoir bouge
                    # les cases intermediaires doivent etre vides
                    # et il ne doit pas y avoir d'echecs le long de la 
                    # trajectoire
                    if not grid.isVoid(i,j) \
                    and grid[(i,0)].name == "Rook" \
                    and not grid[(i,0)].hasMoved \
                    and grid.isVoid(i, 1) and grid.isVoid(i, 2) \
                    and grid.isVoid(i, 3) \
                    and not grid.isChessed(self, i, 2) \
                    and not grid.isChessed(self, i, 3) \
                    and not grid.isChessed(self, i, 4):
                        tabAccess.append((i, 2))
                # tour de droite
                else :
                    if not grid.isVoid(i,j)\
                    and grid[(i,0)].name == "Rook" \
                    and not grid[(i,j)].hasMoved \
                    and grid.isVoid(i, 5) and grid.isVoid(i, 6) \
                    and not grid.isChessed(self, i, 4) \
                    and not grid.isChessed(self, i, 5) \
                    and not grid.isChessed(self, i, 6):
                        tabAccess.append((i, 6))
        return tabAccess
     
    def allowed_moves(self, grid):
        tabAccess = []       
        for coord in self.moves(grid):
            if not grid.isChessed(self,coord[0],coord[1]):
                tabAccess.append(coord)
        return tabAccess
        

class Pawn(Chessman):
    def __init__(self, isWhite, x, y):
        super(Pawn, self).__init__(isWhite, x, y, "Pawn")
        self.__double_done = False

    @property
    def double_done(self):
        return self.__double_done

    @double_done.setter
    def double_done(self, value):
        self.__double_done = value
        

    def moves(self, grid, test = False):
        tabAccess = []
        # direction du deplacement : 1 pour les blanc, -1 pour les noirs
        direction = 2*self.isWhite -1
        if grid.isVoid(self.x+direction,self.y):
            tabAccess.append((self.x+direction,self.y))
        tabAccess.append((self.x+direction,self.y+1))
        tabAccess.append((self.x+direction,self.y-1))
        if self.isWhite and self.x == 1:
            tabAccess.append((self.x+2*direction,self.y))
        if not self.isWhite and self.x == 6:
            tabAccess.append((self.x+2*direction,self.y))
        tab = self.testedTuples(grid, tabAccess)
        finaltab=[]
        for coord in tab:
            if coord[1] == self.y:
                if grid.isVoid(coord[0],coord[1]):
                    finaltab.append(coord)
            else:
                if not grid.isVoid(coord[0],coord[1])\
                    and not grid.sameColor(self, coord[0],coord[1]):
                    finaltab.append(coord)
                        
                if not grid.isVoid(coord[0]-direction,coord[1]) and \
                    grid[(coord[0]-direction,coord[1])].name == "Pawn" and \
                    grid[(coord[0]-direction,coord[1])].isWhite != \
                    self.isWhite and \
                    grid[(coord[0]-direction,coord[1])].double_done:
                        finaltab.append(coord)
        return finaltab
            



        # prise en passant (coup rare)

    def allowed_moves(self, grid):
        tabAccess = []
        direction = 2*self.isWhite -1
        for coord in self.moves(grid):
            if not grid.isChessed(self,coord[0],coord[1]):
                tabAccess.append(coord)
        return tabAccess
