#include <bits/stdc++.h>

using namespace std;

const unordered_map<string, string> binary_map = {
        {"add",   "0000SSSTTT000000"},
        {"addi",  "0001SSS0IIIIIIII"},
        {"addui", "0001SSS1IIIIIIII"},
        {"sub",   "0010SSSTTT000000"},
        {"lw",    "0110SSSTTT000000"},
        {"sw",    "0110SSSTTT000001"},
        {"halt",  "0111000000000000"},
        {"bz",    "1000SSS0IIIIIIII"},
        {"bp",    "1010SSS0IIIIIIII"},
        {"put",   "1111SSS000000000"}
};

void fetch(string &str, ifstream &ifs) {
    
}

int main(int argc, char *argv[]) {
    if (argc != 2) throw invalid_argument("argv.size() error");
    string fileName = string(argv[1]);
    ifstream ifs;
    ofstream ofs;
    ifs.open(fileName);
    string temp;
    ofs.open("instr_bin.bin");
    string binary;
    while (ifs) {
        ifs >> temp;
        auto locator = binary_map.find(temp);
        if (locator != binary_map.end()) {//instruction found
            binary = (*locator).second;
            if (binary.find("SSS") != string::npos) {
                ifs >> temp;
            }
            if (binary.find("TTT") != string::npos) {

            }
            if (binary.find("IIIIIIII") != string::npos) {

            }
        } else throw invalid_argument("Instruction read error :" + instr);
    }
    return 0;

}