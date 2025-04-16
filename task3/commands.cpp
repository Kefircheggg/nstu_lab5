#include "commands.h"
#include <vector>
#include <map>
#include <iostream>
using namespace std;

void createTrain(unordered_map<string, vector<string>>& towns, unordered_map<string, vector<string>>& trains, string trainName, vector<string> trainTowns) {
    trains[trainName] = trainTowns;

    for (auto& town : trainTowns) {
        towns[town].push_back(trainName);
    }
}

vector<string> getTrainsForTown(unordered_map<string, vector<string>>& towns, unordered_map<string, vector<string>>& trains, string townName) {
    if (towns.find(townName) == towns.end()) { return {"notown"}; }
    return towns[townName];
}

unordered_map<string, vector<string>> getTownsForTrain(unordered_map<string, vector<string>>& towns, unordered_map<string, vector<string>>& trains, string trainName) {
    unordered_map<string, vector<string>> townsForTrain;
    if (trains.find(trainName) == trains.end()) { return townsForTrain; }

    for (auto& town : trains[trainName]) {
        for (auto& train : towns[town]) {
            if (train != trainName) {
                townsForTrain[town].push_back(train);
            }
        }
    }
    return townsForTrain;
}

void printTrainsList(unordered_map<string, vector<string>>& trains) {

    for (auto& [train, towns] : trains) {
        cout << ">>> " << train << ": ";
        for (auto& town : towns) {
            cout << town << " ";
        }
        cout << endl;
    }
}

