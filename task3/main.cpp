#include <iostream>
#include <vector>
#include "commands.h"
using namespace std;


vector<string> splitString(string s, const string& divider){
    vector<string> res;
    size_t pos = 0;
    while ((pos = s.find(divider)) != string::npos) {
        res.push_back(s.substr(0, pos));
        s.erase(0, pos + divider.length());
    }
    res.push_back(s); // остаток строки
    return res;
}

Type getCommand(const string& text) {
    const map<string, Type> commands = {
        {"CREATE_TRAIN", Type::CREATE_TRAIN},
        {"TRAINS_FOR_TOWN", Type::TRAINS_FOR_TOWN},
        {"TOWNS_FOR_TRAIN", Type::TOWNS_FOR_TRAIN},
        {"TRAINS", Type::TRAINS},
    };
    return commands.find(text)->second;
}

int main() {
    unordered_map<string, vector<string>> towns;
    unordered_map<string, vector<string>> trains;

    string command;
    while (cout << "<<< ", getline(cin, command)) {

        vector<string> splitted = splitString(command, " ");
        Type task = getCommand(splitted[0]);

        if (task == Type::CREATE_TRAIN) {

            string trainName = splitted[1];
            vector<string> trainTowns;
            trainTowns.insert(trainTowns.end(), splitted.begin()+2, splitted.end());

            if (trainTowns.size() > 1) {
                createTrain(towns, trains, trainName, trainTowns);
            } else {
                cout << ">>> Too little train stops!\n";
            }

        } else if (task == Type::TRAINS_FOR_TOWN) {

            string townName = splitted[1];
            vector<string> trForTown = getTrainsForTown(towns,trains,townName);

            if (trForTown[0] == "notown") { cout << ">>> Нет такого города!\n"; }
            else if (trForTown[0] == "notrains") { cout << ">>> В этом городе поезда не останавливаются!\n"; }
            else {
                cout << ">>> ";
                for (string& train : trForTown) { cout << train << " "; }
                cout << endl;
            }

        } else if (task == Type::TOWNS_FOR_TRAIN) {
            string train = splitted[1];
            unordered_map<string, vector<string>> townsForTrain = getTownsForTrain(towns,trains, train);

            for (auto& [town, trains] : townsForTrain) {
                cout << town << ": ";
                for (auto& train : trains) {
                    cout << train << " ";
                }
                cout << endl;
            }
        } else if (task == Type::TRAINS) {
            printTrainsList(trains);
        }
        else {
            cout << ">>> Unknown command. Try again\n";
        }
    }
    return 0;
}