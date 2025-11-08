#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#include <TList.h>

class myClass {
 public:
  myClass(TList *l) { objList_ = l; }

  TList *getList() const { return objList_; }
  int get_nGen() const { return nGen_; }
  int get_nToys() const { return nToys_; }
  double get_samplingStep() const { return samplingStep_; }
  double get_ySmearing() const { return ySmearing_; }
  double get_yError() const { return yError_; }
  void set_list(TList *l) { objList_ = l; }
  void set_nGen(int n) { nGen_ = n; }
  void set_nToys(int n) { nToys_ = n; }
  void set_samplingStep(double s) { samplingStep_ = s; }
  void set_ySmearing(double s) { ySmearing_ = s; }
  void set_yError(double ey) { yError_ = ey; }

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
