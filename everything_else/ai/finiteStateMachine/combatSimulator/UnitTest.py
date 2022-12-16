import unittest
import unittest.mock as mock
from characters.BaseCharacter import BaseCharacter

class TestBaseCharacterMethods(unittest.TestCase):

    def testBaseCharacterMove(self):
        c1 = BaseCharacter(name ="C1", hp= 50, speed=0, hit= 3, armor= 5,itemModifier= 3,speedModifier= 3,dodgeModifier= 1)
        c2 = BaseCharacter(name ="C2", hp= 50, speed=0, hit= 3, armor= 5,itemModifier= 3,speedModifier= 3,dodgeModifier= 1)
        c3 = BaseCharacter(name ="C3", hp= 50, speed=0, hit= 3, armor= 5,itemModifier= 3,speedModifier= 3,dodgeModifier= 1)
        c4 = BaseCharacter(name ="C4", hp= 50, speed=0, hit= 3, armor= 5,itemModifier= 3,speedModifier= 3,dodgeModifier= 1)
        participants = [c1, c2, c3, c4]
        #test idx 0 forward movement
        c1.move(c2, participants)
        self.assertTrue(participants == [c2, c1, c3, c4])
        #test last idx [-1] backward movement
        c4.move(c3, participants)
        self.assertTrue(participants == [c2, c1, c4, c3])
          

if __name__ == '__main__':
    unittest.main()