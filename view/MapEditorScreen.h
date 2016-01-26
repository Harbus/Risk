#pragma once
#include <unordered_map>
#include "Screen.h"
namespace sfg {
    class Canvas;
    class Adjustment;
}
namespace sf {
    class View;
}
class Tool;

class MapEditorScreen : public Screen {
public:
    MapEditorScreen();
    virtual ~MapEditorScreen();

    void Draw(sf::RenderTarget& target) const override;
    void OnLeftClick();
    void OnHorizontalAdjustmentChanged();
    void OnVerticalAdjustmentChanged();

private:
    void initialize() override;
    sfg::Canvas::Ptr canvas_;
    sf::View* view_;
    sfg::Adjustment::Ptr horizontalAdjustment_;
    sfg::Adjustment::Ptr verticalAdjustment_;
    std::unordered_map<std::string, Tool*> tools_;
    Tool* currentTool_;
};
