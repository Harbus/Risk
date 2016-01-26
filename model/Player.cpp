#include <algorithm>
#include <iostream>
#include "Player.h"
#include "Country.h"
#include "RiskGame.h"
#include "Utilities.h"

//Default Constructor
Player::Player(std::string name) {
    name_ = name;
    ownedCountries_ = {};
    battlesCount_ = 0;
    battlesWon_ = 0;
    eligibleForCard_ = false;
    turn_ = false;
    totalCards_ = 0;
}

//Make player owner of country
void Player::AddOwnedCountry(Country* country) {
    ownedCountries_.push_back(country);
    country->SetOwner(this);
    Notify();
}

//Remove country from players ownership
void Player::RemoveOwnedCountry(Country* country) {
    ownedCountries_.erase(std::remove(ownedCountries_.begin(),
                                      ownedCountries_.end(),
                                      country),
                          ownedCountries_.end());
    Notify();
}

void Player::SetTotalCards() {
    for (int i = 0; i < 4; i++)
        totalCards_ += cardsOwned.at(i);
}

//Add card to players deck of cards
void Player::AddCard(int card) {
    cardsOwned.at(card)++;
    totalCards_++;
    RiskGame::Instance().DecrementCard(card);
    Notify();
}

//Remove card when player turns in
void Player::LoseCard(int card) {
    cardsOwned.at(card)--;
    totalCards_--;
    RiskGame::Instance().IncrementCard(card);
    Notify();
}

int Player::TurnIn() {
    //Check if player is eligible to turn in cards
    if (EligibleForTurnIn()) {
        int totalCardsPicked = 0;
        int numTurnIns = RiskGame::Instance().GetTurnIns();
        RiskGame::Instance().IncrementTurnIns();
        //Show players cards available for turn in
        std::cout << "You are eligible to turn in your cards. Which cards would you like to turn in?" << std::endl;
        std::cout << "1. Soldier cards available: " << GetCard(0) << std::endl;
        std::cout << "2. Cavalry cards available: " << GetCard(1) << std::endl;
        std::cout << "3. Artilery cards available: " << GetCard(2) << std::endl;
        std::cout << "4. Wild cards available: " << GetCard(3) << std::endl;
        do {
            int cardPicked;
            std::cout << "> ";
            std::cin >> cardPicked;
            //Check if card picked is available to turn in
            if (GetCard(cardPicked - 1) < 1)
                std::cout << "You do not have any more of that card. Please pick another card." << std::endl;
            else {
                LoseCard(cardPicked - 1);
                std::cout << "Card chosen and removed..." << std::endl;
                totalCardsPicked++;
            }
        } while (totalCardsPicked < 3); //Goes until 3 cards have been picked
        //Depending on the number of turn ins in the game, player gets a specific amount of cards
        if (numTurnIns < 5) {
            return numTurnIns * 2 + 2;
        } else if (numTurnIns == 6) {
            return 15;
        } else {
            return 15 + (numTurnIns - 6) * 5;
        }
    }
    return 0;
}

//Check how many armies player owns
int Player::ComputeTotalNumberOfArmiesOwned() const {
    int armyCount = 0;
    for (Country* country : ownedCountries_) {
        armyCount += country->GetArmy();
    }
    return armyCount;
}