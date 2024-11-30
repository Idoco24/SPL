#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../include/Simulation.h"
#include "../include/Settlement.h"
#include "../include/Auxiliary.h"
#include "../include/Action.h"
using std::string;
using std::vector;

Simulation* Simulation::backup = nullptr;

Simulation::Simulation(const string &configFilePath): isRunning(true), planCounter(0){
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cerr << "Error opening configuration file: " << configFilePath << std::endl;
        return;
    }

    string line;
    while (std::getline(configFile, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip comments and empty lines
        auto args = Auxiliary::parseArguments(line);
        if (args[0] == "settlement") {
            // Add settlement
            SettlementType type = static_cast<SettlementType>(std::stoi(args[2]));
            addSettlement(new Settlement(args[1], type));
        } else if (args[0] == "facility") {
            // Add facility type
            FacilityCategory category = static_cast<FacilityCategory>(std::stoi(args[2]));
            facilitiesOptions.emplace_back(args[1], category, std::stoi(args[3]), std::stoi(args[4]), std::stoi(args[5]), std::stoi(args[6]));
        } else if (args[0] == "plan") {
            // Add plan
            SelectionPolicy *policy = nullptr;
            if (args[2] == "eco") {
                policy = new EconomySelection();
            } else if (args[2] == "bal") {
                policy = new BalancedSelection(0, 0, 0);
            } else if (args[2] == "sus") {
                policy = new SustainabilitySelection();
            } else if (args[2] == "nve") {
                policy = new NaiveSelection();
            }
            else{
                std::cout << "Unknown selection policy" << std::endl;
            }
            addPlan(getSettlement(args[1]), policy);
        }
    }
    configFile.close();
}

Simulation::~Simulation() {
    for (auto settlement :settlements){
        delete settlement;
    }
    for (auto action : actionsLog){
        delete action;
    }
    
    /* 
    
    No need to delete facilitiesOptions as they are not dynamically allocated
    Plans will handle their own memory management
    
    for (auto facility : facilitiesOptions){
        delete &facility;
    }
    for (auto plan : plans){
        delete &plan;
    }
    */ 
}

Simulation::Simulation(const Simulation &other): isRunning(other.isRunning), planCounter(other.planCounter){
    for (auto settlement : other.settlements){
        settlements.push_back(new Settlement(*settlement));
    }
    for (auto action : other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    for (auto facility : other.facilitiesOptions){
        facilitiesOptions.push_back(facility);
    }
    for (auto plan : other.plans){
        plans.push_back(plan);
    }
}

Settlement &Simulation::getSettlement(const string &settlementName){
    for (auto settlement : settlements){
        if (settlement->getName() == settlementName){
            return *settlement;
        }
    }
    std::cout << "Settlement does not exist" << std::endl;
    throw std::runtime_error("Settlement does not exist");
}

Simulation &Simulation::operator=(const Simulation &other){
    if (this == &other){
        return *this;
    }
    for (auto settlement : settlements){
        delete settlement;
    }
    for (auto action : actionsLog){
        delete action;
    }
    settlements.clear();
    actionsLog.clear();
    facilitiesOptions.clear();
    plans.clear();
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    for (auto settlement : other.settlements){
        settlements.push_back(new Settlement(*settlement));
    }
    for (auto action : other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    for (auto facility : other.facilitiesOptions){
        facilitiesOptions.push_back(facility);
    }
    for (auto plan : other.plans){
        plans.push_back(plan);
    }
    return *this;
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    std::cout << "Simulation::addPlan called with selectionPolicy: " << selectionPolicy << std::endl;
    plans.emplace_back(planCounter, settlement, selectionPolicy, facilitiesOptions);
    planCounter++;
}

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}

void Simulation::step(){
    for (auto &plan : plans){
        plan.step();
    }
}

bool Simulation::addSettlement(Settlement *settlement){
    if (isSettlementExists(settlement->getName())){
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility){
    if (facility.getCost() < 0 || facility.getLifeQualityScore() < 0 || facility.getEconomyScore() < 0 || facility.getEnvironmentScore() < 0){
        return false;
    }

    if ((int)facility.getCategory() < 0 || (int)facility.getCategory() > 2){
        return false;
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName){
    for (auto settlement : settlements){
        if (settlement->getName() == settlementName){
            return true;
        }
    }
    return false;
}

Plan &Simulation::getPlan(const int planID){
    for (auto &plan : plans){
        if (plan.getPlanId() == planID){
            return plan;
        }
    }
    std::cout << "Plan does not exist" << std::endl;
    throw std::runtime_error("Plan does not exist");
}

void Simulation::close(){
    isRunning = false;
}

void Simulation::open(){
    isRunning = true;
}


void Simulation::start(){
    open();
    std::cout << "The simulation has started." << std::endl;
    string command;
    
    while (isRunning) {
        std::getline(std::cin, command);
        auto args = Auxiliary::parseArguments(command);
        if (args.empty()) {
            continue;
        }

        string requestedAction = args[0];
        BaseAction *action = nullptr;

        if (requestedAction == "settlement" && args.size() == 3) {
            SettlementType type = static_cast<SettlementType>(std::stoi(args[2]));
            action = new AddSettlement(args[1], type);
        } else if (requestedAction == "facility" && args.size() == 7) {
            FacilityCategory category = static_cast<FacilityCategory>(std::stoi(args[2]));
            action = new AddFacility(args[1], category, std::stoi(args[3]), std::stoi(args[4]), std::stoi(args[5]), std::stoi(args[6]));
        } else if (requestedAction == "plan" && args.size() == 3) {
            action = new AddPlan(args[1], args[2]);
        } else if (requestedAction == "planStatus" && args.size() == 2) {
            action = new PrintPlanStatus(std::stoi(args[1]));
        } else if (requestedAction == "changePolicy" && args.size() == 3) {
            action = new ChangePlanPolicy(std::stoi(args[1]), args[2]);
        } else if (requestedAction == "step" && args.size() == 2) {
            action = new SimulateStep(std::stoi(args[1]));
        } else if (requestedAction == "log" && args.size() == 1) {
            action = new PrintActionsLog();
        } else if (requestedAction == "close" && args.size() == 1) {
            action = new Close();
        } else if (requestedAction == "backup" && args.size() == 1) {
            action = new BackupSimulation();
        } else if (requestedAction == "restore" && args.size() == 1) {
            action = new RestoreSimulation();
        } else if (requestedAction == "exit" && args.size() == 1) {
            break;
        } else {
            std::cout << "Unknown command or incorrect number of arguments." << std::endl;
            continue;
        }

        if (action != nullptr) {
            action->act(*this);
            addAction(action);
        }
    }
}

vector<BaseAction*> Simulation::getActionsLog(){
    return actionsLog;
}

void Simulation::createBackup(){
    if (backup != nullptr) {
        delete backup;
    }
    backup = new Simulation(*this);
}

void Simulation::getRestore(){
    if (backup == nullptr){
        std::cerr << "No backup available" << std::endl;
        return;
    }
    *this = *backup;
}