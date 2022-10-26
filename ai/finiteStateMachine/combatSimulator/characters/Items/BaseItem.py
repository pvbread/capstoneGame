import random
from unicodedata import name


class BaseItem:
    def __init__(self, name, evasion, defense, agility, tempS, swiftness, healNum, statEffect):
        self.name=name
        self.Evasion=evasion
        self.Defense=defense
        self.Agility=agility
        self.TempS=tempS
        self.Swiftness=swiftness
        self.HealNum=healNum        #for Healing
        self.StatEffect=statEffect  #for status effects

    def __del__(self):
        print(name, " has been deleted")

    def eat(self):
        pass
        #will remove consumables items