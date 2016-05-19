
In file dagMaker2.cpp, 5 inputs are required : 

* string fileIn : The file in which the eventlist is, with the following format : i j t

* string fileOut : The output file in which DAG link sequence is returned

* int delta : The time step in which events are linked if they share at least one node

* bool allowReturn : allow the link creation between between 2 events involving the same nodes, ie. A calls B, then B calls A

* bool directed : is the network directed or not.



Algorithm : 


In eventList.h, the template function 'void DAG' take as template argument the event type considered for the event list, eg. eventWithTime, eventWithDuration, ... 
Then it processes as follows : 


	+ give to each event an ID 
	+ for each event in the event list add the corresponding reversed event with the 		same ID
	+ sort the even list firstly according to the source nodes and if they are the 		same according to the time (so now we have a sorted list of events per user)
	+ for each user, go through his events : 
		+ if the event is a reversed event : 
			add the event to the deque of reversed events 'lastReversedEvents'
		+ else : 
			remove each event of 'lastReversedEvents', for which the difference in time with the considered event is larger than delta
			
			add a link between the current event and the events in 'lastReversedEvents' 
			if the network is undirected : 
				add the current event to lastReversedEvents (because information can flow in both directions). 
				
		
!!!!! WARNING !!!!! 

DO NOT USE AN EDGE LIST HAVING EVENTS HAPPENING AT THE SAME TIME 
