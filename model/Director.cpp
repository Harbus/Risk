#include "Director.h"
#include "RiskGame.h"


void Director::SetBuilder(Builder* newBuilder) {
    builder_ = newBuilder;
}

void Director::LoadInProgress(std::string fileName) {
    RiskGame::Instance().SetPlayers(builder_->GetPlayersFromFile(fileName));
    RiskGame::Instance().SetWorld(builder_->GetWorld(fileName));
}