#pragma once
#include "Screen.h"

class PlayMenuScreen : public Screen {
 public:
    PlayMenuScreen();
    virtual ~PlayMenuScreen() = default;

    void OnMapClick();
    void OnBackClick();

 private:
    void initialize() override;
};

