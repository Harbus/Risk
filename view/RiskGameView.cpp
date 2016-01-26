#include <SFML/Graphics.hpp>
#include "RiskGameView.h"
#include "MainMenuScreen.h"
#include "PlayMenuScreen.h"
#include "MapEditorScreen.h"
#include "GameDisplayScreen.h"
#include "../model/RiskGame.h"

RiskGameView& RiskGameView::Instance() {
    static RiskGameView instance;
    return instance;
}

void RiskGameView::Run() {
    sf::RenderWindow renderer(sf::VideoMode((unsigned int)SCREEN_WIDTH, (unsigned int)SCREEN_HEIGHT), WINDOW_TITLE);
    renderer.resetGLStates();

    renderer_ = &renderer;
    AddScreen("statistics", new GameDisplay());
    SetScreen("statistics");

    sf::Event event;
    sf::Clock clock;
    while (renderer.isOpen() && RiskGame::Instance().IsRunning()) {
        while (renderer.pollEvent(event)) {
            desktop_.HandleEvent(event);

            if (event.type == sf::Event::Closed) {
                renderer.close();
            }
        }

        desktop_.Update(clock.restart().asSeconds());
        renderer.clear();
        currentScreen_->Draw(renderer);
        currentScreen_->Update();
        sfgui_.Display(renderer);
        renderer.display();
    }
}

float RiskGameView::GetHeight() {
    return (float)GetRenderWindow()->getSize().x;
}

float RiskGameView::GetWidth() {
    return (float)GetRenderWindow()->getSize().y;
}

void RiskGameView::SetScreen(std::string name) {
    if (currentScreen_) {
        currentScreen_->Hide();
    }

    if (screens_.find(name) != screens_.end()) {
        currentScreen_ = screens_[name];
        currentScreen_->Show();
    }
}