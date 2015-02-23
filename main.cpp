#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
#include <algorithm>
using namespace std;

class ChildStuntedness5 {
private:
  // 普通の
  int testType;
  int scenario;
  vector<string> training;
  vector<string> testing;
  // 定数
  const double NA = -1000;
  const int IQ_col = 26;
  const vector< vector<int> > used_col =
    { {11, 13, 14, 15, 16, 17},
      {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {10, 12, 18, 19, 20, 21, 22, 23, 24, 25}
    };
  // 自分の
  vector< vector<string> > test_case_st;
  vector< vector<double> > test_case;
  double A[100]; // 係数部分
  double B[100]; // 定数部分 IQ = Ax + B
  double zansa[100];
  // lemmas
  vector<string> split(string S) { // コンマでsprit
    vector<string> ans;
    stringstream ss(S);
    string s;
    while(getline(ss, s, ',')) {
      ans.push_back(s);
    }
    return ans;
  }
  void make_test_case_st() { // testingをsplit
    int N = testing.size();
    for (auto i=0; i<N; i++) {
      vector<string> sps = split(testing[i]);
      test_case_st.push_back(sps);
    }
  }
  void convert_double() { // doubleにする
    test_case = vector< vector<double> >(test_case_st.size(),
                                         vector<double>());
    for (auto i=0; i<test_case_st.size(); i++) {
      for (auto j=0; j<test_case_st[i].size(); j++) {
        if (test_case_st[i][j] == "NA") {
          test_case[i][j] = NA;
        } else {
          test_case[i][j] = stof(test_case_st[i][j]);
        }
      }
    }
  }
  void fill_iq() { // iq埋める
    int iq = NA;
    unsigned int ind = 0;
    while (ind < test_case.size()) {
      if (test_case[ind][IQ_col] == NA) {
        unsigned int atari = ind+1;
        while (test_case[atari][ind] == NA) atari++;
        iq = test_case[atari][ind];
        for (auto i=ind; i<atari; i++) {
          test_case[i][IQ_col] = iq;
        }
        ind = atari;
      }
      ind++;
    }
  }
  bool issameline(int row, int col, vector< vector<double> >::iterator it) {
    return (row > 1 && it[row-1][0] == it[row][0]
            && it[row-1][col] == it[row][col]);
  }
  void calc_AB(int col) { // ABする
    int n = 0;
    int sumx = 0;
    int sumy = 0;
    int sumxy = 0;
    int sumx2 = 0;
    for (auto i=0; i<test_case.size(); i++) {
      if (issameline(i, col, test_case.begin())) {
        continue;
      }
      n++;
      double x = test_case[i][col];
      double y = test_case[i][IQ_col];
      sumx += x;
      sumy += y;
      sumxy += x * y;
      sumx2 += x * x;
    }
    A[col] = (n * sumxy - sumx * sumy) / (n * sumx2 - sumx * sumx);
    B[col] = (sumx2 * sumy - sumxy * sumx) / (n * sumx2 - sumx * sumx);
  }
  void zansa_2jouwa(int col) {
    zansa[col] = 0;
    int n = 0;
    for (auto i=0; i<test_case.size(); i++) {
      if (issameline(i, col, test_case.begin())) {
        continue;
      }
      n++;
      double x = test_case[i][col];
      double y = test_case[i][IQ_col];
      double sa = y - A[col] * x - B[col];
      zansa[col] += sa * sa;
    }
    zansa[col] /= n;
    cerr << col << ": " << zansa[col] << endl;
  }
  void calc_all() {
    for (auto i=0; i<=scenario; i++) {
      for (auto j=0; j<used_col[i].size(); j++) {
        calc_AB(used_col[i][j]);
        zansa_2jouwa(used_col[i][j]);
      }
    }
  }
  void ranking() {
    
  }
  void make_training() {
    make_test_case_st();
    convert_double();
    fill_iq();
    calc_all();
    ranking();
  }
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
};
