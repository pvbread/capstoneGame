import random
import numpy as np
import matplotlib.pyplot as plt

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
            x_branch.pop()
            y_branch.pop()
            
    return x_branch, y_branch, positions_visited


# create matrix from coordinates and save matrix in text file
def createMatrix(coordinates):
    # create matrix filled with zeros
    matrix = np.full((20,20), 3)

    # itterate through coordinates (i = x-cord, j = y-cord) 
    for i, j in coordinates:
        matrix[i][j] = 1
    for row in range(20):
        for col in range(20):
            if matrix[row][col] != 3:
                if matrix[row+1][col] != 3 and matrix[row-1][col] != 3 and matrix[row][col+1] != 3 and matrix [row][col-1] != 3:
                    matrix[row][col] = 11
                elif matrix[row+1][col] != 3 and matrix[row-1][col] == 3 and matrix[row][col+1] == 3 and matrix [row][col-1] != 3:
                    matrix[row][col] = 10
                elif matrix[row+1][col] == 3 and matrix[row-1][col] != 3 and matrix[row][col+1] == 3 and matrix [row][col-1] != 3:
                    matrix[row][col] = 9
                elif matrix[row+1][col] == 3 and matrix[row-1][col] == 3 and matrix[row][col+1] != 3 and matrix [row][col-1] != 3:
                    matrix[row][col] = 8
                elif matrix[row+1][col] not in [7,3] and matrix[row-1][col] == 3 and matrix[row][col+1] not in [7,3] and matrix [row][col-1] not in [7,3]:
                    matrix[row][col] = 7
                elif matrix[row+1][col] != 3 and matrix[row-1][col] == 3 and matrix[row][col+1] != 3 and matrix [row][col-1] == 3:
                    matrix[row][col] = 6
                elif matrix[row+1][col] != 3 and matrix[row-1][col] != 3 and matrix[row][col+1] == 3 and matrix [row][col-1] == 3:
                    matrix[row][col] = 5 
                elif matrix[row+1][col] == 3 and matrix[row-1][col] not in [4,3] and matrix[row][col+1] not in [4,3] and matrix [row][col-1] not in [4,3]:
                    matrix[row][col] = 4
                elif matrix[row+1][col] == 3 and matrix[row-1][col] != 3 and matrix[row][col+1] != 3 and matrix [row][col-1] == 3:
                    matrix[row][col] = 2
                elif matrix[row+1][col] not in [1,3] and matrix[row-1][col] not in [1,3] and matrix[row][col+1] == 3 and matrix [row][col-1] not in [1,3]:
                    matrix[row][col] = 1
                elif matrix[row+1][col] not in [0,3] and matrix[row-1][col] not in [0,3] and matrix[row][col+1] not in [0,3] and matrix [row][col-1] == 3:
                    matrix[row][col] = 0
            else:
                matrix[row][col] = 3
        
    #     TRIGHT = 0,
    #     TLEFT = 1,
    #     UPRIGHT = 2,
    #     BLACK = 3,
    #     TUP = 4,
    #     UPDOWN = 5,
    #     DOWNRIGHT = 6,
    #     TDOWN = 7,
    #     LEFTRIGHT = 8,
    #     LEFTUP = 9,
    #     LEFTDOWN = 10,
    #     FOURWAY = 11
    # save matrix in textfile
    np.savetxt('output.txt', matrix, fmt='%d')
   

"""
Draw the paths with visualization using matplotlib
"""
def plotWalk(path_length):
    # draw the first self avoiding walk path
    x, y, positions_visited = pathMaker(path_length)
    plt.figure(figsize = (8, 8))
    plt.plot(x, y, 'bo-', linewidth = 1)
    plt.plot(x[0], y[0], 'go', ms = 12, label = 'Start')
    plt.plot(x[-1], y[-1], 'ro', ms = 12, label = 'End')
    
    # store the actual length of inital path since the path might not always be the desired length due to being stuck 
    new_length = len(x)
    
    # draw two self avoiding walk that branches in different directions at halfway point of the first initial path
    for i in range(2):
        x_branch, y_branch, positions_visited = branchMaker(x, y, new_length, positions_visited)
        plt.plot(x_branch, y_branch, 'bo-', linewidth = 1)
        plt.plot(x_branch[-1], y_branch[-1], 'ro', ms = 12)
        # append the branches to the x and y coordinates list that will be used to convert to matrix (not including duplicate of midpoint value)   
        x = x + x_branch[1:]
        y = y + y_branch[1:]
    plt.axis('equal')
    plt.legend()
    plt.savefig("map.png")
    plt.show()
    # coordinates is the list of array of the (x,y) coordinates
    coordinates=[]
    # append coordinates to the list
    for i in range(len(x)):
        coordinates.append((x[i],y[i]))
    # call function to create matrix with coordinates and save as textfile
    createMatrix(coordinates)
    

"""
You can change the number in the parameter to change the length of the branches.
Creates 3 branching paths depending on how the first path was generated
"""
plotWalk(40)  



