#pragma once
#include <vector>
#include "Subject.h"
class Country;

class Player : public Subject {
 public:
    Player(std::string name);
    ~Player() = default;

    void AddOwnedCountry(Country* country);
    void RemoveOwnedCountry(Country* country);
    int ComputeTotalNumberOfArmiesOwned() const;

    const std::string& GetName() const { return name_; }
    const std::vector<Country*>& GetOwnedCountries() const { return ownedCountries_; }
    bool GetTurn() { return turn_; }
    int GetBattlesCount() const { return battlesCount_; }
    int GetBattlesWon() const { return battlesWon_; }
    const bool GetEligibilityForCard() { return eligibleForCard_; }
    void SetName(std::string name) { name_ = name; }
    void SetEligibleForCard(bool b) { eligibleForCard_ = b; }
    void SetTurn(bool turn) { turn_ = turn; }
    void SetCard(int card, int amount) { cardsOwned.at(card) = amount; };
    void SetTotalCards();
    void AddCard(int card);
    void LoseCard(int card);
    int GetCard(int card) { return cardsOwned.at(card); };
    int GetCardCount() { return totalCards_; };
    int TurnIn();
    bool EligibleForTurnIn() { return GetCard(0) > 2 || GetCard(1) > 2 || GetCard(2) > 2 || (GetCard(0) > 0 && GetCard(1) > 0 && GetCard(2) > 0) || (GetCard(0) > 0 && GetCard(1) > 0 && GetCard(3) > 0) || (GetCard(1) > 0 && GetCard(2) > 0 && GetCard(3) > 0) || (GetCard(0) > 0 && GetCard(2) > 0 && GetCard(3) > 0); };
    void IncrementBattlesCount() { battlesCount_++; Notify(); }
    void IncrementBattlesWon() { battlesWon_++; Notify(); }

 private:
    std::vector<int> cardsOwned = { 0, 0, 0, 0 }; // Order of cards is Soldier, Cavalry, Artilery, and Wild
    std::string name_;
    std::vector<Country*> ownedCountries_;
    bool eligibleForCard_;
    bool turn_;
    int totalCards_;
    int battlesCount_;
    int battlesWon_;
};
