#include "myClass.hpp"

TList *myClass::getList() const
{
   return objList_;
}
int myClass::get_nGen() const
{
   return nGen_;
}
int myClass::get_nToys() const
{
   return nToys_;
}
double myClass::get_samplingStep() const
{
   return samplingStep_;
}
double myClass::get_ySmearing() const
{
   return ySmearing_;
}
void myClass::set_list(TList *l)
{
   objList_ = l;
}
void myClass::set_nGen(int n)
{
   nGen_ = n;
}
void myClass::set_nToys(int n)
{
   nToys_ = n;
}
void myClass::set_samplingStep(double s)
{
   samplingStep_ = s;
}
void myClass::set_ySmearing(double s)
{
   ySmearing_ = s;
}
void myClass::Generate()
{
   TH1F *h1 = dynamic_cast<TH1F *>(objList_->At(1));
   gBenchmark->Start("With TH1::FillRandom");
   h1->FillRandom(objList_->At(0)->GetName(), nGen_);
   gBenchmark->Show("With TH1::FillRandom");
   TF1 *f = dynamic_cast<TF1 *>(objList_->At(0));
   TH1F *h2 = dynamic_cast<TH1F *>(objList_->At(2));
   gBenchmark->Start("With TF1::Grandom");
   for (int i = 0; i < nGen_; ++i)
   {
      h2->Fill(f->GetRandom());
   }
   gBenchmark->Show("With TF1::Grandom");
   TH1F *h3 = dynamic_cast<TH1F *>(objList_->At(3));
   int points = 0;
   gBenchmark->Start("With hit or miss");
   while (points < nGen_)
   {
      double x = gRandom->Uniform(f->GetXmin(), f->GetXmax());
      double y = gRandom->Uniform(f->GetMaximum(f->GetXmin(), f->GetXmax()));
      if (y < f->Eval(x))
      {
         h3->Fill(x);
         ++points;
      }
   }
   gBenchmark->Show("With hit or miss");
   TGraphErrors *g = dynamic_cast<TGraphErrors *>(objList_->At(4));
   for (int i = 0; i < 100; ++i)
   {
      g->SetPoint(i, (f->GetParameter(1) - 0.03) + (i + samplingStep_),
                  f->Eval((f->GetParameter(1) - 0.03) + (i + samplingStep_)) + gRandom->Gaus(0, ySmearing_));
      g->SetPointError(i, 0, ySmearing_);
   }
}

void myClass::Draw()
{
   TCanvas *c = new TCanvas("c", "diffraction", 800, 600);
   c->Divide(3, 2);
   for (int i = 0; i < objList_->GetEntries(); ++i)
   {
      c->cd(i + 1);
      objList_->At(i)->Draw();
   }
}

void myClass::Analyze()
{
   std::cout << "=== Inizio analisi ===" << std::endl;

   TGraphErrors *g = (TGraphErrors *)objList_->FindObject("graph");
   TF1 *fitFunc = (TF1 *)objList_->FindObject("fitFunction");

   if (!g || !fitFunc)
   {
      std::cerr << "Errore: grafico o funzione non trovati nella lista!" << std::endl;
      return;
   }

   // Esegue il fit con l’opzione ESR
   auto result = g->Fit(fitFunc, "ESR");
   

   std::cout << "Fit completato. Chi^2/NDF = " << fitFunc->GetChisquare() / fitFunc->GetNDF() << std::endl;
   std::cout << "=== Fine analisi ===" << std::endl;
}
