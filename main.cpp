#include "functions.h"
// #include "user.h"
// #include "study_task.h"

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <iostream>
#include <iomanip>
#include <sstream>

// Function to establish a connection to the MySQL database

int main()
{
  Users user, userAdmin;
  bool exited = false;
  int choice;
  sql::Connection *con = createConnection();
  try
  {
    printUI("wellcome to the study task Manager!");
    while (!exited)
    {
      cout << "Login Menu:\n"
           << "1. Admin\n"
           << "2. Sign-In\n"
           << "3. Sign-Up\n"
           << "4. Exit\n"
           << "Enter your choice: \n";
      cin >> choice;
      cin.ignore();
      switch (choice)
      {
      case 1:
        cout << "Enter Admin username: ";
        cin >> userAdmin.name;

        cout << "Enter Admin Password: ";
        cin >> userAdmin.password;
        if (userExists(con, type::Admin, userAdmin.name) && checkPassword(con, type::Admin, userAdmin.name, userAdmin.password))
        {
          system("cls");
          printUI("Welcome, " + userAdmin.name);
          adminPage(userAdmin);
          break;
        }
        else
        {
          cout << "ENTER CORRECT ADMIN NAME AND PASSWORD.\n";
          break;
        }

        // break;
      case 2:
        cout << "Enter username: ";
        cin >> user.name;
        cout << "Enter password: ";
        cin >> user.password;
        if (userExists(con, type::User, user.name) && checkPassword(con, type::User, user.name, user.password))
        {
          system("cls");
          printUI("Welcome, " + user.name);
          studyTaskMenu(user);
          break;
        }
        cout << "ENTER CORRECT user NAME AND PASSWORD.\n";

        break;

      case 3:
        cout << "Enter username: ";
        cin >> user.name;
        cout << "Enter password: ";
        cin >> user.password;
        if (!(userExists(con, type::User, user.name)))
        {
          add(type::User, con, user.name, user.password);
          system("cls");
          printUI("Welcome, " + user.name);
          studyTaskMenu(user);
        }
        else
        {
          std::cout << "the user name already exists\n";
        }
        // show tasks page

        break;
      case 4:
        std::cout << "exiting...\nexiting...";
        exited = true;
        delete con;
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
