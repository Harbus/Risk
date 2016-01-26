#include <algorithm>
#include "PlayerList.h"
#include "Player.h"

PlayerList::PlayerList() {
    players_ = {};
    currentIndex_ = 0;
}

PlayerList::~PlayerList() {
    for (Player* player : players_) {
        delete player;
    }
    players_.clear();
}

int PlayerList::Size() {
    return (int)players_.size();
}

Player* PlayerList::At(int index) {
    return players_.at(index);
}

void PlayerList::Add(Player* player) {
    players_.push_back(player);
}

void PlayerList::Remove(Player* player) {
    players_.erase(std::remove(players_.begin(),
                               players_.end(),
                               player),
                   players_.end());
}

void PlayerList::SetCurrent(int index) {
    currentIndex_ = index;
}

Player* PlayerList::Current() {
    return players_.at(currentIndex_);
}

Player* PlayerList::Next() {
    currentIndex_ = (currentIndex_ + 1) % players_.size();
    return Current();
}

const std::vector<Player*>& PlayerList::GetPlayers() {
    return players_;
}