//#include "Cache.h"
//#include "Tester.h"
#include <iostream>
#include <fstream>
//#include <string>
#include <vector>
using namespace std;
int main() {
    /*
    //Tester tester;
    //tester.testAddress();
     */

    //Testing the read function in class System:
    string filename = "parameters.txt";
    ifstream infile;
    infile.open(filename);

    if(!infile.is_open()){
        cout << "Couldnt open the file!" << endl;
        return -1;
    }

    //get the number of caches:
    int numCache;
    infile >> numCache;

    //Error checking:
    if(numCache != 3){
        cout << "ERROR!" << endl;
        return -1;
    }


    //set the cache sizes:
    vector<int> cacheSizes;
    for(int i = 0; i < numCache; i++){
        int cacheSZ;
        infile >> cacheSZ;
        cacheSizes.push_back(cacheSZ);
    }
    //set the cache latencies:
    vector<int> latencies;
    for(int i = 0; i < numCache; i++){
        int laten;
        infile >> laten;
        latencies.push_back(laten);
    }

    //set the block size:
    int blockSZ;
    infile >> blockSZ;

    //set the associativity(ies):
    vector<int> assos;
    for(int i = 0; i < numCache; i++){
        int asso;
        infile >> asso;
        assos.push_back(asso);
    }

    //set the policy:
    int policy;
    infile >> policy;

    //final checking:
    cout << "Number of layers: " << numCache << endl;
    for(int i = 0; i < numCache; i++){
        cout << "Size of cache layer " << i << ": " << cacheSizes[i] << endl;
    }
    for(int i = 0; i < numCache; i++){
        cout << "Latency of cache layer " << i << ": " << latencies[i] << endl;
    }
    cout << "Block size: " << blockSZ << endl;
    for(int i = 0; i < numCache; i++){
        cout << "Set associativity of cache layer " << i << ": " << assos[i] << endl;
    }
    cout << "Policy: " << policy << endl;

    return 0;
}