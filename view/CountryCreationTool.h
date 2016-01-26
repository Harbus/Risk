#pragma once
#include <vector>
#include "Tool.h"
namespace sf {
    class Drawable;
}

class CountryCreationTool : public Tool {
 public:
     CountryCreationTool(std::vector<sf::Drawable*>* drawables);
    ~CountryCreationTool() = default;
    void Execute(int x, int y) override;

 private:
     std::vector<sf::Drawable*>* drawables_;
};

