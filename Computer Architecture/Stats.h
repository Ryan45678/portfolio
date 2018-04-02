#ifndef __STATS_H
#define __STATS_H
#include <iostream>
#include "Debug.h"
using namespace std;

enum PIPESTAGE { IF1 = 0, IF2 = 1, ID = 2, EXE1 = 3, EXE2 = 4, MEM1 = 5,
                 MEM2 = 6, WB = 7, PIPESTAGES = 8 };

class Stats {
  private:
    long long cycles;
    int flushes;
    int bubbles;
    int stalls;

    int memops;
    int branches;
    int taken;

    // breaks up RAW hazards by pipeline stage (only EXE1 - WB will be used,
    // all 8 are included in order to line up with the stage enums)
    int rawBreakdown[PIPESTAGES];

    int resultReg[PIPESTAGES];
    // keeps track of what stage each instruction's results will be ready in
    // (parallel to the resultReg pipeline)
    int ready[PIPESTAGES];

  public:
    Stats();

    void clock();

    void flush(int count);
    void stall(int);

    // takes a register and a pipeline stage that register data will be needed
    void registerSrc(int r, PIPESTAGE needed);
    // takes a register and a pipeline stage that result will be ready (default
    // is MEM1)
    void registerDest(int r, PIPESTAGE availabe = MEM1);

    void countMemOp() { memops++; }
    void countBranch() { branches++; }
    void countTaken() { taken++; }

    // getters
    long long getCycles() { return cycles; }
    int getFlushes() { return flushes; }
    int getBubbles() { return bubbles; }
    int getMemOps() { return memops; }
    int getBranches() { return branches; }
    int getTaken() { return taken; }
    int getRawBreakdown(int i) {return rawBreakdown[i];}
    int getStalls() {return stalls;}

  private:
    void bubble();
};

#endif
