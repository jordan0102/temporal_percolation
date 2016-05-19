#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "spreading.h"
#include "eventNet.h"




void makeTauEquivalentEventList(EventList<EventWithTime> & e, vector<EventWithTime> taus){
  cerr << "Making tau-equivalent event list." << endl;
  e.events.clear();
  e.makeTauZeroEquivalentList(taus,10);
  cerr << "The new event list has " << e.events.size() << " events." << endl;  

  //cerr << "Sorting the new event list." << endl;
  //e.sort_time();

}



void makeTauEquivalentEventList(EventList<EventWithTime> & e, int tauNumber){
  cerr << "Sorting the event list to source-dest-time order."<<endl; 
  e.sort_sdt();

  cerr << "Creating the index network."<< endl;
  EventNet<EventWithTime> n(e);//=EventNet<EventWithTime>(e);

  cerr << "Creating empty array of taus." << endl;  
  vector<EventWithTime> taus;
  
  cerr << "Calculating taus."<<endl;
  for (int i=0;i<e.getNumberOfNodes();i++) {
    for (DirNet<unsigned>::const_edge_iterator j=(*(n.indexNet))(i).begin();!j.finished();++j) {
      if (i<*j){
	EventWithTime newEvent;
	newEvent.source=i;
	newEvent.dest=*j;

	if (tauNumber==1)
	  newEvent.time=n.getAvgTauOne(i,*j);
	if (tauNumber==0)
	  newEvent.time=n.getAvgTauZero(i,*j);	
	taus.push_back(newEvent);
      }
    }
  }

  cerr << "Making tau-equivalent event list." << endl;
  e.events.clear();
  e.makeTauZeroEquivalentList(taus,10);
  cerr << "The new event list has " << e.events.size() << " events." << endl;  

}


int readEdges(char* fileName,vector<EventWithTime> &  taus,timestamp & start, timestamp & end){
  ifstream the_file (fileName);
  if ( !the_file.is_open() ){
    cerr<<"Could not open file\n";
    return 0;
  }

  string s;

  the_file >> start;
  the_file >> end;
  getline(the_file,s);

  float sum=0.0;
  float t=end-start;
  float cut=1000;

  nodeindex maxNode=0;


  while (getline(the_file,s)){
    istringstream is(s);
    EventWithTime edge;
    float tempTime;
    is >> edge.source;
    is >> edge.dest;
    is >> tempTime;
    edge.time = tempTime;

    //cout << edge.source << " " << edge.dest << " " << edge.time << endl;

    if (maxNode<edge.source) {maxNode=edge.source;}
    if (maxNode<edge.dest) {maxNode=edge.dest;}

    float nEq=t/edge.time-1.0;
    if (nEq>cut) nEq=cut;

    sum=sum + nEq;

    taus.push_back(edge);

    //cerr << edge.source << " " << edge.dest << " " << edge.time << endl;

    //taus.push_back(readFromFile_tsd(the_file));	
  }

  the_file.close();
  
  cerr << "Estimate for total number of events: " << sum << endl;
  cerr << "Max node index: "<< maxNode << endl; 
  return 1;
}

void ietShuffling(EventList<EventWithTime> & e){
  e.sort_sdt();
  EventNet<EventWithTime> net(e);
  EventList<EventWithTime> newE;
  net.makeLinkIetPreservingList(newE,1542341);
  std::swap(e.events,newE.events);
}


int main(int argc, char* argv[]){
  if (argc!=8){
    cerr << "Usage: "<< argv[0] << " [inputfile] [avgInfTime file] [avgInfCount file] [rounds] [shuffling] [format] [time resolution]"<< endl;
    return 1;
  }
  
  size_t nRounds = atoi(argv[4]);
  timestamp timeResolution = atoi(argv[7]);

  EventList<EventWithTime> e(0);

  if (strcmp(argv[5],"taufile")==0){
    vector<EventWithTime> taus;
    timestamp start,end;
    cerr << "Reading the tau values."<< endl;
    readEdges(argv[1],taus,start,end);
    e.startTime=start; e.totalTime=end;
    cerr << "Start time="<< start << ", End time=" << end << endl;
    cerr << "Found " << taus.size() << " tau values." << endl; 
    cerr << "Populating the tau-equivalent event list." << endl;
    makeTauEquivalentEventList(e,taus);
  } else {
    cerr << "Reading in the event list." << endl;
    if (strcmp(argv[6],"binary")==0) e.readFile_binary(argv[1]);
    else if (strcmp(argv[6],"tsd")==0) e.readFile_tsd(argv[1]);
    else {cerr << "Invalid file format identifier." <<endl;return 1;}
    e.findTimeWindowSize();

    //EventList<EventWithTime> e(0,0,10364399);
    //e.readFile_binary("/scratch/networks/mtkivela/mobile/eventLists/eventList_original.bin");

    cerr << "The event list has: " << e.events.size() << " events." << endl; 

    cerr << "Shuffling the event list."<<endl; 
    if (strcmp(argv[5],"timeshuffle")==0) e.timeShuffle();
    else if (strcmp(argv[5],"original")==0) ; // do nothing
    else if (strcmp(argv[5],"tauzero")==0) makeTauEquivalentEventList(e,0); 
    else if (strcmp(argv[5],"tauone")==0) makeTauEquivalentEventList(e,1); 
    else if (strcmp(argv[5],"iet")==0) ietShuffling(e); 
    else if (strcmp(argv[5],"offset")==0) e.offsetShuffle(123456);
    else if (strcmp(argv[5],"linkseq")==0) e.linksequenceShuffle(123456); 
    else {cerr << "Invalid shuffling identifier." <<endl;return 1;}
  }


  cerr << "The event list has: " << e.events.size() << " events." << endl; 

  cerr << "Sorting the event list."<<endl; 
  e.sort_time();

  vector<float> avgInfTime, avgInfCount; //is stack large enough?
  getAvgInfectionCurves<EventWithTime>(e,nRounds,avgInfTime,avgInfCount,timeResolution,10*e.getTimeWindowSize());

  cerr << "Writing results to files." << endl;
  ofstream avgInfTimeFile,avgInfCountFile;
  avgInfTimeFile.open(argv[2]);
  avgInfCountFile.open(argv[3]);
  for(int i=0;i<e.getNumberOfNodes();i++)
    avgInfTimeFile << avgInfTime[i] << endl;
  for(int i=0;i<avgInfCount.size();i++)
    avgInfCountFile << i*timeResolution << " " << avgInfCount[i] << endl;
  return 0;
}
