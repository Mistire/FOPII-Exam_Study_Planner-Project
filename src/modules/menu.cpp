#include <iostream>
#include <fstream>
#include "menu.h"

using std::cout; using std::cin; using std::endl; using std::system; using std::fstream;

void displayAllUsers() {
    if (numOfUsers != 0) {
        for(int i = 0; i < numOfUsers; ++i) {
            cout << i+1 << ". " << users[i].name << "\n"; 
        }
    } else {
        cout << "NO USERS FOUND.\n";
    }
}

void printLine(char symbol, int length)
{
  for (int i = 0; i < length; ++i)
  {
    cout << symbol;
  }
  cout << endl;
}

void printCenteredText(const string &text, int lineLength)
{
  int spaces = (lineLength - text.length()) / 2;
  for (int i = 0; i < spaces; ++i)
  {
    cout << " ";
  }
  cout << text << endl;
}

void printUI(std::string sentence)
{
  cout << '\n';
  printLine('*', 40);
  printCenteredText(sentence, 40);
  printLine('*', 40);
  cout << '\n';
}

void displayTask(const User& user) {
    for(int i = 0; i < user.numOfTasks; ++i) {
      if(user.tasks[i].completed) {
        cout << "[x] Subject: " << user.tasks[i].subject << "\n";
      } else {
        cout << "[] Subject: " << user.tasks[i].subject << "\n";      
      }
      cout << "Description: " << user.tasks[i].description << "\n";
    }
    cout << "====\n";
}

void adminPage(User& currentAdmin) {
    User admin, user;
    string password;
    int choice;
    bool exited = false;

    do {
        system("cls");
        printUI("Welcome, " + currentAdmin.name);
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
            if (numOfAdmins >= MAX_ADMINS) {
                cout << "ADMIN CAPACITY REACHED.\n";
                system("PAUSE");
                break;
            }
            cout << "Enter Admin Username: ";
            cin >> admin.name;
            cout << "Enter Admin Password: ";
            cin >> admin.password;
            createUser(admin, numOfAdmins, admins);
            system("PAUSE");
            break;
        case 2:
            if (numOfAdmins == 1) {
                cout << "YOU ARE NOT ABLE TO DELETE ADMIN ACCOUNT.\n";
                system("PAUSE");
                break;
            }
            deleteUser(currentAdmin.name, numOfAdmins, admins);
            system("PAUSE");
            exited = true;
            break;
        case 3:
            cout << "Enter new password: ";
            cin >> password;
            if (password == currentAdmin.password) { 
                cout << "SAME PASSWORD AS THE PREVIOUS ONE -- tryagain with a new password.\n";
                system("PAUSE");
                break;
            }
            updateUserPassword(currentAdmin.name, password, numOfAdmins, admins);
            system("PAUSE");
            break;
        case 4:
            if (numOfUsers == 0) {
                cout << "NO USERS FOUND.\n";
                system("PAUSE");
                break;
            }
            cout << "Enter username: ";
            cin >> user.name;
            deleteUser(user.name, numOfUsers, users);
            break;
        case 5:
            system("cls");
            cout << "Here are all the users:\n";
            displayAllUsers();
            system("PAUSE");
            break;
        case 6:
            exited = true;
            break;
        default:
            cout << "Invalid choice. Try again.\n";
            system("PAUSE");
            break;
        }
    } while (!exited);
}

void studyTaskMenu (User& currentUser) {
    int choice;
    StudyTask task;
    string password;
    bool exited = false;

    while(!exited) {
      system("cls");
      printUI("Welcome, " + currentUser.name);
      cout << "Menu: \n";
      cout << "1. Add Task\n";
      cout << "2. Remove Task\n";
      cout << "3. Mark Complete\\Incomplete\n";
      cout << "4. Change Password\n";
      cout << "5. Go Back\n";
      cout << "Enter your choice: ";
      cin >> choice;
      cin.ignore();

      switch (choice)
      {
      case 1:
        if(currentUser.numOfTasks >= 100) {
          cout << "TASK CAPACITY REACHED.\n";
          system("PAUSE");
          break; 
        }
        cout << "Enter Task Subject: ";
        cin >> task.subject;
        cout << "Enter Task Description: ";
        cin >> task.description;

        if(taskExists(task.subject, currentUser)) {
          cout << "TASK EXISTS ALREADY.\n";
          system("PAUSE");
          break;
        }
        addTask(task, currentUser);
        system("PAUSE");
        break;
      case 2:
        if(currentUser.numOfTasks == 0) {
          cout << "NO TASKS LEFT.\n";
          system("PAUSE");
          break;
        }
        cout << "Enter the task subject you want removed: ";
        cin >> task.subject;
        removeTask(task.subject, currentUser);
        system("PAUSE");
        break;
      case 3:
        system("cls");
        if(currentUser.numOfTasks == 0) {
          cout << "NO TASKS LEFT.\n";
          system("PAUSE");
          break;
        }
        displayTask(currentUser);
        cout << "Which task do you want to update? [ENTER SUBJECT NAME].";
        cin >> task.subject;
        if(taskExists(task.subject, currentUser)) {
          updateTaskStatus(currentUser, task.subject);
          system("cls");
          displayTask(currentUser);
          system("PAUSE");
          break;
        }
        cout << "TASK DOESN'T EXIST.\n";
        system("PAUSE");
        break;
      case 4:
        cout << "Enter new password: ";
        cin >> password;
        if (password == currentUser.password) { 
            std::cout << "SAME PASSWORD AS THE PREVIOUS ONE -- tryagain with a new password.\n";
            system("PAUSE");
            break;
        }
        updateUserPassword(currentUser.name, password, numOfUsers, users);
        system("PAUSE");
        break;
      case 5:
        exited = true;
        break;
      default:
        cout << "Invalid choice. Try again.\n";
        system("PAUSE");
        break;
      }
    }
}