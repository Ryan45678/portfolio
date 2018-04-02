/******************************
 * Ryan Askew
 * CS 3339.252 - Spring 2017
 * Project 5
 ******************************/
#include "CPU.h"

const string CPU::regNames[] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3",
                                "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
                                "$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
                                "$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

CPU::CPU(uint32_t pc, Memory &iMem, Memory &dMem) : pc(pc), iMem(iMem), dMem(dMem) {
  for(int i = 0; i < NREGS; i++) {
    regFile[i] = 0;
  }
  hi = 0;
  lo = 0;
  regFile[28] = 0x10008000; // gp
  regFile[29] = 0x10000000 + dMem.getSize(); // sp

  instructions = 0;
  stop = false;
}

void CPU::run() {
  while(!stop) {
    instructions++;

    fetch();
    decode();
    execute();
    mem();
    writeback();

    D(printRegFile());
  }
}

void CPU::fetch() {
  instr = iMem.loadWord(pc);
  pc = pc + 4;
  s.clock();
}

void CPU::decode() {
  uint32_t opcode;      // opcode field
  uint32_t rs, rt, rd;  // register specifiers
  uint32_t shamt;       // shift amount (R-type)
  uint32_t funct;       // funct field (R-type)
  uint32_t uimm;        // unsigned version of immediate (I-type)
  int32_t simm;         // signed version of immediate (I-type)
  uint32_t addr;        // jump address offset field (J-type)

  opcode = instr >> 26; // shift instr right until only first 6 bits are seen
  rs = (instr & 0x03e00000) >> 21; // mask out and shift all but the next 5 bits
  rt = (instr & 0x001f0000) >> 16; // mask out and shift all but the next 5 bits
  rd = (instr & 0x0000f800) >> 11; // mask out and shift all but the next 5 bits
  shamt = (instr & 0x000007c0) >> 6; // mask out and shift all but the next 5 bits
  funct = instr & 0x0000003f; // mask all  but the last 6 bits
  uimm = instr & 0x0000ffff; // mask only the first 6 bits (opcode)


  // check if the first bit of the immediate field is a sign bit
  if (uimm & 0x8000)
  {
      simm = ~uimm + 0x1; // do 2's compliment on the immediate field, since it
      // is a negative value
      simm = simm & 0xffff; // mask out any overflow
      simm = -simm; // negate the value since we only found its positive
      // representation
  }
  // otherwise, the value is positive
  else
    simm = uimm;

  addr = instr & 0x03ffffff; // mask out the opcode

  // Hint: you probably want to give all the control signals some "safe"
  // default value here, and then override their values as necessary in each
  // case statement below!

  writeDest = false;
  aluOp = ADD;
  aluSrc1 = pc;
  aluSrc2 = regFile[REG_ZERO];
  destReg = regFile[REG_RA];
  opIsLoad = false;
  opIsMultDiv = false;
  opIsStore = false;
  storeData = regFile[REG_ZERO];

  D(cout << "  " << hex << setw(8) << pc - 4 << ": ");
  switch(opcode) {
    case 0x00:
      switch(funct) {
        case 0x00: D(cout << "sll " << regNames[rd] << ", " << regNames[rs] << ", " << dec << shamt);
                   aluOp = SHF_L; // shift left op
                   writeDest = true; // writes result to rd
                   destReg = rd;
                   aluSrc1 = regFile[rs]; // register and shamt sources
                   aluSrc2 = shamt;

                   s.registerSrc(rs, EXE1); // check for RAW
                   s.registerDest(rd); // record destination register
                   break;

        case 0x03: D(cout << "sra " << regNames[rd] << ", " << regNames[rs] << ", " << dec << shamt);
                   aluOp = SHF_R; // shift right op
                   writeDest = true; // writes result to rd
                   destReg = rd;
                   aluSrc1 = regFile[rs]; // register and shamt sources
                   aluSrc2 = shamt;

                   s.registerSrc(rs, EXE1); // check for RAW
                   s.registerDest(rd); // record destination reg
                   break;

        case 0x08: D(cout << "jr " << regNames[rs]);
                   pc = regFile[rs]; // set the pc according to rs

                   s.registerSrc(rs, ID);// check for RAW
                   s.flush(2); // flush, since a jump was executed
                   break;

        case 0x10: D(cout << "mfhi " << regNames[rd]);
                   aluOp = ADD; // add op
                   writeDest = true; // writes to rd
                   destReg = rd;
                   aluSrc1 = hi; // take from hi
                   aluSrc2 = regFile[REG_ZERO]; // add 0 to hi to effectively move

                   s.registerSrc(REG_HILO, EXE1); // check for RAW
                   s.registerDest(rd); // record dest reg
                   break;

        case 0x12: D(cout << "mflo " << regNames[rd]);
                   aluOp = ADD; // add op
                   writeDest = true; // writes to rd
                   destReg = rd;
                   aluSrc1 = lo; // take from lo
                   aluSrc2 = regFile[REG_ZERO]; // add 0 to lo to effectively move

                   s.registerSrc(REG_HILO, EXE1); // check for RAW
                   s.registerDest(rd); // record dest reg
                   break;

        case 0x18: D(cout << "mult " << regNames[rs] << ", " << regNames[rt]);
                   opIsMultDiv = true; // set mult/div flag
                   writeDest = false; // no write - stored in hi/lo
                   aluOp = MUL; // multiply op
                   aluSrc1 = regFile[rs]; // sources rs and rt
                   aluSrc2 = regFile[rt];

                   s.registerSrc(rs, EXE1); // check for RAW
                   s.registerSrc(rt, EXE1); // check for RAW
                   s.registerDest(REG_HILO, WB); // record dest reg
                   break;

        case 0x1a: D(cout << "div " << regNames[rs] << ", " << regNames[rt]);
                   opIsMultDiv = true; // set mult/div flag
                   aluOp = DIV; // division op
                   writeDest = false; // no write - stored in hi/lo
                   aluSrc1 = regFile[rs]; // sources rs and rt
                   aluSrc2 = regFile[rt];

                   s.registerSrc(rs, EXE1); // check for RAW
                   s.registerSrc(rt, EXE1); // check for RAW
                   s.registerDest(REG_HILO, WB); // record dest reg
                   break;

        case 0x21: D(cout << "addu " << regNames[rd] << ", " << regNames[rs] << ", " << regNames[rt]);
                   aluOp = ADD; // add op
                   writeDest = true; // write to rd
                   destReg = rd;
                   aluSrc1 = regFile[rs]; // sources rs and rt
                   aluSrc2 = regFile[rt];

                   s.registerSrc(rs, EXE1); // check for RAW
                   s.registerSrc(rt, EXE1); // check for RAW
                   s.registerDest(rd); // record dest reg
                   break;

        case 0x23: D(cout << "subu " << regNames[rd] << ", " << regNames[rs] << ", " << regNames[rt]);
                   aluOp = ADD; // add op
                   writeDest = true; // write to rd
                   destReg = rd;
                   aluSrc1 = regFile[rs]; // rs source
                   aluSrc2 = (-1) * regFile[rt]; // negated rt source to subtract

                   s.registerSrc(rs, EXE1); // check for RAW
                   s.registerSrc(rt, EXE1); // check for RAW
                   s.registerDest(rd); // record dest reg
                   break;

        case 0x2a: D(cout << "slt " << regNames[rd] << ", " << regNames[rs] << ", " << regNames[rt]);
                   aluOp = CMP_LT; // set less than op
                   writeDest = true; // write to rd
                   destReg = rd;
                   aluSrc1 = regFile[rs]; // sources rs and rt
                   aluSrc2 = regFile[rt];

                   s.registerSrc(rs, EXE1); // check for RAW
                   s.registerSrc(rt, EXE1); // check for RAW
                   s.registerDest(rd); // record dest reg
                   break;

        default: cerr << "unimplemented instruction: pc = 0x" << hex << pc - 4 << endl;
      }
      break;

    case 0x02: D(cout << "j " << hex << ((pc & 0xf0000000) | addr << 2)); // P1: pc + 4
               pc = (pc & 0xf0000000) | addr << 2; // reassign pc to the jump address
               s.flush(2); // flush because instruction is a jump
               break;

    case 0x03: D(cout << "jal " << hex << ((pc & 0xf0000000) | addr << 2)); // P1: pc + 4
               writeDest = true; destReg = REG_RA; // writes PC+4 to $ra
               aluOp = ADD; // ALU should pass pc thru unchanged
               aluSrc1 = pc;
               aluSrc2 = regFile[REG_ZERO]; // always reads zero
               pc = (pc & 0xf0000000) | addr << 2; // jump to jump address

               s.registerDest(REG_RA); // record dest reg
               s.flush(2); // flush because instruction is a jump
               break;

    case 0x04: D(cout << "beq " << regNames[rs] << ", " << regNames[rt] << ", " << pc + (simm << 2));
               s.countBranch(); // increment branch counter
               s.registerSrc(rs, ID); // check for RAW
               s.registerSrc(rt, ID); // check for RAW
               // branch condition
               if (regFile[rs] == regFile[rt])
                {
                    pc = pc + (simm << 2);
                    s.flush(2); // flush if branch taken
                    s.countTaken(); // increment taken counter
                }
               break;

    case 0x05: D(cout << "bne " << regNames[rs] << ", " << regNames[rt] << ", " << pc + (simm << 2));
               s.countBranch(); // increment branch counter
               s.registerSrc(rs, ID); // check for RAW
               s.registerSrc(rt, ID); // check for RAW
               // branch condition
               if (regFile[rs] != regFile[rt])
                {
                    pc = pc + (simm << 2);
                    s.flush(2); // flush if branch taken
                    s.countTaken(); // increment taken counter
                }
               break;

    case 0x09: D(cout << "addiu " << regNames[rt] << ", " << regNames[rs] << ", " << dec << simm);
               aluOp = ADD; // add op
               writeDest = true; // write to rt
               destReg = rt;
               aluSrc1 = regFile[rs]; // register source
               aluSrc2 = simm; // signed immediate source

               s.registerSrc(rs, EXE1); // check for RAW
               s.registerDest(rt); // record dest reg
               break;

    case 0x0c: D(cout << "andi " << regNames[rt] << ", " << regNames[rs] << ", " << dec << uimm);
               aluOp = AND; // and op
               writeDest = true; // write to rt
               destReg = rt;
               aluSrc1 = regFile[rs]; // register source
               aluSrc2 = uimm; // unsigned immediate source

               s.registerSrc(rs, EXE1); // check for RAW
               s.registerDest(rt); // record dest reg
               break;

    case 0x0f: D(cout << "lui " << regNames[rt] << ", " << dec << simm);
               aluOp = ADD; // add op
               writeDest = true; // write to rt
               destReg = rt;
               aluSrc1 = (simm << 16) & 0xffff0000; // load upper 16 bits of simm
               aluSrc2 = regFile[REG_ZERO]; // add upper 16 bits to 0

               s.registerDest(rt); // record dest reg
               break;

    case 0x1a: D(cout << "trap " << hex << addr);
               switch(addr & 0xf) {
                 case 0x0: cout << endl;
                           break;
                 case 0x1: cout << " " << (signed)regFile[rs];
                           s.registerSrc(rs, EXE1); // check for RAW
                           break;
                 case 0x5: cout << endl << "? "; cin >> regFile[rt];
                           s.registerDest(rt); // record dest reg
                           break;
                 case 0xa: stop = true; break;
                 default: cerr << "unimplemented trap: pc = 0x" << hex << pc - 4 << endl;
                          stop = true;
               }
               break;

    case 0x23: D(cout << "lw " << regNames[rt] << ", " << dec << simm << "(" << regNames[rs] << ")");
               opIsLoad = true; // load op flag
               aluOp = ADD; // add op
               writeDest = true; // write to rt
               destReg = rt;
               aluSrc1 = regFile[rs]; // memory address
               aluSrc2 = simm; // signed memory offset

               s.countMemOp(); // increment memop counter
               s.registerSrc(rs, EXE1); // check for RAW
               s.registerDest(rt, WB); // record dest reg

               break;

    case 0x2b: D(cout << "sw " << regNames[rt] << ", " << dec << simm << "(" << regNames[rs] << ")");
               opIsStore = true; // store op flag
               aluOp = ADD; // add op
               writeDest = false; // don't write - we're doing a store
               aluSrc1 = regFile[rs]; // memory address
               aluSrc2 = simm; // signed memory offset
               storeData = regFile[rt]; // value to store in memory

               s.countMemOp(); // increment memop counter
               s.registerSrc(rs, EXE1); // check for RAW
               s.registerSrc(rt, MEM1); // record dest reg

               break;

    default: cerr << "unimplemented instruction: pc = 0x" << hex << pc - 4 << endl;
  }
  D(cout << endl);
}

void CPU::execute() {
  aluOut = alu.op(aluOp, aluSrc1, aluSrc2);
}

void CPU::mem() {
  if(opIsLoad)
  {
      s.stall(c.access(aluOut, LOAD)); // stall for cache load misses
      writeData = dMem.loadWord(aluOut);
  }
  else
    writeData = aluOut;

  if(opIsStore)
  {
      s.stall(c.access(aluOut, STORE)); // stall for cache store misses
      dMem.storeWord(storeData, aluOut);
  }
}

void CPU::writeback() {
  if(writeDest && destReg > 0) // never write to reg 0
    regFile[destReg] = writeData;

  if(opIsMultDiv) {
    hi = alu.getUpper();
    lo = alu.getLower();
  }
}

void CPU::printRegFile() {
  cout << hex;
  for(int i = 0; i < NREGS; i++) {
    cout << "    " << regNames[i];
    if(i > 0) cout << "  ";
    cout << ": " << setfill('0') << setw(8) << regFile[i];
    if( i == (NREGS - 1) || (i + 1) % 4 == 0 )
      cout << endl;
  }
  cout << "    hi   : " << setfill('0') << setw(8) << hi;
  cout << "    lo   : " << setfill('0') << setw(8) << lo;
  cout << dec << endl;
}

void CPU::printFinalStats() {

        int raws = 0;
        for (int i = EXE1; i < WB; i++)
            raws += s.getRawBreakdown(i);
       cout
            // finishing stats for current running file
            << "\nProgram finished at pc = 0x" << hex << pc << "  ("
            << dec << instructions << " instructions executed)\n\n"

            // number of cycles taken to execute instructions
            << "Cycles: " << s.getCycles()

            // CPI based on cycles and number of instructions
            << fixed << setprecision(2)
            << "\nCPI: " << static_cast<double>(s.getCycles()) / instructions

            // number of bubbles injected
            << "\n\nBubbles: " << s.getBubbles()

            // number of flushes used
            << "\nFlushes: " << s.getFlushes()

            // number of stall cycles due to cache misses
            << "\nStalls: " << s.getStalls()

            << endl << endl;

            c.printFinalStats();
}
