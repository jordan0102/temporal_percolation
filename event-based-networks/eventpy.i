%module eventpy
#define nodeindex unsigned
#define eventindex unsigned
#define timestamp unsigned
%include "carrays.i"
%{
#include "events.h"
#include "eventList.h"
#include "spreading.h"
#include "centralities.h"
#include "eventNet.h"
%}

%include "events.h"
%include "eventList.h"
%include "spreading.h"
%include "centralities.h"
%include "eventNet.h"
%template(elist) EventList<SimpleEvent>; 
%template(ip) InfectionProcess<SimpleEvent>; 
%template(getBC) getBC<SimpleEvent>; 
%template(SimpleEvent) Event<NoTimeStamp,NoReverse,NoDuration>;

%template(etlist) EventList<EventWithTime>;
%template(enet) EventNet<EventWithTime>;
%template(EventWithTime) Event<TimeStamp,NoReverse,NoDuration>;

%extend EventList<SimpleEvent> {
    SimpleEvent __getitem__(size_t index) {
         return $self->events[index];
    }
};

%extend EventList<EventWithTime> {
    EventWithTime __getitem__(size_t index) {
         return $self->events[index];
    }
};




%array_class(long, longArray);


