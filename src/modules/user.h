#ifndef USER_H
#define USER_H

#include <string>

using std::string;

struct StudyTask {
    string subject;
    string description;
    bool completed = false;
};

struct User {
    string name;
    string password;
    StudyTask tasks[100];
    int numOfTasks = 0;
};

const int MAX_USERS = 100;
const int MAX_ADMINS = 8;

extern User admins[MAX_ADMINS];
extern User users[MAX_USERS];

extern int numOfUsers;
extern int numOfAdmins;

bool loadAdminFromFile();
bool loadUsersFromFile();
void createUser(const User& user, int& currentNum, User data[]);
void deleteUser(const string& username, int& currentNum, User data[]);
void loadAdminToFile();
void loadUsersToFile();
void updateUserPassword(const string& username, const string& newPassword, int currentNum, User data[]);
bool userExists(string& username, string& password);

#endif // USER_H
