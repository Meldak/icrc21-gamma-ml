#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/MethodCategory.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

Bool_t UseOffsetMethod = kTRUE;

void TMVAClassificationCategory()
{

std::cout << std::endl << "==> Start TMVAClassificationCategory" << std::endl;
//---------------------------------------------------------------
   // This loads the library
    TMVA::Tools::Instance();

    bool batchMode = true;
    
    // Create a new root output file.
    TString outfileName( "TMVA.root" );
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

    std::string factoryOptions( "!V:!Silent:Transformations=I;D;P;G,D" );
    if (batchMode) factoryOptions += ":!Color:!DrawProgressBar";


    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassificationCategory", outputFile, factoryOptions );

    // Create DataLoader
    TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

    //Varibles deentrada
    dataloader->AddVariable( "lgE", 'F' );
    dataloader->AddVariable( "Xmax", 'F' );

    // Variables expectador; no se usan en el entrenamiento
    dataloader->AddSpectator( "Nmax" );

    TFile *finput(0);
    TFile *pinput(0);
    TString fname = gSystem->GetDirName(__FILE__) + "/../datasets/";

    if (!gSystem->AccessPathName( fname )) {
        // first we try to find tmva_example.root in the local directory
        std::cout << "--- TMVAClassificationCategory: Accessing " << fname+ "Fe.root" << std::endl;
        finput = TFile::Open( fname+ "Fe.root" );
    }

    if (!gSystem->AccessPathName( fname )) {
        // first we try to find tmva_example.root in the local directory
        std::cout << "--- TMVAClassificationCategory: Accessing " << fname+ "Po.root" << std::endl;
        pinput = TFile::Open( fname+ "Po.root" );
    }

    if (!finput || !pinput) {
        std::cout << "ERROR: could not open data file into: " << fname << std::endl;
        exit(1);
    }

    TTree *signalTree = (TTree*)finput->Get("Shower");
    TTree *background = (TTree*)pinput->Get("Shower");
  // Global event weights per tree (see below for setting event-wise weights)
    Double_t signalWeight     = 1.0;
    Double_t backgroundWeight = 1.0;

    // You can add an arbitrary number of signal or background trees
    dataloader->AddSignalTree    ( signalTree,     signalWeight     );
    dataloader->AddBackgroundTree( background, backgroundWeight );

   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";

   // Tell the factory how to use the training and testing events
    dataloader->PrepareTrainingAndTestTree( mycuts, mycutb,
                                        "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

    // Book MVA methods

   // Fisher discriminant
    factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher", "!H:!V:Fisher" );

   // Likelihood
    factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "Likelihood",
                        "!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

   // Categorised classifier
    TMVA::MethodCategory* mcat = 0;

    // The variable sets
    TString theCat1Vars = "lgE:Xmax";
    TString theCat2Vars = (UseOffsetMethod ? "lgE:Xmax" : "lgE:Xmax");

    // Fisher with categories
    TMVA::MethodBase* fiCat = factory->BookMethod( dataloader, TMVA::Types::kCategory, "FisherCat","" );
    mcat = dynamic_cast<TMVA::MethodCategory*>(fiCat);
    mcat->AddMethod( "abs(Xmax)<=650", theCat1Vars, TMVA::Types::kFisher, "Fe","!H:!V:Fisher" );
    mcat->AddMethod( "abs(Xmax)>650",  theCat2Vars, TMVA::Types::kFisher, "PO","!H:!V:Fisher" );

       // Likelihood with categories
    TMVA::MethodBase* liCat = factory->BookMethod( dataloader, TMVA::Types::kCategory, "LikelihoodCat","" );
    mcat = dynamic_cast<TMVA::MethodCategory*>(liCat);
    mcat->AddMethod( "abs(Xmax)<=650",theCat1Vars, TMVA::Types::kLikelihood,
                    "Fe","!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );
    mcat->AddMethod( "abs(Xmax)>650", theCat2Vars, TMVA::Types::kLikelihood,
                    "Po","!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

     // Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
    factory->TrainAllMethods();

   // Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

   // Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
    outputFile->Close();

    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassificationCategory is done!" << std::endl;

   // Clean up
    delete factory;
    delete dataloader;

   // Launch the GUI for the root macros
    if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );
}


int main( int argc, char** argv )
{
    TMVAClassificationCategory();
    return 0;
}