#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <iostream>
#include <iomanip>
#include <sstream>

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
std::string createUser_id(int number)
{
    std::string formattedNumber = std::to_string(number);
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(4) << formattedNumber;
    std::string paddedNumber = oss.str();
    return "ETS" + paddedNumber + "/15";
}
void addAdmin(sql::Connection *con, std::string username, std::string password)
{
    sql::Statement *stmt = con->createStatement();
    std::string query;
    query = "insert into admins(username, password) values('" + username + "' ,'" + password + "')";
    ++numOfAdmins;
    if (stmt->execute(query))
    {
        std::cout << "ADDED SUCCESSFULLY" << std::endl;
    }
    else
    {
        std::cout << "ADMIN IS NOT ADDED" << std::endl;
    }
    delete stmt;
}
void addUser(sql::Connection *con, std::string username, std::string password)
{
    sql::Statement *stmt = con->createStatement();
    std::string query;
    std::string user_id;
    ++noOfusers;
    user_id = createUser_id(noOfusers);
    query = "insert into users(user_id, username, password) values('" + user_id + "' ,'" + username + "' ,'" + password + "')";
    stmt->execute("insert into no_of_task(user_id) values('" + user_id + "'");
    if (stmt->execute(query))
    {
        std::cout << "ADDED SUCCESSFULLY" << std::endl;
    }
    else
    {
        std::cout << "USER IS NOT ADDED" << std::endl;
    }
    delete stmt;
}
void addTask(sql::Connection *con, std::string username)
{
    int amount;
    std::string user_id, query;
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("select user_id from users where username='" + username + "'");
    res->next();
    user_id = res->getString("user_id");

    res = stmt->executeQuery("select amount from no_of_task where user_id='" + user_id + "'");
    res->next();
    amount = res->getInt("amount");
    amount++;
    stmt->execute("update no_of_task set amount='" + std::to_string(amount) + "' where user_id='" + user_id + "'");
    delete res;
    std::string subject, description;
    std::cout << "enter the subject " << std::endl;
    std::cin >> subject;
    std::cin.ignore();
    std::cout << "enter the description " << std::endl;
    std::cin >> description;
    std::cin.ignore();

    query = "insert into task(user_id,task_id, subject, description,completion) values('" + user_id + "' ,'" + std::to_string(amount) + "' ,'" + subject + "' ,'" + description + "','" + std::to_string(false) + "'";
    if (stmt->execute(query))
    {
        std::cout << "TASK ADDED SUCCESSFULLY" << std::endl;
    }
    else
    {
        std::cout << "TASK IS NOT ADDED" << std::endl;
    }
    delete stmt;
}

auto add(type Type, sql::Connection *con, std::string username, std::string password = "")
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

auto deleted(type Type, sql::Connection *con, std::string username)
{
    sql::Statement *stmt = con->createStatement();
    std::string query;

    if (Type == type::Admin)
    {
        query = "delete from admins where admins.username='" + username + "'";
    }
    else
    {
        query = "delete from users where users.username='" + username + "'";
    }

    stmt->execute(query); // Added successfully
    delete stmt;
}

auto display(type Type, sql::Connection *con, std::string user_id = NULL)
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

auto changePassword(type Type, sql::Connection *con, std::string newpassword, std::string username)
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
    stmt->execute(query) ? std::cout << "PASSWORD CHANGED SUCCESSFULLY\n" : std::cout << "PASSWORD NOT CHANGED\n";
    delete stmt;
}
void taskCompletion(sql::Connection *con, std::string username)
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
    if (!cin.fail())
    {
        query = "select amount from no_of_task where user_id='" + user_id + "'";
        res = stmt->executeQuery(query);
        if (num > 0 && num <= res->getInt("amount"))
        {
            query = "update task set completion=true where task_id='" + std::to_string(num);
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
bool userExist(type Type, sql::Connection *con, bool existOnly, std::string username, std::string password = "")
{
    std::string query, queryE, truePass;
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = nullptr;

    if (Type == type::Admin)
    {
        query = "select password from admins where username='" + username + "'";
        queryE = "select count(*) from admins username='" + username + "'";
    }
    else if (Type == type::User)
    {
        query = "select password from users where username='" + username + "'";
        queryE = "select count(*) from users username='" + username + "'";
    }
    if (existOnly)
    {
        res = stmt->executeQuery(queryE);
        res->next();
        int count = res->getInt(1);
        delete res;
        delete stmt;
        if (count == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (stmt->execute(query))
    {
        res = stmt->executeQuery(query);
        res->next();
        truePass = res->getString("password");
        delete res;
        delete stmt;

        return truePass == password;

        cout << "incorrect password\n";
    }
}

#endif
