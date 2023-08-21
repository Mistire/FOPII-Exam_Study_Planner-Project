#include <iostream>
#include "modules/user.h"
#include "modules/menu.h"

using std::cout; using std::cin; using std::system;

int main()
{
  User user, userAdmin;
  int choice;
  bool exited = false, noAdmin = true;

  if (!loadAdminFromFile() 
      /*!loadUsersFromFile()*/) {
        cout << "UNABLE TO OPEN READ FILE.\n";
        system("PAUSE");
        exited = true;
      }
  
  
  while(!exited)
    {
      std::system("cls");
      printUI("Welcome to the Study Task Manager!");
      cout << "Login Menu:\n";
      cout << "1. Admin\n";
      cout << "2. Sign-In\n";
      cout << "3. Sign-Up\n";
      cout << "4. Exit\n";
      cout << "Enter your choice: ";
      cin >> choice;
      cin.ignore();


      switch (choice)
      {
      case 1:
        cout << "Enter Admin name: ";
        cin >> userAdmin.name;
        
        cout << "Enter Admin Password: ";
        cin >> userAdmin.password;
        
        for (int i = 0; i < MAX_ADMINS; ++i) {
          if (userAdmin.name == admins[i].name && userAdmin.password == admins[i].password) {
            adminPage(userAdmin);
            noAdmin = false;
            break;
          }
        }
        if(noAdmin) { cout << "ENTER CORRECT ADMIN NAME AND PASSWORD.\n"; system("PAUSE"); }
        break;
      case 2:
        cout << "Enter username: ";
        cin >> user.name;
        cout << "Enter password: ";
        cin >> user.password;
        
        if(userExists(user.name, user.password)) {
          studyTaskMenu(user);
          break;
        }
        
        cout << "USER DOESN'T EXIST -- try again or create a new account.\n";
        system("PAUSE");
        break;
      case 3:
        if(numOfUsers >= MAX_USERS) {
          cout << "USER CAPACITY REACHED.\n";
          system("PAUSE");
          break;
        }
        cout << "Enter username: ";
        cin >> user.name;
        cout << "Enter password: ";
        cin >> user.password;

        createUser(user, numOfUsers, users);
        studyTaskMenu(user);
        break;
      case 4:
        loadUsersToFile();
        loadAdminToFile();
        system("PAUSE");
        cout << "Exiting....\n";
        exited = true;
        break;
      default:
        cout << "Invalid choice. Try again.\n";
        system("PAUSE");
        break;
      }

    }

}