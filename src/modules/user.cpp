#include <fstream>
#include <iostream>
#include <sstream>
#include "user.h"

using std::cout; using std::cin; using std::system; using std::getline; using std::istringstream;

User admins[MAX_ADMINS];
User users[MAX_USERS];

int numOfUsers = 0;
int numOfAdmins = 0;

bool loadAdminFromFile() {
    std::ifstream in("admin.txt");
    if(in.is_open()) {
        while (numOfAdmins < MAX_ADMINS && (in >> admins[numOfAdmins].name >> admins[numOfAdmins].password)) {
            ++numOfAdmins;
        }
        in.close();
        return true;
    } else {
        return false;
    }
}

bool loadUsersFromFile() {
    std::ifstream in("users.txt");
    if (in.is_open()) {
        User user; string line;
        while (numOfUsers < MAX_USERS && getline(in, line)) {
            if(!(line == "====")) {
                if(user.name.empty()) {
                    string data;
                    getline(in, data);
                    istringstream dataStream(data);
                    dataStream >> user.name >> user.password;
                } else {
                    string data;
                    getline(in, data);
                    istringstream dataStream(data);
                    dataStream >> user.tasks->subject >> user.tasks->description >> user.tasks->completed;
                    ++user.numOfTasks;
                }
            } else {
                users[numOfUsers++] = user;
                user = User();
            }
        }
        in.close();
        return true;
    } else {
        return false;
    }
}

void createUser(const User& user, int& currentNum, User data[]) {
        data[currentNum] = user;
        ++currentNum;
        cout << user.name << " ADDED SUCCESSFULLY.\n";   
}

void deleteUser(const string& username, int& currentNum, User data[]) {
    bool noUser = true;

    for (int i = 0; i < currentNum; i++) {
        if (data[i].name == username) {
            for (int j = i; j < currentNum - 1; j++) {
                data[j] = data[j + 1];
            }
            --currentNum;
            noUser = false;
            cout << username << " DELETED SUCCESFULLY.\n";
            break;
        }
    }

    if(noUser) { cout << username << " NOT FOUND.\n"; }
}

void loadAdminToFile() {
    std::ofstream out("admin.txt", std::ios::trunc);
    if(out.is_open()) {
        for (int i = 0; i < numOfAdmins; ++i) {
            out << admins[i].name << " " << admins[i].password << "\n";
        }
        out.close();
    } else {
        cout << "UNABLE TO OPEN WRITE FILE.\n";
    }
}

void loadUsersToFile() {
    std::ofstream out("users.txt", std::ios::trunc);
    if(out.is_open()) {
        for (int i = 0; i < numOfUsers; ++i) {
            out << users[i].name << " " << users[i].password << "\n";

            for(int j = 0; j < users[i].numOfTasks; ++j) {
                out << users[i].tasks[j].subject << " " << users[i].tasks[j].description << " " << users[i].tasks[j].completed << "\n";
            }
            out << "====\n";
        }
        out.close();
    } else {
        cout << "UNABLE TO OPEN WRITE FILE.\n";
    }
}

void updateUserPassword(const string& username, const string& newPassword, int currentNum, User data[]) {
    bool noUser = true;

    for (int i = 0; i < currentNum; ++i) {
        if (data[i].name == username) {
            data[i].password = newPassword;
            noUser = false;
            cout << "PASSWORD UPDATED SUCCESSFULLY.\n";
            break;
        }
    }

    if(noUser) { cout << "USER NOT FOUND.\n"; }
}

bool userExists(string& username, string& password) {
    if(numOfUsers != 0) {
        for (int i = 0; i < numOfUsers; ++i) {
            if(users[i].name == username && users[i].password == password) {
                return true;
            }
        }
    } 
    return false;
}
