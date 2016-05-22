/**
*author Manthan and Rushil
*/
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include<boost/tokenizer.hpp>
#include<string>
#include "TaskBoard.h"
#include <thread>
#include <list>
#include "omp.h"

#define NUM_THREADS 4

TaskBoard task_board;
thread first;
bool stopAppendDataFlag=false;
list<Task*> pending_tasks;

int max_assign_time=50;

using namespace std;
using namespace boost;

void set_timer(double seconds){
    clock_t startTime = clock(); //Start timer
    double secondsPassed;
    double secondsToDelay = seconds;
    std::cout << "Time to delay: " << secondsToDelay << std::endl;
    bool flag = true;
    while(flag)
    {
        secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
        if(secondsPassed >= secondsToDelay)
        {
            //std::cout << secondsPassed << " seconds have passed" << std::endl;
            flag = false;
        }
    }
}

void process(){
    

    while(1){
    
	#pragma omp parallel
	{       
		int id = omp_get_thread_num()+1;		
		for(int i=id;i<=task_board.getNoOfCooks();i+=NUM_THREADS)
        	{					
			//until the task is not empty it will process the task one by one from task queue
			Cook* currentCook = task_board.getCook(i);
			while (!currentCook->isCookFree()) {
            
				//printf("Cid: %d",task_board.getCook(i)->getCookId());
				//printf("\n");		
				Task* task = currentCook->getFirstTask();
				//printf("%d order is being prepared by %d,%f!!\n",task->getOrderId(),currentCook->getCookId(),currentCook->getBusyTime());
                		set_timer(task->getPrepTime());			

				printf("%d order is ready!! Now cook %d is busy by %f\n",task->getOrderId(),currentCook->getCookId(),currentCook->getBusyTime());
            		}
        	}
	}
    }

/*
	#pragma omp parallel
	{
		int id = omp_get_thread_num()+1;		
		for(int i=id;i<=task_board.getNoOfCooks();i+=NUM_THREADS)
		{
			//until the task is not empty it will process the task one by one from task queue
			while (!task_board.getCook(i)->isCookFree()) {
				Task task = task_board.getCook(i)->getFirstTask();
				set_timer(task.getPrepTime());
				//cout << task.getOrderId() << " is ready" << endl;	            
				printf("%d order is ready!!\n",task.getOrderId());
			}        
		}
	}
*/
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
		//printf("-----------***************------------------%d, %f, %f\n",task_board.getCook(to_be_assigned_cook)->getCookId(),task_board.getCook(to_be_assigned_cook)->getBusyTime(),task->getPrepTime());		
		return false;
	}
    
	//After processing adding to most recent going to free cook		
	task_board.getCook(to_be_assigned_cook)->addTask(task);	
	return true;
    
}

void assign_data_cook(){

	//printf("Console test ------------Inside Assign to cook------- %d\n",pending_tasks.empty());
	while (!pending_tasks.empty()) {
        
		Task* currentTask = pending_tasks.front();
		pending_tasks.erase(pending_tasks.begin());
		//printf("Console test ------------Inside while-------%s\n",currentTask->toString());        
		bool addStatus;
		
		#pragma omp critical
		addStatus = addTaskToCook(currentTask);		

		if(addStatus){
			//stopAppendDataFlag = false;
		}
		else{
			//pending_tasks.erase(pending_tasks.begin());
			//pending_tasks.push_back(currentTask);
			//first.detach(); 
			if(pending_tasks.size() >= task_board.getNoOfCooks())		
			{
				list<Task*>::iterator it = pending_tasks.begin();
				++it;++it;++it;++it;
				pending_tasks.insert(it,currentTask);
			}
			else{
				pending_tasks.insert(pending_tasks.begin(),currentTask);
			}
			//printf("-----------------Not added----------------------------\n");             
			//stopAppendDataFlag = true;
			//break;
		}
  }
}

vector<Task*> generateOrders(int n){

	vector<Task*> orders(n);

  ifstream myfile ("orders1.csv");
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
        //cout << *beg << "\n";
        values[i++]=*beg;
      }
      orders[index++] = new Task(stoi(values[0],NULL,10),stoi(values[1],NULL,10),stod(values[2],NULL));
    }
    
    myfile.close();
  }

  else cout << "Unable to open file";   	
  return orders;

}

void appendNewData(){

	//while(1){
    
		//printf("Console test ------------Flag is %d",stopAppendDataFlag);
		//if(!stopAppendDataFlag)
		//{
			vector<Task*> generated_task = generateOrders(500);
			pending_tasks.insert(pending_tasks.end(),generated_task.begin(),generated_task.end());
		//}			
		//printf("Console test ------------Assign to cook called-------- %d",pending_tasks.size());		
		assign_data_cook();
		//set_timer(5);
    	//}    	
}


void removeOrder(int order_id){
    
   
}

int main(){
	
	// Initialise task board
	task_board = TaskBoard(4,10);

	thread third(process);
	
	//generate data
	//thread first(appendNewData);
	  appendNewData();

	//assign data to cooks
	//thread two(assign_data_cook);
    
	//process tasks
	//thread third(process);
    
	//first.join();
	//two.join();
	third.join();

	//generate data
	//appendNewData();

	//assign data to cooks
	//#pragma omp parallel
	/*	
	{
		int id = omp_get_thread_num();
		for(int i=0;i<pending_tasks.size();i++){
			Task currentTask = pending_tasks[i];
		
			if(addTaskToCook(currentTask)){
				//pending_tasks.erase(i);
			}
			else{
				break;
			}
		}
	}
	*/
	//process tasks
	//process();

	/*
	cout<< "-------------" << endl;
	cout << task_board.getNoOfCooks() << endl;
	Cook* c = task_board.getCook(1);
	c->addTask(Task(1,2,4.5));
	c->addTask(Task(1,3,10));
	cout<< "For cook 1: " << endl << c->toString() << endl;
	cout<<"Task removed: "<< c->getFirstTask().toString() << endl;
	cout<< "For cook 1 total tasks are: " << endl << c->toString() << " after remove"<< endl;
	*/
	//cout<< "-------------" << endl;
	//generateOrders(10);
	return 0;
}
