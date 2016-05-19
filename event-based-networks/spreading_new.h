#ifndef SPREADING
#define SPREADING

#include <vector>
#include "eventList.h"

/*
template<class EventType>
class NoOptimization{
  
};
*/

template<class EventType>
class CopyInfectingEvents{
 public:
  vector<size_t> rEvents;
  size_t rIndex;
  size_t rCut;
  size_t rSize;  

  void initialize_optimization(){
    this->rEvents= *(new vector<size_t>());
    this->rCut=floor(0.01*size);
    if (this->rCut<=2) this->rCut=0;
  }

  size_t getNextInfectingEvent(EventList<EventType> & eventList, size_t currentEvent, vector<bool> & infected){
    
  }

};


template<class EventType, class OptimizationPolicy>
  class InfectionProcess : OptimizationPolicy{
    using OptimizationPolicy::infectNext();
    using OptimizationPolicy::initialize_optimization();

 public:
  vector<bool> infected;
  size_t size;
  size_t numberOfInfected;
  //unsigned long time;
  //unsigned long timeInEvents;
  size_t infectingEventIndex;
  size_t lastInfectedNode;
  unsigned rounds;
  size_t startTime;
  
  EventList<EventType> &eventList;
  
 InfectionProcess(size_t size, EventList<EventType>& eventList): eventList(eventList){
    this->size=size;
    this->infected= *(new vector<bool>(size));
    this->initialize_optimization();
  }

  void reset(size_t startNode, size_t startTime);
  bool infectNext();
  void infectAll();
  unsigned long getTimeInEvents();
};


template<class EventType>
void InfectionProcess<EventType>::reset(size_t startNode, size_t startTime){
  this->startTime=startTime;
  this->rounds=0;
  //this->time=0;
  //this->timeInEvents=0;
  this->numberOfInfected=0;
  this->infectingEventIndex=startTime-1;

  //set all nodes to susceptible
  for(int i=0;i<size;i++){
    this->infected[i]=false;
  }

  //set the first node infected
  size_t candidate;
  if (startNode<0){
	candidate=rand()%this->size;
      } else{
	candidate=startNode;
      }
      this->infected[candidate]=true;
      this->numberOfInfected++;
}

template<class EventType>
bool InfectionProcess<EventType>::infectNext(){
  if (this->numberOfInfected>=this->size)
    return false;

  bool noInfection=true;
  if (this->size - this->numberOfInfected>this->rCut){
  while(noInfection){
    //this->timeInEvents++;
    this->infectingEventIndex++;
    if (this->eventList.size<=this->infectingEventIndex){
      this->infectingEventIndex=0;
      this->rounds++;
    }    

    size_t source=this->eventList[this->infectingEventIndex].source;
    size_t dest=this->eventList[this->infectingEventIndex].dest;

    if(this->infected[source]==true){
      if (this->infected[dest]==false){
	  this->infected[dest]=true;
	  noInfection=false;
	  this->lastInfectedNode=dest;	
      }
    } else if (this->infected[dest]==true){
      this->infected[source]=true;
      noInfection=false;
      this->lastInfectedNode=source;      
    }
  }
  } else{
    if (this->size - this->numberOfInfected==this->rCut){
      /*
      this->rEvents.clear();
      this->rIndex=0;
      for(size_t i=this->infectingEventIndex;i!=this->infectingEventIndex-1;i++){
	if (i==this->eventList.size){
	  i=0;
	}
	size_t source=this->eventList[i].source;
	size_t dest=this->eventList[i].dest;
	if ((!this->infected[source]) || (!this->infected[dest]) ){
	  this->rEvents.push_back(i);
	}
      }      
      this->rSize=rEvents.size();
      */
      this->rEvents.clear();
      bool rIndexNotSet=true;
      for(size_t i=0;i<=this->eventList.size;i++){
	size_t source=this->eventList[i].source;
	size_t dest=this->eventList[i].dest;
	if ((!this->infected[source]) || (!this->infected[dest]) ){
	  this->rEvents.push_back(i);
	  if (rIndexNotSet  && this->infectingEventIndex<i){
	    this->rIndex=this->rEvents.size()-1;
	    rIndexNotSet=false;
	  }
	}
      }      
      this->rSize=rEvents.size();
    }

  while(noInfection){
    this->infectingEventIndex=this->rEvents[this->rIndex];
    this->rIndex++;

    if (this->rEvents.size()<=this->rIndex){
      this->rIndex=0;
      this->rounds++;
    }    

    size_t source=this->eventList[this->infectingEventIndex].source;
    size_t dest=this->eventList[this->infectingEventIndex].dest;


    if(this->infected[source]==true){
      if (this->infected[dest]==false){
	  this->infected[dest]=true;
	  noInfection=false;
	  this->lastInfectedNode=dest;	
      }
    } else if (this->infected[dest]==true){
      this->infected[source]=true;
      noInfection=false;
      this->lastInfectedNode=source;      
    }


  }

  }
  this->numberOfInfected++;

  return true;

}


template<class EventType>
void InfectionProcess<EventType>::infectAll(){
  while(this->infectNext());
}

template<class EventType>
unsigned long InfectionProcess<EventType>::getTimeInEvents(){
  return this->infectingEventIndex+this->rounds*this->eventList.size-this->startTime;
}


#endif //SPREADING
