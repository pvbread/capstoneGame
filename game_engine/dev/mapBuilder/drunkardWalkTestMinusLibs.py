import random
import sys

"""
You can change the value of length at the last line.
"""

"""
Takes the length of the path and generates a self avoiding walk.
If the walker is stuck in the middle of the loop, the walker backtracks by 1 step
and tries to find a different direction to take.
Returns a list of x coordinates and a list of y coordinates,
and returns a set of array of positions that were visited to avoid collisions.
"""
def pathMaker(length):
    
    # lists storing x and y coordinates where starting position is at (0,0)
    x, y = [0], [0]
    
    # positions_visited is a set that keep track of all the coordinates visited by the random walker
    positions_visited = set([(0,0)])

    # create initial path with at most a length of some arbitrary number
    for i in range(length):
        # directions is a list of taking one additional step right, up, left, down (East, North, West, South)
        directions = [(1,0), (0,1), (-1,0), (0,-1)]
        
        # possible_directions is a list that stores the available directions for the next point that are not in positions_visited
        possible_directions = []

        for dx, dy in directions:
            # checks if taking that one additional step leads to a position not visited before
            if (x[-1] + dx, y[-1] + dy) not in positions_visited:
                # if the position have not been visited before and the values are not negative, add it to the possible_directions list
                if ((x[-1] + dx) >= 0 and (y[-1] + dy) >= 0):
                    possible_directions.append((dx,dy))
        # if there is a direction avaliable (meaning there's at least one array in the list)       
        if len(possible_directions) > 0:
            # choose a direction at random among available ones 
            dx, dy = random.choice(possible_directions)
            # add the new point in the positions_visited set
            positions_visited.add((x[-1] + dx, y[-1] + dy))
            # add the x and y coordinates of the new point to the lists
            x.append(x[-1] + dx)
            y.append(y[-1] + dy)
        # otherwise if there are no possible directions, backtracks by one step by popping the x and y coordinates from the list
        else: 
            x.pop()
            y.pop()
            
    return x, y, positions_visited

"""
Takes a matrix as input and fills it with threes
Returns the matrix filled with threes
"""
def fillWithThrees(width, height):
    matrix = [[3 for c in range(width)] for row in range(height)]
    return matrix

"""
Creates a branching path from halfway point of original path.
Takes the lists of x and y coordinates of the first path, the set of positions visisted,
and the length of the original path.
Returns a temporary list for x and y coordinates of the branches,
and returns the set of positions that were visisted before.
"""
def branchMaker(x, y, length, positions_visited):
    
    #get coordinates of halfway point of first original path
    x2 = x[length//2]
    y2 = y[length//2]
    
    # create new temporary lists for x and y coordinates for the branches starting at halfway point 
    x_branch = [x2]
    y_branch = [y2]

    # counter variable to move to the next point in case there are no directions avaliable at the halfway point
    step = 1

    # create the new branching paths length at most half of its first initial path
    for i in range(length//2):
        # getting avaliable directions is the same as pathMaker function
        directions = [(1,0), (0,1), (-1,0), (0,-1)]
        possible_directions = []
        for dx, dy in directions:
            if (x_branch[-1] + dx, y_branch[-1] + dy) not in positions_visited:
                # if the position have not been visited before and the values are not negative, add it to the possible_directions list
                if ((x_branch[-1] + dx) >= 0 and (y_branch[-1] + dy) >= 0):
                    possible_directions.append((dx,dy))
        #if there is a direction avaliable, select a direction at random and add new coordinates to the lists
        if len(possible_directions) > 0:  
            dx, dy = random.choice(possible_directions) 
            positions_visited.add((x_branch[-1] + dx, y_branch[-1] + dy))
            x_branch.append(x_branch[-1] + dx)
            y_branch.append(y_branch[-1] + dy)
        #if there are no avaliable directions from the start of creating a new branch, creates a new branch of n steps away from midpoint
        elif ((len(x_branch) == 1 and len(y_branch) == 1)):
            # remove midpoint coordinates from the lists
            x_branch.pop()
            y_branch.pop()
            # make the next starting point n steps away from the midpoint
            x2 = x[length//2 + step]
            y2 = y[length//2 + step]
            # add new starting point to the lists
            x_branch.append(x2)
            y_branch.append(y2)
            # increment the number of steps by one in case the next point also had no avaliable directions
            step+=1
        # otherwise, backtrack by one step by popping the x and y coordinates from the branches  
        else:
            # tries to prevent making a short branch unless the procedural generation is stuck no matter how many itterations
            if len(x_branch) > length//4:
                x_branch.pop()
                y_branch.pop()
    
            
    return x_branch, y_branch, positions_visited

"""
encodes the martix to values that correlate to a enum tile type (does not include t-shaped tiles and 4-way tiles)

enum TileType
{
    TRIGHT = 0,
    TLEFT = 1,
    UPRIGHT = 2,
    BLACK = 3,
    TUP = 4,
    UPDOWN = 5,
    DOWNRIGHT = 6,
    TDOWN = 7,
    LEFTRIGHT = 8,
    LEFTUP = 9,
    LEFTDOWN = 10,
    FOURWAY = 11
};
"""
def encodePath(matrix, path):

    # variables that start with 'r' represents the row, variables that start with 'c' represents the col

    # itterate through the path list and encode the path with the proper tile type within the matrix
    for i in range(len(path)-2):
        
        """
        We needed to keep track of 3 coordinates to determine the tile shape and direction
        since we know what the tile shape and direction of the first coordinate of the path, we can ignore and skip to the next coordinate
        """
        
        # these variables will represent the previous coordinates
        rFirst, cFirst = path[i]
        # these variables will represent the current coordinates
        rMid, cMid = path[i+1]
        # these variables will represent the next coordinates
        rLast, cLast = path[i+2]

        #Switch case to detemine the tile type: the AND conditions helps deterine the tile shape, the OR conditions determine the direction of the path
        
        # Case: LEFTRIGHT
        if (rFirst == rMid and rLast == rMid and cFirst + 1 == cMid and cLast - 1 == cMid) or \
                (rFirst == rMid and rLast == rMid and cFirst - 1 == cMid and cLast + 1 == cMid):
            matrix[rMid][cMid] = 8
        # Case: UPDOWN
        elif (rFirst + 1 == rMid and rLast - 1 == rMid and cFirst == cMid and cLast == cMid) or \
                (rFirst - 1 == rMid and rLast + 1 == rMid and cFirst == cMid and cLast == cMid):
            matrix[rMid][cMid] = 5
        # Case: DOWNRIGHT
        elif (rFirst - 1 == rMid and rLast == rMid and cFirst == cMid and cLast - 1 == cMid) or \
                (rFirst == rMid and rLast - 1 == rMid and cFirst - 1 == cMid and cLast == cMid):
            matrix[rMid][cMid] = 6
        # Case: LEFTDOWN
        elif(rFirst == rMid and rLast - 1 == rMid and cFirst + 1 == cMid and cLast == cMid) or \
                (rFirst - 1 == rMid and rLast == rMid and cFirst == cMid and cLast + 1 == cMid):
            matrix[rMid][cMid] = 10
        # Case: LEFTUP  
        elif(rFirst == rMid and rLast + 1 == rMid and cFirst + 1 == cMid and cLast == cMid) or \
                (rFirst + 1 == rMid and rLast == rMid and cFirst == cMid and cLast + 1 == cMid):
            matrix[rMid][cMid] = 9
        # Case: UPRIGHT
        elif(rFirst + 1 == rMid and rLast == rMid and cFirst == cMid and cLast - 1 == cMid) or \
                (rFirst  == rMid and rLast + 1 == rMid and cFirst - 1 == cMid and cLast == cMid):
            matrix[rMid][cMid] = 2
        # the number 12 will represent the end point of the path
        matrix[rLast][cLast] = 12

    # return an updated version of the matrix
    return matrix
"""
encodes the the branching tile into one of the T-shaped tiles

enum TileType
{
    TRIGHT = 0,
    TLEFT = 1,
    UPRIGHT = 2,
    BLACK = 3,
    TUP = 4,
    UPDOWN = 5,
    DOWNRIGHT = 6,
    TDOWN = 7,
    LEFTRIGHT = 8,
    LEFTUP = 9,
    LEFTDOWN = 10,
    FOURWAY = 11
};
"""
def encodeTBranches(matrix,path,rMidPt,cMidPt,rStep,cStep):
    # Case: TDOWN
    if ((matrix[rMidPt][cMidPt] == 8 and rMidPt + 1 == rStep and cMidPt == cStep) or \
           (matrix[rMidPt][cMidPt] == 2 and rMidPt == rStep and cMidPt - 1 == cStep) or \
           (matrix[rMidPt][cMidPt] == 10 and rMidPt == rStep and cMidPt + 1 == cStep)):
        matrix[rMidPt][cMidPt] = 7
    # Case: TUP
    elif ((matrix[rMidPt][cMidPt] == 8 and rMidPt - 1 == rStep and cMidPt == cStep) or \
            (matrix[rMidPt][cMidPt] == 9 and rMidPt == rStep and cMidPt + 1 == cStep) or \
            (matrix[rMidPt][cMidPt] == 6 and rMidPt == rStep and cMidPt - 1 == cStep)):
        matrix[rMidPt][cMidPt] = 4
    # Case: TRIGHT
    elif ((matrix[rMidPt][cMidPt] == 5 and rMidPt == rStep and cMidPt + 1 == cStep) or \
            (matrix[rMidPt][cMidPt] == 2 and rMidPt - 1 == rStep and cMidPt == cStep) or \
            (matrix[rMidPt][cMidPt] == 6 and rMidPt + 1 == rStep and cMidPt == cStep)):
        matrix[rMidPt][cMidPt] = 0
    # Case: TLEFT
    elif ((matrix[rMidPt][cMidPt] == 5 and rMidPt == rStep and cMidPt - 1 == cStep) or \
            (matrix[rMidPt][cMidPt] == 10 and rMidPt - 1 == rStep and cMidPt == cStep) or \
            (matrix[rMidPt][cMidPt] == 9 and rMidPt + 1 == rStep and cMidPt == cStep)):
        matrix[rMidPt][cMidPt] = 1
    return matrix

"""
creates a matrix from coordinates of the three paths and save matrix in text file (x -> row, y -> col)
"""
def createMatrix(path1, path2, path3, x_max, y_max):

    # create the dimensions of the matrix based on the maximum values of x and y
    # default dimensions 12 x 16 matrix

    # if the highest x value is less than 12, set height to 12
    if x_max < 12:
        height = 12
    # else set height to x_max plus 2 for extra spacing
    else:
        height = x_max + 2

    # if the highest y value is less than 16, set width to 16
    if y_max < 16:
        width = 16
    # else set width to y_max plus 2 for extra spacing
    else:
        width = y_max + 2
    
    
    # create matrix filled with threes

    matrix = fillWithThrees(width, height)

    # variables that start with 'r' represents the row, variables that start with 'c' represents the col

    """
    encode first path
    """
    
    # origin point will only have the UPDOWN tile or the LEFTRIGHT tile (the origin will always start at (0,0): the beginning of the matrix)
    rStart, cStart = path1[0]
    # need to see the next step to figure out the direction the origin tile should be
    rStep, cStep = path1[1]

    # if horizontal line, LEFTRIGHT tile
    if rStart == rStep and cStart + 1 == cStep: 
        matrix[rStart][cStart] = 8
    # if vertical line, UPDOWN tile
    elif rStart + 1 == rStep and cStart == cStep:
        matrix[rStart][cStart] = 5

    # calls function to encode the first path
    matrix = encodePath(matrix, path1)

    """
    encode second path
    """

    # the first coordinate of the branching paths always start with a point somewhere in the middle of the first path
    rMidPt, cMidPt = path2[0]
    # need to see the next step to figure out what T-shaped tile should be replaced for the branching path (T-shaped tiles only used to add a branching path)
    rStep, cStep = path2[1]

    # calls function to encode the branch tile
    matrix = encodeTBranches(matrix, path2, rMidPt, cMidPt, rStep, cStep)
        
    # calls function to encode the second path
    matrix = encodePath(matrix, path2)

    # Rare case: if there's only two elements in the path list thus making the previous function returns the matrix unchanged, then the second element will be the endpoint
    if len(path2)==2:
        matrix[rStep][cStep] = 12

    """
    encode third path
    """

    # now find the start of the third path which will also start somewhere in the middle of the first path
    rMidPt, cMidPt = path3[0]
    # need to see the next step to figure out if either a T-shaped tile or a FOURWAY tile is needed to represent branching path in the matrix
    rStep, cStep = path3[1]

    # if the second path and the third path has the same starting point, replace the point with a FOURWAY tile
    if path2[0] == path3[0]:
        matrix[rMidPt][cMidPt] = 11
    # else replace with a T-shaped tile
    else:
        matrix = encodeTBranches(matrix, path3, rMidPt, cMidPt, rStep, cStep)


    # calls function to encode the third path
    matrix = encodePath(matrix, path3)
    
    # Rare case: if there's only two elements in the path list thus making the previous function returns the matrix unchanged, then the second element will be the endpoint
    if len(path3)==2:
        matrix[rStep][cStep] = 12

    # save matrix in textfile
    #first line prints height and width and format matrix as whole number digits
    text = ""
    text += str(height)+ " " + str(width) + "\n"
    for row in matrix:
        for col in row:
            if col > 9:
                text += " " + str(col)
            else:
                text += " 0" + str(col) 
        text += "\n"
    file_path = "../assets/maps/testLevelIntegration" + sys.argv[1] + ".map"
    with open(file_path, 'w') as f:
        f.write(text)
        

   

"""
Draw the paths with visualization using matplotlib
"""
def plotWalk(path_length):
    # draw the first self avoiding walk path (the original first path)
    x1, y1, positions_visited = pathMaker(path_length)
    
    # store the actual length of inital path since the path might not always be the desired length due to being stuck 
    new_length = len(x1)
    
    # draw a branch somewhere on the original path (the second path)
    x2, y2, positions_visited = branchMaker(x1, y1, new_length, positions_visited)
    
    # draw another branch somewhere on the original path (the third path)
    x3, y3, positions_visited = branchMaker(x1, y1, new_length, positions_visited)

    # get max value for x for the height of the matrix
    x_max = max(x1)
    if x_max < max(x2):
        x_max = max(x2)
    if x_max < max(x3):
        x_max = max(x3)

    # get max value for y for the width of the matrrix
    y_max = max(y1)
    if y_max < max(y2):
        y_max = max(y2)
    if y_max < max(y3):
        y_max = max(y3)

    
    # these variables will store a list of array of the (x,y) coordinates of the 3 branching paths in order to draw the matrix
    path1 = []
    path2 = []
    path3 = []
    
    # append coordinates to its corresponding path list
    
    # first initial path
    for i in range(len(x1)):
        path1.append((x1[i], y1[i]))
    # the second path
    for i in range(len(x2)):
        path2.append((x2[i], y2[i]))
    # the third path
    for i in range(len(x3)):
        path3.append((x3[i], y3[i]))
    
    # call function to create matrix with coordinates and save as textfile
    createMatrix(path1, path2, path3, x_max, y_max)
    

"""
You can change the number in the parameter to change the length of the branches.
Creates 3 branching paths depending on how the first path was generated
"""
plotWalk(40)  



