#ifndef CODE_SYSTEM_H
#define CODE_SYSTEM_H
/*
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#include "Cache.h"

#define POLICY_RBRA 1
#define POLICY_RTNWA 2
#define LATENCY_MEM 100

class System{

private:
    // Total latency:
    int time;

    // System-level configurable parameters:
    int policy;
    int numCache;

    // The cache objects:
    vector <CACHE> caches;

    //setter functions for configurable parameters:
    void setNumCache(int);
    void setPolicy(int);
    void setBlockSize(int) const;
    void setCacheSize(int, int); //cache size is in K-bytes
    void setLatencies(int, int);
    void setAssociativity(int, int);

public:
    //Constructors:
    System();

    //Read from instruction file:
    bool readInstr(string);

    //getter functions for configurable parameters:
    int getNumCache() const;
    int getPolicy() const;
    int getBlockSize() const;
    int getCacheSize(int) const;
    int getLatencies(int) const;
    int getAssociativity(int) const;

    //getter functions for system outputs:
    int getTime() const;
    vector<float> getHitRates() const;
    vector<float> getMissRates() const;
    vector<vector<int>> getImages() const;
};
 */
#endif //CODE_SYSTEM_H
