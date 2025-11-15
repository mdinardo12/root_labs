#include <TF1.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TMath.h>
#include <TROOT.h>
#include <TRandom.h>
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
      (TMath::Sqrt(TMath::Power((xx - par[1]), 2) + TMath::Power(par[2], 2)) *
       par[3]);

  if (TMath::Abs(alpha) < 1.E-9) {
    alpha = 1.E-9;
  }
  double val = (par[4] * TMath::Power((TMath::Sin(alpha) / alpha), 2)) + par[5];
  return val;
}

void main_module() {
  setStyle();

  const double d{6.E-5};   // slit width
  const double x0{0.064};  // peak position
  const double L{1.};      // slit-screen distance

  TList *alist = new TList();
  myClass obj(alist);
  obj.set_nGen(1E6);
  obj.set_nToys(1);
  obj.set_samplingStep(0.0006);
  obj.set_ySmearing(1.);
  obj.set_yError(1.);

  TF1 *fDif = new TF1("funcDiffraction", myFunction, x0 - 0.03, x0 + 0.03, 6);
  fDif->SetParameters(d, x0, L, 632.8E-9, 0.770, 0.);  // d, x0, L, lambda, I, B
  alist->Add(fDif);

  TH1F *h[3];
  TString n[3] = {"1", "2", "3"};
  for (int i{}; i < 3; ++i) {
    h[i] = new TH1F("h" + n[i], "Histo " + n[i], 100, x0 - 0.03, x0 + 0.03);
    alist->Add(h[i]);
  }

  TGraphErrors *graph = new TGraphErrors("centrale60.txt", "%lg%lg%lg");
  alist->Add(graph);

  TF1 *fFit = new TF1("funcFit", myFunction, x0 - 0.03, x0 + 0.03, 6);
  fFit->SetParameters(d, x0, L, 632.8E-9, 0.770, 0.);  // d, x0, L, lambda, I, B
  fFit->FixParameter(0, d);
  fFit->FixParameter(1, x0);
  fFit->FixParameter(2, L);
  alist->Add(fFit);

  /*TH1F *k[3];
  TString m[3] = {"1", "3", "4"};
  for (int i{}; i < 3; ++i) {
    k[i] = new TH1F("par" + m[i], "Pull " + m[i], 100, -5, 5);
    alist->Add(k[i]);
  }*/

  // TH1F *hLambda = new TH1F("hLambda", "Lambda", 100, 630E-9, 635E-9);
  // alist->Add(hLambda);
  obj.Generate();
  // for (int i = 0; i < obj.get_nToys(); ++i) {
  // fFit->FixParameter();
  // fFit->SetParameter();
  obj.Analyse();
  // hLambda->Fill(fFit->GetParameter(3));
  //}
  obj.Draw();
}