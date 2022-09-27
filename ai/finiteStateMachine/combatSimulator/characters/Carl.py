from BaseCharacter import BaseCharacter

class Carl(BaseCharacter):
    def __init__(self, name, hp, speed, hitModifier, speedModifier, dodgeModifier):
        super().__init__(name, hp, speed, hitModifier, speedModifier, dodgeModifier)