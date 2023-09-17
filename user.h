#pragma once
#ifndef USER_H
#define USER_H

#include <iostream>

#define MAX_ADMINS 8

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using std::cin;
using std::cout;
using std::string;

struct Users
{
    string name;
    string password;
};
int noOfusers = 0;
int numOfAdmins = 0;

void adminPage(Users &currentAdmin)
{
    Users admin, user;
    string password;
    int choice;
    bool exited = false;
    sql::Connection *con = createConnection();
    try
    {
        do
        {
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
                if (!(numOfAdmins < MAX_ADMINS))
                {
                    cout << "ADMIN CAPACITY REACHED.\n";
                    break;
                }
                cout << "Enter Admin Username: ";
                cin >> admin.name;
                cout << "Enter Admin Password: ";
                cin >> admin.password;
                add(type::Admin, con, admin.name, admin.password);
                break;
            case 2:
                if (numOfAdmins == 1)
                {
                    cout << "YOU ARE NOT ABLE TO DELETE ADMIN ACCOUNT.\n";
                    break;
                }
                deleted(type::Admin, con, admin.name);
                exited = true;
                break;
            case 3:
                cout << "Enter new password: ";
                cin >> password;
                if (password == currentAdmin.password)
                {
                    cout << "SAME PASSWORD AS THE PREVIOUS ONE -- tryagain with a new password.\n";
                    break;
                }
                changePassword(type::Admin, con, password, currentAdmin.name);
                break;
            case 4:
                if (noOfusers == 0)
                {
                    cout << "NO USERS FOUND.\n";
                    break;
                }
                cout << "Enter username: ";
                cin >> user.name;
                if (userExist(type::User, con, true, user.name))
                {
                    deleted(type::User, con, user.name);
                    cout << "DELETED SUCCESSFULLY\n";
                }
                else
                {
                    cout << "THIS UESR DOESN'T EXIST";
                }
                break;
            case 5:
                system("cls");
                cout << "Here are all the users:\n";
                display(type::User, con);
                break;
            case 6:
                cout << "Exiting....\n";
                exited = true;
                system("cls");
                break;
            default:
                cout << "Invalid choice. Try again.\n";
                break;
            }

        } while (!exited);
    }
    catch (sql::SQLException &e)
    {
        cout << "SQL exeption error: " << e.what() << std::endl;
    }
}

#endif
