#ifndef EVENTS
#define EVENTS

#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

//#define nodeindex uint32_t
//#define eventindex uint32_t
//#define timestamp uint32_t

#define nodeindex unsigned
#define eventindex unsigned
#define timestamp float

#define NOTIME 0
#define DEFAULTDURATION 0
#define DEFAULTDELAY 0

using namespace std;

/** Policy classes for storing time stamps. **/
class TimeStamp{
 private:
  timestamp time;
 protected:
  void readTime(ifstream &the_file);
 public:
  timestamp getTime() const;
  void setTime(timestamp time);
  TimeStamp();
  TimeStamp( const TimeStamp &event);
  string getLongTimeStr();
};
timestamp TimeStamp::getTime() const{
  return this->time;
}
void TimeStamp::setTime(timestamp time){
  this->time=time;
}
void TimeStamp::readTime(ifstream &the_file){
  the_file >> this->time;
}
TimeStamp::TimeStamp(){
}
TimeStamp::TimeStamp(const TimeStamp &event){
  this->time=event.time;
}
string TimeStamp::getLongTimeStr(){//convert TimeStamp time into string to print it
  stringstream s; 
  s << this->time;
  return string("time: ") + s.str();
}



class NoTimeStamp{
 public:
  timestamp getTime() const;
  void setTime(timestamp time);
  NoTimeStamp();
  NoTimeStamp( const NoTimeStamp &event);
  string getLongTimeStr();
 protected:
  void readTime(ifstream &the_file);
};
timestamp NoTimeStamp::getTime() const{
  return NOTIME;
}
void NoTimeStamp::setTime(timestamp time){
}
void NoTimeStamp::readTime(ifstream &the_file){
  timestamp dummy;
  the_file >> dummy;
}
NoTimeStamp::NoTimeStamp(){
}
NoTimeStamp::NoTimeStamp(const NoTimeStamp &event){
}
string NoTimeStamp::getLongTimeStr(){
  return string("no time");
}
//--


/** Policy classes for storing reversed information. **/
class Reverse{
 private:
  bool reversed;
 protected:
  void toggleReversed(); //For public access, use getReversedEvent()
 public:
  bool isReversed();
  Reverse();
  Reverse( const Reverse &event);
  string getLongReverseStr();
};
void Reverse::toggleReversed(){
  this->reversed = !this->reversed;
}
bool Reverse::isReversed(){
  return this->reversed;
}
Reverse::Reverse(){
  this->reversed=false;
}
Reverse::Reverse(const Reverse &event){
  this->reversed=event.reversed;
}
string Reverse::getLongReverseStr(){
  if(this->reversed){
    return string("reversed: true");
      }else{
    return string("reversed: false");
  }
}


class NoReverse{
 protected:
  void toggleReversed();
 public:
  bool isReversed();
  NoReverse();
  NoReverse( const NoReverse &event);
  string getLongReverseStr();
};
void NoReverse::toggleReversed(){
  cout << "Warning: trying to toggle reversed status of an event that cannot be reversed!" <<endl;
} //this should not be called
bool NoReverse::isReversed(){
  return false;
}
NoReverse::NoReverse(){
}
NoReverse::NoReverse(const NoReverse &event){
}
string NoReverse::getLongReverseStr(){
  return string("no reversing");
}

//--

/** Policy classes for storing durations. **/
class Duration{
 private:
  timestamp duration;
 protected:
  void readDuration(ifstream &the_file);
 public:
  timestamp getDuration() const;
  void setDuration(timestamp duration);
  Duration();
  Duration( const Duration &event);
  string getLongDurationStr();
};
timestamp Duration::getDuration() const{
  return this->duration;
}
void Duration::readDuration(ifstream &the_file){
  the_file >> this->duration;
}
Duration::Duration(){
}
Duration::Duration(const Duration &event){
  this->duration=event.duration;
}
void Duration::setDuration(timestamp duration){
  this->duration=duration;
}
string Duration::getLongDurationStr(){
  stringstream s;
  s << this->duration;
  return string("duration: ")+s.str();  
}



class NoDuration{
 public:
  timestamp getDuration() const;
  void setDuration(timestamp duration);
  NoDuration();
  NoDuration( const NoDuration &event);
  string getLongDurationStr();
 protected:
  void readDuration(ifstream &the_file);
};
timestamp NoDuration::getDuration() const{
  return DEFAULTDURATION;
}
void NoDuration::readDuration(ifstream &the_file){
  timestamp dummy;
  the_file >> dummy;
}
NoDuration::NoDuration(){
}
NoDuration::NoDuration(const NoDuration &event){
}
void NoDuration::setDuration(timestamp duration){
}
string NoDuration::getLongDurationStr(){
  return string("no duration");
}


/** Policy classes for storing index. **/
class Index{
 public:
  eventindex index;
  eventindex getIndex();
  eventindex setIndex(eventindex index);
  Index();
  Index( const Index &event);
  string getLongIndexStr();
};
eventindex Index::getIndex(){
  return this->index;
}
Index::Index(){
}
Index::Index( const Index &event){
  this->index=event.index;
}
eventindex Index::setIndex(eventindex index){
  this->index=index;
}
string Index::getLongIndexStr(){
  stringstream s;
  s << this->index;
  return string("index: ")+s.str();  
}


class NoIndex{
 public:
  string getIndex() const;
  void setIndex(int index);
  NoIndex();
  NoIndex( const Index &event);
};
string NoIndex::getIndex() const{
  return "No Index";
}
NoIndex::NoIndex(){
}
NoIndex::NoIndex(const Index &event){
}
void NoIndex::setIndex(int index){
}


//--




/** The main event class. **/
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
  class Event : public TimeStampPolicy, public ReversedPolicy, public DurationPolicy, public IndexPolicy
{
 public:
  nodeindex source;
  nodeindex dest;
  void readFromFile_mobile(ifstream &the_file);
  void readFromFile_sdt(ifstream &the_file);
  void copyAllButTime(Event& targetEvent);
  nodeindex getSource();
  nodeindex getDest();
  Event(); //Constructor
  Event( const Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> &event);//Copy constructor
  Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> getReversedEvent();
  string getLongEventInformation();
  
  friend ostream& operator << (ostream& os, const Event& event)// overload the operator <<
 {
  os << event.source << " " << event.dest << " ";
  return os ;
 }
  
};

//function returning the source node of an event
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
  nodeindex Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getSource(){
  return this->source;
}

//function returning the destination node of an event
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
nodeindex Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getDest(){
  return this->dest;
}

//a way to read an event from the_file
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::readFromFile_mobile(ifstream &the_file){
  unsigned dummy;
  this->readTime(the_file);
  the_file >> this->source;
  the_file >> dummy; //type 
  the_file >> this->dest;
  the_file >> dummy; //duration
}

//another way to read an event from the_file
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::readFromFile_sdt(ifstream &the_file){
  the_file >> this->source;
  the_file >> this->dest;
  this->readTime(the_file);
}

//copy the source and destination of an event to targetEvent
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::copyAllButTime(Event& targetEvent){
  targetEvent.source=this->source;
  targetEvent.dest=this->dest;
}

//Constructor
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
  Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::Event() : TimeStampPolicy(),ReversedPolicy(),DurationPolicy(),IndexPolicy(){
}

//Copy constructor
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
  Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::Event( const Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> &event) : TimeStampPolicy(event),ReversedPolicy(event),DurationPolicy(event),IndexPolicy(event){
    this->source=event.source;
    this->dest=event.dest;
}


//create the reversed event of the considered event with same characteristics 
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getReversedEvent(){
  Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> newEvent=Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>(*this);
  newEvent.toggleReversed();
  newEvent.dest=this->source;
  newEvent.source=this->dest;
  return newEvent;
}


//print information about the considered event
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
string Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getLongEventInformation(){
  stringstream s;
  s << "Event(source: " << this->source << ", dest: " << this->dest <<", " << this->getLongTimeStr()<< ", "<< this->getLongReverseStr() << ", " << this->getLongDurationStr() << ", " << this->getLongIndexStr() << ")";
  return s.str();
}

//Some simple names for event types
typedef Event<NoTimeStamp,NoReverse,NoDuration,NoIndex> SimpleEvent;
typedef Event<TimeStamp,NoReverse,NoDuration,NoIndex> EventWithTime;
typedef Event<TimeStamp,Reverse,NoDuration,Index> EventWithTimeRev;
typedef Event<TimeStamp,NoReverse,Duration,NoIndex> EventWithDuration;



//Misc

template<class EventType>
bool compare_sdt(const EventType & event1,const EventType & event2){//compare 2 events according to the source, then the destination, then the time
  if (event1.source!=event2.source)
    return event1.source<event2.source;
  else if (event1.dest!=event2.dest)
    return event1.dest<event2.dest;
  else
    return event1.getTime()<event2.getTime();
}

template<class EventType>
bool compare_time(const EventType & event1,const EventType & event2){//compare 2 events according to the time
  return event1.getTime()<event2.getTime();
}

template<class EventType>
bool compare_stime(const EventType & event1,const EventType & event2){//compare 2 events according to the source, then the time, then the destination 
  if (event1.source!=event2.source)
    return event1.source<event2.source;
  else if (event1.getTime()!=event2.getTime())
    return event1.getTime()<event2.getTime();
  else
    return event1.dest<event2.dest;
}


#endif //EVENTS
