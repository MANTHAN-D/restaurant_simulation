/** author Manthan and Rushil */
/**Class to generate orders in csv format */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include<boost/tokenizer.hpp>
#include<string>
#include "Task.h"

using namespace std;
using namespace boost;

void generateOrders(string file_name, int n){
   ofstream myfile (file_name);
  
   if (myfile.is_open())
   {
      	//Initialize random seed	
  	srand(time(NULL));
    myfile<< "Format: Order_No,Item_No,Prep_Time\n";  
  	int order_number = 1;
  	for(int i=0;i<n;i++)
  	{
  		double prep_time = (double)(rand() % 8) / 10;
  		prep_time = 3.0 + prep_time * (5.0);
  		myfile << order_number++ << "," << rand()%4 + 1 << "," << prep_time << "\n";
  	}

    myfile.close();
  }
  else cout << "Unable to create orders file";
}
void printTasks(string file_name){

  ifstream myfile (file_name);
  if (myfile.is_open())
  {
    string line;
    string values[3];int i;
    getline (myfile,line);
    while ( getline (myfile,line) )
    {      
      tokenizer<escaped_list_separator<char> > tok(line);
      i=0;
      for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg){
        //cout << *beg << "\n";
        values[i++]=*beg;
      }
      Task *t = new Task(stoi(values[0],NULL,10),stoi(values[1],NULL,10),stod(values[2],NULL));
      cout<<t->toString()<<endl;
    }
    myfile.close();
  }

  else cout << "Unable to open file"; 
}

int main () {
  generateOrders("orders50.csv",50);
  //To check the sanity of code
  //printTasks();
  return 0;
}
