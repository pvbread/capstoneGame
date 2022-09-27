class BaseCharacter:
    def __init__(self, name, hp, speed, hit, itemModifier, speedModifier, dodgeModifier):
        self.name = name
        self.hp = hp
        self.speed = speed
        self.hit = hit
        self.itemModifier = itemModifier
        self.speedModifier = speedModifier
        self.dodgeModifier = dodgeModifier
        

    