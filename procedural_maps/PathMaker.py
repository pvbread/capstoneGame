import numpy as np
import random
import matplotlib.pyplot as plt

def PathMaker(length):
    """
    Takes the length of the path and generates a self avoiding walk.
    If the walker is stuck, the walker backtracks by 1 step and tries to find a different path.
    Returns a list of x coordinates and a list of y coordinates, boolean variable (stuck) if the path maker had to backtrack,
    and the total number of steps of path.
    """    
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

def main():
    
    path_length=40
    x, y, stuck, steps = PathMaker(path_length)
    plt.figure(figsize = (8, 8))
    plt.plot(x, y, 'bo-', linewidth = 1)
    plt.plot(0, 0, 'go', ms = 12, label = 'Start')
    plt.plot(x[-1], y[-1], 'ro', ms = 12, label = 'End')
    plt.axis('equal')
    plt.legend()
    if stuck:
        plt.title('Backtracked path with a length of ' + str(steps))
    else:
        plt.title('Path of length ' + str(steps))
    plt.show()


if __name__ == "__main__":
    main()
