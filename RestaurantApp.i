%module RestaurantApp
%{
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <fstream>
#include<boost/tokenizer.hpp>
#include<string>
#include "TaskBoard.h"
#include <thread>
#include <list>
#include "omp.h"
#define NUM_THREADS 4
extern TaskBoard task_board;
extern list<Task*> pending_tasks;
extern queue<string> order_completion_queue;
extern list<Task*> pending_tasks_list;
extern int max_assign_time;

extern void process();
extern bool addTaskToCook(Task* task);
extern void assign_data_cook();
extern vector<Task*> generateOrders(string file_name, int n);
extern void appendNewData(string file_name, int n);
extern int testSquare(string name,int n);
extern void setTaskBoard();
extern void startApp();
extern string getPendingTaskStatus();
extern string getCompletedTaskStatus();
extern void startAppendData();
%}

%include "TaskBoard.h"
%include<std_string.i>

#define NUM_THREADS 4

extern TaskBoard task_board;
extern list<Task*> pending_tasks;
extern queue<string> order_completion_queue;
extern list<Task*> pending_tasks_list;

extern int max_assign_time;

extern void process();
extern bool addTaskToCook(Task* task);
extern void assign_data_cook();
extern vector<Task*> generateOrders(string file_name, int n);
extern void appendNewData(string file_name, int n);
extern int testSquare(string name,int n);
extern void setTaskBoard();
extern void startApp();
extern string getPendingTaskStatus();
extern string getCompletedTaskStatus();
extern void startAppendData();
