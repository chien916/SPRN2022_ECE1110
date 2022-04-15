#include <iostream>
#include <queue>
using namespace std;

#define DELAY 2

//An instruction is a struct
struct instr{
    int ID;
    bool op;
    int addr;
    int expectedInTime;
    int actualInTime;
    int expectedOutTime;
};
//The actual process to access the cache, subject to change
int access(instr in){
    return DELAY;
}
//The run() function in class system:
int main() {
    std::cout << "This is a cache simulator!" << std::endl;

    //After reading the instruction file, we know the number of instructions
    int numInstr = 5;
    //Put in instructions in a queue;
    queue<instr> instrs;
    for (int i = 0; i < numInstr; i++){
        instr in;
        in.ID = i;
        in.addr = i;
        in.op = false;
        in.expectedInTime = i * 2;
        instrs.push(in);
    }

    //System not busy:
    bool avaliable = true;
    //Queue not finished:
    bool finished = false;
    //Init clock cycle:
    int time = 0;
    //Begin:
    while(!finished){
        cout << "Currect cycle is " << time << endl;
        //Get the next instruction:
        instr currInstr = instrs.front();
        if (currInstr.expectedInTime <= time) {
            currInstr.actualInTime = time;
            cout << "Start instruction " << currInstr.ID << " at clock cycle " << time << endl;
            //Start process and get the total delay:
            int expectedDelay = access(currInstr);
            currInstr.expectedOutTime = currInstr.actualInTime + expectedDelay - 1;
            avaliable = false;
        }
        while(!avaliable) {
            cout << "Currect cycle is " << time << endl;
            if (currInstr.expectedOutTime == time) {
                cout << "Finished instruction " << currInstr.ID << " at clock cycle " << time << endl;
                avaliable = true;
                instrs.pop();
            }else {
                //Increment cycle when access not finished
                time++;
            }
        }
        //Increment cycle when access finished
        time++;
        //Check if all the instructions are finished
        if(instrs.empty()){
            finished = true;
        }
    }

    return 0;
}
