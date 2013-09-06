#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TString.h>
#include <iostream>

bool debug = false;

//   root -q -l  extractHistoMass_POWHEG_LHE.C+
void extractHiggsMass_POWHEG_LHE(TString inputfile="lhe2root_500.root", float masspoint=500, TString outputfile="massH_500GeV_lhe.root"){

  // Open lhe2root inputfile
  TFile *f = TFile::Open(inputfile,"READ");
  std::cout<<"file: " << f<<std::endl;
  TTree *tree = 0;
  tree= (TTree*)f->Get("Physics");
  if(tree==NULL) {
       cout << "tree = NULL, exiting... " << endl;
       return;
   }
   std::cout<< "tree: " << tree << std::endl;
   if(debug) tree->Print();

  // branches
  vector<double> * Mass = 0;
  vector<int> * PID = 0;
  tree->SetBranchAddress("M",&Mass);
  tree->SetBranchAddress("PID",&PID);

  // Output Histogram
  float hmin=0;
  float hmax=masspoint+500;
  int NBins = (int) (hmax-hmin)*10./100;
//  int NBins = 400;
//  float hmin=masspoint-350;
//  float hmax=masspoint+350;
  if (hmin <0 ) hmin = 0;
  TH1F *massH = new TH1F("massH","",NBins,hmin,hmax);

  // filling through looping
  int nev = int(tree->GetEntries());
  std::cout << "nev: " << nev << std::endl;
  for(int i = 0; i < nev ; ++i){
        tree->GetEntry(i);
        // std::cout << "i " << i << "  " ;
        //if(i>20) break;
        for(unsigned int j = 0; j < Mass->size(); ++j) {
           //if(PID->at(j)==25) std::cout << Mass->at(j) << " " ;
           if(PID->at(j)==25) {
             massH->Fill(Mass->at(j)) ;
             break;
           }
        }
        //std::cout << " " << std::endl ;
        //Mass->clear();
        //PID->clear();
  }
  std::cout << "passed loop " << std::endl;
 
  TH1F *massHNorm = (TH1F*) massH->Clone("massHNorm");
  massHNorm->Scale(1./massHNorm->Integral());

  // write massH to outputfile
  TFile *fout = TFile::Open(outputfile,"RECREATE");
  massH->Write(); 
  massHNorm->Write();
  fout->Close();
  f->Close();
  std::cout << "Script finished, outputfile: " << outputfile << std::endl;
}
