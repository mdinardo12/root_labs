#include <TMath.h>

#include "myClass.hpp"

void setStyle() {
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
}

double MyFunction(double *x, double *par) {
  double xx = x[0];
  double a =
      (TMath::Pi() * par[0] * (xx - par[1])) /
      (std::sqrt(std::pow((xx - par[1]), 2) + std::pow(par[2], 2)) * par[3]);
  double val = par[4] * std::pow((std::sin(a) / a), 2);
  return val;
}

void main_module() {
  TF1 *fdif = new TF1("Fdif", MyFunction, 0.057 - 0.03, 0.057 + 0.03, 5);
  fdif->SetParameters(0.0001, 0.057, 1, 632.8E-9, 500);
  TList *alist = new TList();
  alist->Add(fdif);
  myClass obj1(alist);
  TString names[3] = {"1", "2", "3"};
  int nbins{100};
  TH1F *h[3];
  for (int i{}; i < 3; ++i) {
    h[i] = new TH1F("h" + names[i], "Histo " + names[i], nbins, 0.057 - 0.03,
                    0.057 + 0.03);
    h[i]->SetFillColor(kBlue);
    alist->Add(h[i]);
  }

  /*h[0]->SetFillColor(kBlue);
  h[1]->SetFillColor(kRed);*/

  obj1.set_list(alist);
  obj1.set_nGen(1E6);
  TGraphErrors *graph = new TGraphErrors();
  alist->Add(graph);
  graph->SetMarkerSize(2);
  obj1.set_samplingStep(0.0006);
  obj1.set_ySmearing(1);
  TF1 *fitFunction = new TF1("Fit", MyFunction, 0.057 - 0.03, 0.057 + 0.03, 5);
  // fitFunction->SetParameters(0.0001, 0.057, 1, 632.8E-9, 500);
  fitFunction->SetParameter(1, 0.057);
  fitFunction->SetParameter(3, 632.8E-9);
  fitFunction->SetParameter(4, 500);
  fitFunction->FixParameter(0, 0.0001);
  fitFunction->FixParameter(2, 1);
  alist->Add(fitFunction);
  obj1.set_yError(1);
  obj1.Generate();
  obj1.Analyze();
  obj1.Draw();
}