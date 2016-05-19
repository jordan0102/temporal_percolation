#ifndef EVENTNET
#define EVENTNET

#include <utility>
#include <vector>
#include <algorithm>
#include "eventList.h"
#include "lcelib/Nets.H"

using namespace std;


template<class elementType>
class pVector{ //a periodic vector
 public:
  vector<elementType> & baseVector;
  elementType range;
 pVector(vector<elementType> & baseVector, elementType range) : baseVector(baseVector), range(range){}
  elementType operator[](int index){
    int multiples,returnIndex;
    if (index <0){ // because %-operator only works for positive numbers
      multiples = 0;
      returnIndex=index;
      while (returnIndex<0){
	returnIndex=returnIndex+this->baseVector.size();
	multiples--;
      }
    } else {
      returnIndex = index % this->baseVector.size();
      multiples = (index - returnIndex)/this->baseVector.size();
    }
    return this->baseVector[returnIndex]+multiples * this->range;
  }  
};




template<class EventType> 
class EventNet{
 private:


 public:
  EventList<EventType> eventList;
  DirNet<unsigned> *indexNet;

  EventNet(EventList<EventType> & eventList);  
  ~EventNet();
  
  void getLinkEventsDirected(nodeindex a, nodeindex b, vector<EventType> & linkEvents);
  void getLinkEventsDirected(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & linkEvents);
  void getLinkEventsUndirected(nodeindex a, nodeindex b, vector<EventType> & linkEvents);
  void getLinkEventsUndirected(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & linkEvents);

  void getLinkEventTimesDirected(nodeindex a, nodeindex b, vector<timestamp> & linkTimes);
  void getLinkEventTimesDirected(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & linkEvents);

  void getLinkEventTimesUndirected(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & linkEvents);
  void getLinkEventTimesUndirected(nodeindex a, nodeindex b, vector<timestamp> & linkTimes);


  void getInterEventTimes(nodeindex a, nodeindex b, vector<timestamp> & linkIets, vector<size_t> & eventIndices);
  void getInterEventTimes(long * linkIets);
  void getInterEventTimes(vector<timestamp> & times, vector<timestamp> & iets);

  void getNeighborEvents(nodeindex a, nodeindex b, vector<timestamp> & neighborEvents);
  void getNeighborEvents(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & neighborEvents);
  void getNeighborEvents(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & neighborEvents);

  float getAvgTauZero(nodeindex a, nodeindex b);

  void getTauOnes(nodeindex a, nodeindex b, vector<timestamp> & tauOnes);
  float getAvgTauOne(nodeindex a, nodeindex b);

  float getAvgTauInfApprox(nodeindex a, nodeindex b, long * iets);
  float getAvgTauInfApprox(nodeindex a, nodeindex b, long * iets, SymmNet<unsigned> & aggNet);
  float getAvgTauInfApprox_neighborhoodIet(nodeindex a, nodeindex b);

  void makeLinkIetPreservingList(EventList<EventWithTime> & outList,int seed);

};

template<class EventType> 
void EventNet<EventType>::makeLinkIetPreservingList(EventList<EventWithTime> & outList,int seed){
  EventNet<EventWithTime> & inNet=*this;
  EventList<EventWithTime> & inList=inNet.eventList;
  
  if (inList.isSDTSorted()){
    cerr << "Error: the eventList needs to be sorted with SDT.\n" ;
    return;
  }
  

  timestamp timeWindowSize=inList.totalTime- inList.startTime;
  timestamp startTime=inList.startTime;
  outList.startTime=inList.startTime;
  outList.totalTime=inList.totalTime;

  outList.events.clear();

  RandNumGen<> rands(seed);
  
  vector<timestamp> iets;
  vector<size_t> dummy;

  int nLinks=0;
 
  for (int i=0;i<inList.getNumberOfNodes();i++) { 
    for (DirNet<unsigned>::const_edge_iterator j=(*(inNet.indexNet))(i).begin();!j.finished();++j) {
      if (i<*j){
	timestamp currentTime=rands.next(timeWindowSize);
	EventWithTime event;
	event.source=i;
	event.dest=*j;
	
	iets.clear();
	inNet.getInterEventTimes(i,*j,iets,dummy);
	//shuffle the iets.
	std::random_shuffle(iets.begin(),iets.end());

	nLinks++;

	for(int index=0;index<iets.size();++index){
	  currentTime = currentTime+iets[index];
	  event.setTime((currentTime%timeWindowSize)+startTime);
	  outList.events.push_back(event);
	}
      }
    }
  }

  outList.size=outList.events.size();

}


template<class EventType> 
EventNet<EventType>::EventNet(EventList<EventType> & eventList) : eventList(eventList){
//: indexNet(DirNet<unsigned>(eventList.getNumberOfNodes())){
//: indexNet(*(new DirNet<unsigned>(eventList.getNumberOfNodes()))){
  //events need to be sorted and normalized properly!


  //Create an aggregated network of indices refering to the first element of the edge in the event list.
  this->indexNet=new DirNet<unsigned>(this->eventList.getNumberOfNodes());
  DirNet<unsigned> &indexNet=*this->indexNet;
  for(size_t i = 0; i < this->eventList.size;++i){
    nodeindex source,dest;
    source=this->eventList[i].source;
    dest=this->eventList[i].dest;
    if (indexNet[source][dest]==0){
      indexNet[source][dest]=i+1;
      }
  }
}


template<class EventType> 
EventNet<EventType>::~EventNet(){
  //delete (this->indexNet);
}


template<class EventType>
void EventNet<EventType>::getLinkEventsDirected(nodeindex a, nodeindex b, vector<EventType> & linkEvents){
  size_t eventIndex=(*this->indexNet)[a][b];
  if (eventIndex!=0){
    eventIndex--;
  while(this->eventList[eventIndex].getSource()==a  && this->eventList[eventIndex].getDest()==b){
    linkEvents.push_back(this->eventList[eventIndex]);
    eventIndex++;
  }
  }
}

template<class EventType>
void EventNet<EventType>::getLinkEventsDirected(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & linkEvents){
  size_t eventIndex=(*this->indexNet)[a][b];
  if (eventIndex!=0){
    eventIndex--;
  while(this->eventList[eventIndex].getSource()==a  && this->eventList[eventIndex].getDest()==b){
    linkEvents.push_back(pair<EventType,size_t>(this->eventList[eventIndex],eventIndex));
    eventIndex++;
  }
  }
}


template<class EventType>
void EventNet<EventType>::getLinkEventTimesDirected(nodeindex a, nodeindex b, vector<timestamp> & linkTimes){
  size_t eventIndex=(*this->indexNet)[a][b];
  if (eventIndex!=0){
    eventIndex--;
    while(this->eventList[eventIndex].getSource()==a  && this->eventList[eventIndex].getDest()==b){
      linkTimes.push_back(this->eventList[eventIndex].getTime());
      eventIndex++;
    }
  }
}

template<class EventType>
void EventNet<EventType>::getLinkEventTimesDirected(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & linkEvents){
  size_t eventIndex=(*this->indexNet)[a][b];
  if (eventIndex!=0){
    eventIndex--;
    while(this->eventList[eventIndex].getSource()==a  && this->eventList[eventIndex].getDest()==b){
      linkEvents.push_back(pair<timestamp,size_t>(this->eventList[eventIndex].getTime(),eventIndex));
      eventIndex++;
    }
  }
}

template<class EventType>
void EventNet<EventType>::getLinkEventTimesUndirected(nodeindex a, nodeindex b, vector<timestamp> & linkTimes){
  this->getLinkEventTimesDirected(a,b,linkTimes);
  this->getLinkEventTimesDirected(b,a,linkTimes);
  sort(linkTimes.begin(), linkTimes.end());
}

template<class EventType>
void EventNet<EventType>::getLinkEventTimesUndirected(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & linkEvents){
  this->getLinkEventTimesDirected(a,b,linkEvents);
  this->getLinkEventTimesDirected(b,a,linkEvents);
  sort(linkEvents.begin(), linkEvents.end());
}




template<class EventType>
void EventNet<EventType>::getLinkEventsUndirected(nodeindex a, nodeindex b, vector<EventType> & linkEvents){
  this->getLinkEventsDirected(a,b,linkEvents);
  this->getLinkEventsDirected(b,a,linkEvents);
}

template<class EventType>
void EventNet<EventType>::getLinkEventsUndirected(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & linkEvents){
  this->getLinkEventsDirected(a,b,linkEvents);
  this->getLinkEventsDirected(b,a,linkEvents);
}

template<class EventType>
void EventNet<EventType>::getInterEventTimes(vector<timestamp> & times, vector<timestamp> & iets){
  pVector<timestamp> pTimes(times,this->eventList.totalTime-this->eventList.startTime);
  for (int i=0;i<times.size();++i){
    iets.push_back(pTimes[i]-pTimes[i-1]);
  } 
}

template<class EventType>
void EventNet<EventType>::getInterEventTimes(nodeindex a, nodeindex b, vector<timestamp> & linkIets, vector<size_t> & eventIndices){
  /*
    Writes the inter-event times of the edge sequences to the interEventTimes array.
   */  
  vector<pair<timestamp,size_t> > eventsAB;//=vector<pair<timestamp,size_t> >();
  eventsAB.clear();
  getLinkEventTimesUndirected(a,b,eventsAB);

  bool first=true;
  timestamp previous;
  vector<pair<timestamp,size_t> >::const_iterator i=eventsAB.begin();
  for (;i<eventsAB.end();++i){
    timestamp time=(*i).first;
    size_t index=(*i).second;
    if (first){ 
      first=false;
    } else {
      linkIets.push_back(time-previous);
      eventIndices.push_back(index);
    } 
    previous=time;
  }
  i=eventsAB.begin();
  linkIets.push_back(this->eventList.totalTime+(*i).first-previous-this->eventList.startTime);
  eventIndices.push_back((*i).second);

}

template<class EventType>
void EventNet<EventType>::getInterEventTimes(long * linkIets){
  //Iterate all the edges in the network
  for (int i=0;i<this->eventList.getNumberOfNodes();i++) {
    for (DirNet<unsigned>::const_edge_iterator j=(*indexNet)(i).begin();!j.finished();++j) {
      if (i<*j){
	vector<timestamp> abIets;vector<size_t> eventIndices;
	this->getInterEventTimes(i,*j,abIets,eventIndices);
	for (int k = 0;k<abIets.size();k++){
	  linkIets[eventIndices[k]]=abIets[k];
	}
      }
    } 
  }
}


template<class EventType>
void EventNet<EventType>::getNeighborEvents(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & neighborEvents){
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(a).begin();!j.finished();++j) {
    if (*j!=b)
      this->getLinkEventTimesUndirected(a,*j,neighborEvents);
  }
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(b).begin();!j.finished();++j) {
    if (*j!=a)
      this->getLinkEventTimesUndirected(b,*j,neighborEvents);
  }
  sort(neighborEvents.begin(), neighborEvents.end());
 
}


template<class EventType>
void EventNet<EventType>::getNeighborEvents(nodeindex a, nodeindex b, vector<timestamp> & neighborEvents){
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(a).begin();!j.finished();++j) {
    if (*j!=b)
      this->getLinkEventTimesUndirected(a,*j,neighborEvents);
  }
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(b).begin();!j.finished();++j) {
    if (*j!=a)
      this->getLinkEventTimesUndirected(b,*j,neighborEvents);
  }
  sort(neighborEvents.begin(), neighborEvents.end());
 
}


template<class EventType>
bool compare_event_time(const pair<EventType,size_t> & event1,const pair<EventType,size_t> & event2){
  return event1.first.getTime()<event2.first.getTime();
}

template<class EventType>
void EventNet<EventType>::getNeighborEvents(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & neighborEvents){
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(a).begin();!j.finished();++j) {
    if (*j!=b)
      this->getLinkEventsUndirected(a,*j,neighborEvents);
  }
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(b).begin();!j.finished();++j) {
    if (*j!=a)
      this->getLinkEventsUndirected(b,*j,neighborEvents);
  }
  sort(neighborEvents.begin(), neighborEvents.end(),compare_event_time<EventType>);
 
}


template<class EventType>
void EventNet<EventType>::getTauOnes(nodeindex a, nodeindex b, vector<timestamp> & tauOnes){
  vector<pair<timestamp,size_t> > neighborEvents;
  neighborEvents.clear();
  this->getNeighborEvents(a,b,neighborEvents);

  vector<timestamp> abTimes;
  this->getLinkEventTimesUndirected(a, b, abTimes);
  pVector<timestamp> abTimesPeriodic(abTimes,this->eventList.totalTime-this->eventList.startTime);

  int nextEventIndex=0;
  for (int i=0;i<neighborEvents.size();i++){
    timestamp neighborTime=neighborEvents[i].first;
    while(abTimesPeriodic[nextEventIndex]<neighborTime){
      nextEventIndex++;
    }
    tauOnes.push_back(abTimesPeriodic[nextEventIndex]-neighborTime);
  }
}


template<class EventType>
float EventNet<EventType>::getAvgTauOne(nodeindex a, nodeindex b){
  float sum=0;
  vector<timestamp> tauOnes;
  this->getTauOnes(a,b,tauOnes);
  for (int i=0;i<tauOnes.size();i++)
    sum+=tauOnes[i];
  return sum/float(tauOnes.size());
}
 

template<class EventType>
float EventNet<EventType>::getAvgTauZero(nodeindex a, nodeindex b){
  vector<timestamp> iets;
  vector<size_t> eventIndices; //we dont need this
  this->getInterEventTimes(a, b, iets, eventIndices);

  float tsum=0.0;
  float ttsum=0.0;
  for (vector<timestamp>::const_iterator i=iets.begin();i!=iets.end();++i){
    float iet=*i;
    tsum=tsum+iet;
    ttsum=ttsum+iet*iet;
  }
  return ttsum/2.0/tsum;
}



template<class EventType>
float EventNet<EventType>::getAvgTauInfApprox(nodeindex a, nodeindex b, long * iets){
  float weighedSum=0;
  float ietSum=0;

  vector<pair<timestamp,size_t> > neighborEvents;
  neighborEvents.clear();
  this->getNeighborEvents(a,b,neighborEvents);

  vector<timestamp> abTimes;
  this->getLinkEventTimesUndirected(a, b, abTimes);
  pVector<timestamp> abTimesPeriodic(abTimes,this->eventList.totalTime-this->eventList.startTime);

  int nextEventIndex=0;
  for (int i=0;i<neighborEvents.size();i++){
    timestamp neighborTime=neighborEvents[i].first;
    while(abTimesPeriodic[nextEventIndex]<neighborTime){
      nextEventIndex++;
    }
    timestamp tauOne=abTimesPeriodic[nextEventIndex]-neighborTime;
    long iet=iets[neighborEvents[i].second];
    weighedSum+=tauOne*iet;
    ietSum+=iet;
  }

  return weighedSum/ietSum;
}


template<class EventType>
float EventNet<EventType>::getAvgTauInfApprox(nodeindex a, nodeindex b, long * iets, SymmNet<unsigned> & aggNet){
  float weighedSum=0;
  float ietSum=0;

  vector<pair<EventType,size_t> > neighborEvents;
  neighborEvents.clear();
  this->getNeighborEvents(a,b,neighborEvents);

  vector<timestamp> abTimes;
  this->getLinkEventTimesUndirected(a, b, abTimes);
  pVector<timestamp> abTimesPeriodic(abTimes,this->eventList.totalTime-this->eventList.startTime);

  int nextEventIndex=0;
  for (int i=0;i<neighborEvents.size();i++){
    timestamp neighborTime=neighborEvents[i].first.getTime();
    while(abTimesPeriodic[nextEventIndex]<neighborTime){
      nextEventIndex++;
    }
    timestamp tauOne=abTimesPeriodic[nextEventIndex]-neighborTime;
    long iet=iets[neighborEvents[i].second];
    float weight=aggNet[neighborEvents[i].first.getSource()][neighborEvents[i].first.getDest()]; //wrong!
    weighedSum+=tauOne*iet*weight;
    ietSum+=iet*weight;
  }

  return weighedSum/ietSum;
}


template<class EventType>
float EventNet<EventType>::getAvgTauInfApprox_neighborhoodIet(nodeindex a, nodeindex b){
  float weighedSum=0;
  float ietSum=0;

  vector<timestamp> neighborEvents;
  neighborEvents.clear();
  this->getNeighborEvents(a,b,neighborEvents);

  vector<timestamp> neighborIets;
  this->getInterEventTimes(neighborEvents,neighborIets);

  vector<timestamp> abTimes;
  this->getLinkEventTimesUndirected(a, b, abTimes);
  pVector<timestamp> abTimesPeriodic(abTimes,this->eventList.totalTime-this->eventList.startTime);

  int nextEventIndex=0;
  for (int i=0;i<neighborEvents.size();i++){
    timestamp neighborTime=neighborEvents[i];
    while(abTimesPeriodic[nextEventIndex]<neighborTime){
      nextEventIndex++;
    }
    timestamp tauOne=abTimesPeriodic[nextEventIndex]-neighborTime;
    long iet=neighborIets[i];
    weighedSum+=tauOne*iet;
    ietSum+=iet;
  }

  return weighedSum/ietSum;
}



#endif //EVENTNET
