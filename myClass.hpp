#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#include <TBenchmark.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TList.h>
#include <TMatrixD.h>
#include <TRandom.h>
#include <TString.h>
#include<TStyle.h>
#include<TROOT.h>

class myClass {
 public:
  myClass(TList *l) { objList_ = l; }

  TList *getList() const;
  int get_nGen() const;
  int get_nToys() const;
  double get_samplingStep() const;
  double get_ySmearing() const;
  double get_yError() const;
  void set_list(TList *l);
  void set_nGen(int n);
  void set_nToys(int n);
  void set_samplingStep(double s);
  void set_ySmearing(double s);
  void set_yError(double ey);

  void Generate();
  void Draw();
  void Analyze();

 private:
  TList *objList_;
  int nGen_;
  int nToys_;
  double samplingStep_;
  double ySmearing_;
  double yError_;
};

#endif
