#include "Statistics.h"
#include "Subject.h"
#include "Player.h"
#include "PlayerList.h"

Statistics::Statistics() {
    statisticsString_ = "";
}

Statistics::~Statistics() {
    for (Player* mock : mocks_) {
        mock->Detach(this);
    }
}

void Statistics::AddPlayer(Player* mock) {
    mocks_.push_back(mock);
    mock->Attach(this);
}

void Statistics::AddPlayers(PlayerList* mocks) {
    for (Player* mock : mocks->GetPlayers()) {
        AddPlayer(mock);
    }
}

void Statistics::Update(Subject* s) {
    statisticsString_ = "";
    for (Player* mock : mocks_) {
        statisticsString_ += ToString(mock) + "\n";
    }
}