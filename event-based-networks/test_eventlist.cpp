#include "eventList.h"
#include "spreading.h"

int main(int argc, char *argv[]){
  cout << "Size of a simple event: " << sizeof(SimpleEvent) << endl;
  cout << "Size of an event with time: "<< sizeof(EventWithTime) << endl;

  int numberOfEvents=atoi(argv[2]);

  // simple events

  EventList<SimpleEvent> testList=EventList<SimpleEvent>(numberOfEvents);
  testList.readFile_tsd(argv[1]);

  cout << "Simple event list after reading the tsd file:" << endl;
  for (int i=0;i<testList.size;i++){
    cout << i<< ": " << testList[i].getLongEventInformation()<< endl;
  }

  //testList.normalizeNodeIndices();
  testList.writeFile_binary("/tmp/set2.bin");

  EventList<SimpleEvent> testList2=EventList<SimpleEvent>(0);  
  testList2.readFile_binary("/tmp/set2.bin");

  cout << "Simple event list after writing and reading binary file:" << endl;
  for (int i=0;i<testList2.size;i++){
    cout << i<< ": " << testList[i].getLongEventInformation()<< endl;
  }


  // events with time
  EventList<EventWithTime> testList_time=EventList<EventWithTime>(numberOfEvents);
  testList_time.readFile_tsd(argv[1]);

  cout << "Timed event list after reading the tsd file:" << endl;
  for (int i=0;i<testList_time.size;i++){
    cout << i<< ": " << testList_time[i].getLongEventInformation()<< endl;
  }

  //testList_time.normalizeNodeIndices();
  testList_time.writeFile_binary("/tmp/set2.bin");

  EventList<EventWithTime> testList_time2=EventList<EventWithTime>(0);  
  testList_time2.readFile_binary("/tmp/set2.bin");

  cout << "Timed event list after writing and reading binary file:" << endl;
  for (int i=0;i<testList_time2.size;i++){
    cout << i<< ": " << testList_time2[i].getLongEventInformation()<< endl;
  }
  

  // Reversed events with time
  typedef Event<TimeStamp,Reverse,NoDuration> EventWithTimeRev;
  EventList<EventWithTimeRev> testList_rev=EventList<EventWithTimeRev>(2*numberOfEvents);
  testList_rev.readFile_tsd(argv[1]);
  //Copy reversed
  for (int i=0;i<numberOfEvents;i++){
    testList_rev[i+numberOfEvents]=testList_rev[i].getReversedEvent();
  }

  cout << "Timed and reversed event list after reading the tsd file:" << endl;
  for (int i=0;i<testList_rev.size;i++){
    cout << i<< ": " << testList_rev[i].getLongEventInformation()<< endl;
  }

  testList_rev.writeFile_binary("/tmp/set2.bin");

  EventList<EventWithTimeRev> testList_rev2=EventList<EventWithTimeRev>(0);  
  testList_rev2.readFile_binary("/tmp/set2.bin");

  cout << "Timed and reversed event list after writing and reading binary file:" << endl;
  for (int i=0;i<testList_rev2.size;i++){
    cout << i<< ": " << testList_rev2[i].getLongEventInformation()<< endl;
  }



}
