#include "eventNet.h";

int main(){
  cerr << "Reading in the event list." << endl;
  //EventList<EventWithTime> e(0,1167610118,1177970196);
  //e.readFile_tsd("tests/events_city_3_sdt.txt");

  //EventList<EventWithTime> e(0,0,10);
  //e.readFile_tsd("tests/smalleventlist.txt");

  EventList<EventWithTime> e(0,0,10364399);
  ////e.readFile_tsd("/scratch/networks/rajkp/EventLists/eventList_original.txt");
  e.readFile_binary("/scratch/networks/mtkivela/mobile/eventLists/eventList_original.bin");

  cerr << "Sorting the event list."<<endl; 
  e.sort_sdt();

  cerr << "Creating the index network."<< endl;
  EventNet<EventWithTime> n(e);//=EventNet<EventWithTime>(e);

  cerr << "Creating empty array of iets." << endl;
  //long a[e.size]; 
  long* a = new long[e.size];
  cerr << "Calculating the iets." <<endl;
  n.getInterEventTimes(a);

  /*
  vector<timestamp> tauOnes;
  n.getTauOnes(0,1,tauOnes);
  for (int i=0;i<tauOnes.size();i++)
    cout << tauOnes[i]<<endl;
  */

  cerr << "Aggregating the network." <<endl;
  SymmNet<unsigned> aggNet;
  for(size_t i = 0; i < e.size;++i){
    nodeindex source,dest;
    source=e[i].source;
    dest=e[i].dest;
    aggNet[source][dest]++;
  }
 



  cerr << "Calculating tauone."<<endl;
  for (int i=0;i<e.getNumberOfNodes();i++) {
    for (DirNet<unsigned>::const_edge_iterator j=(*(n.indexNet))(i).begin();!j.finished();++j) {
      if (i<*j){
	cout << i<< " " << *j<<" " <<n.getAvgTauOne(i,*j)<< " "<< n.getAvgTauInfApprox(i,*j,a)<< " " << n.getAvgTauInfApprox(i,*j,a,aggNet)  << " " << n.getAvgTauInfApprox_neighborhoodIet(i,*j)  <<endl;
      }
    }
  }

  /*
  cout << n.getAvgTauOne(0,1) << endl;
  cout << n.getAvgTauInfApprox(0,1,a)<<endl;
  */
}
