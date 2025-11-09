#include "myClass.hpp"

#include <iostream>

TList *myClass::getList() const { return objList_; }
int myClass::get_nGen() const { return nGen_; }
int myClass::get_nToys() const { return nToys_; }
double myClass::get_samplingStep() const { return samplingStep_; }
double myClass::get_ySmearing() const { return ySmearing_; }
double myClass::get_yError() const { return yError_; }
void myClass::set_list(TList *l) { objList_ = l; }
void myClass::set_nGen(int n) { nGen_ = n; }
void myClass::set_nToys(int n) { nToys_ = n; }
void myClass::set_samplingStep(double s) { samplingStep_ = s; }
void myClass::set_ySmearing(double s) { ySmearing_ = s; }
void myClass::set_yError(double ey) { yError_ = ey; }

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
  TF1 *fitFunc = static_cast<TF1 *>(objList_->At(5));
  for (int i = 0; i < nToys_; ++i) {
    g->Set(0);
    for (int j = 0; j < 100; ++j) {
      auto x = gRandom->Uniform(f->GetParameter(1) - 0.03,
                                f->GetParameter(1) + 0.03);
      g->SetPoint(j, x, f->Eval(x));
    }
    Analyze();
    TH1F *h[3];
    h[0] = static_cast<TH1F *>(objList_->At(5));
    h[1] = static_cast<TH1F *>(objList_->At(6));
    h[2] = static_cast<TH1F *>(objList_->At(7));
    double pull_1 =
        (fitFunc->GetParameter(1) - 0.057) / (fitFunc->GetParError(1));
    double pull_3 =
        (fitFunc->GetParameter(3) - 632.8E-9) / (fitFunc->GetParError(3));
    double pull_4 =
        (fitFunc->GetParameter(4) - 500) / (fitFunc->GetParError(4));
    h[0]->Fill(pull_1);
    h[1]->Fill(pull_3);
    h[2]->Fill(pull_4);
  }
}

void myClass::Draw() {
  TCanvas *c = new TCanvas("c", "Diffraction", 800, 600);
  c->Divide(5, 2);
  for (int i = 0; i < (objList_->GetEntries()); ++i) {
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

  TGraphErrors *g = static_cast<TGraphErrors *>(objList_->At(4));
  TF1 *fitFunc = static_cast<TF1 *>(objList_->At(5));

  auto result = g->Fit(fitFunc, "SEM");
  TMatrixD cor = result->GetCorrelationMatrix();
  TMatrixD cov = result->GetCovarianceMatrix();

  cor.Print();
  cov.Print();

  std::cout << "Probabilità di Chi^2 = " << fitFunc->GetProb() << std::endl;
  std::cout << "=== Fine analisi ===" << std::endl;
}
