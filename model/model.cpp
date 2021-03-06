#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <math.h>
#include "../event-based-networks/eventList.h"
#include "../lcelib/Nets.H"
#include "../lcelib/nets/models/ErdosRenyi.H"
#include "../lcelib/nets/NetExtras.H"
#include "../lcelib/misc/DisjointSets.H"

using namespace std;


template<typename Generator>
double exp_distr(double beta, Generator & generator){
double proba;
proba=generator.nextNormed();//generate a random number uniformly from [0,1)
return (-log(proba)/beta);//generate exponentially distributed random number with parameter beta
}

template<typename Generator>
void generate_sequence_exp(vector<float>& l, int startTime, int endTime, double beta, Generator & generator){//generate in vector l a time sequence exponentially distributed going from startTime to endTime.
  float tdist = exp_distr(beta, generator);//compute the 1st time of the time sequence
  float trdist;
  bool b=true;
  if (tdist+startTime>endTime){//if it's over endTime stop the function without adding the 1st time.
    return;
  }
  else{
    l.push_back(tdist+startTime);//else add the new time to the sequence.
  }
  
  while(b==true){
    trdist = exp_distr(beta, generator);//compute the new time of the time sequence
    if(l.back()+trdist > endTime){//if it's over endTime stop the function without adding the new time.
      break;
    }
    l.push_back(l.back()+trdist);//add the new time to the time sequence vector
  }
  return;
}

template<typename Generator> 
void eveListGen(EventList<EventWithTimeRev>& events, size_t netSize, float k_ave, double beta, int startTime, int endTime, size_t randseed, Generator & generator){//generate an eventList in EventList<EventWithTimeRev>& events starting at startTime and ending at endTime, with a network size netSize, an average degree k_ave, a time sequence exponentially distributed with parameter beta. randseed is the value of the seed to generate 'randomly' the numbers. This function create a network and then for each edge create a time sequence at which the edges get activated.

float p = (k_ave/netSize);//compute the proba for the corresponding ER graph
int edge_number = ceil((netSize * k_ave)/2);//compute number of edge in the corresponding ER graph

EventWithTimeRev eve;//declaration of the event we are going to add to the event sequence

typedef SymmNet<float> NetType;
NetType G(netSize);//declaration of the network
ErdosRenyi(G, netSize, k_ave, generator);//creation of the ER network named G, with size netSize, average degree k_ave.


vector<float> timeSeq;//declaration of the time sequence

int i;
for (i=0; i != netSize; i++) {

  eve.source=i;//define the source of the event eve
  
  for (NetType::const_edge_iterator j=G(i).begin() ; !j.finished() ; ++j) {
    if(i<*j){//this double loop with if statement goes over all the neighbors of each node while the source remains smaller than the destination, ie. goes over all the edges in the network once.
    
      //cout << i << " " << *j << "\n";

      eve.dest=*j;//define the destination of the event eve
      
      timeSeq.clear();//clear the time sequence remaining from the previous edge
      generate_sequence_exp(timeSeq, startTime, endTime, beta, generator);//generate the time sequence for the current edge
      for (vector<float>::iterator it = timeSeq.begin() ; it != timeSeq.end(); ++it){//for each time in the time sequence
        //cout << " " << *it;

        eve.setTime(*it);//define the time of the event eve
        events.addEvent(eve);//add the event eve to the eventList events
        //cout << eve.getLongEventInformation() << "\n";
        
      }
    }
  }
}
return;
}

bool compare_dag(DAGevent i, DAGevent j){//compare 2 elements according to theirs dt
return (i.timeBetween < j.timeBetween);
}



int main(){

//==========Model Input============
size_t netSize = 1000;//Network size

size_t randseed = 10;//number of seeds for random number generator

float k_ave = 4;//average degree of the network

double beta = 2;//parameter of the exponential distribution

int startTime = 0;//starting time of the time window

int endTime = 1;//ending time of the time window
//===========================	

//==========DAG Input=============
string fileOut = "DAG.edg";//the file where the DAG will be saved, only if you want it to be saved, then uncomment the call of function DAGfile at line 137 
	
int delta = (endTime - startTime);//the dt for which we compute the DAG
	
bool allowReturn = true;//allow the 'ping pong' effect between 2 nodes. 
	
bool directed = false;//is the network directed
//===========================

//==========Analysis Input========
string pathResults = "distri/";//path where to save the distribution results

float step = 0.001;//the dt step at which measures are taken
//===========================




float p = (k_ave/netSize);//compute the proba for the corresponding ER graph
int edge_number = ceil((netSize * k_ave)/2);//compute number of edge in the corresponding ER graph


EventList<EventWithTimeRev> events;//declaration of the eventList

RandNumGen<> generator(randseed);//initialisation of the random number generator

eveListGen(events, netSize, k_ave, beta, startTime, endTime, randseed, generator);//creation of the eventList

//events.DAGfile(fileOut, delta, allowReturn, directed); //save the DAG in a file


vector<DAGevent> dag;//declaration of the vector, which will contain the DAG
events.DAG(dag, delta, allowReturn, directed);//creation of the DAG
sort(dag.begin(),dag.end(),compare_dag);//sort the dag with increasing dt


DisjointSetsForest<unsigned,true,true,TrackLengths<unsigned,float,true> > ds(events.size/2);//declaration of the disjoint sets forest, having (events.size/2) subsets of 1 event.

int j;
for(j=0; j<(events.size/2); j++){
  ds.setValue(j, events[j].getTime());//set the time at which the event happens
}


string sizeDfile;//declaration of the file name, where the size distribution will be saved
string lenDfile;//declaration of the file name, where the length distribution will be saved
stringstream count;
ofstream giantSize ("giantSize.dat");//open the file where the evolution of the giant component in size will be saved
ofstream scdGiantSize ("scdGiantSize.dat");//open the file where the evolution of the 2nd giant component in size will be saved
ofstream susceptibilitySize ("suscSize.dat");//open the file where the evolution of the susceptibility for the size will be saved
ofstream giantLen ("giantLen.dat");//open the file where the evolution of the giant component in duration will be saved
ofstream scdGiantLen ("scdGiantLen.dat");//open the file where the evolution of the 2nd giant component in duration will be saved
ofstream susceptibilityLen ("suscLen.dat");//open the file where the evolution of the susceptibility for the duration will be saved
ofstream avgDegree ("avgDegree.dat");//open the file where the evolution of the average degree will be saved

int counter=0;
int multiple=1;


vector<int> nodes;

for (vector<DAGevent>::iterator it = dag.begin() ; it != dag.end(); ++it){//goes over the DAG edges

    ds.mergeSets((*it).sourceEvent, (*it).destEvent);//merge 2 events in the disjoint sets forest, when those 2 events are linked in the DAG
    
    
    if(nodes.empty()==true){
      nodes.push_back((*it).sourceEvent);
      nodes.push_back((*it).destEvent);
    }
    if(find(nodes.begin(), nodes.end(), (*it).sourceEvent) != nodes.end()){}
    else{
      nodes.push_back((*it).sourceEvent);}
    
    if(find(nodes.begin(), nodes.end(), (*it).destEvent) != nodes.end()){}
    else{
      nodes.push_back((*it).destEvent);
    }
    
    
    if((float)trunc((*it).timeBetween/step)*step == (float)multiple*step){//after a certain increase in dt, look at the size distribution, the duration distribution and the giant components in size (LaCC) and duration (LoCC)
    
    
      //cout << "\n sizeDist : \n";
      //ds.printSizeDist();
      count.str("");
      count << (*it).timeBetween;
      sizeDfile=pathResults+"SizeDistribution_"+count.str()+".dat";//update the name of the file
      ds.printFileSizeDist(sizeDfile);//save the distribution in the file
    
      
      //cout << "\n lenDist : \n";
      //ds.printLenDist();
      lenDfile=pathResults+"LenDistribution_"+count.str()+".dat";//update the name of the file
      ds.printFileLenDist(lenDfile);//save the distribution in the file
      
      giantSize << (*it).timeBetween << " " << (float)ds.getGiantSize()/(events.size/2) << endl;//update the LaCC
      scdGiantSize << (*it).timeBetween << " " << (float)ds.getScdGiantSize(ds.getGiantSize())/(events.size/2) << endl;//update the 2nd LaCC
      susceptibilitySize << (*it).timeBetween << " " << ds.getSuscSize(ds.getGiantSize()) << endl;//update the susceptibility of the size
      
      cout << ds.getGiantLen() << "\n";
      giantLen << (*it).timeBetween << " " << (float)ds.getGiantLen()/(endTime - startTime) << endl;//update the LoCC
      scdGiantLen << (*it).timeBetween << " " << (float)ds.getScdGiantLen(ds.getGiantLen())/(endTime - startTime) << endl;//update the 2nd LaCC
      susceptibilityLen << (*it).timeBetween << " " << ds.getSuscLen(ds.getGiantLen()) << endl;//update the susceptibility of the length
      
      /*
      sort(source.begin(),source.end());
      int track = -1;
      int deg=0;
      for(vector<int>::iterator src = source.begin() ; src != source.end(); ++src){
        if(track != *src and track == -1){
          track = *src;
          deg+=1;
        }
        else if(track != *src and track != -1){
          sumDeg+=deg;
          nb_DAGnodes+=1;
          
          track = *src;
          deg+=1;
        }
        else{
          deg+=1;
        }
      }
      */
      avgDegree << (*it).timeBetween << " " << (float)counter/nodes.size() << endl;//save the average degree
      
      
      multiple +=1;
    }
    

    counter+=1;
}
giantSize.close();
scdGiantSize.close();
susceptibilitySize.close();
giantLen.close();
scdGiantLen.close();
susceptibilityLen.close();
avgDegree.close();

cout << "\n" << events.size << "\n";
cout << dag.size() << "\n";
cout << nodes.size() << "   "; 
//for (vector<int>::iterator pr = nodes.begin() ; pr != nodes.end(); ++pr){
//  cout << " " << *pr;
//}

return 0;
}
