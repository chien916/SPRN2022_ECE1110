/*
#include "System.h"

System::System() {
    time = 0;
    readInstr("parameters.txt");
}

bool System::readInstr(string filename) {
    ifstream infile;
    infile.open(filename);

    //get the number of caches:
    infile >> numCache;

    //Error checking:
    if(numCache < 1){
        cout << "ERROR: Number of Caches must be least 1!" << endl;
        return false;
    }

    //set the cache sizes:
    for(int i = 0; i < numCache; i++){
        int cacheSZ;
        infile >> cacheSZ;
        setCacheSize(i+1, cacheSZ);
    }

    //set the cache latencies:
    for(int i = 0; i < numCache; i++){
        int laten;
        infile >> laten;
        setLatencies(i+1, laten);
    }

    //set the block size:
    int blockSZ;
    infile >> blockSZ;
    setBlockSize(blockSZ);

    //set the associativity(ies):
    for(int i = 0; i < numCache; i++){
        int a;
        infile >> a;
        setAssociativity(i+1, a);
    }

    //set the policy:
    int po;
    infile >> po;
    setPolicy(po);

    //finished reading:
    return true;

}
void System::setPolicy(int p) {
    policy = p;
}

void System::setNumCache(int n) {
    numCache = n;
}

void System::setBlockSize(int b) const{
    for (int i = 0; i < numCache; i++){
        caches[i].setBlockSZ(b);
    }
}

void System::setCacheSize(int level, int size) {
    caches[level-1].setCacheSZ(size);
}

void System::setLatencies(int level, int la) {
    caches[level-1].setLatency(la);
}

void System::setAssociativity(int level, int asso) {
    caches[level-1].setAsso(asso);
}

int System::getBlockSize() const{
    return caches[0].getBlockSZ();
}

int System::getCacheSize(int level) const{
    return caches[level-1].getCacheSZ();
}

int System::getLatencies(int level) const{
    return caches[level-1].getLatency();
}

int System::getAssociativity(int level) const{
    return caches[level-1].getAsso();
}

int System::getTime() const {
    return time;
}

int System::getNumCache() const {
    return numCache;
}

int System::getPolicy() const {
    return policy;
}

vector<float> System::getHitRates() const{
    vector<float> rates;
    for(int i = 0; i < numCache; i++){
        rates.push_back(caches[i].getHitRate());
    }
    return rates;
}
vector<float> System::getMissRates() const{
    vector<float> rates;
    for(int i = 0; i < numCache; i++){
        rates.push_back(caches[i].getMissRate());
    }
    return rates;
}
vector<vector<int>> System::getImages() const{
    vector<vector<int>> images;
    for(int i = 0; i < numCache; i++){
        images.push_back(caches[i].getImage());
    }
    return images;
}

*/