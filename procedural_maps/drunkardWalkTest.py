import random
import numpy as np
import matplotlib.pyplot as plt

"""
Takes the length of the path and generates a self avoiding walk.
If the walker is stuck, the walker backtracks by 1 step
and tries to find a different path.
Returns a list of x coordinates and a list of y coordinates,
boolean variable (stuck) if the path maker had to backtrack,
and the total number of steps of path.
"""
def PathMaker(length):    
    x, y = [0], [0]
    positions = set([(0,0)])  #positions is a set that stores all sites visited by the walk
    stuck = False
    stuck_counter=0
    for i in range(length):
        options = [(1,0), (0,1), (-1,0), (0,-1)]
        possible_options = []  #possible_options stores the available directions 
        for dx, dy in options:
            if (x[-1] + dx, y[-1] + dy) not in positions:  #checks if direction leads to a site not visited before
                possible_options.append((dx,dy))
        if possible_options:  #checks if there is a direction available
            dx, dy = possible_options[np.random.randint(0,len(possible_options))]  #choose a direction at random among available ones
            positions.add((x[-1] + dx, y[-1] + dy))
            x.append(x[-1] + dx)
            y.append(y[-1] + dy)
        else:  #backtracks in that case the walk is stuck
            positions.pop()
            x.pop()
            y.pop()
            stuck = True
            stuck_counter +=1 
        steps = length - stuck_counter
    return x, y, stuck, steps

"""
Create branching drunkard's paths from halfway point of original path
"""
def drunkWalk(x,y,length):
    #get coordinates of halfway point of first original path
    x2 = x[length//2]
    y2 = y[length//2]
    # create new branches for x and y at halfway point
    x_temp = [x2]
    y_temp = [y2]
    positions = set([x2,y2]) # starting position at halfway point
    for i in range(length//2): # make the new branching paths shorter
        options = [(1,0),(0,1),(-1,0),(0,-1)]
        possible_options = []
        for dx, dy in options:
            if (x_temp[-1] + dx, y_temp[-1] + dy) not in positions and (x_temp[-1] + dx, y_temp[-1] + dy) != (0,0):  #checks if direction leads to a site not visited before and not go trhough origin
                possible_options.append((dx,dy))
        if possible_options:  #checks if there is a direction available
            dx, dy = possible_options[np.random.randint(0,len(possible_options))]  #choose a direction at random among available ones
            positions.add((x_temp[-1] + dx, y_temp[-1] + dy))
            x_temp.append(x_temp[-1] + dx)
            y_temp.append(y_temp[-1] + dy)
        else:  #backtracks in that case the walk is stuck
            positions.pop()
            x_temp.pop()
            y_temp.pop()
    return x_temp, y_temp

"""
Draw the paths with visualization
"""
def main():
    path_length = 50
    # draw first self avoiding walk path
    x, y, stuck, steps = PathMaker(path_length)
    plt.figure(figsize = (8, 8))
    plt.plot(x, y, 'bo-', linewidth = 1)
    plt.plot(0, 0, 'go', ms = 12, label = 'Start')
    plt.plot(x[-1], y[-1], 'ro', ms = 12, label = 'End')
    new_length=len(x)
    # draw two drunkard's path branching out at halfway point of first path
    for i in range(2):
        new_x, new_y = drunkWalk(x,y,new_length)
        plt.plot(new_x, new_y, 'bo-', linewidth = 1)
        plt.plot(new_x[-1], new_y[-1], 'ro', ms = 12)
    plt.axis('equal')
    plt.legend()
    plt.title('Longest path with a length of ' + str(steps))
    plt.show()

if __name__ == "__main__":
    main()

