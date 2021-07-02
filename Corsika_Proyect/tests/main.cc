#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <filesystem>
#include <dirent.h>



#include <crsRead/MCorsikaReader.h>

#include <crs/TSubBlock.h>
#include <crs/MRunHeader.h>
#include <crs/MEventHeader.h>
#include <crs/MEventEnd.h>
#include <crs/MParticleBlock.h>
#include <crs/MLongitudinalBlock.h>
#include <crs/MParticle.h>


#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TH2D.h"

#include "TMVA/MethodCategory.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"


using namespace std;
using namespace TMVA;
using namespace crsRead;
using namespace crs;
namespace fs = std::filesystem;


string DATASET;
string CLASSES;

string DATA_GAMMA;
string DATA_IRON;
string DATA_PROTON;

Double_t hs = 2000.0;
Int_t nBins = hs;


string usage();
int getOptions(int argc, char** argv, int& n);

/* void get_files(){
    int n=0, i=0;
    struct dirent *dir;
    DIR *d;

    cout << endl << "==> Reading sub directories" << endl;
    string path = "/path/to/directory";
    int iter = 0;
    for (const auto & entry : fs::directory_iterator(DATASET)){
            switch (iter)
            {
            case 0:
                DATA_GAMMA = entry.path();
                break;
            case 1:
                DATA_IRON = entry.path();
                break;
            case 2:
                DATA_PROTON = entry.path();
                break;
            default:
                break;
            }
            iter++;
    }
            cout << DATA_GAMMA << endl;
            cout << DATA_IRON << endl;
            cout << DATA_PROTON << endl;

    d = opendir(DATA_GAMMA.c_str());

    while((dir = readdir(d)) != NULL) {
        if ( !strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") )
        {

        } else {
            n++;
        }
    }
    rewinddir(d);

    char *filesList[n];

    //Put file names into the array
    while((dir = readdir(d)) != NULL) {
        if ( !strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") )
        {

        }
        else {
            filesList[i] = (char*) malloc (strlen(dir->d_name)+1);
            strncpy (filesList[i],dir->d_name, strlen(dir->d_name) );
            i++;
        }
    }
    rewinddir(d);

    for(i=0; i<=n; i++)
        printf("%s\n", filesList[i]);
    
}
 */




void Build_Factory(){
    cout << endl << "==> Start TMVAClassificationCategory" << endl;
    Tools::Instance();

    

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
    cout << "Data Classes: " << CLASSES << endl;

    cout << endl << "==> Reading sub directories" << endl;
    cout << DATASET << endl;
    MCorsikaReader cr(DATASET,0);
    MRunHeader run;

    

    
    TH2D* xy_tot = new TH2D("xy_tot","xy_tot",nBins,-hs,hs,nBins,-hs,hs);
    TH2D* xy_e = new TH2D("xy_e","xy_e",nBins,-hs,hs,nBins,-hs,hs);
    TH2D* xy_c = new TH2D("xy_c","xy_c",nBins,-hs,hs,nBins,-hs,hs);
    TH2D* xy_m = new TH2D("xy_m","xy_m",nBins,-hs,hs,nBins,-hs,hs);
    TH2D* xy_n = new TH2D("xy_n","xy_n",nBins,-hs,hs,nBins,-hs,hs);

    Int_t i=0;
    Double_t logE = 0.0;
    Double_t obsLev = 0.0;

      while( cr.GetRun( run ) ) // Loop over the runs. Usually one
  {
    crs::MEventHeader shower;
    while( cr.GetShower( shower ) )
    {
      i++;
      if(i != shIndex)
	continue;

      std::cout << "-----------------------------" << std::endl;
      std::cout << "plot of the shower nr: " << i << std::endl;
      logE = log10( shower.GetEnergy() );
      obsLev = shower.GetObservationHeight( shower.GetNObservationLevels()-1 )/100.0;
      std::cout << "shower obs level: " << obsLev << " m" << std::endl;
      
      crs::TSubBlock sub_block;
      while( cr.GetData( sub_block ) )
      {
	//std::cout << "Block type: " << data.GetBlockTypeName() << std::endl;

	crs::MParticleBlock data(sub_block);
	//double *pData = data.GetData();
	
	
	
	if( data.GetBlockType() == crs::TSubBlock::ePARTDATA )
	{
	  for( crs::MParticleBlock::ParticleListConstIterator p_it = data.FirstParticle();
	       p_it != data.LastParticle();
	       ++p_it )
	  {
	    crs::MParticle part(*p_it);

	  Int_t pID = part.GetParticleID();

	    if( pID < 4 )
	      xy_e->Fill(part.GetX()/100.0, part.GetY()/100.0);

	    if( pID == 5 || pID == 6 )
	      xy_m->Fill(part.GetX()/100.0, part.GetY()/100.0);
	    
	    if( part.IsCherenkov() )
	      xy_c->Fill(part.GetX()/100.0, part.GetY()/100.0);
	    
	    if( part.IsNucleus() || ( pID >6 && pID <200 ) && !part.IsMuonProductionInfo())
	      xy_n->Fill(part.GetX()/100.0, part.GetY()/100.0);

            if( !part.IsMuonProductionInfo() )
              xy_tot->Fill(part.GetX()/100.0, part.GetY()/100.0);
	  }
	}
	
	
      }
      break;
    }
  }

    Build_Factory();

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
            break;
        case 'c':
            CLASSES = optarg;
            return 2;
        case 'h':
            return -2;

        default:
            return -2;
        }
    }
    return optind;

}