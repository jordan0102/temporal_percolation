#ifndef CENTRALITIES
#define CENTRALITIES

template<class EventType>
void getBC(EventList<EventType>& eventList,unsigned nodes,unsigned firstNode, unsigned firstEvent,long* eventBc, long* nodeBc){
  unsigned *tree=new unsigned[nodes]; // links from infected nodes to infecting
  unsigned *eventIndexInTree= new unsigned[nodes]; //event that got the node infected
  unsigned *infectionOrder= new unsigned[nodes];
  infectionOrder[0]=firstNode;
  InfectionProcess<SimpleEvent> ip=InfectionProcess<SimpleEvent>(nodes,eventList);
  ip.reset(firstNode,firstEvent);
  unsigned orderCounter=0;
  while(ip.infectNext()){
    unsigned i=ip.infectingEventIndex;
    unsigned infected=ip.lastInfectedNode;
    unsigned infecting;
    if (infected==eventList[i].source){
      infecting=eventList[i].dest;
    }else{
      infecting=eventList[i].source;
    }

    //cerr << infecting << " " << infected<<endl;

    tree[infected]=infecting;
    eventIndexInTree[infected]=i;

    orderCounter++;
    infectionOrder[orderCounter]=infected;
  }

  unsigned *tempNodeBc= new unsigned[nodes]; 
  for (int i=nodes-1;i>=0;i--){
    tempNodeBc[i]=0;
  }



  for (int i=nodes-1;i>0;i--){
    unsigned child=infectionOrder[i];
    unsigned parent=tree[child];
    if (parent!=firstNode){
      tempNodeBc[parent]=tempNodeBc[parent]+1+tempNodeBc[child];
      unsigned eventIndex=eventIndexInTree[child];
      eventBc[eventIndex]+=1+tempNodeBc[child];
    }
  }

  for (int i=nodes-1;i>=0;i--){
    nodeBc[i]+=tempNodeBc[i];
  }
  
  delete tree;
  delete eventIndexInTree;
  delete infectionOrder;
  delete tempNodeBc;

}


#endif //CENTRALITIES
