#pragma once
#include <vector>
class Player;

class PlayerList {
 public:
    PlayerList();
    ~PlayerList();

    int Size();
    Player* At(int index);
    void Add(Player* player);
    void Remove(Player* player);
    void SetCurrent(int index);
    Player* Current();
    Player* Next();
    const std::vector<Player*>& GetPlayers();

 private:
    std::vector<Player*> players_;
    int currentIndex_;
};
