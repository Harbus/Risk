#pragma once
#include "Builder.h"

class RiskGame;

class Director {
public:
    void SetBuilder(Builder* newBuilder);

    void LoadInProgress(std::string fileName);

private:
    Builder* builder_;
};