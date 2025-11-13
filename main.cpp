#include <TF1.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TMath.h>
#include <TROOT.h>
#include <TString.h>
#include <TStyle.h>

#include "myClass.hpp"

void setStyle() {
  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(1111);
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
}

double myFunction(double *x, double *par) {
  double xx = x[0];
  double alpha =
      (TMath::Pi() * par[0] * (xx - par[1])) /
      (std::sqrt(std::pow((xx - par[1]), 2) + std::pow(par[2], 2)) * par[3]);

  if (TMath::Abs(alpha) < 1.E-9) {
    alpha = 1.E-9;
  }
  double val = par[4] * std::pow((std::sin(alpha) / alpha), 2);
  return val;
}

void main_module() {
  setStyle();

  const double d{0.0001};  // slit width
  const double L{1.};      // slit-screen distance
  const double x0{0.057};  // peak intensity position

  TList *alist = new TList();
  myClass obj(alist);
  // obj.set_List(alist);
  obj.set_nGen(1E6);
  obj.set_nToys(1000);
  obj.set_samplingStep(0.0006);
  obj.set_ySmearing(1);
  obj.set_yError(1); 

  TF1 *fDif = new TF1("funcDiffraction", myFunction, x0 - 0.03, x0 + 0.03, 5);
  fDif->SetParameters(d, x0, L, 632.8E-9, 500);  // d, x0, L, lamda, I
  alist->Add(fDif);

  TH1F *h[3];
  TString n[3] = {"1", "2", "3"};
  for (int i{}; i < 3; ++i) {
    h[i] = new TH1F("h" + n[i], "Histo " + n[i], 100, x0 - 0.03, x0 + 0.03);
    alist->Add(h[i]);
  }

  TGraphErrors *graph = new TGraphErrors();
  alist->Add(graph);

  TF1 *fFit = new TF1("funcFit", myFunction, x0 - 0.03, x0 + 0.03, 5);
  fFit->SetParameters(d, x0, L, 632.8E-9, 500);  // d, x0, L, lamda, I
  alist->Add(fFit);

  /*TH1F *k[3];
  TString m[3] = {"1", "3", "4"};
  for (int i{}; i < 3; ++i) {
    k[i] = new TH1F("par" + m[i], "Pull " + m[i], 100, -5, 5);
    alist->Add(k[i]);
  }*/

  TH1F *hlambda= new TH1F("hlambda", "Lambda", 100, 630E-9, 635E-9);
  alist->Add(hlambda);
  obj.Generate();
  //obj.Analyse();
  obj.Draw();
}