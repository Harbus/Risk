#pragma once
#include <string>
#include <vector>
class Continent;
class Player;

class Country {
 public:
    Country();
    Country(std::string name);
    Country(std::string name, Player* owner, int army);
    ~Country();

    std::vector<Country*> GetEmptyAndPlayerOwnedConnections(Player* player);
    std::vector<Country*> GetPlayerNotOwnedConnections(Player* player);
    bool IsConnected(Country* country);
    void ToString();

    const std::string& GetName() const { return name_; }
    Player* GetOwner() const { return owner_; }
    int GetArmy() const { return army_; }
    Continent* GetContinent() const { return continent_; }
    const std::vector<Country*>& GetConnections() const { return connections_; }
    void SetOwner(Player* owner) { owner_ = owner; }
    void SetArmy(int army) { army_ = army; }
    void SetContinent(Continent* continent) { continent_ = continent; }
    void IncrementArmy(int armyIncrement) { army_ += armyIncrement; }

    static void Connect(Country* countryA, Country* countryB);
    static void Disconnect(Country* countryA, Country* countryB);

 private:
    std::string name_;
    Player* owner_;
    int army_;
    Continent* continent_;
    std::vector<Country*> connections_;
};
