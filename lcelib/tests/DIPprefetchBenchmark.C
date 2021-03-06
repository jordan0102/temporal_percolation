#define NDEBUG
#define GNU_PREFETCH
#include<iostream>
#include<cstdlib>
#include"../Containers.H"
#include<cassert>
#include<ctime>
#include<climits>

#define START_SIZE 1
#define CHECKS 1000000
#define PREF_DIST 6

using namespace std;

struct MyPolicy:public SetContainerPolicy<size_t> {
  static const size_t MagicEmptyKey=UINT_MAX;
};

struct MyParams:public DefaultContainerParams {
  typedef void StatusPolicy;
  typedef SmallHashController<85> HashController;
};


typedef Set<size_t, LinearHash, ValueTable, MyPolicy, MyParams> SetType;


int main(int argc, char* argv[]) {
  
  unsigned hashSize;
  bool successful=false;
  size_t prefDist;

  //std::cerr << MyPolicy::MagicEmptyKey << "is magic.";
  
  if (argc < 2) {
    prefDist=PREF_DIST;
    std::cerr << "Using default prefetch distance:" << prefDist << "\n";
  } else {  
    prefDist=atoi(argv[1]);
    std::cerr << "Prefetch distance:" << prefDist << "\n";
  }

  std::cerr << "Performing succesful seeks\n";
  successful=true;
  
  for (size_t initSize=7; initSize<14; initSize++) {
    std::cout << "\n";
    size_t log_num_hashes=11;
    size_t log_num_passes=11; /* Total: 2^27=128M*/

    size_t num_hashes=1 << log_num_hashes;
    size_t num_passes=1 << log_num_passes;
    //size_t maxSize=16*num_hashes*num_passes*4;
    hashSize=initSize;
    //std::cout << hashSize << " " << num_hashes << " " << num_passes << "\n";
    //std::cout << "Clocks per sec:" << CLOCKS_PER_SEC << "\n";
    //std::cout << "Max hash size in bytes=" << maxSize << "\n";
    do {
      clock_t totTime=0;
      for (size_t j=0;  j < num_hashes; ++j) {
	SetType hash(hashSize);
	//std::cerr << hash.getTableSize();
	size_t * randvals=new size_t[hashSize];
	if (successful) {
	  for (size_t i=0; i < hashSize; ++i) {
	    do {
	      randvals[i]=rand();
	    } while (hash.put(randvals[i])); /* Was present. */
	  }
	} else {
	  while (hash.size() < hashSize) hash.put(rand());
	  for (size_t i=0; i < hashSize; ++i) {
	    do {
	      randvals[i]=rand();
	    } while (hash.contains(randvals[i])); /* Was present. */
	  }
	}
	//std::cerr << hash.getTableSize();
	//std::cerr << "I " << j << " " << num_hashes << "\n";
	unsigned numFound=0;
	clock_t cpustart=clock();
	for (size_t k=0; k<num_passes; ++k) {
	  size_t i=0;
	  for (; i<prefDist; ++i) {
	    hash.prefetch(randvals[i]);
	  }
	  for (; i<hashSize; ++i) {
	    hash.prefetch(randvals[i]);
	    if (hash.contains(randvals[i-prefDist])) numFound++;
	  }
	  for (i=hashSize-prefDist; i<hashSize; ++i) {
	    if (hash.contains(randvals[i])) numFound++;
	  }
	}
	totTime+=(clock()-cpustart);
	assert((successful && (numFound==hashSize*num_passes)) || 
	       ((!successful) && (numFound==0)));
	delete[] randvals;
      }
      std::cout << hashSize << " " 
		<< ((((float) totTime) 
		     / hashSize*1000) / (num_hashes*num_passes)) << "\n";
      //<< " " << num_hashes << " " << num_passes << "\n";
      
      if (num_passes > num_hashes) { 
	num_passes >>= 1;
      } else {
	num_hashes >>= 1;
      }
      hashSize*=2;
    } while (num_hashes > 0);
  
  }
}

















