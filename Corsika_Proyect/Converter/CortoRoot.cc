#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <unistd.h>
#include <stdio.h>


// #include <crsRead/MCorsikaReader.h>
#include <crs2r/MCorsikaReader.h>
#include <crs2r/TC2R.h>


// #include <crs/TSubBlock.h>
// #include <crs/MRunHeader.h>
// #include <crs/MEventHeader.h>
// #include <crs/MEventEnd.h>
// #include <crs/MParticleBlock.h>
// #include <crs/MLongitudinalBlock.h>
// #include <crs/MParticle.h>


// #include "TChain.h"
// #include "TFile.h"
// #include "TTree.h"
// #include "TString.h"
// #include "TObjString.h"
// #include "TSystem.h"
// #include "TROOT.h"
// #include "TH1D.h"
// #include "TH2D.h"




using namespace std;
using namespace crs2r;
namespace fs = std::filesystem;

string DATASET;

string usage();
int getOptions(int argc, char** argv, int& n);



void get_files(){
    cout << endl << "==> Reading sub directories" << endl;
    string path = "/path/to/directory";
    for (const auto & entry : fs::directory_iterator(DATASET))
            cout << entry.path() << endl;

}

int main(int argc, char **argv )
{
    int shIndex = 1;
    const int nOptions = getOptions(argc, argv, shIndex);

    if(nOptions < 0 || argc-nOptions < 1 )
    {
        cout << usage() << endl;
        return 1;
    }
    cout << "Data Directory: " << DATASET << endl;
    
    // get_files();

    

    MCorsikaReader Reader (DATASET);
    Reader.Read ();




    return 0;
}




string usage()
{
    char str[] = "usage: \n \t ./main -d <corsika file name> -c <class name> \n\n where: \n \t -d - Receives the absolute corsika file path. \n \t -c - Receives the class name that belong to those cosrika events.";

    return str;
}

int getOptions( int argc, char** argv, int &n )
{

    int c;
    while ( (c = getopt(argc, argv, "d:c:h")) != -1 )
    {
        switch(c)
        {
        case 'd':
            // n = atoi(optarg);
            // cout << endl << n << endl;
            DATASET = optarg;
            return 2;
        case 'h':
            return -2;

        default:
            return -2;
        }
    }
    return optind;

}