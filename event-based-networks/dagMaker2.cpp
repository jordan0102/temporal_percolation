#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include "eventList.h"


using namespace std;


int main () 
{
//==========Input============
	string fileIn = "test.dat";//the dataset file
	
	string fileOut = "DAG.edg";//the file where the DAG will be saved
	
	int delta=2;//the dt for which we compute the DAG
	
	bool allowReturn = true;//allow the 'ping pong' effect between 2 nodes. 
	
	bool directed = false;//is the network directed

	EventList<EventWithTimeRev> evenements; //the input is the <EventType>
//===========================	
	
	evenements.readFile_sdt((char*)fileIn.c_str()); //create the eventList from the file fileIn 
	
	evenements.DAGfile(fileOut, delta, allowReturn, directed); //makes the DAG
		
	return 0;
}
