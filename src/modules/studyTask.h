#ifndef STUDYTASK_H
#define STUDYTASK_H

#include "user.h"


void addTask(const StudyTask& task, User& user);
void removeTask(const std::string& taskSubject, User& user);
bool taskExists(const string taskSubject, const User& user);
void updateTaskStatus(User& user, string taskSubject);

#endif // STUDYTASK_H
