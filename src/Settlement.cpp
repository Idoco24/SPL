// #pragma once
#include "../include/Settlement.h"
#include <string>
#include <vector>
using std::string;
using std::vector;



Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type) {}

const string &Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

const string Settlement::toString() const {
    string typeStr;
    switch (type) {
        case SettlementType::VILLAGE:
            typeStr = "Village";
            break;
        case SettlementType::CITY:
            typeStr = "City";
            break;
        case SettlementType::METROPOLIS:
            typeStr = "Metropolis";
            break;
    }
    return name + " (" + typeStr + ")";
}