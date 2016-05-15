/**
*author Manthan and Rushil
*/
#include<iostream>
#include<string>
#include<sstream>

using namespace std;

class Task{
	private:
	int order_id;
	int item_id;
	double prep_time;	
	bool is_completed;
	
	public:
	Task(){
		//default
	}		
	Task(int orderId, int itemId, double prepTime)
	{
		order_id = orderId;
		item_id = itemId;
		prep_time = prepTime;
		is_completed = false;
	}
	
	int getOrderId(){
		return order_id;
	}

	int getItemId(){
		return item_id;
	}	
	double getPrepTime(){
		return prep_time;
	}	
	bool isCompleted(){
		return is_completed;
	}

	void setOrderId(int orderid){
		order_id=orderid;
	}
	void setItemId(int itemid){
		item_id=itemid;
	}
	void setPrepTime(double preptime){
		prep_time=preptime;
	}
	void setCompleted(bool completed){
		is_completed=completed;
	}

	string toString(){
		stringstream ss;
		ss <<"Order Id: " << order_id << " Item Id: " << item_id << " Prep time: "<< prep_time << " Completed: "<< is_completed;
		return ss.str();
	}
};

