#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#include <TList.h>

class myClass {
 public:
  myClass() { objList_ = new TList(); }  // default constructor
  myClass(TList *l) { objList_ = l; }    // parametric constructor

  inline TList *get_List() const { return objList_; }
  inline int get_nGen() const { return nGen_; }
  inline int get_nToys() const { return nToys_; }
  inline double get_samplingStep() const { return samplingStep_; }
  inline double get_ySmearing() const { return ySmearing_; }
  inline double get_yError() const { return yError_; }
  inline void set_List(TList *l) { objList_ = l; }
  inline void set_nGen(int n) { nGen_ = n; }
  inline void set_nToys(int n) { nToys_ = n; }
  inline void set_samplingStep(double s) { samplingStep_ = s; }
  inline void set_ySmearing(double s) { ySmearing_ = s; }
  inline void set_yError(double ey) { yError_ = ey; }

  void Generate();
  void Draw();
  void Analyse();

  ~myClass();  // destructor

 private:
  TList *objList_;
  int nGen_ = 1E6;
  int nToys_;
  double samplingStep_;
  double ySmearing_;
  double yError_;
};

#endif
