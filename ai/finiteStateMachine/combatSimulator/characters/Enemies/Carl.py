from . import BaseEnemy

import random

#name = className("print name", hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
#carl = Carl("Carl", 50, 0, 3, 5, 3, 3, 1)

class Carl(BaseEnemy.BaseEnemy):
    #need to figure out how to neatly do an inheritance of this
    def __init__(self, name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier):
        super().__init__(name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
        
        #self.moveSet = [self.attack]
        #This is just a hack until we can figure out how to inherit values that arent init
        #self.moveSet = [self.attack, self.buff, self.debuff, self.move] #is it true everyone has these moves
        #self.moveType = {0: 'attack', 1:'buff', 2:'debuff', 3:'move'} #Index from moveset maps to string that discribes the type

        
    

#