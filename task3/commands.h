#include <map>
#include <vector>
#include <string>
using namespace std;

#ifndef COMMANDS_H
#define COMMANDS_H
enum class Type {
    CREATE_TRAIN,
    TRAINS_FOR_TOWN,
    TOWNS_FOR_TRAIN,
    TRAINS
};
void createTrain(unordered_map<string, vector<string>>& towns, unordered_map<string, vector<string>>& trains, string trainName, vector<string> trainTowns);
vector<string> getTrainsForTown(unordered_map<string, vector<string>>& towns, unordered_map<string, vector<string>>& trains, string townName);
unordered_map<string, vector<string>> getTownsForTrain(unordered_map<string, vector<string>>& towns, unordered_map<string, vector<string>>& trains, string trainName);
void printTrainsList(unordered_map<string, vector<string>>& trains);
#endif //COMMANDS_H