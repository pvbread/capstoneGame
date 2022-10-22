import math

def backendsAttOther(self, targetCharacter, participants):
    #Target: Position distance 7
    shoter = participants.index(self)
    target = participants.index(targetCharacter)
    if shoter ==0 and target==7:
        target.hp -=2
    elif shoter ==7 and target==0:
        target.hp -=2
    #Effect: Damage 2
    return None

def Flutter():
    pass
    #Target: Position Distance 5&6
    #Effect: Damage 2

def Smash():
    pass
    #Target: Position Distance 1
    #Effect: Damage 1, 50% Stun chance

def att2():
    pass
    #Target: Position Distance 1,2,3
    #Effect: 2 Damage

def att1():
    pass
    #Target: Position DIstance 2
    #Effect: Move forward 1, 4 Damage, knockbacck 1 if possible

def att1():
    pass
    #Target: Position target 1 and 4
    #Effect: 3 Damage

def movable_check(self, target):
    pass
    #Target: self, target position
    #Effect: check to see if it can be moved
