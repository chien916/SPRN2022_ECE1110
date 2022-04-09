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

const unordered_map<string, char> bin2hex_map = {
        {"0000", '0'},
        {"0001", '1'},
        {"0010", '2'},
        {"0011", '3'},
        {"0100", '4'},
        {"0101", '5'},
        {"0110", '6'},
        {"0111", '7'},
        {"1000", '8'},
        {"1001", '9'},
        {"1010", 'A'},
        {"1011", 'B'},
        {"1100", 'C'},
        {"1101", 'D'},
        {"1110", 'E'},
        {"1111", 'F'}
};

void fetch(string &str, ifstream &ifs) {

}

int main(int argc, char *argv[]) {
    puts("-----ECE1110 Project1 Instruction Decompiler 1.0-----");
    if (argc != 1) throw invalid_argument("argv.size() error");
    string fileName = string("instr_asm.asm");
    ifstream ifs;
    ofstream ofs;
    ifs.open(fileName);
    string temp;
    ofs.open("instr_bin.bin");
    string binary;
    ofs << "v2.0 raw\n";
    while (ifs.peek() != EOF) {
        ifs >> temp;
        auto locator = binary_map.find(temp);
        if (locator != binary_map.end()) {//instruction found
            cout << left << setw(5) << temp << " ";
            binary = (*locator).second;
            if (binary.find("SSS") != string::npos) {
                ifs >> temp;
                if ((*temp.begin()) == 'r') {
                    temp = string(temp.begin() + 1, temp.end());//rs number
                    if (temp.find_first_not_of("0123456789") != string::npos)//check if only digits
                        throw invalid_argument("Register rs read error :" + temp);
                    int rs = stoi(temp);
                    cout << " R" << left << setw(2) << rs;//report
                    binary.at(6) = rs % 2 + '0';
                    binary.at(5) = rs / 2 % 2 + '0';
                    binary.at(4) = rs / 4 % 2 + '0';
                } else throw invalid_argument("Register rs read error :" + temp);
            } else cout << " -" << left << setw(2) << "-";//report
            if (binary.find("TTT") != string::npos) {
                ifs >> temp;
                if ((*temp.begin()) == 'r') {
                    temp = string(temp.begin() + 1, temp.end());//rt number
                    if (temp.find_first_not_of("0123456789") != string::npos)//check if only digits
                        throw invalid_argument("Register rt read error :" + temp);
                    int rt = stoi(temp);
                    cout << " R" << left << setw(4) << rt;//report
                    binary.at(9) = rt % 2 + '0';
                    binary.at(8) = rt / 2 % 2 + '0';
                    binary.at(7) = rt / 4 % 2 + '0';
                } else throw invalid_argument("Register rt read error :" + temp);
            } else if (binary.find("IIIIIIII") != string::npos) {
                ifs >> temp;
                bool neg = false;
                if (temp.find_first_not_of("-0123456789") != string::npos)//check if only digits
                    throw invalid_argument("Immediate read error :" + temp);
                if ((*temp.begin()) == '-') {
                    neg = true;
                    temp = string(temp.begin() + 1, temp.end());//get rid of negative sign
                }
                int imm = stoi(temp);
                if (abs(imm) > 128) throw out_of_range("Immediate range error :" + temp);
                if (neg) imm = abs(imm) - 1;
                cout << " " << left << setw(5) << imm;//report
                binary.at(15) = abs(neg - imm % 2) + '0';
                binary.at(14) = abs(neg - imm / 2 % 2) + '0';
                binary.at(13) = abs(neg - imm / 4 % 2) + '0';
                binary.at(12) = abs(neg - imm / 8 % 2) + '0';
                binary.at(11) = abs(neg - imm / 16 % 2) + '0';
                binary.at(10) = abs(neg - imm / 32 % 2) + '0';
                binary.at(9) = abs(neg - imm / 64 % 2) + '0';
                binary.at(8) = neg ? '1' : abs(neg - imm / 128 % 2) + '0';
            } else cout << " -" << left << setw(4) << "-";//report
            cout << " -> " << binary;
            //bin to hex
            string hex = "uuuu";
            hex.at(0) = bin2hex_map.at(string(binary.begin(), binary.begin() + 4));
            hex.at(1) = bin2hex_map.at(string(binary.begin() + 4, binary.begin() + 8));
            hex.at(2) = bin2hex_map.at(string(binary.begin() + 8, binary.begin() + 12));
            hex.at(3) = bin2hex_map.at(string(binary.begin() + 12, binary.begin() + 16));
            cout << " -> " << hex << endl;
            ofs << hex << ' ';
        }
    }
    ifs.close();
    ofs.close();
    puts("-----Press any keys to exit-----");
    cin.get();
    return 0;
}