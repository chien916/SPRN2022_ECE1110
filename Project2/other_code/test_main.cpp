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
bool access(instr in){
    return true;
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
            currInstr.expectedOutTime = currInstr.actualInTime + DELAY;
            avaliable = false;
        }
        while(!avaliable) {
            cout << "Currect cycle is " << time << endl;
            if (currInstr.expectedOutTime == time) {
                bool yeah = access(currInstr); //access the cache
                if(yeah) {
                    cout << "Finished instruction " << currInstr.ID << " at clock cycle " << time << endl;
                }else{
                    cout << "instruction " << currInstr.ID << "failed at clock cycle " << time << endl;
                }
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
