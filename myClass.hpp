#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#include <TList.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TRandom.h>
#include <TString.h>
#include <TBenchmark.h>
#include <TGraphErrors.h>
#include<TMatrixD.h>
#include<TFitResult.h>
#include<TFitResultPtr.h>

class myClass
{
public:
   myClass(TList *l) { objList_ = l; }

   // Getters
   TList *getList() const;
   int get_nGen() const;
   int get_nToys() const;
   double get_samplingStep() const;
   double get_ySmearing() const;
   double get_yError() const;

   void set_list(TList *);
   void set_nGen(int);
   void set_nToys(int);
   void set_samplingStep(double);
   void set_ySmearing(double);
   void set_yError(double);

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
