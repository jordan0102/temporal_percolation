#include "eventNet.h"
#include "spreading.h"
#include "lcelib/Randgens.H"
#include <algorithm>



void makeLinkIetPreservingList(EventNet<EventWithTime> & inNet, EventList<EventWithTime> & outList,int seed){
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
	  event.time=(currentTime%timeWindowSize)+startTime;
	  outList.events.push_back(event);
	}
      }
    }
  }

  outList.size=outList.events.size();
}


int main(){
  cerr << "Reading in the event list." << endl;
  //EventList<EventWithTime> e(0,1167610118,1177970196);
  //e.readFile_tsd("tests/events_city_3_sdt.txt");

  //EventList<EventWithTime> e(0,0,10);
  //e.readFile_tsd("tests/smalleventlist.txt");

  EventList<EventWithTime> e(0,0,10364399);
  ////e.readFile_tsd("/scratch/networks/rajkp/EventLists/eventList_original.txt");
  e.readFile_binary("/scratch/networks/mtkivela/mobile/eventLists/eventList_original.bin");


  cerr << "The event list has: " << e.events.size() << " events." << endl; 

  cerr << "Sorting the event list."<<endl; 
  e.sort_sdt();

  cerr << "Creating the index network."<< endl;
  EventNet<EventWithTime> n(e);//=EventNet<EventWithTime>(e);

  cerr << "Creating the new event list." << endl;
  EventList<EventWithTime>* temp= new EventList<EventWithTime>(0,1167610118,1177970196);
  EventList<EventWithTime> & ne = *temp;
  //makeLinkIetPreservingList(n,ne,1542341);
  n.makeLinkIetPreservingList(ne,1542341);
  cerr << "The new event list has " << ne.events.size() << " events." << endl;  


  
  cerr << "Sorting the event list." << endl;
  ne.sort_time();
  
  cerr << "Filling the infection time array." <<endl;
  InfectionProcess<EventWithTime> inf(e.getNumberOfNodes(),ne);
  float* avgInfTime = new float[e.getNumberOfNodes()];
  for(int i=0;i<ne.getNumberOfNodes();i++) avgInfTime[i]=0;

  RandNumGen<> rands(1234);

  int nRounds=100;

  for (int round=0;round<nRounds;round++){
    cerr << "Entering round: " << round+1 <<endl;
    nodeindex startEvent=rands.next(e.size);
    timestamp startTime=ne.events[startEvent].time;
    nodeindex startNode;
    if (rands.next(1)==1)
      startNode=ne.events[startEvent].source;
    else
      startNode=ne.events[startEvent].dest;
    inf.reset(startNode,startEvent);

    while (inf.infectNext()){
      avgInfTime[inf.numberOfInfected]=avgInfTime[inf.numberOfInfected]+ne.events[inf.infectingEventIndex].time-startTime+(ne.totalTime-ne.startTime)*(inf.rounds);
    }
    
  }

  cerr << "Writing results to a file." << endl;
  for(int i=0;i<ne.getNumberOfNodes();i++)
    cout << avgInfTime[i]/float(nRounds) << endl;
 

  /*
  for (vector<EventWithTime>::iterator i=ne.events.begin();i!=ne.events.end();++i){
    cout << (*i).getSource() << " "<< (*i).getDest() << " "<< (*i).getTime() << endl; 
  }
  */

  /*
  EventNet<EventWithTime> nne(ne);
  cerr << ne.totalTime << endl;

  float sumTauOrig=0.0;
  float sumTauNew=0.0;

  for (int i=0;i<ne.getNumberOfNodes();i++) {
    for (DirNet<unsigned>::const_edge_iterator j=(*(nne.indexNet))(i).begin();!j.finished();++j) {
      if (i<*j){
	cout << i << " "<< (*j) << " " << n.getAvgTauZero(i,*j)<< " " << nne.getAvgTauZero(i,*j) << endl;
	sumTauOrig=sumTauOrig+n.getAvgTauZero(i,*j);
	sumTauNew=sumTauNew+nne.getAvgTauZero(i,*j);
      }
    }
  }
  cout << sumTauOrig << " " << sumTauNew << endl;
  */
}
