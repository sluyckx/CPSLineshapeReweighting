#include <iostream>
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


#include "BW.C"
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

void fit_some_BW(TString fileIn_path = "~/cms/HWW2012/2HDM/realCode/rootfiles/v0/analysis/" , 
                 TString fileIn = "massH_500GeV_analysis.root", TString histoIn_name = "massH", 
                 int fit_kind = 0, 
                 TString output_path = "./output/"
              ) {
   TH1::AddDirectory(kFALSE);
   gStyle->SetOptFit(111111);

   TFile *f = TFile::Open(fileIn_path+"/"+fileIn,"READ");
   if(debug) std::cout << "file: " << f <<std::endl;
   // open Histogram
   TH1F * massH = (TH1F*) f->FindObjectAny(histoIn_name);
   TH1F * HiggsMass = 0;
   HiggsMass=(TH1F*) massH->Clone();
   if(HiggsMass==0) {
      cout << "Histogram not found: " << histoIn_name << " in file " << fileIn /*<< "/" << treeIn*/ << endl; 
      return;
   }

   int   division = massH->GetNbinsX();
   float massMIN = massH->GetBinLowEdge(1);
   float massMAX = massH->GetBinLowEdge(division+1);
   //float BIN_SIZE = massH->GetBinWidth(1);

   /////////////////////
   //  Breit-Wigner  //
   ////////////////////
   // See BW.C
   // fit_kind parameter
   // 0 : CMS formula from https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookHowToFit
   // 1 : AN2012_317_v18 : ZZ 2l2nu  Breit-Wigner function
   ///////////////
   TF1 *func = 0;
   TString fit_string="";
   if(fit_kind==0){
     fit_string="mybw1";
     func = new TF1("mybw1",mybw1,massMIN, massMAX,3);
     func->SetParameter(0,100000);   func->SetParName(0,"const");
     func->SetParameter(1,70);     func->SetParName(1,"sigma");
     func->SetParameter(2,500);    func->SetParName(2,"mean");
   }
   else if(fit_kind==1){
     fit_string="mybw2";
     func = new TF1("mybw2",mybw2,massMIN, massMAX,3);
     func->SetParameter(0,100000);   func->SetParName(0,"const");
     func->SetParameter(1,70);     func->SetParName(1,"sigma");
     func->SetParameter(2,500);    func->SetParName(2,"mean");
   }
   else {
     cout << "Wrong Fit type !!!" << endl;
     return;
   }

   TCanvas *c1 = new TCanvas("c1","c1");

   HiggsMass->Fit(fit_string,"QR");
   TF1 *fit = HiggsMass->GetFunction(fit_string);

   // Plotting
   fit->SetLineColor(4);
   fit->SetLineWidth(3);

   HiggsMass->SetMarkerStyle(20);
   HiggsMass->SetMarkerSize(1.0);
   HiggsMass->SetMarkerColor(2);
   HiggsMass->SetLineWidth(2.0);
   HiggsMass->SetXTitle("Z Mass (in GeV/c^{2})"); 
   HiggsMass->GetXaxis()->SetTitleOffset(1.0);

   HiggsMass->Draw("PE0");

   
 
   // Fetch analysis or lhe from the fileIn
   TString fileIn_part = fileIn.ReplaceAll(".root",""); 

   char CommandToExec[300];
   sprintf(CommandToExec,"mkdir "+output_path);
   gSystem->Exec(CommandToExec);


   TString outname=fileIn+"_"+fit_string+"_Histo"+histoIn_name;
   c1->Print(output_path+"/"+outname+".png");

   // Write .dat file with the fit values
   FILE *fp = fopen(output_path+"fitparam_"+outname+".dat","w");
   cout << "Printing to file: " << "fitparam_"+outname+".dat" << " " << fp << endl;
   if (fp!=NULL) {
      fprintf(fp,"# ");
      for (int i=0;i<fit->GetNpar();i++) {
         fprintf(fp,"%d %s",i,fit->GetParName(i));
      }
      fprintf(fp," Chi^2" );
      fprintf(fp,"\n");
      for (int i=0;i<fit->GetNpar();i++) {
         Float_t value = fit->GetParameter(i);
         fprintf(fp,"%d %f",i,value);
      }
      fprintf(fp," %f",fit->GetChisquare());
   }
   fclose(fp);

}
