#include "myClass.hpp"

#include <TBenchmark.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TMatrixD.h>
#include <TRandom.h>
#include <TString.h>

#include <iostream>

void myClass::Generate() {
  // [1]: first histogram
  TH1F *h1 = static_cast<TH1F *>(objList_->At(1));
  gBenchmark->Start("TH1::FillRandom");
  h1->FillRandom(objList_->At(0)->GetName(), nGen_);
  gBenchmark->Stop("TH1::FillRandom");
  gBenchmark->Show("TH1::FillRandom");

  // [2]: second histogram
  TF1 *f = static_cast<TF1 *>(objList_->At(0));
  TH1F *h2 = static_cast<TH1F *>(objList_->At(2));
  gBenchmark->Start("TF1::GetRandom");
  for (int i{}; i < nGen_; ++i) {
    h2->Fill(f->GetRandom());
  }
  gBenchmark->Stop("TF1::GetRandom");
  gBenchmark->Show("TF1::GetRandom");

  // [3]: third histogram
  TH1F *h3 = static_cast<TH1F *>(objList_->At(3));
  int points{};
  auto fmax = f->GetMaximum(f->GetXmin(), f->GetXmax());
  gBenchmark->Start("Hit or Miss");
  while (points < nGen_) {
    double x = gRandom->Uniform(f->GetXmin(), f->GetXmax());
    double y = gRandom->Uniform(0, fmax);
    if (y < f->Eval(x)) {
      h3->Fill(x);
      ++points;
    }
  }
  gBenchmark->Stop("Hit or Miss");
  gBenchmark->Show("Hit or Miss");

  // TGraphErrors *g = static_cast<TGraphErrors *>(objList_->At(4));
  /*for (int i{}; i < 100; ++i) {
    auto x = ((f->GetParameter(1) - 0.03) + (i * samplingStep_));
    g->SetPoint(i, x, gRandom->Gaus(f->Eval(x), ySmearing_));
    g->SetPointError(i, 0, yError_);
  }*/

  // TF1 *funcFit = static_cast<TF1 *>(objList_->At(5));
  // TH1F *h = static_cast<TH1F *>(objList_->At(6));
  // for (int i{}; i < nToys_; ++i) {
  /*for (int j{}; j < 100; ++j) {
    auto x = gRandom->Uniform(f->GetParameter(1) - 0.03,
                              f->GetParameter(1) + 0.03);

    double ytrue = f->Eval(x);
    double sigmaY = yError_;
    double ymeas = gRandom->Gaus(ytrue, sigmaY);  // aggiungi rumore
    g->SetPoint(j, x, ymeas);
    g->SetPointError(j, 0.0, sigmaY);  // errore coerente al rumore
  }*/
  // h->Fill(funcFit->GetParameter(3));
  // Analyse();

  /*TH1F *h[3];
  h[0] = static_cast<TH1F *>(objList_->At(6));
  h[1] = static_cast<TH1F *>(objList_->At(7));
  h[2] = static_cast<TH1F *>(objList_->At(8));
  auto pull_1 =
      (funcFit->GetParameter(1) - 0.057) / (funcFit->GetParError(1));
  auto pull_3 =
      (funcFit->GetParameter(3) - 632.8E-9) / (funcFit->GetParError(3));
  auto pull_4 = (funcFit->GetParameter(4) - 500) / (funcFit->GetParError(4));
  h[0]->Fill(pull_1);
  h[1]->Fill(pull_3);
  h[2]->Fill(pull_4);
}*/
}

void myClass::Draw() {
  TCanvas *c = new TCanvas("c", "Diffraction", 10, 20, 800, 600);
  c->Divide(5, 2);
  for (int i{}; i < (objList_->GetEntries()); ++i) {
    c->cd(i + 1);
    if (objList_->At(i)->InheritsFrom("TH1F")) {
      TH1F *h = static_cast<TH1F *>(objList_->At(i));
      h->SetFillColor(38);
      h->SetLineColor(9);
      h->SetLineWidth(2);
      h->SetBarWidth(0.45);
      h->SetBarOffset(0.05);
      h->Draw();
    } else if (objList_->At(i)->InheritsFrom("TF1")) {
      objList_->At(i)->Draw();
    } else if (objList_->At(i)->InheritsFrom("TGraphErrors")) {
      TGraphErrors *g = static_cast<TGraphErrors *>(objList_->At(i));
      g->SetMarkerStyle(20);
      g->SetMarkerSize(0.7);
      g->SetMarkerColor(9);
      g->SetLineColor(9);
      g->SetLineWidth(2);
      g->SetFillColor(38);
      g->Draw("APE");
    } else {
      TGraph *g = static_cast<TGraph *>(objList_->At(i));
      g->SetMarkerStyle(20);
      g->SetMarkerSize(0.7);
      g->SetMarkerColor(9);
      g->SetLineColor(9);
      g->SetLineWidth(2);
      g->SetFillColor(38);
      g->Draw();
    }
  }
  c->Update();
  // c->Print("Diffraction.png");
}

void myClass::Analyse() {
  std::cout << "=== Start of analysis ===" << std::endl;

  TGraphErrors *g = static_cast<TGraphErrors *>(objList_->At(4));
  TF1 *funcFit = static_cast<TF1 *>(objList_->At(5));
  funcFit->SetParName(0, "d");       // slit width
  funcFit->SetParName(1, "x0");      // peak position
  funcFit->SetParName(2, "L");       // slit-screen distance
  funcFit->SetParName(3, "lambda");  // wavelength
  funcFit->SetParName(4, "I");       // intensity
  funcFit->SetParName(5, "B");       // background

  TFitResultPtr r = g->Fit(funcFit, "ESR");
  std::cout << std::endl;

  // default statistics printing

  // matrix printing
  std::cout << "Correlation matrix:";
  TMatrixD cor = r->GetCorrelationMatrix();
  cor.Print();
  std::cout << "Covariance matrix:";
  TMatrixD cov = r->GetCovarianceMatrix();
  cov.Print();
  std::cout << "Hessian matrix:";
  TMatrixD hess = cov.Invert();
  hess.Print();

  std::cout << "=== End of analysis ===" << std::endl;
}

myClass::~myClass() { delete objList_; }