#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
using std::string;
using std::vector;
#include "../include/Plan.h"
#include "../include/Facility.h" // Include the full definition of Facility

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE), facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0){
    std::cout << "Plan created with selectionPolicy: " << selectionPolicy << std::endl;
}

Plan::~Plan()
{
    for (Facility *facility : facilities)
    {
        delete facility;
    }
    for (Facility *facility : underConstruction)
    {
        delete facility;
    }

    if (selectionPolicy != nullptr)
    {
        delete selectionPolicy;
    }
}

const Settlement &Plan::getSettlement() const
{
    return settlement;
}

const int Plan::getPlanId() const
{
    return plan_id;
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}   

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    if(this->selectionPolicy != nullptr)
    {
        delete this->selectionPolicy;
    }
    this->selectionPolicy = selectionPolicy;
}

const PlanStatus Plan::getStatus() const
{
    if ((int)settlement.getType()+1 - underConstruction.size() > 0 )
    {
        return PlanStatus::AVALIABLE;
    }
    else
    {
        return PlanStatus::BUSY;
    }
}

void Plan::addFacility(Facility* facility)
{
    underConstruction.push_back(facility);
    facility->setStatus(FacilityStatus::UNDER_CONSTRUCTIONS);
    life_quality_score += facility->getLifeQualityScore();
    economy_score += facility->getEconomyScore();
    environment_score += facility->getEnvironmentScore();
}

void Plan::step(){
    std::cout << "Plan::step() called" << std::endl;
    std::cout << "Plan status: " << statusToString() << std::endl;

    if (this-> getStatus() == PlanStatus::AVALIABLE){
        this-> status = PlanStatus::AVALIABLE;
        if (selectionPolicy == nullptr) {
            std::cerr << "Error: selectionPolicy is null" << std::endl;
            return;
        }
        std::cout << "Selecting facility..." << std::endl;
        std::cout << "Number of facility options: " << facilityOptions.size() << std::endl;
        for (const auto& facility : facilityOptions) {
            std::cout << "Facility: " << facility.getName() << std::endl;
        }
        std::cout << "selectionPolicy address: " << selectionPolicy << std::endl;
        const FacilityType &selectedFacilityType = selectionPolicy->selectFacility(facilityOptions);
        std::cout << "Facility selected: " << selectedFacilityType.getName() << std::endl;
        Facility* selectedFacility = new Facility(selectedFacilityType, settlement.getName());
        this -> addFacility(selectedFacility);
    }

    int count = 0;
    for (Facility *facility : underConstruction){   
        facility -> step();
        if (facility->getStatus() == FacilityStatus::OPERATIONAL){
            facilities.push_back(facility);
            underConstruction.erase(underConstruction.begin() + count);
        }
        else{
            count++;
        }   
    }
    if (this-> getStatus() == PlanStatus::BUSY){
        this-> status = PlanStatus::BUSY;
        return;
    }
}

string Plan::statusToString() const
{
    if (status == PlanStatus::AVALIABLE)
    {
        return "AVALIABLE";
    }
    else
    {
        return "BUSY";
    }
}

void Plan::printStatus()
{
    std::cout << "PlanID " << plan_id << std::endl;
    std::cout << "SettlementName: " << settlement.getName() << std::endl;
    std::cout << "PlanStatus: " << this->statusToString() << std::endl;
    std::cout << "SelectionPolicy: " << selectionPolicy->toString() << std::endl;
    std::cout << "LifeQualityscore: " << life_quality_score << std::endl;
    std::cout << "EconomyScore: " << economy_score << std::endl;
    std::cout << "EnvironmentScore: " << environment_score << std::endl;
    
    for (Facility *facility : facilities)
    {
        std::cout << "FacilityName: "<< facility->getName()<< std::endl;
        std::cout << "FacilityStatus: "<< facility->statusToString() << std::endl;
    }
    for (Facility *facility : underConstruction)
    {
        std::cout << "FacilityName: "<< facility->getName()<< std::endl;
        std::cout << "FacilityStatus: "<< facility->statusToString() << std::endl;
    }
}

const vector<Facility*>& Plan::getFacilities() const
{
    return facilities;
}