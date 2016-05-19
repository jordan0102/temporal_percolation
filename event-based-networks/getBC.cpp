#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <limits.h>

#include "eventList.h"
#include "spreading.h"
#include "centralities.h"

/*
unsigned* getBC(EventList<Event>& eventList,size_t nodes,size_t firstNode, size_t firstEvent,size_t* eventBc){
  unsigned tree[nodes]; // links from infected nodes to infecting
  unsigned eventIndexInTree[nodes]; //event that got the node infected
  size_t infectionOrder[nodes];
  infectionOrder[0]=firstNode;
  InfectionProcess<Event> ip=InfectionProcess<Event>(nodes,eventList);
  ip.reset(firstNode,firstEvent);
  size_t orderCounter=0;
  while(ip.infectNext()){
    size_t i=ip.infectingEventIndex;
    size_t infected=ip.lastInfectedNode;
    size_t infecting;
    if (infected==eventList[i].source){
      infecting=eventList[i].dest;
    }else{
      infecting=eventList[i].source;
    }

    cerr << infecting << " " << infected<<endl;

    tree[infected]=infecting;
    eventIndexInTree[infected]=i;

    orderCounter++;
    infectionOrder[orderCounter]=infected;
  }

  unsigned* bc=new unsigned[nodes];
  for (int i=nodes-1;i>=0;i--){
    bc[i]=0;
  }



  for (int i=nodes-1;i>0;i--){
    size_t child=infectionOrder[i];
    size_t parent=tree[child];
    if (parent!=firstNode){
      bc[parent]=bc[parent]+1+bc[child];
      size_t eventIndex=eventIndexInTree[child];
      eventBc[eventIndex]+=1+bc[child];
    }
  }


  return bc;
}
*/


unsigned time_seed(){
  /*
    Return seed for a random number generator from system time.
    Code copied from: http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx
  */
  time_t now = time ( 0 );
  unsigned char *p = (unsigned char *)&now;
  unsigned seed = 0;
  size_t i;
  
  for ( i = 0; i < sizeof now; i++ )
    seed = seed * ( UCHAR_MAX + 2U ) + p[i];
  
  return seed;
}


void getInitialConditions(EventList<Event> e,nodeindex & startNode,size_t & startEvent,RandNumGen<> & rands){
  startEvent=rands.next(e.size);
  if (rands.next(1)==1)
    startNode=e.events[startEvent].source;
  else
    startNode=e.events[startEvent].dest;

}

int main(int argc, char *argv[]){
  /*
  ifstream startFile(argv[3]);
  if ( !startFile.is_open() ){
    cerr<<"Could not open file\n";
    return EXIT_FAILURE;
  }
  */

  if (argc!=6){
    cerr << "Usage: "<< argv[0] << " [eventlist] [numberOfEvent] [numberOfRounds] [nodeBcFile] [eventBcFile]" << endl;
    return EXIT_FAILURE;
  }

  int nRounds=atoi(argv[3]);

  unsigned seed=time_seed();
  cerr<< "Random seed: "  << seed << endl;
  RandNumGen<> rands(seed);
  //cerr << "First random number: " << rands.next(100000) << endl;


  ofstream nodebcFile(argv[4]);
  if ( !nodebcFile.is_open() ){
    cerr<<"Could not open file"<< argv[4]<<"\n";
    return EXIT_FAILURE;
  }
  
  ofstream eventbcFile(argv[5]);
  if ( !eventbcFile.is_open() ){
    cerr<<"Could not open file"<< argv[5]<< "\n";
    return EXIT_FAILURE;
  }
  

  int numberOfEvents=atoi(argv[2]);
  //int numberOfNodes=atoi(argv[3]);

  //EventList<Event> eventList=EventList<Event>(numberOfEvents);
  EventList<Event> eventList=EventList<Event>(numberOfEvents);
  eventList.readFile_tsd(argv[1]);
  //eventList.readFile_binary(argv[1]);
  eventList.sort_time();
  //eventList.timeSorted=true;
  eventList.normalized=true;
  //eventList.normalizeNodeIndices();
  int numberOfNodes=eventList.getNumberOfNodes();
  cerr << numberOfNodes<<endl;

  numberOfEvents=eventList.size;
  cerr << numberOfEvents<<endl;

  long *eventBC=new long[numberOfEvents];
  for (int i=0;i<numberOfEvents;i++){
    eventBC[i]=0;
  }

  cerr<<"eventBC filled."<<endl;
  long *nodeBC=new long[numberOfNodes];
  for (int i=0;i<numberOfNodes;i++){
    nodeBC[i]=0;
  }
  
  /*
  for (int eventIndex=0;eventIndex<numberOfEvents;eventIndex++){
    if (eventIndex%100==0)
      cerr << eventIndex <<endl;
    unsigned sNode=eventList[eventIndex].source;
    getBC<Event>(eventList,numberOfNodes,sNode,eventIndex,eventBC,nodeBC); 
   unsigned dNode=eventList[eventIndex].dest;
     getBC<Event>(eventList,numberOfNodes,sNode,eventIndex,eventBC,nodeBC); 
  }
  */
  cerr << "Now calculating the bc values..."<<endl;
  //while (!startFile.eof()){
  for (int round=0;round<nRounds;round++){
    cerr << "At round: "<< round << endl;
    nodeindex startNode;
    size_t startEvent;
    //startFile >> startNode;
    //startFile >> startEvent;
    getInitialConditions(eventList,startNode,startEvent,rands);
    cout << startNode << " " << startEvent << endl;
    getBC<Event>(eventList,numberOfNodes,startNode,startEvent,eventBC,nodeBC); 
  }

  //startFile.close();

  cerr << "Writing the bc values to disk..."<<endl;
  for (int i=0;i<numberOfNodes;i++)
    nodebcFile << nodeBC[i] <<endl;
  nodebcFile.close();

  for (int i=0;i<numberOfEvents;i++)
    eventbcFile << eventBC[i] <<endl;
  eventbcFile.close();

  //cerr << sizeof(eventList.events) <<endl;
  return EXIT_SUCCESS;
}

