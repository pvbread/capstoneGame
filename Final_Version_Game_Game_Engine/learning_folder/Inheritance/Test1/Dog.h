#pragma once

#include "Animal.h"
#include <iostream>
#include <string>
using namespace std;

class Dog : public Animal
{

protected:
    string animalSays;
public:
    Dog(string x): Animal(x) {}

   
    
};