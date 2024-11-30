#include "../include/SelectionPolicy.h"
#include "../include/Facility.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <stdexcept>
using std::vector;
using std::string;

// NaiveSelection class implementation

NaiveSelection::NaiveSelection() : lastSelectedIndex(0) 
{
}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    std::cout << "NaiveSelection::selectFacility called" << std::endl;
    if (facilitiesOptions.empty()) {
        std::cerr << "Error: No facilities available for selection" << std::endl;
        throw std::runtime_error("No facilities available for selection");
    }
    if (lastSelectedIndex >= facilitiesOptions.size()) {
        std::cerr << "Error: lastSelectedIndex out of bounds" << std::endl;
        throw std::runtime_error("lastSelectedIndex out of bounds");
    }
    std::cout << "Selecting facility at index: " << lastSelectedIndex << std::endl;
    const FacilityType& selectedFacility = facilitiesOptions[lastSelectedIndex];
    std::cout << "Selected facility: " << selectedFacility.getName() << std::endl;
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return selectedFacility;
}

const string NaiveSelection::toString() const 
{
    return "nve"; 
}

SelectionPolicy* NaiveSelection::clone() const 
{
    return new NaiveSelection(*this);
}

// BalancedSelection class implementation

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) 
    : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) 
{
}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) 
{
    FacilityType* selectedFacility = nullptr;
    int minDifference = std::numeric_limits<int>::max();

    for (const FacilityType& facility : facilitiesOptions) 
    {
        int lifeQualityScore = facility.getLifeQualityScore() + LifeQualityScore;
        int economyScore = facility.getEconomyScore() + EconomyScore;
        int environmentScore = facility.getEnvironmentScore() + EnvironmentScore;

        int scores[] = {lifeQualityScore, economyScore, environmentScore};
        int maxScore = *std::max_element(std::begin(scores), std::end(scores));
        int minScore = *std::min_element(std::begin(scores), std::end(scores));
        int difference = maxScore - minScore;

        if (difference < minDifference) 
        {
            minDifference = difference;
            selectedFacility = const_cast<FacilityType*>(&facility);
        }
    }

    if (selectedFacility == nullptr) 
    {
        throw std::runtime_error("No facility found.");
    }

    return *selectedFacility;
}

const string BalancedSelection::toString() const 
{
    return "bal"; 
}

SelectionPolicy* BalancedSelection::clone() const 
{
    return new BalancedSelection(*this);
}

// EconomySelection class implementation

EconomySelection::EconomySelection() : lastSelectedIndex(0) 
{
}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) 
{
    const FacilityType* selectedFacility = nullptr;

    for (int i = lastSelectedIndex; i < lastSelectedIndex + facilitiesOptions.size(); i++) 
    {
        int index = i % facilitiesOptions.size();
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY) 
        {
            selectedFacility = &facilitiesOptions[index];
            lastSelectedIndex = (index + 1) % facilitiesOptions.size();
            break;
        }
    }

    if (selectedFacility == nullptr) 
    {
        throw std::runtime_error("No facility in the ECONOMY category found.");
    }

    return *selectedFacility;
}

const string EconomySelection::toString() const 
{
    return "eco"; 
}

SelectionPolicy* EconomySelection::clone() const 
{
    return new EconomySelection(*this);
}

// SustainabilitySelection class implementation

SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(0) 
{
}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) 
{
    const FacilityType* selectedFacility = nullptr;

    for (int i = lastSelectedIndex; i < lastSelectedIndex + facilitiesOptions.size(); i++) 
    {
        int index = i % facilitiesOptions.size();
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ENVIRONMENT) 
        {
            selectedFacility = &facilitiesOptions[index];
            lastSelectedIndex = (index + 1) % facilitiesOptions.size();
            break;
        }
    }

    if (selectedFacility == nullptr) 
    {
        throw std::runtime_error("No facility in the ENVIRONMENT category found.");
    }

    return *selectedFacility;
}

const string SustainabilitySelection::toString() const 
{
    return "env"; 
}

SelectionPolicy* SustainabilitySelection::clone() const 
{
    return new SustainabilitySelection(*this);
}