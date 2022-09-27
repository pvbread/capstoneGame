from turtle import speed


class Character:
    def __init__(self, name, hp, speed, speed_modifier):
        self.name = name
        self.hp = hp
        self.speed = speed
        self.speed_modifier = speed_modifier