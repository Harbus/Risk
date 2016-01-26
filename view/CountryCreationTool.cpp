#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>
#include "CountryCreationTool.h"

CountryCreationTool::CountryCreationTool(std::vector<sf::Drawable*>* drawables) {
    drawables_ = drawables;
}

void CountryCreationTool::Execute(int x, int y) {
    float radius = 40.0f;
    std::cout << "Country created (" << x - radius << ", " << y - radius << ") " << std::endl;
    sf::CircleShape* circle = new sf::CircleShape(radius);
    circle->setFillColor(sf::Color(rand() % 235 + 20, rand() % 235 + 20, rand() % 235 + 20));
    circle->setPosition(x - radius, y - radius);
    drawables_->push_back(circle);
}