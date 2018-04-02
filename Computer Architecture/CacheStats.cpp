#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "CacheStats.h"
using namespace std;

CacheStats::CacheStats() {
  cout << "Cache Config: ";
  if(!CACHE_EN) {
    cout << "cache disabled" << endl;
  } else {
    cout << (SETS * WAYS * BLOCKSIZE) << " B (";
    cout << BLOCKSIZE << " bytes/block, " << SETS << " sets, " << WAYS << " ways)" << endl;
    cout << "  Latencies: Lookup = " << LOOKUP_LATENCY << " cycles, ";
    cout << "Read = " << READ_LATENCY << " cycles, ";
    cout << "Write = " << WRITE_LATENCY << " cycles" << endl;
  }
  // initialize counters to 0
  loads = 0;
  stores = 0;
  load_misses = 0;
  store_misses = 0;
  writebacks = 0;

  /* TODO: your code here */
  // go through all the cache entries
  for (int i = 0; i < SETS; i++)
  {
      // initialize the round robin indices to 0
      index[i] = 0;
      for (int j = 0; j < WAYS; j++)
      {
          // initialize cache entries to be invalid
          cache.CacheSet[i].SetSlot[j].dirty = 0;
          cache.CacheSet[i].SetSlot[j].valid = 0;
          cache.CacheSet[i].SetSlot[j].tag = -1;
      }
  }
}

int CacheStats::access(uint32_t addr, ACCESS_TYPE type) {
  if(!CACHE_EN) { // no cache
    return (type == LOAD) ? READ_LATENCY : WRITE_LATENCY;
  }

  /* TODO: your code here */

  int Set = (addr / BLOCKSIZE) % SETS; // parse the set number from addr
  int tag = addr / (BLOCKSIZE * SETS); // parse the tag from addr

    // process loads
  if (type == LOAD)
    return loadAccess(Set, tag);

    // process stores
  if (type == STORE)
    return storeAccess(Set, tag);
}

// helper function for processing load accesses
int CacheStats::loadAccess(int Set, int tag)
{
    // increment number of loads, initialize number of stall cycles
    loads++;
    int cycles = LOOKUP_LATENCY;

    // check for a hit
    for (int i = 0; i < WAYS; i++)
    {
        if (cache.CacheSet[Set].SetSlot[i].valid == 1 &&
        cache.CacheSet[Set].SetSlot[i].tag == tag)
            return cycles;
    }

    // miss penalty & increment number of load misses
    cycles += READ_LATENCY;
    load_misses++;

    //write dirty entries to memory
    if (cache.CacheSet[Set].SetSlot[index[Set]].dirty == 1)
    {
        cycles+= WRITE_LATENCY;
        writebacks++;
    }

    // write new data to cache, increment round robin counter
    cache.CacheSet[Set].SetSlot[index[Set]].dirty = 0;
    cache.CacheSet[Set].SetSlot[index[Set]].tag = tag;
    cache.CacheSet[Set].SetSlot[index[Set]].valid = 1;
    index[Set] = (index[Set] + 1) % WAYS;

    return cycles;
}

// helper function for processing store accesses
int CacheStats::storeAccess(int Set, int tag)
{
    // increment number of stores, initialize number of stall cycles
    stores++;
    int cycles = LOOKUP_LATENCY;

    //check for a hit
    for (int i = 0; i < WAYS; i++)
    {
        if (cache.CacheSet[Set].SetSlot[i].valid &&
            cache.CacheSet[Set].SetSlot[i].tag == tag)
            {   // set dirty bit for a hit to be written back later
                cache.CacheSet[Set].SetSlot[i].dirty = 1;
                return cycles;
            }
    }

    // miss penalty & increment number of store misses
    store_misses++;
    cycles += READ_LATENCY;

    // write dirty entries to memory
    if (cache.CacheSet[Set].SetSlot[index[Set]].dirty == 1)
    {
        cycles+= WRITE_LATENCY;
        writebacks++;
    }

    // write new data to cache, increment round robin counter
    cache.CacheSet[Set].SetSlot[index[Set]].dirty = 1;
    cache.CacheSet[Set].SetSlot[index[Set]].tag = tag;
    cache.CacheSet[Set].SetSlot[index[Set]].valid = 1;
    index[Set] = (index[Set] + 1) % WAYS;

    return cycles;
}

void CacheStats::printFinalStats() {
  /* TODO: your code here (don't forget to drain the cache of writebacks) */

  for (int i = 0; i < SETS; i++)
  {
      for (int j = 0; j < WAYS; j++)
      {
          // drain writebacks from cache
          if (cache.CacheSet[i].SetSlot[j].dirty)
            writebacks++;
      }
  }

  int accesses = loads + stores;
  int misses = load_misses + store_misses;
  cout << "Accesses: " << accesses << endl;
  cout << "  Loads: " << loads << endl;
  cout << "  Stores: " << stores << endl;
  cout << "Misses: " << misses << endl;
  cout << "  Load misses: " << load_misses << endl;
  cout << "  Store misses: " << store_misses << endl;
  cout << "Writebacks: " << writebacks << endl;
  cout << "Hit Ratio: " << fixed << setprecision(1) << 100.0 * (accesses - misses) / accesses;
  cout << "%" << endl;
}
