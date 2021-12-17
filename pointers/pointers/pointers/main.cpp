//
//  main.cpp
//  pointers
//
//  Created by Diego Capoccitti on 15/09/21.
//

#include <iostream>

struct Animal
{
    virtual void move()
    {
        std::cout << "Mi muovo" << std::endl;
    }
};

struct Fish :  Animal
{
public:
    void move() override
    {
        std::cout << "Nuoto!" << std::endl;
    }
};

class Dog : public Animal
{
public:
    void move() override
    {
        std::cout << "Corro!" << std::endl;
    }
};

void doSomething(Animal* animal)
{
    if (animal)
    {
        animal->move();
    }
}

int main()
{
    std::unique_ptr<Fish> fish;
    fish = std::make_unique<Fish>();
    fish->move();
    
    doSomething(fish.get());

    return 0;
}
