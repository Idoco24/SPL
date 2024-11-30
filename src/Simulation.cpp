#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../include/Simulation.h"
#include "../include/Auxiliary.h"
#include "../include/Action.h"
using std::string;
using std::vector;

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
                policy = new BalancedSelection(3, 3, 3); // Example scores
            } else if (args[2] == "sus") {
                policy = new SustainabilitySelection();
            }
            addPlan(getSettlement(args[1]), policy);
        }
    }
    configFile.close();
}

void Simulation::start()
{   std::cout << "The simulation has started." << std::endl;
    
    string command;

    while(isRunning){
        std::getline(std::cin, command);

        auto args = Auxiliary::parseArguments(command);
        if(args.size() == 0){
            continue;
        }
        
        else if (args[0] == "close"){
            close();                    // Close Summery + Delete all children.
            isRunning = false;
        }

        else if (args[0] == "step"){
            int steps = std::stoi(args[1]);
            for (int i=0; i<steps; i++){
                step();
            }

            if (args[1].empty() || args.size() != 2){
                std::cout << "Usage: step <number of steps>" << std::endl;
            }
        }

        else if (args[0] == "plan"){
            string settlementName = args[1];
            SelectionPolicy *selectionPolicy = nullptr;
            
            if (args.size() != 3){
                std::cout << "Usage: plan <Settlement_name> <Selection_policy>" << std::endl;
                continue;
            }
            
            if (!isSettlementExists(settlementName)){
                std::cout << "Error: Settlement " << settlementName << " does not exist." << std::endl;
                continue;
            }
            
            
            if(args[2] == "nve"){
                selectionPolicy = new NaiveSelection();
            }
            else if(args[2] == "bal"){
                selectionPolicy = new BalancedSelection(0,0,0);
            }
            else if(args[2] == "eco"){
                selectionPolicy = new EconomySelection();
            }
            else if(args[2] == "env"){
                selectionPolicy = new SustainabilitySelection();
            }
            else{
                std::cout << "Error: Unknown selection policy" << std::endl;
                continue;
            } 
                
            addPlan( getSettlement(settlementName), selectionPolicy);
        
        }
            
        else if (args[0] == "settlement"){
            if (args.size() != 3){
                std::cout << "Usage: settlement <Settlement_name> <Settlement_type>" << std::endl;
                continue;

            if (isSettlementExists(args[1])){
                std::cout << "Error: Settlement " << args[1] << " already exists." << std::endl;
                continue;
            }

            string settlementName = args[1];
            
            if (args[2] == "0"){
                addSettlement(new Settlement(settlementName, SettlementType::VILLAGE));
            }
            else if (args[2] == "1"){
                addSettlement(new Settlement(settlementName, SettlementType::CITY));
            }
            else if (args[2] == "2"){
                addSettlement(new Settlement(settlementName, SettlementType::METROPOLIS));
            }
            
            else{
                std::cout << "Error: Unknown settlement type" << std::endl;
                continue;
            }
            continue;
            
        }
        else if (args[0]=="facility"){
            if (args.size() != 7){
                std::cout << "Usage: facility <Facility_name> <Facility_category> <Price> <Life_quality_score> <Economoy_score> <Environment_score>" << std::endl;
                continue;
            }

            FacilityCategory category;

            if (args[2] == "0"){
                category = FacilityCategory::LIFE_QUALITY;
            }
            else if (args[2] == "1"){
                category = FacilityCategory::ECONOMY;
            }
            else if (args[2] == "2"){
                category = FacilityCategory::ENVIRONMENT;
            }
            else{
                std::cout << "Error: Unknown facility category" << std::endl;
                continue;
            }

            if (stoi(args[3]) < 0 || stoi(args[4]) < 0 || stoi(args[5]) < 0 || stoi(args[6]) < 0){
                std::cout << "Error: Negative facility values are not allowed" << std::endl;
                continue;
            }

            addFacility(FacilityType(args[1], category, stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]));
        }

        else if (args[0] == "planStatus"){
            if (args.size() != 2){
                std::cout << "Usage: planStatus <Plan_ID>" << std::endl;
                continue;
            }

            else if(stoi(args[1]) < 0 || stoi(args[1]) > plans.size()){
                std::cout << "Error: Plan ID " << args[1] << " does not exist." << std::endl;
                continue;
            }
            getPlan(stoi(args[1])).printStatus();
        }

        else if (args[0]== "changePolicy"){
            if (args.size() != 3){
                std::cout << "Usage: changePolicy <Plan_ID> <Selection_policy>" << std::endl;
                continue;
            }


            else if(stoi(args[1]) < 0 || stoi(args[1]) > plans.size()){
                std::cout << "Error: Plan ID " << args[1] << " does not exist." << std::endl;
                continue;
            }

            SelectionPolicy *selectionPolicy = nullptr;
            if(args[2] == "nve"){
                selectionPolicy = new NaiveSelection();
            }
            else if(args[2] == "bal"){
                selectionPolicy = new BalancedSelection(0,0,0);
            }
            else if(args[2] == "eco"){
                selectionPolicy = new EconomySelection();
            }
            else if(args[2] == "env"){
                selectionPolicy = new SustainabilitySelection();
            }
            else{
                std::cout << "Error: Unknown selection policy" << std::endl;
                continue;
            }

            getPlan(stoi(args[1])).setSelectionPolicy(selectionPolicy);
        }

        else if(args[0]=="log"){
            for (auto action : actionsLog){
                std::cout<< action->toString() << std::endl;
            }
        }

        else{
            std::cout << "Error: Unknown command" << std::endl;
        }

    }
}
