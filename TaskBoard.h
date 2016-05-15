/**
*author Manthan and Rushil
*/
#include<iostream>
#include<queue>
#include<vector>
#include "Cook.h"

using namespace std;


class TaskBoard{
	private:
	vector<Cook*> task_board;	
	int NO_OF_COOK;
	int LAST_ORDER;
	
	public:	
	int SLOT_SPAN;
		
	TaskBoard(){
		//dummy
	}
	TaskBoard(int noc, int ss)
	{
		NO_OF_COOK = noc;
		SLOT_SPAN = ss;
		LAST_ORDER = 0;
		//Initialize the task_board with default values		
		//task_board = new int[NO_OF_COOK][SLOT_SPAN]();
		
		//Intialize queue for each cook
		task_board.reserve(NO_OF_COOK);
		for(int i=1;i<=NO_OF_COOK;i++){
			task_board.push_back(new Cook(i));			
		}
	}

	int getNoOfCooks(){
		return task_board.size();
	}
	void setNoOfCook(int n){
		NO_OF_COOK = n;
	}
	
	Cook* getCook(int cook){
		return task_board[cook-1];
	}

	int getLastOrder(){
		return LAST_ORDER;
	}

	void setLastOrder(int on){
		LAST_ORDER = on;
	}	
};
