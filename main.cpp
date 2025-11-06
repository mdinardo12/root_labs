#include "myClass.hpp"

double MyFunction(double *x, double *par)
{
   double xx = x[0];
   double a = (M_PI * par[0] * (xx - par[1])) / (std::sqrt(pow((xx - par[1]), 2) + pow(par[2], 2)) * par[3]);
   double val = par[4] * pow((sin(a) / a), 2);
   return val;
}

void main_module()
{
   TF1 *fdif = new TF1("Fdif", MyFunction, -M_PI, M_PI, 5);
   fdif->SetParameters(0.0001, 0.057, 1, 632.8E-9, 500);
   TList *alist = new TList();
   alist->Add(fdif);
   myClass obj1(alist);
   obj1.Draw();
   for (int i = 0; i < 3; ++i) {

      TH1F *h = new TH1F("h", "Histo", 100, fdif->GetParameter(1) - 0.03, fdif->GetParameter(1) + 0.03);
      h->SetFillColor(kBlue);
      h->GetXaxis()->SetTitle("x");
      h->GetYaxis()->SetTitle("Entries");
      h->GetXaxis()->SetTitleSize(1.2);
      h->GetYaxis()->SetTitleSize(1.2);
      h->GetXaxis()->SetTitleOffset(1.2);
      h->GetYaxis()->SetTitleOffset(1.2);
      alist->Add(h);
   }
   obj1.set_list(alist);
   obj1.set_nGen(1E6);
   TGraphErrors *graph = new TGraphErrors();
   alist->Add(graph);
   obj1.set_samplingStep(0.0006);
   obj1.set_ySmearing(1);
   obj1.Generate();
   obj1.Draw();
}