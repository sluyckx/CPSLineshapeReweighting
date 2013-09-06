#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2F.h>
#include <TH1D.h>
#include <TMath.h>
#include <TStyle.h>
#include <TString.h>
#include <TSystem.h>
#include <TLatex.h>

using namespace std;

bool debug=true;

//____________________________________________________________________________________________________________________
template <typename T>
  string NumberToString ( T Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }

//____________________________________________________________________________________________________________________

void calculate_Weight(TString fileIn_path_ana = "~/cms/HWW2012/2HDM/realCode/rootfiles/v0/analysis/" , 
                 TString fileIn_ana = "massH_500GeV_analysis.root", 
                 TString fileIn_path_lhe = "~/cms/HWW2012/2HDM/realCode/rootfiles/v0/POWHEG_lhe/" ,
                 TString fileIn_lhe = "massH_500GeV_lhe.root",
                 TString histoIn_name_ana = "massHNorm", 
                 TString histoIn_name_lhe = "massHNorm", 
                 TString outputfile = "out.root",
                 TString output_path = "."
              ) {
   TH1::AddDirectory(kFALSE);
   gStyle->SetOptFit(111111);

   // File ana
   TFile *f_ana = TFile::Open(fileIn_path_ana+"/"+fileIn_ana,"READ");
   if(debug) std::cout << "Analysis file: " << f_ana <<std::endl;
   // open Histogram
   TH1F * massH_ana = (TH1F*) f_ana->FindObjectAny(histoIn_name_ana);
   TH1F * HiggsMass_ana = 0;
   HiggsMass_ana=(TH1F*) massH_ana->Clone("massH_ana");
   if(HiggsMass_ana==0) {
      cout << "Analysis histogram not found: " << histoIn_name_ana << " in file " << fileIn_ana /*<< "/" << treeIn*/ << endl; 
      return;
   }


   // File lhe
   TFile *f_lhe = TFile::Open(fileIn_path_lhe+"/"+fileIn_lhe,"READ");
   if(debug) std::cout << "Lhe file: " << f_lhe <<std::endl;
   // open Histogram
   TH1F * massH_lhe = (TH1F*) f_lhe->FindObjectAny(histoIn_name_lhe);
   TH1F * HiggsMass_lhe = 0;
   HiggsMass_lhe=(TH1F*) massH_lhe->Clone("massH_lhe");
   if(HiggsMass_lhe==0) {
      cout << "Lhe histogram not found: " << histoIn_name_lhe << " in file " << fileIn_lhe /*<< "/" << treeIn*/ << endl;
      return;
   }

/* // TestPlotting
   massH_ana->Draw();
   gPad->WaitPrimitive();
   massH_lhe->Draw();
   gPad->WaitPrimitive();
*/
 
   cout << "NBins ana : " << massH_ana->GetNbinsX() << endl;
   cout << "NBins lhe : " << massH_lhe->GetNbinsX() << endl;

   // -------------------------------------------
   // Not Smoothed
   // -------------------------------------------
   // Divide to get the weights   (weight = lhe / ana)
   TH1F * weight_notsmoothed = (TH1F*) massH_lhe->Clone("Weight_notsmoothed");
   weight_notsmoothed->Sumw2();
   weight_notsmoothed->Divide(massH_ana);

   // Put all the bins that are 0 to 1. and find the maximum weight
   float theMax_notsmoothed = 1;
   float theMin_notsmoothed = -1;
   for (int i=0; i <= weight_notsmoothed->GetNbinsX(); i++) {
      if( weight_notsmoothed->GetBinContent(i) == 0 )
          weight_notsmoothed->SetBinContent(i,1.);

      // find max and min dont do for over and underflow bin
      if( i == 0 || i == weight_notsmoothed->GetNbinsX() ) continue;
      if (weight_notsmoothed->GetBinContent(i) > theMax_notsmoothed)
         theMax_notsmoothed = weight_notsmoothed->GetBinContent(i);
      if (weight_notsmoothed->GetBinContent(i) < theMin_notsmoothed)
         theMin_notsmoothed = weight_notsmoothed->GetBinContent(i);
   }
   weight_notsmoothed->SetMinimum(0.90 * theMin_notsmoothed);
   weight_notsmoothed->SetMaximum(1.10 * theMax_notsmoothed);


   // -------------------------------------------
   // Smoothed
   // -------------------------------------------

   // can only do technically  if enough bins are not equal to 0
   // but we can use this even if there are only a few bins the whole thing is normalised anyway and 
   // if everything becomes 0 we divide anyway and say the weight is 1.
   TH1F* massH_ana_smoothed = (TH1F*) massH_ana->Clone("massH_ana_smoothed");
   TH1F* massH_lhe_smoothed = (TH1F*) massH_lhe->Clone("massH_lhe_smoothed");
   massH_ana_smoothed->Smooth();
   massH_lhe_smoothed->Smooth();


   // Divide to get the weights   (weight = lhe / ana)
   TH1F * weight_smoothed = (TH1F*) massH_lhe_smoothed->Clone("Weight_smoothed");
   weight_smoothed->Sumw2();
   weight_smoothed->Divide(massH_ana_smoothed);
   // Put all the bins that are 0 to 1. and find the maximum weight
   float theMax = 1;
   float theMin = -1;
   for (int i=0; i <= weight_smoothed->GetNbinsX(); i++) {
      if( weight_smoothed->GetBinContent(i) == 0 ) 
          weight_smoothed->SetBinContent(i,1.);

      // find max and min dont do for over and underflow bin
      if( i == 0 || i == weight_smoothed->GetNbinsX() ) continue;
      if (weight_smoothed->GetBinContent(i) > theMax)
         theMax = weight_smoothed->GetBinContent(i);
      if (weight_smoothed->GetBinContent(i) < theMin)
         theMin = weight_smoothed->GetBinContent(i);
   }
   weight_smoothed->SetMinimum(0.90 * theMin);
   weight_smoothed->SetMaximum(1.10 * theMax);

   // Printing Original Bin Entries
   cout << " Printing bins SMOOTHED : " << endl << " Hist ana -> Hist ana smoothed  and Hist lhe -> Hist lhe smoothed  --> Weight "<< endl;
   int setw_int = 11;
   for (int i=0; i< weight_smoothed->GetNbinsX(); i++) {
     cout << "Bin " << setw(3) << i << " : " << setw(setw_int) << massH_ana->GetBinContent(i) << " -> " << setw(setw_int) << massH_ana_smoothed->GetBinContent(i) << "  and  " << setw(setw_int) << massH_lhe->GetBinContent(i) << " -> " << setw(setw_int) << massH_lhe_smoothed->GetBinContent(i) << "  ----> " << setw(setw_int) << weight_smoothed->GetBinContent(i) << endl;
   }

   // -------------------------------------------
   // Smoothed
   // -------------------------------------------

   /*TH1F * weight_smoothspline = (TH1F*) massH_lhe_smoothspline->Clone("Weight_smoothspline");
   weight_smoothspline->Sumw2();
   weight_smoothspline->Divide(massH_ana_smoothspline);
*/


   
   // -----------------------------------------------
   // Writing to file
   // -----------------------------------------------

   TString outname=outputfile;
   TFile *f_out = TFile::Open(output_path+"/"+outname,"RECREATE");
   massH_ana->Write(histoIn_name_ana+"_ana");
   massH_lhe->Write(histoIn_name_lhe+"_lhe");

   //smoothed
   massH_ana_smoothed->Write(histoIn_name_ana+"_ana_smoothed");
   massH_lhe_smoothed->Write(histoIn_name_lhe+"_lhe_smoothed");   
   weight_smoothed->Write();
   // not smoothed
   weight_notsmoothed->Write();
   // smooth + spline fit
   //weight_smoothspline->Write();

   f_out->Close();
}
