#include <iostream>
#include <sstream>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TH1D.h>
#include <TMath.h>
#include <TString.h>


bool debug=false;

template <typename T>
string NumberToString ( T Number ) {
     ostringstream ss;
     ss << Number;
     return ss.str();
}

void extractHiggsMass_Analysis(int mass = 500, TString input = "~/cms/HWW2012/LatinoTrees2012/R53X_S1_V08_S2_V09_S3_V12/tree_skim_wwmin_moriondpreapp_MR/nominals/latino_1500_ggToH500toWWTo2LAndTau2Nu.root", TString out_name="output_ana.root", bool cuts_on = true){

  TH1::AddDirectory(kFALSE);
  TFile *f = TFile::Open(input,"READ");
  std::cout<<"file: " << f<<std::endl;
  TTree *tree = 0;
  tree= (TTree*)f->Get("latino");
  if(tree==NULL) {
       cout << "tree = NULL, exiting... " << endl;
       return;
  }
  std::cout << "opened tree:  " << input << std::endl;
  if(debug){
     std::cout<< "tree: " << tree << std::endl;
     tree->Print();
  }

  //Higgs Mass 
  float_t MHiggs = 0;
  tree->SetBranchAddress("MHiggs",&MHiggs);

  //Selection
  float trigger =0;
  float pfmet = 0;
  float mpmet =0;
  float mll = 0;
  int sameflav = 0;
  int zveto = 0;
  float njet = 0;
  float nvtx = 0;
  float itpu = 0;
  float nextra = 0;
  int dphiveto = 0;
  float dphilljetjet = 0;
  int bveto_mu = 0;
  int bveto_ip = 0;
  float nbjettche = 0;
  float ch1 = 0;
  float ch2 = 0;
  float dymva1 = 0;
  vector<float> pt(2,0);

  // Selection
  tree->SetBranchAddress("trigger",&trigger);
  tree->SetBranchAddress("pfmet",&pfmet);
  tree->SetBranchAddress("mpmet",&mpmet);
  tree->SetBranchAddress("sameflav",&sameflav);
  tree->SetBranchAddress("mll",&mll);
  tree->SetBranchAddress("zveto",&zveto);
  tree->SetBranchAddress("njet",&njet);
  tree->SetBranchAddress("nvtx",&nvtx);
  tree->SetBranchAddress("itpu",&itpu);
  tree->SetBranchAddress("nextra",&nextra);
  tree->SetBranchAddress("dphiveto",&dphiveto);
  tree->SetBranchAddress("dphilljetjet",&dphilljetjet);
  tree->SetBranchAddress("bveto_mu",&bveto_mu);
  tree->SetBranchAddress("bveto_ip",&bveto_ip);
  tree->SetBranchAddress("nbjettche",&nbjettche);
  tree->SetBranchAddress("ch1",&ch1);
  tree->SetBranchAddress("ch2",&ch2);
  tree->SetBranchAddress("dymva1",&dymva1);
  tree->SetBranchAddress("pt1",&pt.at(0));
  tree->SetBranchAddress("pt2",&pt.at(1));
 
   // higgses only 
//  int NBins = 400;
//  float hmin=mass-350;
//  float hmax=mass+350;
//  if(hmin<0) hmin=0;
  float hmin=0;
  float hmax=mass+500;
  int NBins = (int) (hmax-hmin)*1000./100;

  TH1F *massH = new TH1F("massH","",NBins,hmin,hmax);
  TH1F *massH_cut = new TH1F("massH_cut","",NBins,hmin,hmax);
  std::cout << "Print: " << tree->GetBranch("MHiggs")->GetClassName() << std::endl;
  int nev = int(tree->GetEntries());
  std::cout << "nev: " << nev << std::endl;
  for(int i = 0; i < nev ; ++i){
        tree->GetEntry(i);
        //std::cout << "i " << i << "  " ;
        //if(i>20) break;
        //std::cout << MHiggs << std::endl;
        massH->Fill(MHiggs) ;
        bool WWLvlCut = (trigger==1.&&ch1*ch2==-1.&&pt.at(0)>20&&pt.at(1)>10&&nextra==0&&pfmet>20.&&mll>12&&(zveto==1||!sameflav)&&mpmet>20.&&(!sameflav||((njet!=0||dymva1>0.88)&&(njet!=1||dymva1>0.84)&&(njet==0||njet==1||(pfmet>45.0))))&&(njet==0||njet==1||(dphilljetjet<TMath::Pi()/180.*165.||!sameflav))&&bveto_mu==1&&(bveto_ip==1&&nbjettche==0));

        if(cuts_on && WWLvlCut && njet <= 3) massH_cut->Fill(MHiggs);
  }

  TH1F *massHNorm = (TH1F*) massH->Clone("massHNorm");
  massHNorm->Scale(1./massHNorm->Integral());  
  //write higgs mass histo to a file
  TFile *myFile = new TFile(out_name,"RECREATE");
  massH->Write();
  massHNorm->Write();
  if(cuts_on) { 
      massH_cut->Write();
      TH1F *massH_cutNorm = (TH1F*) massH_cut->Clone("massH_cutNorm");
      massH_cutNorm->Scale(1./massH_cutNorm->Integral());
      massH_cutNorm->Write();
      delete massH_cutNorm;
  }
  delete massH;
  delete massHNorm;
  
  myFile->Close();
  std::cout << "Writen histogram to file: "<< out_name  << std::endl;
}
