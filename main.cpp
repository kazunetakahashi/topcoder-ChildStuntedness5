#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
using namespace std;

class ChildStuntedness5 {
  // 普通の
  int testType;
  int scenario;
  vector<string> training;
  vector<string> testing;
  // 自分の
  vector< vector<string> > test_case_st;
  //  main theorems
public:
  vector<double> predict(int _testType, int _scenario, 
                         vector<string> _training, vector<string> _testing) {
    testType = _testType;
    scenario = _scenario;
    training = _training;
    testing = _testing;
    make_training();
    vector<double> X;
    return X;
  }
  // lemmas
private:
  vector<string> split(string S) { // コンマでsprit
    vector<string> ans;
    stringstream ss;
    string s;
    while(getline(ss, s, ',')) {
      ans.push_back(s);
    }
    return ans;
  }
  void make_test_case_st() {
    int N = testing.size();
    for (auto i=0; i<N; i++) {
      vector<string> sps = split(testing[i]);
      test_case_st.push_back(sps);
    }
  }
  void make_training() {
    
  }
};
