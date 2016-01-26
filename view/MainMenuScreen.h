#pragma once
#include "Screen.h"

class MainMenuScreen : public Screen {
 public:
    MainMenuScreen();
    virtual ~MainMenuScreen() = default;

    void OnPlayClick();
    void OnLoadClick();
    void OnMapEditorClick();
    void OnQuitClick();

 private:
    void initialize() override;
};

