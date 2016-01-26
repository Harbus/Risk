#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>
#include "SelectionTool.h"

SelectionTool::SelectionTool(std::vector<sf::Drawable*>* drawables) {
    drawables_ = drawables;
}

void SelectionTool::Execute(int x, int y) {
    std::cout << "Selected (" << x << ", " << y << ") " << std::endl;
}