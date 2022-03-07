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

int main(int argc, char *argv[]) {
    if (argc != 2) throw invalid_argument("argv.size() error");
    string fileName = string(argv[1]);
    ifstream ifs;
    ofstream ofs;
    ifs.open(fileName);
        string instr;
        ofs.ope\
    while (ifs) {
        ifs >> instr;
        //cout << instr << ' ';
        auto locator = binary_map.find(instr);
        if (locator != binary_map.end()) {
            cout << (*locator).second;
        }
    }
    return 0;

}