/**
*author Manthan and Rushil
*/
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <ctime>
#include <cstdlib>
#include "TaskBoard.h"
#include <thread>
#include <list>
#include "omp.h"

#define NUM_THREADS 4

TaskBoard task_board;
thread first;
bool stopAppendDataFlag=false;
list<Task*> pending_tasks;

int max_assign_time=100;

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

vector<Task*> generateOrders(int n){

	vector<Task*> orders(n);

	//Initialize random seed	
	srand(time(NULL));

	int order_number = task_board.getLastOrder();
	for(int i=0;i<n;i++)
	{
		double prep_time = (double)(rand() % 8) / 100;
		prep_time = 6.0 + prep_time * (2.0);
		orders[i] = new Task(order_number++,rand()%4 + 1,prep_time);
		cout<< orders[i]->toString() << endl;
	}
	task_board.setLastOrder(order_number);
	return orders;

}

void process(){
    

    while(1){
    
	#pragma omp parallel
	{       
		int id = omp_get_thread_num()+1;		
		for(int i=id;i<=task_board.getNoOfCooks();i++)
        	{					
			//until the task is not empty it will process the task one by one from task queue
			Cook* currentCook = task_board.getCook(i);
			while (!currentCook->isCookFree()) {
            
				//printf("Cid: %d",task_board.getCook(i)->getCookId());
				//printf("\n");		
				Task* task = currentCook->getFirstTask();
				//printf("%d order is being prepared by %d,%f!!\n",task->getOrderId(),currentCook->getCookId(),currentCook->getBusyTime());
                		set_timer(task->getPrepTime());
				printf("%d order is ready!!\n",task->getOrderId());
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
		printf("-----------***************------------------%d, %f, %f\n",task_board.getCook(to_be_assigned_cook)->getCookId(),task_board.getCook(to_be_assigned_cook)->getBusyTime(),task->getPrepTime());		
		return false;
	}
    
	//After processing adding to most recent going to free cook		
	task_board.getCook(to_be_assigned_cook)->addTask(task);	
	return true;
    
}

void assign_data_cook(){

        while (!pending_tasks.empty()) {
        
            Task* currentTask = pending_tasks.front();
            pending_tasks.erase(pending_tasks.begin());
        
            if(addTaskToCook(currentTask)){
		stopAppendDataFlag = false;
            }
            else{
                //pending_tasks.erase(pending_tasks.begin());
                //pending_tasks.push_back(currentTask);
		//first.detach(); 
		if(pending_tasks.size() >= task_board.getNoOfCooks())		
		{
			pending_tasks.insert(pending_tasks.begin(),task_board.getNoOfCooks(),currentTask);
		}
		else{
			pending_tasks.insert(pending_tasks.begin(),0,currentTask);
		}
		printf("-----------------Not added----------------------------\n");             
		stopAppendDataFlag = true;
		break;
            }
        }
}

void appendNewData(){

	while(1){
    
		if(!stopAppendDataFlag)
		{
			vector<Task*> generated_task = generateOrders(10);
			pending_tasks.insert(pending_tasks.end(),generated_task.begin(),generated_task.end());
		}			
		assign_data_cook();
		set_timer(5);
    	}    	
}


void removeOrder(int order_id){
    
   
}

int main(){
	
	// Initialise task board
	task_board = TaskBoard(4,10);

	//generate data
	thread first(appendNewData);

	//assign data to cooks
	//thread two(assign_data_cook);
    
	//process tasks
	thread third(process);
    
	first.join();
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
