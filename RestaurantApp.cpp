/**
*author Manthan and Rushil
*/
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

TaskBoard task_board;
list<Task*> pending_tasks;

int max_assign_time=50;

using namespace std;
using namespace boost;

void process(){
    
	#pragma omp parallel
	{       
		int id = omp_get_thread_num()+1;		
		for(int i=id;i<=task_board.getNoOfCooks();i+=NUM_THREADS)
    {					
			//until the task is not empty it will process the task one by one from task queue
			Cook* currentCook = task_board.getCook(i);
			while (!currentCook->isCookFree()) {
   		
				Task* task = currentCook->getFirstTask();
				printf("%d order is being prepared by %d,%f!!\n",task->getOrderId(),currentCook->getCookId(),currentCook->getBusyTime());
                		usleep(task->getPrepTime()*1000000);			

				printf("%d order is ready!! Now cook %d is busy by %f\n",task->getOrderId(),currentCook->getCookId(),currentCook->getBusyTime());
				delete task;
      }
    }
	}
}

bool addTaskToCook(Task* task){
    
    int min_value = 1000;
    int to_be_assigned_cook = 1;

	
    
	for(int i=1;i<=task_board.getNoOfCooks();i++){
		Cook* currentCook = task_board.getCook(i);
		if(min_value > currentCook->getBusyTime()){
			min_value = currentCook->getBusyTime();
			to_be_assigned_cook = i;
            
		}
	}
    
	//If the cook is going to have minimum value but the whole stack for now is surpassing
	//the time that can be assigned then sending false as to know that task hasn't been assigned
	if((task_board.getCook(to_be_assigned_cook)->getBusyTime()+task->getPrepTime()) > max_assign_time){		
		return false;
	}
    
	//After processing adding to most recent going to free cook		
	task_board.getCook(to_be_assigned_cook)->addTask(task);	
	return true;
    
}

void assign_data_cook(){
	while (pending_tasks.size() > 0) {
        
		Task* currentTask = pending_tasks.front();
		pending_tasks.erase(pending_tasks.begin());
		bool addStatus;
		
		#pragma omp critical
		addStatus = addTaskToCook(currentTask);		

		if(addStatus){
			//Task added suucessfully
		}
		else{
			if(pending_tasks.size() >= task_board.getNoOfCooks())		
			{
				list<Task*>::iterator it = pending_tasks.begin();
				++it;++it;++it;++it;
				pending_tasks.insert(it,currentTask);				
			}
			else{
				pending_tasks.insert(pending_tasks.begin(),currentTask);
			}
			usleep(5000000);
		}
  }
}

vector<Task*> generateOrders(string file_name, int n){

	vector<Task*> orders(n);

  ifstream myfile (file_name);
  if (myfile.is_open())
  {
    string line;
    string values[3];int i,index=0;
    getline (myfile,line);
    while ( getline (myfile,line) )
    {      
      tokenizer<escaped_list_separator<char> > tok(line);
      i=0;
      for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg){
        values[i++]=*beg;
      }
      orders[index++] = new Task(stoi(values[0],NULL,10),stoi(values[1],NULL,10),stod(values[2],NULL));
    }
    
    myfile.close();
  }

  else cout << "Unable to open file";   	
  return orders;

}

void appendNewData(string file_name, int n){

    vector<Task*> generated_task = generateOrders(file_name, n);
    pending_tasks.insert(pending_tasks.end(),generated_task.begin(),generated_task.end());
	
		assign_data_cook();	
}


void removeOrder(int order_id){
    
   
}

int main(){
	
	// Initialise task board
	task_board = TaskBoard(4,10);

  //process tasks
	thread third(process);
	
	//generate data
	appendNewData("orders50.csv", 50);
  
	third.join();
	return 0;
}
