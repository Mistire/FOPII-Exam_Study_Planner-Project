#include "user.h"
#include "study_task.h"

int main()
{
  User user, userAdmin;
  int choice;
  bool exited = false, noAdmin = true;

  if (!(loadUsersFromFile("admin.txt", numOfAdmins, MAX_ADMINS, admins) && 
      loadUsersFromFile("users.txt", numOfUsers, MAX_USERS, users))) {
        std::cout << "UNABLE TO OPEN READ FILE.\n";
        exited = true;
      }
  
  
  std::system("cls");
  printUI("Welcome to the Study Task Manager!");

  while(!exited)
    {
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
            std::system("cls");
            printUI("Welcome, " + userAdmin.name);
            adminPage(userAdmin);
            noAdmin = false;
            break;
          }
        }
        if (noAdmin == true) {cout << "ENTER CORRECT ADMIN NAME AND PASSWORD.\n"; }
        break;
      case 2:
        cout << "Enter username: ";
        cin >> user.name;
        cout << "Enter password: ";
        cin >> user.password;
        
        if(userExists(user.name, user.password)) {
          std::system("cls");
          printUI("Welcome, " + user.name);
          studyTaskMenu(user);
          break;
        }
        
        cout << "USER DOESN'T EXIST -- try again or create a new account.\n";
        break;
      case 3:
        if(!(numOfUsers < MAX_USERS)) {
          cout << "USER CAPACITY REACHED.\n";
          break;
        }
        cout << "Enter username: ";
        cin >> user.name;
        cout << "Enter password: ";
        cin >> user.password;

        createUser(user, numOfUsers, MAX_USERS, users);
        std::system("cls");
        printUI("Welcome, " + user.name);
        //show tasks page
        break;
      case 4:
        loadUsersToFile("users.txt", numOfUsers, users);
        exited = true;
        break;
      default:
        cout << "Invalid choice. Try again.\n";
        break;
      }

    }

}