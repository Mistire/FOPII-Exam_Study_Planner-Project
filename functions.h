#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/prepared_statement.h>

#include <iostream>
#include <iomanip>
#include <sstream>

#define MAX_ADMINS 8

using std::cin;
using std::cout;
using std::endl;
using std::endl;
using std::string;

struct Users
{
    string name;
    string password;
};
int noOfusers = 0;
int numOfAdmins = 0;

enum type
{
    Admin,
    User,
    Task
};

sql::Connection *createConnection()
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "mindahun@2119");
    con->setSchema("exam_study_planner");

    return con;
}
int addNum(sql::Connection *con)
{
    sql::Statement *stmt(con->createStatement());

    sql::ResultSet *res(stmt->executeQuery("select number from no_of_users"));
    res->next();
    int32_t num = (res->getInt("number"));

    delete res;
    delete stmt;
    sql::PreparedStatement *pstmt(con->prepareStatement("update no_of_users set number=?"));
    pstmt->setInt(1, ++num);
    pstmt->execute();
    delete pstmt;
    return num;
}
int addNum(sql::Connection *con)
{
    sql::Statement *stmt(con->createStatement());

    sql::ResultSet *res(stmt->executeQuery("select number from no_of_users"));
    res->next();
    int32_t num = (res->getInt("number"));

    delete res;
    delete stmt;
    sql::PreparedStatement *pstmt(con->prepareStatement("update no_of_users set number=?"));
    pstmt->setInt(1, ++num);
    pstmt->execute();
    delete pstmt;
    return num;
}
std::string createUser_id(int number)
{
    try
    {
        std::string formattedNumber = std::to_string(number);
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(4) << formattedNumber;
        std::string paddedNumber = oss.str();
        return "ETS" + paddedNumber + "/15";
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << endl;
    }
}
void addAdmin(sql::Connection *con, std::string username, std::string password)
{
    try
    {
        sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO admins (username, password) VALUES (?, ?)");

        pstmt->setString(1, username);
        pstmt->setString(2, password);

        if (pstmt->executeUpdate() > 0)
        {
            std::cout << "ADDED SUCCESSFULLY" << std::endl;
        }
        else
        {
            std::cout << "ADMIN IS NOT ADDED" << std::endl;
        }

        delete pstmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << std::endl;
        throw e;
    }
}

void addUser(sql::Connection *con, std::string username, std::string password)
{
    try
    {
        sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO users VALUES(?, ?, ?)");
        int noOfusers = addNum(con);
        std::string user_id = createUser_id(noOfusers); // Assuming you have a createUser_id function.

        pstmt->setString(1, user_id);
        pstmt->setString(2, username);
        pstmt->setString(3, password);

        if (pstmt->executeUpdate() > 0)
        {

            // Now, insert a record into the 'no_of_task' table
            pstmt = con->prepareStatement("INSERT INTO no_of_task (user_id) VALUES (?)");
            pstmt->setString(1, user_id);
            pstmt->executeUpdate();
            std::cout << "ADDED SUCCESSFULLY" << std::endl;
        }
        else
        {
            std::cout << "USER IS NOT ADDED" << std::endl;
        }

        delete pstmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
}

// void addUser(sql::Connection* con, std::string username, std::string password)
//{
//     try
//     {
//         sql::Statement* stmt = con->createStatement();
//         std::string query;
//         std::string user_id;
//         ++noOfusers;
//         user_id = createUser_id(noOfusers);
//         query = "insert into users(user_id, username, password) values('" + user_id + "' ,'" + username + "' ,'" + password + "')";
//         stmt->execute("insert into no_of_task(user_id) values('" + user_id + "')");
//         if (stmt->execute(query))
//         {
//             std::cout << "ADDED SUCCESSFULLY" << std::endl;
//         }
//         else
//         {
//             std::cout << "USER IS NOT ADDED" << std::endl;
//         }
//         delete stmt;
//     }
//     catch (sql::SQLException& e)
//     {
//         std::cerr << e.what() << endl;
//     }
//
// }
void addTask(sql::Connection *con, std::string username)
{
    try
    {
        int amount;
        std::string user_id, query;
        sql::PreparedStatement *pstmt = con->prepareStatement("SELECT user_id FROM users WHERE username = ?");
        pstmt->setString(1, username);
        sql::ResultSet *res = pstmt->executeQuery();

        if (res->next())
        {
            user_id = res->getString("user_id");

            pstmt = con->prepareStatement("SELECT amount FROM no_of_task WHERE user_id = ?");
            pstmt->setString(1, user_id);
            res = pstmt->executeQuery();

            if (res->next())
            {
                amount = res->getInt("amount");
                amount++;

                std::string subject, description;
                std::cout << "Enter the subject: " << std::endl;
                std::cin >> subject;
                std::cin.ignore();
                std::cout << "Enter the description: " << std::endl;
                std::getline(std::cin, description);

                pstmt = con->prepareStatement("INSERT INTO task (user_id, task_id, subject, description, completion) VALUES (?, ?, ?, ?, ?)");
                pstmt->setString(1, user_id);
                pstmt->setInt(2, amount);
                pstmt->setString(3, subject);
                pstmt->setString(4, description);
                pstmt->setBoolean(5, false);

                /*bool executed = pstmt->execute();*/

                if (pstmt->executeUpdate())
                {
                    std::cout << "TASK ADDED SUCCESSFULLY" << std::endl;
                    pstmt = con->prepareStatement("UPDATE no_of_task SET amount = ? WHERE user_id = ?");
                    pstmt->setInt(1, amount);
                    pstmt->setString(2, user_id);
                    pstmt->executeUpdate();
                }
                else
                {
                    std::cout << "TASK IS NOT ADDED" << std::endl;
                }
            }
            else
            {
                std::cout << "No records found for user: " << username << std::endl;
            }
        }
        else
        {
            std::cout << "User not found: " << username << std::endl;
        }

        delete pstmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

// void addTask(sql::Connection* con, std::string username)
//{
//     try
//     {
//         int amount;
//         std::string user_id, query;
//         sql::Statement* stmt = con->createStatement();
//         sql::ResultSet* res = stmt->executeQuery("select user_id from users where username='" + username + "'");
//         res->next();
//         user_id = res->getString("user_id");
//
//         res = stmt->executeQuery("select amount from no_of_task where user_id='" + user_id + "'");
//         res->next();
//         amount = res->getInt("amount");
//         amount++;
//         stmt->execute("update no_of_task set amount='" + std::to_string(amount) + "' where user_id='" + user_id + "'");
//         delete res;
//         std::string subject, description;
//         std::cout << "enter the subject " << std::endl;
//         std::cin >> subject;
//         std::cin.ignore();
//         std::cout << "enter the description " << std::endl;
//         std::getline(cin,description);
//         std::cin.ignore();
//
//         query = "insert into task(user_id,task_id, subject, description,completion) values('" + user_id + "' ,'" + std::to_string(amount) + "' ,'" + subject + "' ,'" + description + "','" + std::to_string(false) + "')";
//         bool executed = stmt->execute(query);
//         if (executed)
//         {
//             std::cout << "TASK ADDED SUCCESSFULLY" << std::endl;
//         }
//         else
//         {
//             std::cout << "TASK IS NOT ADDED" << std::endl;
//         }
//         delete stmt;
//         }
//     catch (sql::SQLException& e)
//     {
//         std::cerr << e.what() << endl;
//     }
//
// }

auto add(type Type, sql::Connection *con, std::string username, std::string password = "")
{
    try
    {

        if (Type == type::Admin)
        {
            addAdmin(con, username, password);
        }
        else if (Type == type::User)
        {

            addUser(con, username, password);
        }
        else
        {
            // move this to the calling
            //  add task
            addTask(con, username);
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << endl;
    }
}

auto deleted(type Type, sql::Connection *con, std::string username)
{
    try
    {
        sql::Statement *stmt = con->createStatement();
        std::string query;

        if (Type == type::Admin)
        {
            query = "delete from admins where username='" + username + "'";
        }
        else
        {
            query = "delete from users where username='" + username + "'";
        }

        stmt->execute(query); // Added successfully
        system("cls");
        cout << "\ndeleted successful\n";
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << endl;
    }
}

auto display(type Type, sql::Connection *con, std::string user_id = "")
{
    try
    {
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = nullptr;
        std::string query;

        if (Type == type::User)
        {
            query = "select username from users";
            res = stmt->executeQuery(query);
            while (res->next())
            {
                std::cout << res->getString("username") << std::endl;
            }
        }
        else if (Type == type::Task)
        {
            query = "select subject, description, completion from task where user_id='" + user_id + "'";
            res = stmt->executeQuery(query);
            while (res->next())
            {
                bool comp = res->getBoolean("completion");

                if (comp)
                {
                    std::cout << "[x]";
                }
                else
                {
                    std::cout << "[]";
                }
                std::cout << res->getString("subject") << "     ";
                std::cout << res->getString("description") << std::endl;
            }
        }

        delete res;
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << endl;
    }
}

auto changePassword(type Type, sql::Connection *con, std::string newpassword, std::string username)
{
    try
    {
        sql::Statement *stmt = con->createStatement();
        std::string query;
        if (Type == type::Admin)
        {
            query = "update admins set password ='" + newpassword + "' where username='" + username + "'";
        }
        else
        {
            query = "update users set password ='" + newpassword + "' where username='" + username + "'";
        }
        bool queryResult = stmt->executeUpdate(query);
        system("cls");
        if (queryResult)
        {
            std::cout << "PASSWORD CHANGED SUCCESSFULLY\n";
        }
        else
        {
            (std::cout << "PASSWORD NOT CHANGED\n");
        }
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << endl;
    }
}
void taskCompletion(sql::Connection *con, std::string username)
{
    try
    {
        sql::Statement *stmt = con->createStatement();
        int num;
        std::string query;
        query = "select user_id from users where username='" + username + "'";
        sql::ResultSet *res = stmt->executeQuery(query);
        res->next();
        std::string user_id = res->getString("user_id");
        display(type::Task, con, user_id);
        std::cout << "\nwhich one is complleted\nenter task_number: ";
        std::cin >> num;
        cin.ignore();
        if (!cin.fail())
        {
            query = "select amount from no_of_task where user_id='" + user_id + "'";
            res = stmt->executeQuery(query);
            res->next();
            if (num > 0 && num <= res->getInt("amount"))
            {
                query = "update task set completion=true where task_id='" + std::to_string(num) + "'";
                stmt->execute(query);
            }
            else
            {
                std::cout << "task number " << num << " not exist\n";
            }
        }
        else
        {
            std::cout << "wrong input \n";
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << endl;
    }
}
void printCenteredText(const std::string &text)
{

    int spaces = (40 - text.length()) / 2;
    for (int i = 0; i < spaces; ++i)
    {
        std::cout << " ";
    }
    std::cout << text << std::endl;
}
void printLine(char symbol, int length)
{
    for (int i = 0; i < length; ++i)
    {
        std::cout << symbol;
    }
    std::cout << std::endl;
}
void printUI(std::string sentence)
{
    std::cout << '\n';
    printLine('*', 40);
    printCenteredText(sentence);
    printLine('*', 40);
    std::cout << '\n';
}
bool userExists(sql::Connection *con, type Type, std::string username)
{
    try
    {
        std::string query;
        size_t count;
        // sql::PreparedStatement* pstmt = nullptr;
        // sql::ResultSet* res = nullptr;

        if (Type == type::Admin)
        {
            query = "select username from admins where username=?";
        }
        else if (Type == type::User)
        {
            query = "select username from users where username=?";
        }

        sql::PreparedStatement *pstmt(con->prepareStatement(query));
        pstmt->setString(1, username);
        sql::ResultSet *res(pstmt->executeQuery());

        count = res->rowsCount();

        delete res;
        delete pstmt;

        return (count == 1);
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}
bool checkPassword(sql::Connection *con, type Type, std::string username, std::string password)
{
    try
    {
        std::string query, truePass;
        // sql::PreparedStatement* pstmt = nullptr;
        // sql::ResultSet* res = nullptr;

        if (Type == type::Admin)
        {
            query = "select password from admins where username=?";
        }
        else if (Type == type::User)
        {
            query = "select password from users where username=?";
        }

        sql::PreparedStatement *pstmt(con->prepareStatement(query));
        pstmt->setString(1, username);
        sql::ResultSet *res = pstmt->executeQuery();

        if (res->next())
        {
            truePass = res->getString("password");
            delete res;
            delete pstmt;

            return (truePass == password);
        }
        else
        {
            delete res;
            delete pstmt;
            return false;
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

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
                deleted(type::Admin, con, currentAdmin.name);
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
                cout << "Enter username: ";
                cin >> user.name;
                if (userExists(con, type::User, user.name))
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
                display(type::User, con, "");
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
        std::cerr << e.what() << std::endl;
    }
}

void studyTaskMenu(Users &currentUser)
{
    sql::Connection *con = createConnection();
    int choice;
    string password;
    sql::Statement *stmt(con->createStatement());
    sql::ResultSet *res;
    bool exited = false;

    try
    {

        while (!exited)
        {
            cout << "Menu: \n";
            cout << "1. Add Task\n";
            cout << "2. Display Task\n";
            cout << "3. Change your Password\n";
            cout << "4. change task completion\n";
            cout << "5. Go Back\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            switch (choice)
            {
            case 1:
                add(type::Task, con, currentUser.name);
                break;
            case 2:
                system("cls");
                cout << "Here are all your tasks.\n";
                stmt = con->createStatement();
                res = stmt->executeQuery("select user_id from users where username='" + currentUser.name + "'");
                res->next();
                display(type::Task, con, res->getString("user_id"));
                break;
            case 3:
                cout << "Enter new password: ";
                cin >> password;
                if (password == currentUser.password)
                {
                    cout << "SAME PASSWORD AS THE PREVIOUS ONE -- tryagain with a new password.\n";
                    break;
                }
                changePassword(type::User, con, password, currentUser.name);
                break;
            case 4:
                taskCompletion(con, currentUser.name);
                break;
            case 5:
                cout << "Exiting....\n";
                exited = true;
                system("cls");
                break;
            default:
                cout << "Invalid choice. Try again.\n";
                break;
            }
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

#endif
