#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include "World.h"
#include "Country.h"
#include "Player.h"
#include "Continent.h"
#include "Director.h"
#include "GameBuilder.h"

//default constructor
World::World()
    : World("") {
}

//constructor taking a string 
World::World(std::string name) {
    name_ = name;
    continents_ = {};
}

//destructor
World::~World() {
    for (Continent* continent : continents_) {
        delete continent;
    }
    continents_.clear();
}

//adds a continent to the continent vector in world.
void World::AddContinent(Continent* continent) {
    continents_.push_back(continent);
}

//removes a continent from the continent vector in world
void World::RemoveContinent(Continent* continent) {
    continents_.erase(std::find(continents_.begin(), continents_.end(), continent));
    delete continent;
}

//checks if the world contains x continent
bool World::HasContinent(Continent* continent) {
    return std::find(continents_.begin(), continents_.end(), continent) != continents_.end();
}

//remove the country from the continent in the vector of continent.
void World::RemoveCountry(Country* country) {
    for (Continent* continent : continents_) {
        if (continent->HasCountry(country)) {
            continent->RemoveCountry(country);
        }
    }
}

//returns a country, if said country can be found by name
Country* World::GetCountryByName(const std::string& name) {
    for (Country* country : GenerateCountryList()) {
        if (country->GetName() == name) {
            return country;
        }
    }
    return nullptr;
}

//returns a continent, if said continent can be found by name
Continent* World::GetContinentByName(const std::string& name) {
    for (Continent* continent : continents_) {
        if (continent->GetName() == name) {
            return continent;
        }
    }
    return nullptr;
}

//returns a list of all the countries in the world.
std::vector<Country*> World::GenerateCountryList() {
    std::vector<Country*> countries = std::vector<Country*>();
    for (Continent* continent : continents_) {
        for (Country* country : continent->GetCountries()) {
            countries.push_back(country);
        }
    }
    return countries;
}

//this method checks if the world is a connected graph by making sure that each continent are connected to each other
//by having at least one country have a connection to a country from another continent.
bool World::ConnectedGraph() {
    std::vector<Continent*> ConnectedContinents;
    if (GetContinents().size() > 1) {
        for (Continent* continent : GetContinents()) {
            for (Country* country : continent->GetCountries()) {
                for (Country* connection : country->GetConnections()) {
                    if (connection->GetContinent()->GetName() != country->GetContinent()->GetName()) {
                        if (!(std::find(ConnectedContinents.begin(), ConnectedContinents.end(), connection->GetContinent()) != ConnectedContinents.end())) {
                            ConnectedContinents.push_back(connection->GetContinent());
                        }
                        if (!(std::find(ConnectedContinents.begin(), ConnectedContinents.end(), country->GetContinent()) != ConnectedContinents.end())) {
                            ConnectedContinents.push_back(country->GetContinent());
                        }
                    }
                }
            }
        }
    }
    return (ConnectedContinents.size() == GetContinents().size());
}

//checks the correctness of the world, used to check if the players creates an appropriate map.
int World::CheckCorrecteness() {
    //World has at least 1 continent
    if (continents_.empty()) {
        return 1;
    }

    //Check if continent is only there once
    for (Continent* continent : continents_) {
        for (Continent* otherContinent : continents_) {
            if (continent != otherContinent) {
                if (continent->GetName() == otherContinent->GetName()) {
                    return 2;
                }
            }
        }
    }

    //Check if country is there only once
    for (Country* country : GenerateCountryList()) {
        for (Country* otherCountry : GenerateCountryList()) {
            if (country != otherCountry) {
                if (country->GetName() == otherCountry->GetName()) {
                    return 3;
                }
            }
        }
    }

    //Continent at least two country
    for (Continent* continent : continents_) {
        if (continent->GetCountries().size() < 2) {
            return 4;
        }
    }

    //Country needs at least 1 connection
    for (Country* country : GenerateCountryList()) {
        if (country->GetConnections().empty()) {
            return 5;
        }
    }

    if (!ConnectedGraph()) {
        return 6;
    }
    return 0;
}

//method to return an error string appropriate to the error code sent in as a parameter
std::string World::ErrorToString(int errorNumber) {
    std::string errorMessage = "";
    if (errorNumber == 1) {
        errorMessage = "***Error: World does not contain any continents!***";
    } else if (errorNumber == 2) {
        errorMessage = "***Error: Multiple continents with the same name!***";
    } else if (errorNumber == 3) {
        errorMessage = "***Error: Multiple countries with the same name!***";
    } else if (errorNumber == 4) {
        errorMessage = "***Error: Continent only contains one or less countries!***";
    } else if (errorNumber == 5) {
        errorMessage = "***Error: Some countries do not have any connections!***";
    } else if (errorNumber == 6) {
        errorMessage = "***Error: The map is not a connected graph!***";
    }
    return errorMessage;
}

//tostring method for world.
void World::ToString() {
    int SIZE = (int)continents_.size();
    std::cout << "The world map contains the following continents: " << std::endl;
    for (int i = 0; i < SIZE; i++) {
        std::cout << continents_[i]->GetName() << std::endl;
    }
}

//increments army count of a coutnry
void World::IncrementCountryArmy(Country* country, int armyIncrement) {
    country->IncrementArmy(armyIncrement); 
    Notify(); 
}

//save method for saving a map to a .map file
//saving in a specific format to allow the load methods to parse properly from a .map file
void World::SaveToFile(World* world) {
    std::ofstream out(world->GetName());
    for (Continent* continent : world->GetContinents()) {
        out << "continent \"" << continent->GetName() << "\" ";
        out << continent->GetControlValue() << std::endl;
    }

    for (Country* country : world->GenerateCountryList()) {
        out << "country \"" << country->GetName() << "\" ";
        out << "\"" << country->GetContinent()->GetName() << "\"" << std::endl;
    }

    for (Country* country : world->GenerateCountryList()) {
        out << "connection \"" << country->GetName() << "\" [";
        for (Country* connection : country->GetConnections()) {
            if (!connection->GetConnections().empty()) {
                out << connection->GetName() << ",";
            }
        }
        out << "]" << std::endl;
    }
    out.close();
}

//this method will be used in 2 seperate occasions, one when the user wants to load a map
//the other when the user wants to load a game in progress, hence the boolean parameter 
World* World::LoadFromFile(std::string fileName, bool loadInProgress) {
    std::ifstream in(fileName);
    World* world = new World(fileName);
    std::string line;
    //making use of maps for loading process
    std::map<std::string, Continent*> continentMap;
    std::map<std::string, Country*> countryMap;
    while (std::getline(in, line)) {
        if (line.find("[World]") != std::string::npos)
            continue;
        if (line.find("continent") != std::string::npos) {
            int continentStart = (int)line.find("\"") + 1;
            int continentEnd = (int)line.find("\"", continentStart) - 1;
            int controlValueStart = continentEnd + 3;
            std::string continentName = line.substr(continentStart, continentEnd - continentStart + 1);
            int controlValue = atoi(line.substr(controlValueStart).c_str());
            Continent* continent = new Continent(continentName, controlValue);
            continentMap[continentName] = continent;
            world->AddContinent(continent);
        }
        else if (line.find("country") != std::string::npos) {
            int countryStart = (int)line.find("\"") + 1;
            int countryEnd = (int)line.find("\"", countryStart) - 1;
            int continentStart = countryEnd + 4;
            int continentEnd = (int)line.find("\"", continentStart) - 1;
            std::string countryName = line.substr(countryStart, countryEnd - countryStart + 1);
            std::string continentName = line.substr(continentStart, continentEnd - continentStart + 1);
            Country* country = new Country(countryName);
            //this if will only be accessed when loading in progress, this will load the information of the country
            //concerning the player owning it as well as the army count.
            if (loadInProgress) {
                int playerNameStart = continentEnd + 4;
                int playerNameEnd = (int)line.find("\"", playerNameStart) - 1;
                std::string playerName = line.substr(playerNameStart, playerNameEnd - playerNameStart + 1);
                int army = atoi(line.substr(playerNameEnd + 2).c_str());
                for (Player* player : RiskGame::Instance().GetPlayers()) {
                    if (player->GetName() == playerName) {
                        country->SetOwner(player);
                        player->AddOwnedCountry(country);
                    }
                }
                country->SetArmy(army);
            }
            countryMap[countryName] = country;
            continentMap[continentName]->AddCountry(country);
        }
        else if (line.find("connection") != std::string::npos) {
            int countryStart = (int)line.find("\"") + 1;
            int countryEnd = (int)line.find("\"", countryStart) - 1;
            int connectionsStart = countryEnd + 4;
            int connectionsEnd = (int)line.length() - 3;
            std::string countryName = line.substr(countryStart, countryEnd - countryStart + 1);
            std::string connectionsNames = line.substr(connectionsStart, connectionsEnd - connectionsStart + 1);
            std::string currentConnection;
            std::istringstream ss(connectionsNames);
            while (std::getline(ss, currentConnection, ',')) {
                Country::Connect(countryMap[countryName], countryMap[currentConnection]);
            }
        }
    }
    in.close();
    return world;
}

//default map, hard coded, mostly used for testing as well as to play the game without creating a new map.
World* World::CreateTestMap() {
    World* world = new World("Default.map");

    Continent* north_america = new Continent("North America", 9);

    //creating all the countries in NA
    Country* alaska = new Country("Alaska");
    Country* northwest_territory = new Country("Northwest Territory");
    Country* alberta = new Country("Alberta");
    Country* ontario = new Country("Ontario");
    Country* greenland = new Country("Greenland");
    Country* eastern_canada = new Country("Eastern Canada");
    Country* western_united_states = new Country("Western United States");
    Country* eastern_united_states = new Country("Eastern United States");
    Country* central_america = new Country("Central America");

    //creating all the edges between the countries in the NA
    Country::Connect(alaska, northwest_territory);
    Country::Connect(alaska, alberta);
    Country::Connect(northwest_territory, alberta);
    Country::Connect(northwest_territory, ontario);
    Country::Connect(northwest_territory, greenland);
    Country::Connect(alberta, ontario);
    Country::Connect(alberta, western_united_states);
    Country::Connect(ontario, greenland);
    Country::Connect(ontario, eastern_canada);
    Country::Connect(ontario, eastern_united_states);
    Country::Connect(ontario, western_united_states);
    Country::Connect(eastern_canada, eastern_united_states);
    Country::Connect(eastern_canada, greenland);
    Country::Connect(eastern_united_states, western_united_states);
    Country::Connect(eastern_united_states, central_america);
    Country::Connect(western_united_states, central_america);

    //adding all the country of NA to continent north_america
    north_america->AddCountry(alaska);
    north_america->AddCountry(northwest_territory);
    north_america->AddCountry(alberta);
    north_america->AddCountry(ontario);
    north_america->AddCountry(greenland);
    north_america->AddCountry(eastern_canada);
    north_america->AddCountry(eastern_united_states);
    north_america->AddCountry(western_united_states);
    north_america->AddCountry(central_america);

    //adding north_america to the map
    world->AddContinent(north_america);

    Continent* south_america = new Continent("South America", 4);

    //creating all the countries in SA
    Country* venezuela = new Country("Venezuela");
    Country* brazil = new Country("Brazil");
    Country* peru = new Country("Peru");
    Country* argentina = new Country("Argentina");

    //creating all of the edges between the countries in SA
    Country::Connect(central_america, venezuela);
    Country::Connect(venezuela, peru);
    Country::Connect(venezuela, brazil);
    Country::Connect(brazil, peru);
    Country::Connect(brazil, argentina);
    Country::Connect(argentina, peru);

    //adding all the countries in SA to south_america continent
    south_america->AddCountry(venezuela);
    south_america->AddCountry(brazil);
    south_america->AddCountry(peru);
    south_america->AddCountry(argentina);

    //adding south_america to the world map
    world->AddContinent(south_america);

    Continent* europe = new Continent("Europe", 7);

    //creating all the countries in Europe
    Country* iceland = new Country("Iceland");
    Country* great_britain = new Country("Great Britain");
    Country* western_europe = new Country("Western Europe");
    Country* scandinavia = new Country("Scandinavia");
    Country* northern_europe = new Country("Northern Europe");
    Country* southern_europe = new Country("Southrern Europe");
    Country* russia = new Country("Russia");

    //creating all the edges between the europe countries
    Country::Connect(greenland, iceland);
    Country::Connect(iceland, great_britain);
    Country::Connect(iceland, scandinavia);
    Country::Connect(great_britain, scandinavia);
    Country::Connect(great_britain, western_europe);
    Country::Connect(great_britain, northern_europe);
    Country::Connect(western_europe, southern_europe);
    Country::Connect(western_europe, northern_europe);
    Country::Connect(scandinavia, russia);
    Country::Connect(scandinavia, northern_europe);
    Country::Connect(northern_europe, southern_europe);
    Country::Connect(northern_europe, russia);
    Country::Connect(southern_europe, russia);

    //adding all the europe countries to the europe continent
    europe->AddCountry(iceland);
    europe->AddCountry(great_britain);
    europe->AddCountry(scandinavia);
    europe->AddCountry(western_europe);
    europe->AddCountry(northern_europe);
    europe->AddCountry(southern_europe);
    europe->AddCountry(russia);

    //adding europe to the world map
    world->AddContinent(europe);

    Continent* africa = new Continent("Africa", 6);

    //creating all the countries in africa
    Country* north_africa = new Country("North Africa");
    Country* egypt = new Country("Egypt");
    Country* east_africa = new Country("East Africa");
    Country* central_africa = new Country("Central Africa");
    Country* south_africa = new Country("South Africa");
    Country* madagascar = new Country("Madagascar");

    //creating all the edges between the countries in africa
    Country::Connect(north_africa, brazil);
    Country::Connect(north_africa, egypt);
    Country::Connect(north_africa, central_africa);
    Country::Connect(north_africa, east_africa);
    Country::Connect(north_africa, western_europe);
    Country::Connect(north_africa, southern_europe);
    Country::Connect(egypt, southern_europe);
    Country::Connect(egypt, east_africa);
    Country::Connect(east_africa, central_africa);
    Country::Connect(east_africa, madagascar);
    Country::Connect(east_africa, south_africa);
    Country::Connect(south_africa, central_africa);
    Country::Connect(south_africa, madagascar);

    //adding all the countries to the africa continent
    africa->AddCountry(north_africa);
    africa->AddCountry(egypt);
    africa->AddCountry(east_africa);
    africa->AddCountry(central_africa);
    africa->AddCountry(south_africa);
    africa->AddCountry(madagascar);

    //adding africa to the world map
    world->AddContinent(africa);

    Continent* asia = new Continent("Asia", 12);

    //creating all the countries in Asia
    Country* middle_east = new Country("Middle East");
    Country* afghanistan = new Country("Afghanistan");
    Country* ural = new Country("Ural");
    Country* siberia = new Country("Siberia");
    Country* yakutsk = new Country("Yakutsk");
    Country* kamchatka = new Country("Kamchatka");
    Country* irkutsk = new Country("Irkutsk");
    Country* mongolia = new Country("Mongolia");
    Country* japan = new Country("Japan");
    Country* china = new Country("China");
    Country* southeast_asia = new Country("Southeast Asia");
    Country* india = new Country("India");

    //creating all the edges between the countries in Asia
    Country::Connect(middle_east, east_africa);
    Country::Connect(middle_east, southern_europe);
    Country::Connect(middle_east, russia);
    Country::Connect(afghanistan, russia);
    Country::Connect(ural, russia);
    Country::Connect(kamchatka, alaska);
    Country::Connect(middle_east, afghanistan);
    Country::Connect(middle_east, india);
    Country::Connect(afghanistan, india);
    Country::Connect(afghanistan, ural);
    Country::Connect(afghanistan, china);
    Country::Connect(ural, china);
    Country::Connect(ural, siberia);
    Country::Connect(siberia, yakutsk);
    Country::Connect(siberia, china);
    Country::Connect(siberia, irkutsk);
    Country::Connect(siberia, mongolia);
    Country::Connect(yakutsk, kamchatka);
    Country::Connect(yakutsk, irkutsk);
    Country::Connect(kamchatka, irkutsk);
    Country::Connect(kamchatka, japan);
    Country::Connect(kamchatka, mongolia);
    Country::Connect(mongolia, irkutsk);
    Country::Connect(mongolia, japan);
    Country::Connect(mongolia, china);
    Country::Connect(china, southeast_asia);

    //adding all the countries in asia into the asia continent
    asia->AddCountry(middle_east);
    asia->AddCountry(afghanistan);
    asia->AddCountry(ural);
    asia->AddCountry(siberia);
    asia->AddCountry(yakutsk);
    asia->AddCountry(kamchatka);
    asia->AddCountry(irkutsk);
    asia->AddCountry(mongolia);
    asia->AddCountry(japan);
    asia->AddCountry(china);
    asia->AddCountry(india);
    asia->AddCountry(southeast_asia);

    //adding asia to the world map
    world->AddContinent(asia);

    Continent* australia = new Continent("Australia", 4);

    //creating all the countries in Australia
    Country* indonesia = new Country("Indonesia");
    Country* new_guinea = new Country("New Guinea");
    Country* western_australia = new Country("Western Australia");
    Country* eastern_australia = new Country("Eastern Australia");

    //creating all the edges between the countries in australia
    Country::Connect(indonesia, southeast_asia);
    Country::Connect(indonesia, new_guinea);
    Country::Connect(indonesia, western_australia);
    Country::Connect(new_guinea, western_australia);
    Country::Connect(new_guinea, eastern_australia);
    Country::Connect(eastern_australia, western_australia);

    //adding all the countries in australia to the continent australia
    australia->AddCountry(indonesia);
    australia->AddCountry(new_guinea);
    australia->AddCountry(western_australia);
    australia->AddCountry(eastern_australia);

    //adding australia to the world map
    world->AddContinent(australia);
    return world;
}
