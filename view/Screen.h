#pragma once
#include <vector>
#include <memory>
namespace sf {
    class Drawable;
    class RenderTarget;
}
namespace sfg {
    class Widget;
    class Desktop;
}

class Screen {
 public:
    Screen() = default;
    ~Screen();

    void AddDrawable(sf::Drawable* drawable);
    virtual void Draw(sf::RenderTarget& target) const;
    virtual void Update();
    void Show() const;
    void Hide() const;

 protected:
    virtual void initialize();
    std::vector<sf::Drawable*> drawables_;
    std::vector<std::shared_ptr<sfg::Widget>> widgets_;
};