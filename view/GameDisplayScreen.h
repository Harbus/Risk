#pragma once
#include "Screen.h"
#include "../model/Observer.h"
namespace sfg {
    class Label;
}

class GameDisplay : public Screen {
public:
    GameDisplay();
    virtual ~GameDisplay() = default;

    void Update() override;

private:
    void initialize() override;
    sfg::Label::Ptr lblStatistics_;
};

