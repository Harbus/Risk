#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include "Continent.h"
#include "Country.h"
#include "Player.h"

//Constructors
Country::Country() 
    : Country("") {
}

Country::Country(std::string name) 
    : Country(name, nullptr, 0) {
}

Country::Country(std::string name, Player* owner, int army) {
    name_ = name;
    owner_ = owner;
    army_ = army;
    continent_ = nullptr;
}

//Destructor
Country::~Country() {
    connections_.clear();
}

//Gets countries without owners and countries owned by player
std::vector<Country*> Country::GetEmptyAndPlayerOwnedConnections(Player* player) {
    std::vector<Country*> ownedConnections;
    for (Country* connection : connections_) {
        if (connection->GetOwner() == player || connection->GetOwner() == nullptr) {
            ownedConnections.push_back(connection);
        }
    }
    return ownedConnections;
}

//Returns countries not owned by player
std::vector<Country*> Country::GetPlayerNotOwnedConnections(Player* player) {
    std::vector<Country*> ownedConnections;
    for (Country* connection : connections_) {
        if (connection->GetOwner() != player) {
            ownedConnections.push_back(connection);
        }
    }
    return ownedConnections;
}

//Check if country is connected to another country
bool Country::IsConnected(Country* country) {
    if (connections_.empty()) return false;
    return std::find(connections_.begin(), connections_.end(), country) != connections_.end();
}

//Prints out country properties
void Country::ToString() {
    int SIZE = (int)connections_.size();
    std::cout << "The country name is " << GetName() << " , the current owner and army count are " << GetOwner()->GetName() << " , " << GetArmy() << "." << "The country belongs to the following continent: " << GetContinent()->GetName() << std::endl;
    std::cout << "This country is adjacent to the following countries: " << std::endl;
    for (int i = 0; i < SIZE; i++) {
        std::cout << connections_[i]->GetName() << std::endl;
    }
}

//Connects countries to one another
void Country::Connect(Country* countryA, Country* countryB) {
    if (!countryA || !countryB) return;

    if (!countryA->IsConnected(countryB)) {
        countryA->connections_.push_back(countryB);
    }

    if (!countryB->IsConnected(countryA)) {
        countryB->connections_.push_back(countryA);
    }
}

//Disconects two countries from each other
void Country::Disconnect(Country* countryA, Country* countryB) {
    if (!countryA || !countryB) return;

    if (countryA->IsConnected(countryB)) {
        if (!countryA->connections_.empty()) {
            auto foundCountyIterator = std::find(countryA->connections_.begin(), countryA->connections_.end(), countryB);
            if (foundCountyIterator != countryA->connections_.end()) {
                countryA->connections_.erase(foundCountyIterator);
            }
        }
    }

    if (countryB->IsConnected(countryA)) {
        if (!countryB->connections_.empty()) {
            auto foundCountyIterator = std::find(countryB->connections_.begin(), countryB->connections_.end(), countryA);
            if (foundCountyIterator != countryB->connections_.end()) {
                countryB->connections_.erase(foundCountyIterator);
            }
        }
    }
}
