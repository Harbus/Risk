#pragma once
#include <vector>
#include "Observer.h"
class Player;
class Subject;
class PlayerList;

class Statistics : public Observer {
public:
    Statistics();
    virtual ~Statistics();

    void AddPlayer(Player* mock);
    void AddPlayers(PlayerList* mocks);
    virtual std::string ToString(Player* mock) = 0;
    void Update(Subject* s = nullptr) override;
    std::string GetStatisticsString() const { return statisticsString_; }

protected:
    std::string statisticsString_;

private:
    std::vector<Player*> mocks_;
};