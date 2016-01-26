#pragma once
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <unordered_map>
namespace sf {
    class Drawable;
}
class Screen;

const std::string WINDOW_TITLE = "RISK";
const float SCREEN_WIDTH = 380.0f;
const float SCREEN_HEIGHT = 650.0f;

class RiskGameView {
 public:
    static RiskGameView& Instance();

    void Run();
    float GetHeight();
    float GetWidth();
    void SetScreen(std::string name);

    sfg::SFGUI& GetSFGUI() { return sfgui_; }
    sfg::Desktop& GetDesktop() { return desktop_; }
    sf::RenderWindow* GetRenderWindow() { return renderer_; }
    Screen* GetScreen() { return currentScreen_;  }
    void AddScreen(std::string name, Screen* screen) { screens_[name] = screen; }

 private:
    RiskGameView() = default;
    ~RiskGameView() = default;

    sfg::SFGUI sfgui_;
    sfg::Desktop desktop_;
    sf::RenderWindow* renderer_;
    Screen* currentScreen_;
    std::unordered_map<std::string, Screen*> screens_;
};