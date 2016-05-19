#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "events.h"
#include "eventList.h"

int main(int argc, char* argv[]){
  if (argc!=3) cerr << "Usage: sdt2bin [inputfile] [outputfile]" << endl;

  EventList<EventWithTime> e(0);
  e.readFile_tsd(argv[1]);
  e.writeFile_binary(argv[2]);

  return 0;
}
