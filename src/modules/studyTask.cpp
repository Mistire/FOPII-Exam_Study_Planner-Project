#include "studyTask.h"
#include "user.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

void addTask(const StudyTask& task, User& user) {
    user.tasks[user.numOfTasks++] = task;
    cout << task.subject << " ADDED SUCCESSFULLY.\n";
}

void removeTask(const std::string& taskSubject, User& user) {
  bool noTask = true;
   for (int i = 0; i < user.numOfTasks; ++i) {
    if(user.tasks[i].subject == taskSubject) {
      for (int j = i; j < user.numOfTasks - 1; ++j) {
        user.tasks[j] = user.tasks[j + 1];
        --user.numOfTasks;
        noTask = false;
        cout << taskSubject << " DELETED SUCCESSFULLY";
        break; 
      }
    }
   }

   if (noTask) { cout << "TASK NOT FOUND.\n"; }
}

bool taskExists(const string taskSubject, const User& user) {

  for(int i = 0; i < user.numOfTasks; ++i) {
    if(taskSubject == user.tasks[i].subject) {
      return true;
    }
  }
  return false;
}

void updateTaskStatus(User& user, string taskSubject) {
  for(int i = 0; i < user.numOfTasks; i++) {
    if(user.tasks[i].subject == taskSubject) {
      bool temp = user.tasks[i].completed;
      user.tasks[i].completed = !temp;
      break;
    }
  }
}