#pragma once

#include <iostream>
#include <string>
using namespace std;

class Animal
{
protected:
    string animalSays;

public:
    Animal(string x)
    {
        animalSays = x;
    }
    virtual void speak()
    {
        cout << Animal::animalSays << endl;
    }
    
    string getSays() const
    {
        return animalSays;
    }

    void setSays(string x)
    {
        animalSays = x;
    }
};