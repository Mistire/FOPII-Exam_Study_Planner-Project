#pragma once
#ifndef STUDY_TASK_H
#define STUDY_TASK_H

#include <string>
#include <iostream>
#include <fstream>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

void studyTaskMenu(Users &currentUser)
{
  int choice;
  string password;
  bool exited = false;
  sql::Connection *con = createConnection();
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
        cout << "Here are all your tasks.";
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("select user_id from users where usermane='" + currentUser.name + "'");
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
    cout << "SQL exeption error: " << e.what() << std::endl;
  }
}

#endif