#pragma once
#include <vector>
#include <string>
class Country;
class Player;

class Continent {
 public:
    Continent(std::string name, int controlValue);
    ~Continent();

    void AddCountry(Country* country);
    void RemoveCountry(Country* country);
    bool HasCountry(Country* country);
    bool IsPlayerControllingContinent(Player* player);
    void ToString();

    const std::string& GetName() const { return name_; }
    int GetControlValue() const { return controlValue_; }
    const std::vector<Country*>& GetCountries() const { return countries_; }
    void SetControlValue(int controlValue) { controlValue_ = controlValue; }

 private:
    std::string name_;
    std::vector<Country*> countries_;
    int controlValue_;
};
