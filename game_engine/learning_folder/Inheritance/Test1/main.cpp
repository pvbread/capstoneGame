#include <iostream>
#include "Animal.h"
#include "Dog.h"
#include <unordered_map>
#include <map>
#include <utility>

using namespace std;

int main()
{
//so the idea is that the default
//inherited constructor is called,
//since in this case the default ctor
//hard sets a thing, we need to not hard set it//
    Animal a("i'm an animal");
    Dog d("boof boof");
    a.speak();
    d.Dog::speak();
    d.setSays("bark bark");
    d.speak();
    a.speak();

    map<pair<int,int>, int> m;
    
    return 0;
}