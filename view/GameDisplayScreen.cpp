#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include "GameDisplayScreen.h"
#include "RiskGameView.h"
#include "../model/RiskGame.h"
#include "../model/BasicStatistics.h"

GameDisplay::GameDisplay() {
    initialize();
}

void GameDisplay::initialize() {
    sf::RectangleShape* background = new sf::RectangleShape(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    background->setFillColor(sf::Color(70, 70, 70));
    drawables_.push_back(background);

    lblStatistics_ = sfg::Label::Create();
    lblStatistics_->SetPosition(sf::Vector2f(SCREEN_WIDTH / 10, SCREEN_HEIGHT / 10));

    widgets_.push_back(lblStatistics_);
    Screen::initialize();
}

void GameDisplay::Update() {
    if (RiskGame::Instance().GetStatistics()) {
        lblStatistics_->SetText(RiskGame::Instance().GetStatistics()->GetStatisticsString());
    }
}