#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "RiskGameView.h"

Screen::~Screen() {
    for (const sf::Drawable* drawable : drawables_) {
        delete drawable;
    }

    for (const std::shared_ptr<sfg::Widget>& widget : widgets_) {
        RiskGameView::Instance().GetDesktop().Remove(widget);
    }
}

void Screen::AddDrawable(sf::Drawable* drawable) {
    drawables_.push_back(drawable);
}

void Screen::Draw(sf::RenderTarget& target) const {
    for (const sf::Drawable* drawable : drawables_) {
        target.draw(*drawable);
    }
}

void Screen::Update() {
}

void Screen::Show() const {
    for (const std::shared_ptr<sfg::Widget>& widget : widgets_) {
        widget->Show(true);
    }
}

void Screen::Hide() const {
    for (const std::shared_ptr<sfg::Widget>& widget : widgets_) {
        widget->Show(false);
    }
}

void Screen::initialize() {
    // Call children override before base class
    for (auto widget : widgets_) {
        RiskGameView::Instance().GetDesktop().Add(widget);
        widget->Show(false);
    }
}