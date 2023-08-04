#include <iostream>
#include <fstream>
#include "study_task.h"

using namespace std;

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
    cout << "**** Tasks **** \n";
    while (getline(file, line))
    {
      if (line == "====")
      {
        cout << endl;
      }
      else
      {
        cout << line;
      }
    }
    file.close();
  }
  else
  {
    cout << "Unable to open this file. \n";
  }
}
void saveTaskToFile(const StudyTask &task, const string &filename)
{
  ofstream file(filename, ios::app);

  if (file.is_open())
  {
    file << task.subject << '\n';
    file << task.description << '\n';
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

void printUI()
{
  printLine('*', 40);
  printCenteredText("Welcome to the Study Task Manager!", 40);
  printLine('*', 40);
}

int main()
{
  const string filename = "study_tasks.txt";
  char choice;
  do
  {
    StudyTask task;

    printUI();

    cout << "Menu: \n";
    cout << "1. Add task \n";
    cout << "2. Display Task\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case '1':
      addTask(task);
      saveTaskToFile(task, filename);
      break;
    case '2':
      displayTask(filename);
      break;
    case '3':
      cout << "Exiting....\n";
      break;
    default:
      cout << "Invalid choice. Try sgain.\n";
      break;
    }

  } while (choice != '3');
  return 0;
}