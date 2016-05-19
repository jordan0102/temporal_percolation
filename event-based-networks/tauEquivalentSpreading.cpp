#include "eventNet.h";
#include "spreading.h"
#include "lcelib/Randgens.H"

int main(){
  cerr << "Reading in the event list." << endl;
  EventList<EventWithTime> e(0,1167610118,1177970196);
  e.readFile_tsd("tests/events_city_3_sdt.txt");

  //EventList<EventWithTime> e(0,0,10);
  //e.readFile_tsd("tests/smalleventlist.txt");

  //EventList<EventWithTime> e(0,0,10364399);
  ////e.readFile_tsd("/scratch/networks/rajkp/EventLists/eventList_original.txt");
  //e.readFile_binary("/scratch/networks/mtkivela/mobile/eventLists/eventList_original.bin");


  cerr << "The event list has: " << e.events.size() << " events." << endl; 

  cerr << "Sorting the event list."<<endl; 
  e.sort_sdt();

  cerr << "Creating the index network."<< endl;
  EventNet<EventWithTime> n(e);//=EventNet<EventWithTime>(e);

  cerr << "Creating empty array of taus." << endl;  
  vector<EventWithTime> tauZeros;
  vector<EventWithTime> tauOnes;
  
  cerr << "Calculating taus."<<endl;
  for (int i=0;i<e.getNumberOfNodes();i++) {
    for (DirNet<unsigned>::const_edge_iterator j=(*(n.indexNet))(i).begin();!j.finished();++j) {
      if (i<*j){
	EventWithTime newEvent;
	newEvent.source=i;
	newEvent.dest=*j;

	//newEvent.time=n.getAvgTauOne(i,*j);
	//tauOnes.push_back(newEvent);

	newEvent.time=n.getAvgTauZero(i,*j);	
	tauZeros.push_back(newEvent);
      }
    }
  }

  cerr << "Making tau-equivalent event list." << endl;
  //EventList<EventWithTime>* temp= new EventList<EventWithTime>(0,0,10364399);
  EventList<EventWithTime>* temp= new EventList<EventWithTime>(0,1167610118,1177970196);
  EventList<EventWithTime> & ne = *temp;
  //EventList<EventWithTime> ne(0,0,10);
  ne.makeTauZeroEquivalentList(tauZeros,10,1542341);
  cerr << "The new event list has " << ne.events.size() << " events." << endl;  


  /*
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
  */

  /*
  for (vector<EventWithTime>::iterator i=ne.events.begin();i!=ne.events.end();++i){
    cout << (*i).getSource() << " "<< (*i).getDest() << " "<< (*i).getTime() << endl; 
  }
  */


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

}
