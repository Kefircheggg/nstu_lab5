#include <iostream>
#include <set>
using namespace std;

void addFriend(unordered_map<string, set<string>>& guys, const string& person1, const string& person2) {
    guys[person1].insert(person2);
    guys[person2].insert(person1);
}

int friendsCount (unordered_map<string, set<string>>& guys, const string& person1) {
    return guys[person1].size();
}

bool isFriends (unordered_map<string, set<string>>& guys, const string& person1, const string& person2) {
    return find(guys[person1].begin(), guys[person1].end(), person2) != guys[person1].end();
}

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


int main() {
    unordered_map<string, set<string>> people;

    string command;
    while (cout << "<<< ", getline(cin, command)) {
        vector<string> splitted = splitString(command, " ");
        string task = splitted[0];

        if (task == "FRIENDS") {
            addFriend(people, splitted[1], splitted[2]);
            cout << ">>> OK\n";
        } else if (task == "COUNT") {
            cout << ">>> " << friendsCount(people, splitted[1]) << "\n";
        } else if (task == "QUESTION") {
            cout << ">>> ";
            isFriends(people,splitted[1],splitted[2]) ? cout << "YES" : cout << "NO";
            cout << endl;
        } else {
            cout << ">>> Unknown command. Try again\n";
        }
    }
    return 0;
}