import random
from unicodedata import name


class BaseItem:
    def __init__(self, name, tempS, swiftness, healNum, statEffect):
        self.name=name
        self.tempS=tempS
        self.swiftness=swiftness
        self.healNum=healNum        #for Healing
        self.statEffect=statEffect  #for status effects

    def __del__(self):
        print(name, " has been deleted")

    def eat(self):
        pass
        #will remove consumables items