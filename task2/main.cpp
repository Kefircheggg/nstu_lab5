#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct ticketS {
    string id;
    int duration;
};
string pushTicket(vector<ticketS>& tickets, const int time) {
    int ticketN;
    if (!tickets.empty()) {
        ticketN = stoi(tickets.back().id.substr(1, string::npos));
    } else {
        ticketN = 0;
    }
    string ticketId = "T" + to_string(ticketN+1);
    ticketS newTicket = {ticketId, time};
    tickets.push_back(newTicket);
    return newTicket.id;
}

vector<string> splitString(const string& s) {
    vector<string> res;
    size_t pos = s.find(' ');
    if (pos != string::npos) {
        res.push_back(s.substr(0, pos));
        res.push_back(s.substr(pos + 1));
    } else {
        res.push_back(s); // если пробела нет, вернём строку как один элемент
    }
    return res;
}

int main() {
    int threadsAmount;
    cout << ">>> Введите количество окон:\n<<< ";
    cin >> threadsAmount;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<ticketS> tickets;

    string command;
    while(cout << "<<< ", getline(cin,command)) {
        vector<string> splitted = splitString(command);
        string task = splitted[0];

        if (task == "ENQUEUE") {
            string ticketId = pushTicket(tickets, stoi(splitted[1]));
            cout << ">>> " << ticketId << "\n";
        } else if (task == "DISTRIBUTE") {
            vector<pair<vector<ticketS>, int>> queue(threadsAmount);

            vector<ticketS> sortedTickets = tickets;
            sort(sortedTickets.begin(), sortedTickets.end(), [](const ticketS& a, const ticketS& b) {
                return a.duration > b.duration;
            });

            for (const auto& ticket : sortedTickets) {
                auto minIt = queue.begin();

                for (auto it = queue.begin(); it != queue.end(); ++it) {
                    if (it->second < minIt->second) {
                        minIt = it;
                    }
                }

                minIt->first.push_back(ticket);
                minIt->second += ticket.duration;
            }
            int windN = 0;
            for (auto& window : queue) {
                cout << ">>> Окно " << windN+1 << " (" << window.second << " минут): ";
                windN+=1;
                for (auto& ticket : window.first) {
                    cout << ticket.id << " ";
                }
                cout << "\n";
            }
        } else if (task == "EXIT") {
            cout << ">>> Executing exit" << endl;
            break;
        } else {
            cout << ">>> Unknown command. Try again\n";
        }
    }
    return 0;
}