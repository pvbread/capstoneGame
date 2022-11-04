#include <iostream>
#include "Characters/BaseCharacter.h"
#include "Characters/Players/Bass.h"

int main()
{
    Bass Bass("Bassist", 30, 1, 3, 0, 3, 3, 3, false);
    std::string name = Bass.getName();
    std::cout << name;
    return 0;

}