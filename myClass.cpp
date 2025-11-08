#include "myClass.hpp"

#include <TBenchmark.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TRandom.h>
#include <TString.h>

#include <iostream>

void myClass::Generate() {
  TH1F *h1 = static_cast<TH1F *>(objList_->At(1));
  gBenchmark->Start("With TH1::FillRandom");
  h1->FillRandom(objList_->At(0)->GetName(), nGen_);
  gBenchmark->Show("With TH1::FillRandom");
  TF1 *f = static_cast<TF1 *>(objList_->At(0));
  TH1F *h2 = static_cast<TH1F *>(objList_->At(2));
  gBenchmark->Start("With TF1::Grandom");
  for (int i = 0; i < nGen_; ++i) {
    h2->Fill(f->GetRandom());
  }
  gBenchmark->Show("With TF1::Grandom");
  TH1F *h3 = static_cast<TH1F *>(objList_->At(3));
  int points = 0;
  gBenchmark->Start("With hit or miss");
  while (points < nGen_) {
    double x = gRandom->Uniform(f->GetXmin(), f->GetXmax());
    double y = gRandom->Uniform(f->GetMaximum(f->GetXmin(), f->GetXmax()));
    if (y < f->Eval(x)) {
      h3->Fill(x);
      ++points;
    }
  }
  gBenchmark->Show("With hit or miss");
  TGraphErrors *g = static_cast<TGraphErrors *>(objList_->At(4));
  for (int i = 0; i < 100; ++i) {
    auto x = ((f->GetParameter(1) - 0.03) + (i * samplingStep_));
    g->SetPoint(i, x, gRandom->Gaus(f->Eval(x), ySmearing_));
  }
}

void myClass::Draw() {
  TCanvas *c = new TCanvas("c", "Diffraction", 800, 600);
  c->Divide(3, 2);
  for (int i = 0; i < objList_->GetEntries(); ++i) {
    c->cd(i + 1);
    if ((objList_->At(i))->InheritsFrom("TGraphErrors")) {
      objList_->At(i)->Draw("APE");
    } else {
      objList_->At(i)->Draw();
    }
  }
}

void myClass::Analyze() {
  std::cout << "=== Inizio analisi ===" << std::endl;

  TGraphErrors *g = (TGraphErrors *)objList_->FindObject("graph");
  TF1 *fitFunc = (TF1 *)objList_->FindObject("fitFunction");

  if (!g || !fitFunc) {
    std::cerr << "Errore: grafico o funzione non trovati nella lista!"
              << std::endl;
    return;
  }

  // Esegue il fit con l’opzione ESR
  auto result = g->Fit(fitFunc, "ESR");

  std::cout << "Fit completato. Chi^2/NDF = "
            << fitFunc->GetChisquare() / fitFunc->GetNDF() << std::endl;
  std::cout << "=== Fine analisi ===" << std::endl;
}
