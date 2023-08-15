#ifndef USER_H
#define USER_H

#include <iostream>
#include <fstream>
#include <algorithm>

#define MAX_USERS 100
#define MAX_ADMINS 8

using namespace std;

struct User
{
    string name;
    string password;
};

User admins[MAX_ADMINS];

User users[MAX_USERS];

int numOfUsers = 0;
int numOfAdmins = 0;

bool loadUsersFromFile(string filename, int& currentNum, int max, User (&data)[]) {
    ifstream in(filename);
    if (in.is_open()) {
        while (currentNum < max && (in >> data[currentNum].name >> data[currentNum].password)) {
            currentNum++;
        }
        in.close();
        return true;
    } else {
        return false;
    }
}

void createUser(const User& user, int& currentNum, int max, User (&data)[]) {
        data[currentNum].name = user.name;
        data[currentNum].password = user.password;
        ++currentNum;
        cout << user.name << " ADDED SUCCESSFULLY.\n";   
}

void updateUserPassword(const string& username, const string& newPassword, int currentNum, User (&data)[]) {
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

void deleteUser(const string& username, int& currentNum, User (&data)[]) {
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

void displayAllUsers() {
    if (numOfUsers != 0) {
        for(int i = 0; i < numOfUsers; ++i) {
            cout << i+1 << ". " << users[i].name << "\n"; 
        }
    } else {
        cout << "NO USERS FOUND.\n";
    }
}

void loadUsersToFile(string filename, int currentNum, User data[]) {
    ofstream out(filename, ios::trunc);
    if(out.is_open()) {
        for (int i = 0; i < currentNum; ++i) {
            out << data[i].name << " " << data[i].password << "\n";
        }
        out.close();
    } else {
        cout << "UNABLE TO OPEN WRITE FILE.\n";
    }
}

void adminPage(User& currentAdmin) {
    User admin, user;
    string password;
    int choice;
    bool exited = false;

    do {
        cout << "Admin Menu:\n";
        cout << "1. Add Admin\n";
        cout << "2. Delete Your Admin Account\n";
        cout << "3. Change Password\n";
        cout << "4. Delete User\n";
        cout << "5. Display All Users\n";
        cout << "6. Go Back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:        
            if (!(numOfAdmins < MAX_ADMINS)) {
                cout << "ADMIN CAPACITY REACHED.\n";
                break;
            }
            cout << "Enter Admin Username: ";
            cin >> admin.name;
            cout << "Enter Admin Password: ";
            cin >> admin.password;
            createUser(admin, numOfAdmins, MAX_ADMINS, admins);
            break;
        case 2:
            if (numOfAdmins == 1) {
                cout << "YOU ARE NOT ABLE TO DELETE ADMIN ACCOUNT.\n";
                break;
            }
            deleteUser(currentAdmin.name, numOfAdmins, admins);
            loadUsersToFile("admin.txt", numOfAdmins, admins);
            exited = true;
            break;
        case 3:
            cout << "Enter new password: ";
            cin >> password;
            if (password == currentAdmin.password) { 
                cout << "SAME PASSWORD AS THE PREVIOUS ONE -- tryagain with a new password.\n";
                break;
            }
            updateUserPassword(currentAdmin.name, password, numOfAdmins, admins);
            break;
        case 4:
            if (numOfUsers == 0) {
                cout << "NO USERS FOUND.\n";
                break;
            }
            cout << "Enter username: ";
            cin >> user.name;
            deleteUser(user.name, numOfUsers, users);
            loadUsersToFile("users.txt", numOfUsers, users);
            break;
        case 5:
            system("cls");
            cout << "Here are all the users:\n";
            displayAllUsers();
            break;
        case 6:
            cout << "Exiting....\n";
            loadUsersToFile("admin.txt", numOfAdmins, admins);
            exited = true;
            system("cls");
            break;
        default:
            cout << "Invalid choice. Try again.\n";
            break;
        }
    } while (!exited);
}


#endif