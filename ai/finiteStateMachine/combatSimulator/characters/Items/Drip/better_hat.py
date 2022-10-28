from .. import BaseItem
import random


class Better_Hat(BaseItem.BaseItem):
    def __init__(self, name, evasion, defense, agility, tempS, swiftness, healNum, statEffect):
        super().__init__(self, name, evasion, defense, agility, tempS, swiftness, healNum, statEffect)

    name= "Yankees Cap"
    defense= 1
