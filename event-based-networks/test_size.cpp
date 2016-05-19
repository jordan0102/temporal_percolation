#include "eventList.h"
//#include "spreading.h"
struct empty {};
int main(int argc, char *argv[]){
  
  Uevent<unsigned,unsigned,unsigned> t=Uevent<unsigned,unsigned,unsigned>();
  Uevent<empty,empty,empty> s=Uevent<empty,empty,empty>();
  //Event t=Event();
  cout << sizeof(s) <<endl;
  /*
  int numberOfEvents=atoi(argv[2]);
  //numberOfEvents=atoi(argv[2]);
  EventList<Event> testList=EventList<Event>(numberOfEvents);
  testList.readFile_tsd(argv[1]);
  //testList.normalizeNodeIndices();
  testList.writeFile_binary("/tmp/set2.bin");

  EventList<Event> testList2=EventList<Event>(0);  
  testList2.readFile_binary("/tmp/set2.bin");

  cout <<sizeof(testList2.events)<<endl;
  cout <<testList2.size <<endl;
  */

}
