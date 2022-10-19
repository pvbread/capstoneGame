import random
from unicodedata import name


class BaseItem:
    def __init__(self, name, Evasion, Defense, Agility, TempS, Swiftness, HealNum, StatEffect):
        self.name=name
        self.Evasion=Evasion
        self.Defense=Defense
        self.Agility=Agility
        self.TempS=TempS
        self.Swiftness=Swiftness
        self.HealNum=HealNum        #for Healing
        self.StatEffect=StatEffect  #for status effects

    def __del__(self):
        print(name, " has been deleted")
