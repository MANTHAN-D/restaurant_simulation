/**
*author Manthan and Rushil
*/
#include<iostream>
#include<queue>
#include<sstream>
#include "Task.h"


using namespace std;

class Cook{
	private:
	int cook_id;	
	double current_occupied_time;	
	queue<Task*> task_queue;
	
	public:			
	Cook(int id)
	{
		cook_id = id;		
		current_occupied_time=0;
	}
	
	double getBusyTime(){
		return current_occupied_time;
	}

	int getCookId(){
		return cook_id;
	}

	int getTaskCount(){
		return task_queue.size();
	}

	void addTask(Task* t){	
		task_queue.push(t);
		current_occupied_time+=t->getPrepTime();
	}

	Task* getFirstTask(){
		Task* t = task_queue.front();
		task_queue.pop();
		current_occupied_time-=t->getPrepTime();
		return t;
	}

	bool isCookFree(){     	
		return task_queue.empty();
    	}
	
	string toString(){
		stringstream ss;
		ss << "Cook " << cook_id << ": Occupied time: " << current_occupied_time << " Tasks in queue: " << task_queue.size();
		return ss.str();
	}
};

