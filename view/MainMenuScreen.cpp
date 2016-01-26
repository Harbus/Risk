#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include "MainMenuScreen.h"
#include "RiskGameView.h"

MainMenuScreen::MainMenuScreen() {
    initialize();
}

void MainMenuScreen::initialize() {
    RiskGameView::Instance().GetDesktop().SetProperty(".title", "FontSize", "300");
    RiskGameView::Instance().GetDesktop().SetProperty(".mainMenuButton", "FontSize", "55");

    sf::RectangleShape* background = new sf::RectangleShape(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    background->setFillColor(sf::Color(70, 70, 70));
    drawables_.push_back(background);

    auto font = new sf::Font();
    font->loadFromFile("../../dependencies/fonts/AIRBORNE_GP.ttf");

    auto text = new sf::Text(WINDOW_TITLE, *font);
    text->setCharacterSize(200);
    drawables_.push_back(text);

    auto btnPlay = sfg::Button::Create("Play");
    btnPlay->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&MainMenuScreen::OnPlayClick, this));
    btnPlay->SetClass("mainMenuButton");

    auto btnLoad = sfg::Button::Create("Load");
    btnLoad->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&MainMenuScreen::OnLoadClick, this));
    btnLoad->SetClass("mainMenuButton");

    auto btnMapEditor = sfg::Button::Create("Map Editor");
    btnMapEditor->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&MainMenuScreen::OnMapEditorClick, this));
    btnMapEditor->SetClass("mainMenuButton");

    auto btnQuit = sfg::Button::Create("Quit");
    btnQuit->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&MainMenuScreen::OnQuitClick, this));
    btnQuit->SetClass("mainMenuButton");

    auto bxBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.0f);
    bxBox->Pack(btnPlay, false, false);
    bxBox->Pack(btnLoad, false, false);
    bxBox->Pack(btnMapEditor, false, false);
    bxBox->Pack(btnQuit, false, false);

    text->setPosition(sf::Vector2f(SCREEN_WIDTH / 2 - text->getLocalBounds().width / 2, SCREEN_HEIGHT / 10 - 50));
    bxBox->SetPosition(sf::Vector2f(SCREEN_WIDTH / 2 - bxBox->GetAllocation().width / 2, SCREEN_HEIGHT / 1.5));

    widgets_.push_back(bxBox);
    Screen::initialize();
}

void MainMenuScreen::OnPlayClick() {
    RiskGameView::Instance().SetScreen("play");
}

void MainMenuScreen::OnLoadClick() {
}

void MainMenuScreen::OnMapEditorClick() {
    RiskGameView::Instance().SetScreen("mapedit");
}

void MainMenuScreen::OnQuitClick() {
    RiskGameView::Instance().GetRenderWindow()->close();
}

// Example code to remember how to do some ui stuff:
/*auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
box->Pack(lblTitle_);
box->Pack(button, false);

auto window = sfg::Window::Create();
window->SetTitle("Test window");
window->Add(box);

auto window2 = sfg::Window::Create();
window2->SetTitle("Second window");
window2->SetRequisition(sf::Vector2f(150, 100));
window2->SetPosition(sf::Vector2f(window->GetAbsolutePosition().x + 110, window->GetAbsolutePosition().y));

desktop_.Add(window);
desktop_.Add(window2);*/

/*auto myRect = sfgui_.GetRenderer().CreateRect(sf::FloatRect(25, 25, 50, 50));
sfgui_.GetRenderer().RemovePrimitive(myRect);*/

/*sf::CircleShape* circle = new sf::CircleShape(10.0f);
circle->setFillColor(sf::Color::White);
circle->setPosition(0, 0);
drawables_.push_back(circle);*/