/******************************
 * Ryan Askew
 * CS 3339 - Spring 2017
 * Project 5
 ******************************/
#include "Stats.h"

Stats::Stats() {
  cycles = PIPESTAGES - 1; // pipeline startup cost
  flushes = 0;
  bubbles = 0;
  stalls = 0;

  memops = 0;
  branches = 0;
  taken = 0;

  for(int i = IF1; i < PIPESTAGES; i++) {
    resultReg[i] = -1;
    // initialize hazards per stage to 0
    rawBreakdown[i] = 0;
  }
}

void Stats::clock() {
  cycles++;
  // run all pipeline flops
  for(int i = WB; i > IF1; i--) {
    resultReg[i] = resultReg[i-1];
    // cycle array containing data on when each instruction's data will be available
    ready[i] = ready[i - 1];
  }
  // inject no-op into IF1
  resultReg[IF1] = -1;
}

void Stats::registerSrc(int r, PIPESTAGE needed) {
        // check each pipeline stage for RAW hazards
        for (int i = EXE1; i < WB; i++)
        {
            // RAW hazard found
            if (r == resultReg[i])
            {
                // find out how many cycles until source data is needed
               int cyclesTilNeeded = needed - ID;
               // find out how many cycles until source data becomes available
               int cyclesTilAvailable = ready[i] - i;
                // increment the RAW counter for the corresponding pipe stage
               rawBreakdown[i]++;

               // if the instruction would need the data before it can
               // be forwarded, add bubbles until it can be forwarded
                for (int i = 0; i < (cyclesTilAvailable - cyclesTilNeeded); i++)
                    bubble();
                // break once the hazard has been dealt with to avoid duplicating
                // the same hazard
                break;
            }
        }

}

void Stats::registerDest(int r, PIPESTAGE available) {
    // keep track of the destination register so it can be determined if it
    // still needs to be written to before a future instruction can read that
    // register
    resultReg[ID] = r;
    // record the stage that the result will be ready (parallel to pipeline)
    ready[ID] = available;
}

void Stats::flush(int count) { // count == how many ops to flush
    for (int i = 0; i < count; i++)
    {
        // increment flushes and do a noop cycle
        flushes++;
        clock();
    }
}

void Stats::bubble() {
    // do a noop cycle and increment bubbles
    clock();
    bubbles++;
}

void Stats::stall(int waits) {
    cycles += waits;
    stalls += waits;
}
