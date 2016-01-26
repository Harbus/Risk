#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include "PlayMenuScreen.h"
#include "RiskGameView.h"

PlayMenuScreen::PlayMenuScreen() {
    initialize();
}

void PlayMenuScreen::initialize() {
    RiskGameView::Instance().GetDesktop().SetProperty(".title", "FontSize", "300");
    RiskGameView::Instance().GetDesktop().SetProperty(".mainMenuButton", "FontSize", "55");

    sf::RectangleShape* background = new sf::RectangleShape(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    background->setFillColor(sf::Color(70, 70, 70));
    drawables_.push_back(background);

    auto font = new sf::Font();
    font->loadFromFile("../../dependencies/fonts/AIRBORNE_GP.ttf");

    auto text = new sf::Text("PLAY", *font);
    text->setCharacterSize(200);
    drawables_.push_back(text);
    
    auto btnMap1 = sfg::Button::Create("Map 1");
    btnMap1->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&PlayMenuScreen::OnMapClick, this));
    btnMap1->SetClass("playMenuButton");

    auto btnMap2 = sfg::Button::Create("Map 2");
    btnMap2->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&PlayMenuScreen::OnMapClick, this));
    btnMap2->SetClass("playMenuButton");

    auto btnBack = sfg::Button::Create("Back");
    btnBack->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&PlayMenuScreen::OnBackClick, this));
    btnBack->SetClass("playMenuButton");

    auto bxBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
    bxBox->Pack(btnMap1, false, false);
    bxBox->Pack(btnMap2, false, false);
    bxBox->Pack(btnBack, false, false);

    text->setPosition(sf::Vector2f(SCREEN_WIDTH / 2 - text->getLocalBounds().width / 2, SCREEN_HEIGHT / 10 - 50));
    bxBox->SetPosition(sf::Vector2f(SCREEN_WIDTH / 2 - bxBox->GetAllocation().width / 2, SCREEN_HEIGHT / 2.5));
    
    widgets_.push_back(bxBox);
    Screen::initialize();
}

void PlayMenuScreen::OnMapClick() {

}

void PlayMenuScreen::OnBackClick() {
    RiskGameView::Instance().SetScreen("main");
}