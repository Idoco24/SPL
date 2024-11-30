#include <string>
#include <vector>
#include <iostream>
#include "../include/Facility.h"
using std::string;
using std::vector;

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name),
    category(category),
    price(price),
    lifeQuality_score(lifeQuality_score),
    economy_score(economy_score),
    environment_score(environment_score) {}

const string &FacilityType::getName() const { 
    return name;
}

int FacilityType::getCost() const {
    return price;
}

int FacilityType::getLifeQualityScore() const {
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const {
    return environment_score;
}

int FacilityType::getEconomyScore() const {
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const {
    return category;
}

// Facility class implementation
Facility::Facility(const string &name, const string &settlementName, int category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, intToFacilityCategory(category), price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(3) {}

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

const string &Facility::getSettlementName() const {
    return settlementName;
}

const int Facility::getTimeLeft() const {
    return timeLeft;
}

FacilityStatus Facility::step() {
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS) {
        timeLeft--;
        if (timeLeft == 0) {
            status = FacilityStatus::OPERATIONAL;
        }
    }
    return status;
}

void Facility::setStatus(FacilityStatus status) {
    this->status = status;
}

const FacilityStatus& Facility::getStatus() const {
    return status;
}

const string Facility::toString() const {
    string statusStr = (status == FacilityStatus::UNDER_CONSTRUCTIONS) ? "UNDER_CONSTRUCTIONS" : "OPERATIONAL";
    return "Facility: " + getName() + " in " + getSettlementName() + " is " + statusStr + " and has " + std::to_string(getTimeLeft()) + " time left";
}

FacilityCategory Facility::intToFacilityCategory(int category) {
    switch (category) {
        case 0:
            return FacilityCategory::LIFE_QUALITY;
        case 1:
            return FacilityCategory::ECONOMY;
        case 2:
            return FacilityCategory::ENVIRONMENT;
        default:
            return FacilityCategory::LIFE_QUALITY;
    }
}

string Facility::statusToString() const {
    switch (status) {
        case FacilityStatus::UNDER_CONSTRUCTIONS:
            return "UNDER_CONSTRUCTIONS";
        case FacilityStatus::OPERATIONAL:
            return "OPERATIONAL";
        default:
            return "UNKNOWN";
    }
}