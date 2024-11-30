#include "../include/Action.h"
#include "../include/Simulation.h"
#include <iostream>

void AddSettlement::act(Simulation &simulation) {
    if (simulation.isSettlementExists(settlementName)) {
        error("Settlement already exists");
        return;
    }
    simulation.addSettlement(new Settlement(settlementName, settlementType));
    complete();
}

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName),
                                                                                           settlementType(settlementType) {}

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const {
    return "addSettlement " + settlementName + " " + std::to_string((int) settlementType);
}


void AddPlan::act(Simulation &simulation) {
    if (!simulation.isSettlementExists(settlementName)) {
        std::cerr << "Error: Settlement does not exist" << std::endl;
        return;
    }

    SelectionPolicy *policy = nullptr;
    if (selectionPolicy == "nve") {
        policy = new NaiveSelection();
    } else if (selectionPolicy == "eco") {
        policy = new EconomySelection();
    } else if (selectionPolicy == "bal") {
        policy = new BalancedSelection(0, 0, 0);
    } else {
        std::cerr << "Error: Unknown selection policy" << std::endl;
        return;
    }

    std::cout << "Adding plan with selection policy: " << selectionPolicy << " at address: " << policy << std::endl;
    simulation.addPlan(simulation.getSettlement(settlementName), policy);
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName),
                                                                                 selectionPolicy(selectionPolicy) {}

AddPlan *AddPlan::clone() const {

    return new AddPlan(*this);
}

const string AddPlan::toString() const {
    return "addPlan " + settlementName + " " + selectionPolicy;
}

void SimulateStep::act(Simulation &simulation) {
    simulation.step();
    complete();
}

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

const string SimulateStep::toString() const {
    return "simulateStep " + std::to_string(numOfSteps);
}

void PrintPlanStatus::act(Simulation &simulation) {
    try {
        Plan &plan = simulation.getPlan(planId);
        std::cout << "Plan ID: " << plan.getPlanId() << std::endl
                  << "Settlement name: " << plan.getSettlement().getName() << std::endl
                  << "Life quality score: " << plan.getlifeQualityScore() << std::endl
                  << "Economy score: " << plan.getEconomyScore() << std::endl
                  << "Environment score: " << plan.getEnvironmentScore() << std::endl;
        complete();
    } catch (...) {
        error("Plan does not exist");
    }
}


PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const {
    return "printPlanStatus " + std::to_string(planId);
}


void AddFacility::act(Simulation &simulation) {
    if (price < 0 || lifeQualityScore < 0 || economyScore < 0 || environmentScore < 0) {
        error("Invalid facility parameters");
        return;
    }
    if ((int) facilityCategory < 0 || (int) facilityCategory > 2) {
        error("Invalid facility category");
        return;
    }
    simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore,
                                        environmentScore));
    complete();
}


AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price,
                         const int lifeQualityScore, const int economyScore, const int environmentScore)
        : facilityName(facilityName), facilityCategory(facilityCategory), price(price),
          lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}


AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}



const string AddFacility::toString() const {
    return "addFacility " + facilityName + " " + std::to_string((int) facilityCategory) + " " + std::to_string(price) +
           " " + std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " +
           std::to_string(environmentScore);
}


void ChangePlanPolicy::act(Simulation &simulation) {
    try {
        Plan &plan = simulation.getPlan(planId);
        SelectionPolicy *selectionPolicy = nullptr;
        if (newPolicy == "nve") {
            selectionPolicy = new NaiveSelection();
        } else if (newPolicy == "eco") {
            selectionPolicy = new EconomySelection();
        } else if (newPolicy == "env") {
            selectionPolicy = new SustainabilitySelection();
        } else if (newPolicy == "bal") {
            selectionPolicy = new BalancedSelection(0, 0, 0);
        } else {
            error("Unknown selection policy");
            return;
        }
        plan.setSelectionPolicy(selectionPolicy);
        complete();
    } catch (...) {
        error("Plan does not exist");
    }
}


ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) : planId(planId), newPolicy(newPolicy) {} 

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const {
    return "changePlanPolicy " + std::to_string(planId) + " " + newPolicy;
}

void PrintActionsLog::act(Simulation &simulation) {
    
    string actionStatus;
    for (auto action : simulation.getActionsLog()) {
        if((int)action->getStatus() == 0) {
            actionStatus = "COMPLETED";
        } else {
            actionStatus = "ERROR";
        }
        std::cout << action->toString() << " " << actionStatus << std::endl;
    }
    complete();
}

PrintActionsLog::PrintActionsLog() {}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const {
    return "printActionsLog";
}


void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

Close::Close() {}

Close *Close::clone() const {
    return new Close(*this);
}

const string Close::toString() const {
    return "close";
}

void BackupSimulation::act(Simulation &simulation) {
    simulation.createBackup();
    complete();
}


BackupSimulation::BackupSimulation() {}

BackupSimulation *BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}


const string BackupSimulation::toString() const {
    return "backup";
}

void RestoreSimulation::act(Simulation &simulation) {
    simulation.getRestore();
    complete();
}

RestoreSimulation::RestoreSimulation() {}

RestoreSimulation *RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const {
    return "restore";
}

void error(const string &message) {
    std::cerr << "Error: " << message << std::endl;
}


void complete() {
    std::cout << "Action completed" << std::endl;
}

void BaseAction::act(Simulation &simulation) {
    std::cout << "Base action" << std::endl;
}


const string BaseAction::toString() const {
    return "Base action";
}


ActionStatus BaseAction::getStatus() const {
    return status;
}


void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

const string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

BaseAction::BaseAction() : status(ActionStatus::ERROR) {}

