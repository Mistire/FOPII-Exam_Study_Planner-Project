#ifndef STUDY_TASK_H
#define STUDY_TASK_H

#include <string>
#include <iostream>
#include <fstream>

struct StudyTask {
  User user;
  std::string subject;
  std::string description;
  std::string date;
};

void addTask(StudyTask &task)
{
  cout << "Enter subject: ";
  getline(cin, task.subject);

  cout << "Enter task description: ";
  getline(cin, task.description);
}

void displayTask(const string &filename)
{
  ifstream file(filename);
  string line;

  if (file.is_open())
  {
    cout << "\n**** Tasks **** \n";
    while (file >> line)
    {
      if (line == "====")
      {
        cout << endl;
      }
      else
      {
        cout << line << "\n";
      }
    }
    file.close();
  }
  else
  {
    cout << "Unable to open this file. \n";
  }
}

void saveTaskToFile(const StudyTask &task, const string &filename, User& user)
{
  ofstream file(filename, ios::app);

  if (file.is_open())
  {
    file << user.name << "\n";
    file << task.subject << " " << task.description << '\n';
    file << "====\n";
    file.close();
    cout << "Task saved successfully!\n";
  }
  else
  {
    cout << "Unable to open the file.\n";
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

void studyTaskMenu (User& currentUser) {
    int choice;
    string filename = "study_tasks.txt";
    StudyTask task;
    string password;
    bool exited = false;

    while(!exited) {
      cout << "Menu: \n";
      cout << "1. Add Task\n";
      cout << "2. Display Task\n";
      cout << "3. Change User Password\n";
      cout << "4. Go Back\n";
      cout << "Enter your choice: ";
      cin >> choice;
      cin.ignore();

      switch (choice)
      {
      case 1:
        addTask(task);
        saveTaskToFile(task, filename, currentUser);
        break;
      case 2:
        system("cls");
        cout << "Here are all your tasks.";
        displayTask(filename);
        break;
      case 3:
        cout << "Enter new password: ";
        cin >> password;
        if (password == currentUser.password) { 
            std::cout << "SAME PASSWORD AS THE PREVIOUS ONE -- tryagain with a new password.\n";
            break;
        }
        updateUserPassword(currentUser.name, password, numOfUsers, users);
      case 4:
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

#endif