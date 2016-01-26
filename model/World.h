#pragma once
#include <vector>
#include "Subject.h"
class Continent;
class Country;

class World : public Subject {
 public:
    World();
    World(std::string name);
    ~World();

    void AddContinent(Continent* continent);
    void RemoveContinent(Continent* continent);
    bool HasContinent(Continent* continent);
    void RemoveCountry(Country* country);
    Country* GetCountryByName(const std::string& name);
    Continent* GetContinentByName(const std::string& name);
    std::vector<Country*> GenerateCountryList();
    bool ConnectedGraph();
    int CheckCorrecteness();
    void ToString();

    // Used to ensure Observer gets notified
    void IncrementCountryArmy(Country* country, int armyIncrement);

    const std::string& GetName() const { return name_; }
    const std::vector<Continent*>& GetContinents() const { return continents_; }

    void SetName(std::string name) { name_ = name; }

    static void SaveToFile(World* world);
    static World* LoadFromFile(std::string fileName, bool loadInProgress);
    static World* CreateTestMap();
    static std::string ErrorToString(int errorNumber);

 private:
    std::string name_;
    std::vector<Continent*> continents_;
};
