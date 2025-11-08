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
   void set_nGen(const int);
   void set_nToys(const int);
   void set_samplingStep(const double);
   void set_ySmearing(const double);
   void set_yError(const double);

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
