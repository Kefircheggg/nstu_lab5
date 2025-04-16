#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Cell {
    map<string, int> items;
};

struct Storage {
    vector<string> zones;
    vector<int> racks;
    vector<int> vSects;
    vector<int> hSects;
    map<string,int> loadedZones;
    int totalCapacity = zones.size()*racks.size()*vSects.size()*hSects.size()*10;
    int freeCapacity = totalCapacity;
    map<string, Cell> cells;
};

enum result {
    SUCCESS,
    NOCELL,
    TOOMANY
};

result add(Storage& procStorage, const string& item, const int amount, const string& cell) {
    if (procStorage.cells.find(cell) != procStorage.cells.end()) {
        Cell& storageCell = procStorage.cells[cell];

        int currQty = 0;
        for (auto[itemName,qty] : storageCell.items) {
            currQty+=qty;
        }
        if (amount + currQty <= 10) {
            storageCell.items[item]+= amount;
            procStorage.freeCapacity-=amount;
            procStorage.loadedZones[cell.substr(0,1)]+=amount;
            return SUCCESS;
        } else {
            return TOOMANY;
        }
    } else {
        return NOCELL;
    }
}

result remove(Storage& procStorage, const string& item, const int amount, const string& cell) {
    if (procStorage.cells.find(cell) != procStorage.cells.end()) {
        Cell& storageCell = procStorage.cells[cell];

        int currQty = storageCell.items[item];
        if (currQty - amount >= 0) {
            storageCell.items[item]-= amount;
            procStorage.freeCapacity+=amount;
            procStorage.loadedZones[cell.substr(0,1)]-=amount;
            return SUCCESS;
        } else {
            return TOOMANY;
        }
    } else {
        return NOCELL;
    }
}

void printInfo(Storage& procStorage) {
    double totalLoad = float((procStorage.totalCapacity - procStorage.freeCapacity)*100) / float(procStorage.totalCapacity);
    cout << "Весь склад загружен на " << setprecision(4) <<  totalLoad << "%\n";

    int zoneAm = procStorage.loadedZones.size();
    for (auto& [zone, load] : procStorage.loadedZones) {
        double zoneLoad = double(load) / (procStorage.totalCapacity / zoneAm) * 100;
        cout << "Зона " << zone << " загружена на " << setprecision(4) << zoneLoad << "%\n";
    }

    for (auto& [cell, items] : procStorage.cells) {
        for (auto& [item, quantity] : items.items) {
            if (quantity) {
                cout << cell << " " << item << " " << quantity << "\n";
            }
        }
    }

    for (auto& zone : procStorage.zones) {
        for (auto& rack : procStorage.racks) {
            for (auto& vSect : procStorage.vSects) {
                for (auto& hSect : procStorage.hSects) {
                    string address = zone + to_string(rack) + to_string(vSect) + to_string(hSect);
                    Cell cell = procStorage.cells[address];
                    if (cell.items.empty()) {
                        cout << address << " ";
                    }
                }
            }
            cout << "\n";
        }
    }

}

vector<string> splitString(string s, string divider){
    vector<string> res;
    int pos = 0;
    while (pos <= s.size()) {
        pos = s.find(divider);
        res.push_back(s.substr(0,pos));
        s.erase(0,pos+divider.length());
    }
    return res;
}

int main () {
    vector<string> zones = {"A", "B", "C"};
    vector<int> racks = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    vector<int> vSects = {1, 2, 3, 4, 5};
    vector<int> hSects = {1, 2};
    map<string,int> loadedZones {{"A", 0},{"B", 0},{"C", 0}};

    Storage mainStorage = {
        zones, racks, vSects, hSects, loadedZones
    };

    for (auto& zone : mainStorage.zones) {
        for (auto& rack : mainStorage.racks) {
            for (auto& vSect : mainStorage.vSects) {
                for (auto& hSect : mainStorage.hSects) {
                    string address = zone + to_string(rack) + to_string(vSect) + to_string(hSect);
                    Cell cell;
                    mainStorage.cells[address] = cell;
                }
            }
        }
    }

     string command;
     while (getline(cin, command)) {
         vector<string> splitted = splitString(command, " ");
         string task = splitted[0];
         if (task == "ADD") {
             if (splitted.size() != 4) {
                 cout << "Invalid arguments amount. Try again\n";
             } else {
                 result status = add(mainStorage, splitted[1], stoi(splitted[2]), splitted[3]);
                 if (status == SUCCESS) {
                     cout << "Элемент успешно добавлен в ячейку\n";
                 } else if (status == NOCELL) {
                     cout << "Ячейка не найдена\n";
                 } else if (status == TOOMANY) {
                     cout << "Недостаточно места в ячейке\n";
                 }
             }
         } else if (task == "REMOVE") {
             if (splitted.size() != 4) {
                 cout << "Invalid arguments amount. Try again\n";
             }

             result status = remove(mainStorage, splitted[1], stoi(splitted[2]), splitted[3]);
             if (status == SUCCESS) {
                 cout << "Товар успешно удален из ячейки\n";
             } else if (status == NOCELL) {
                 cout << "Ячейка не найдена\n";
             } else if (status == TOOMANY) {
                 cout << "Недостаточно товаров в ячейке\n";
             }
         } else if (task == "INFO") {
             printInfo(mainStorage);
         } else if (task == "EXIT") {
             cout << "Executing exit" << endl;
             break;
         } else {
             cout << "Unknown command. Try again\n";
         }
     }
    return 0;
}
