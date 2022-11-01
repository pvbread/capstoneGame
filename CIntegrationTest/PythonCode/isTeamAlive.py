def isTeamAlive(team):
    '''
    Input: takes in a array of charaters representing a team
    checks isAlive for each one
    output: returns true if at least one member is alive, false otherwise 
    '''
    for char in team:
        if char.isAlive:
            return True

    return False
