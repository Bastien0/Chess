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
    
#    def allowed_moves(self, grid):
#        tabAccess = []
#        print("allowedMoves1")        
#        for coord in self.moves(grid):
#            print("AllowedMoves2")
#            if grid.isChessed(coord):
#                print("AM3")
#                tabAccess.append(coord)
#        return tabAccess
        
    
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
    def moves(self, grid):
        return self.move_straight(grid, -1, 0) + \
               self.move_straight(grid, 1, 0)  + \
               self.move_straight(grid, 0, -1) + \
               self.move_straight(grid, 0, 1)
    
    
        
    @property
    def hasMoved(self):
        return self.__hasMoved
        
    def allowed_moves(self, grid):
        tabAccess = []
        print("allowedMoves1")        
        for coord in self.moves(grid):
            print("AllowedMoves2")
            if grid.isChessed(self,coord[0],coord[1]):
                print("AM3")
                tabAccess.append(coord)
        return tabAccess    


class Bishop(Chessman):
    def __init__(self, isWhite, x,y):
        super(Bishop,self).__init__(isWhite, x, y, "Bishop")
    
    # Fonction de déplacement
    def moves(self, grid):
        return self.move_straight(grid, -1, -1) + \
               self.move_straight(grid, -1, 1)  + \
               self.move_straight(grid, 1, -1)  + \
               self.move_straight(grid, 1, 1)
    def allowed_moves(self, grid):
        tabAccess = []
        print("allowedMoves1")        
        for coord in self.moves(grid):
            print("AllowedMoves2")
            if grid.isChessed(self,coord[0],coord[1]):
                print("AM3")
                tabAccess.append(coord)
        return tabAccess           

class Queen(Chessman):
    def __init__(self, isWhite, x, y):
        super(Queen, self).__init__(isWhite, x, y, "Queen")
    
    # Fonction de deplacement
    def moves(self, grid):
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
        print("allowedMoves1")        
        print(len(self.moves(grid)))
        for coord in self.moves(grid):
            print("AllowedMoves2")
            if grid.isChessed(self,coord[0],coord[1]):
                print("AM3")
                tabAccess.append(coord)
        return tabAccess          

class Knight(Chessman):
    def __init__(self, isWhite, x, y):
        super(Knight, self).__init__(isWhite, x, y, "Knight")
    
    # Fonction de deplacement
    def moves(self, grid):
        tab = [(i,j) for i in [-1,1,-2,2] for j in [-1,1,-2,2] \
                    if abs(i)!=abs(j) ]
        return self.testedTuples(grid, tab)
        
    def allowed_moves(self, grid):
        tabAccess = []
        print("allowedMoves1")        
        for coord in self.moves(grid):
            print("AllowedMoves2")
            if grid.isChessed(self,coord[0],coord[1]):
                print("AM3")
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
    def hasMoved(self):
        self.__hasMoved = True
    
    def moves(self, grid):
        tab = [(i,j) for i in [-1, 0, 1] for j in [-1, 0, 1] if (i,j) != 0 ]
        tabAccess = self.testedTuples(grid, tab) # position accessibles
        # hors roque
        
        # roque
        if not self.__hasMoved:
            for (i,j) in [(0,0),(0,7),(7,7)]:
                #tour de gauche (quelque soit la couleur)
                if j == 0:
                    # la tour ne doit pas avoir bouge
                    # les cases intermediaires doivent etre vides
                    # et il ne doit pas y avoir d'echecs le long de la 
                    # trajectoire
                    if grid[(i,j)] != None\
                    and not grid[(i,j)].hasMoved \
                    and grid.isVoid(1, j) and grid.isVoid(2, j) \
                    and not grid.isChessed(self, 2, j) \
                    and not grid.isChessed(self, 3, j) \
                    and not grid.isChessed(self, 4, j):
                        tabAccess.append((1, j))
                # tour de droite
                else :
                    if grid[(i,j)] != None\
                    and not grid[(i,j)].hasMoved \
                    and grid.isVoid(5, j) and grid.isVoid(6, j) \
                    and not grid.isChessed(self, 4, j) \
                    and not grid.isChessed(self, 5, j) \
                    and not grid.isChessed(self, 6, j):
                        tabAccess.append((6, j))
        return tabAccess
     
    def allowed_moves(self, grid):
        tabAccess = []
        print("allowedMoves1")        
        for coord in self.moves(grid):
            print("AllowedMoves2")
            if grid.isChessed(self,coord[0],coord[1]):
                print("AM3")
                tabAccess.append(coord)
        return tabAccess
        

class Pawn(Chessman):
    def __init__(self, isWhite, x, y):
        super(Pawn, self).__init__(isWhite, x, y, "Pawn")

    def moves(self, grid):
        tabAccess = []
        direction = 2*self.isWhite -1
        #tabAccess.append((self.x+direction,self.y))
        print("lub")
        print(tabAccess) 
        # direction du deplacement : 1 pour les blanc, -1 pour les noirs
 
        direction = 2*self.isWhite -1 
        
        # deplacement d'une case vers l'avant
        # s'il est possble d'avancer
        # si la case est vide
        # si la nouvelle position ne nous met pas le roi en echec
        if  0 <= self.y + direction < 8 \
            and grid.isVoid(self.x + direction, self.y):
            #and not grid.isChessed(self, self.x+ direction, self.y ):
                tabAccess.append((self.x + direction, self.y))
        
        # premier déplacement de deux cases possible
        # si on est en position initiale pour les blancs ou les noirs
        # si les deux cases devant sont vides
        # si la nouvelle position ne met pas le roi en echec
        if ((self.isWhite and self.x == 1) or (not self.isWhite \
                                                    and self.x == 6)):
            if grid.isVoid(self.x + direction, self.y) \
            and grid.isVoid(self.x + 2*direction, self.y):
            #and not grid.isChessed(self, self.x, self.y + 2*direction):
                tabAccess.append((self.x + 2*direction, self.y))
                 
        # prise d'une piece    
        # a gauche
        # si on ne sort pas de l'echiquier
        # si la case qu'on veut prendre est occupée
        # si le deplacement ne met pas le roi en echec
                 #plus compliqu" : tu as le roit 'être en 0 si tu es blanc
                 #mais pas si tu es noir (et pareil en 7)
        if self.y > 0 and 0 <= self.x + direction < 7 \
        and not grid.isVoid(self.x + direction, self.y - 1) :
            #if not grid.isChessed(self, self.x + direction, self.y - 1):
            tabAccess.append((self.x + direction, self.y - 1))
        # à droite
        if self.y < 7 and 0 <= self.x + direction < 7:
            #if not grid.isChessed(self, self.x + direction, self.y + 1) \
            if not grid.isVoid(self.x + direction, self.y + 1):
                tabAccess.append((self.x + direction, self.y + 1))
        print("grr")
        print(tabAccess)        
        return tabAccess

        # prise en passant (coup rare)

    def allowed_moves(self, grid):
        tabAccess = []
        print("allowedMoves1")        
        for coord in self.moves(grid):
            print("AllowedMoves2")
            if not grid.isChessed(self,coord[0],coord[1]):
                print("AM3")
                tabAccess.append(coord)
        return tabAccess    