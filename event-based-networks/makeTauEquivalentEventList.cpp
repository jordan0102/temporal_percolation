#include "eventNet.h";

int main(){
  cerr << "Reading in the event list." << endl;
  //EventList<EventWithTime> e(0,1167610118,1177970196);
  //e.readFile_tsd("tests/events_city_3_sdt.txt");

  EventList<EventWithTime> e(0,0,10);
  e.readFile_tsd("tests/smalleventlist.txt");

  //EventList<EventWithTime> e(0,0,10364399);
  ////e.readFile_tsd("/scratch/networks/rajkp/EventLists/eventList_original.txt");
  //e.readFile_binary("/scratch/networks/mtkivela/mobile/eventLists/eventList_original.bin");

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

	newEvent.time=n.getAvgTauOne(i,*j);
	tauOnes.push_back(newEvent);

	newEvent.time=n.getAvgTauZero(i,*j);
	cerr << newEvent.time<< " "<< n.getAvgTauZero(i,*j) <<endl;
	tauZeros.push_back(newEvent);
      }
    }
  }

  cerr << "Making tau-equivalent event list." << endl;
  //EventList<EventWithTime> ne(0,0,10364399);
  EventList<EventWithTime> ne(0,0,10);
  ne.makeTauZeroEquivalentList(tauZeros,10);
  ne.sort_sdt();
  
  /*
  for (vector<EventWithTime>::iterator i=ne.events.begin();i!=ne.events.end();++i){
    cout << (*i).getSource() << " "<< (*i).getDest() << " "<< (*i).getTime() << endl; 
  }
  */

  
  EventNet<EventWithTime> nne(ne);
  cerr << ne.totalTime << endl;

  for (int i=0;i<ne.getNumberOfNodes();i++) {
    for (DirNet<unsigned>::const_edge_iterator j=(*(nne.indexNet))(i).begin();!j.finished();++j) {
      if (i<*j){
	cout << i << " "<< (*j) << " " << n.getAvgTauZero(i,*j)<< " " << nne.getAvgTauZero(i,*j) << endl;
      }
    }
  }
  

  /*
  cout << n.getAvgTauOne(0,1) << endl;
  cout << n.getAvgTauInfApprox(0,1,a)<<endl;
  */
}
