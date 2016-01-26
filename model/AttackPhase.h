#pragma once
#include "Phase.h"
class Country;

class AttackPhase : public Phase {
 public:
    AttackPhase(World* world, PlayerList* players);
    virtual ~AttackPhase() = default;
    void Start() override;

 private:
     Country* chooseAttackingCountry();
     Country* chooseDefendingCountry(Country* attackingCountry);
     int chooseAttackArmies(Country* attackingCountry);
     bool attackCountry(Country* attackingCountry, Country* defendingCountry, int& attackingArmies);
     void chooseSendArmies(Country* attackingCountry, Country* conqueredCountry, int attackingArmies);
     void getCard();
};
