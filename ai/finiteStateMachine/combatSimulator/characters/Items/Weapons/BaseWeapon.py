from .. import BaseItem
import random

class Weapon(BaseItem.BaseItem):
    def __init__(self, name, tempS, swiftness, healNum, statEffect, rarity):
        super().__init__(self, name, tempS, swiftness, healNum, statEffect)

        self.rarity=rarity

    def weaponPrint(self):
        print("This instrument is called ", self.name, " with a rarity of ", self.rarity, ".")

    def calculateAttack(self, attack_modifier):
        damage = 0
        if (self.rarity == "common"):
            rng = random.randint(1,4)
        if (self.rarity == "un-common"):
            rng = random.randint(2,6)
        if (self.rarity == "rare"):
            rng = random.randint(4,8)
        if (self.rarity == "very-rare"):
            rng = random.randint(5,10)
        if (self.rarity == "legendary"):
            rng = random.randint(6,12)
        damage = rng + attack_modifier
        return damage
    #Calculate attack:

    # Attacking Damage
    # Number on Stat sheet is Attack_modifier
    # Numbers on Weapon is Diceroll (range)

    # Each weapon has a range of dmg it can do 
    # When attacking you roll a number and add the dmg number on the stat sheet of the attack
    # Let’s say you have a weapon that has (7-11)  dmg, and an attack that has dmg 2 on stat sheet
    # This weapon would essentially have a dmg range of 6 +1D5
    # Min dmg is 6+1+2 = 9
    # Max dmg is 6+5+2 = 13

    # Each rarity of weapon has a damage dice correlated to the rarity. When you deal damage to an enemy you do “The damage roll + Damage Modifier + Any other temp damage modifier like buffs”.
    # EX- your character gets buffed to do an additional 3 damage per attack. Your attack modifier for the attack is 2 damage. The current rarity of weapon is a rare. The damage that will be calculated is ((4-8) + 2 + 3). 
    # EX- your character has no buffs. They have a damage modifier of 4 and a rarity of uncommon. The damage that will be calculated is ((2-6) + 4)
    # EX- your character gets a debuff of -3 damage. Your damage modifier 3. You have a legendary weapon. Your damage is calculated with ((6-12) + 3 - 3) 

    # Weapons- (ex 5-10 means you can roll a 5, 6, 7, 8, 9 or 10)
    # Common- 1-4 + damage modifier
    # Un-common- 2-6 + damage modifier
    # Rare- 4-8 + damage modifier
    # Very Rare- 5-10 + damage modifier
    # Legendary- 6-12 + damage modifier
