#pragma once
#include <vector>
#include "Tool.h"
namespace sf {
    class Drawable;
}

class SelectionTool : public Tool {
 public:
    SelectionTool(std::vector<sf::Drawable*>* drawables);
    ~SelectionTool() = default;
    void Execute(int x, int y) override;

 private:
     std::vector<sf::Drawable*>* drawables_;
};

