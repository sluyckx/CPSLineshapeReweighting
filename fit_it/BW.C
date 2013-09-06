#include "TMath.h"

//Gaussian
/*Double_t mygauss(Double_t * x, Double_t * par)
{
  Double_t arg = 0;
  if (par[2]<0) par[2]=-par[2];  // par[2]: sigma
  if (par[2] != 0) arg = (x[0] - par[1])/par[2];  // par[1]: mean
 
  //return par[0]*BIN_SIZE*TMath::Exp(-0.5*arg*arg)/
  //   (TMath::Sqrt(2*TMath::Pi())*par[2]); 
   return par[0]*TMath::Exp(-0.5*arg*arg)/
     (TMath::Sqrt(2*TMath::Pi())*par[2]); // par[0] is constant
 
}
*/
//   https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookHowToFit 

// relativistic Breit-Wigner function
Double_t mybw1(Double_t* x, Double_t* par)
{
  Double_t arg1 = 14.0/22.0; // 2 over pi
  Double_t arg2 = par[1]*par[1]*par[2]*par[2]; //Gamma=par[1]  M=par[2]
  Double_t arg3 = ((x[0]*x[0]) - (par[2]*par[2]))*((x[0]*x[0]) - (par[2]*par[2]));
  Double_t arg4 = x[0]*x[0]*x[0]*x[0]*((par[1]*par[1])/(par[2]*par[2]));
  return par[0]*arg1*arg2/(arg3 + arg4);
  // toplayer has Gamma^2*Mean^2 --> not depending on X

}

// AN2012_317_v18.pdf : ZZ 2l2nu  relativistic Breit-Wigner function
Double_t mybw2(Double_t* x, Double_t* par)
{
  Double_t arg1 = 14.0/22.0; // 2 over pi
  Double_t arg2 = x[0]*par[1]/par[2]; //Gamma=par[1]  M=par[2]
  Double_t arg3 = ((x[0]*x[0]) - (par[2]*par[2]))*((x[0]*x[0]) - (par[2]*par[2]));
  Double_t arg4 = x[0]*x[0]*x[0]*x[0]*((par[1]*par[1])/(par[2]*par[2]));
  return par[0]*arg1*arg2/(arg3 + arg4);
  // toplayer has Gamma/Mean*x in top layer --> depending on X !!!!!!!!1
}
