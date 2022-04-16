#include "Include.h"
#include "Core.h"

int main(int argc, char** argv) {
	if(argc!=3) throw std::invalid_argument("ERR Argc Wrong Number");
	std::pair<std::string,std::string> filenames{{argv[1]},{argv[2]}};
	Core running_core{filenames};
	running_core.loadInstructions();



	return 0;
    /*
    //Tester tester;
    //tester.testAddress();
     */

    //Testing the read function in class System:

}