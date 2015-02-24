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
  const double NA = -1000000;
  const int IQ_col = 26;
  const vector< vector<int> > used_col =
    { {11, 13, 14, 15, 16, 17},
      {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {10, 12, 18, 19, 20, 21, 22, 23, 24, 25}
    };
  // 自分の
  vector< vector<string> > train_case_st;
  vector< vector<double> > train_case;
  vector< vector<string> > test_case_st;
  vector< vector<double> > test_case;
  double A[100]; // 係数部分
  double B[100]; // 定数部分 IQ = Ax + B
  double zansa[100];
  vector< pair<double, int> > ranking;
  vector<double> X; // answer
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
  void make_train_case_st() { // trainingをsplit
    int N = training.size();
    for (auto i=0; i<N; i++) {
      vector<string> sps = split(training[i]);
      train_case_st.push_back(sps);
    }
    cerr << "split completed" << endl;
  }
  void convert_double() { // doubleにする
    train_case = vector< vector<double> >(train_case_st.size(),
                                          vector<double>());
    for (auto i=0; i<train_case_st.size(); i++) {
      train_case[i] = vector<double>(train_case_st[i].size());
      for (auto j=0; j<train_case_st[i].size(); j++) {
        if (train_case_st[i][j][0] == 'N') {
          train_case[i][j] = NA;
        } else {
          // cerr << train_case_st[i][j] << endl;
          train_case[i][j] = stof(train_case_st[i][j]);
        }
      }
    }
    cerr << "convert completed" << endl;
  }
  void fill_iq() { // iq埋める
    int iq = NA;
    unsigned int ind = 0;
    while (ind < train_case.size()) {
      if (train_case[ind][IQ_col] == NA) {
        unsigned int atari = ind+1;
        while (train_case[atari][IQ_col] == NA) atari++;
        iq = train_case[atari][IQ_col];
        for (auto i=ind; i<atari; i++) {
          train_case[i][IQ_col] = iq;
        }
        ind = atari;
        // cerr << "ind: " << ind << "/ size: " << train_case.size() << endl;
      }
      ind++;
    }
    cerr << "iq filled" << endl;
  }
  bool issameline(int row, int col, vector< vector<double> >::iterator it) {
    return (row > 1 && it[row-1][0] == it[row][0]
            && it[row-1][col] == it[row][col]);
  }
  void calc_AB(int col) { // ABする
    int n = 0;
    double sumx = 0;
    double sumy = 0;
    double sumxy = 0;
    double sumx2 = 0;
    for (auto i=0; i<train_case.size(); i++) {
      if (issameline(i, col, train_case.begin())) {
        continue;
      }
      double x = train_case[i][col];
      double y = train_case[i][IQ_col];
      if (x == NA) continue;
      n++;
      sumx = sumx + x;
      sumy = sumy + y;
      sumxy = sumxy + x * y;
      sumx2 = sumx2 + x * x;
    }
    /* cerr << "sumx: " << sumx << endl;
    cerr << "sumy: " << sumy << endl;
    cerr << "sumxy: " << sumxy << endl;
    cerr << "sumx2: " << sumx2 << endl; */
    A[col] = (n * sumxy - sumx * sumy) / (n * sumx2 - sumx * sumx);
    B[col] = (sumx2 * sumy - sumxy * sumx) / (n * sumx2 - sumx * sumx);
  }
  void zansa_2jouwa(int col) {
    zansa[col] = 0;
    int n = 0;
    for (auto i=0; i<train_case.size(); i++) {
      if (issameline(i, col, train_case.begin())) {
        continue;
      }
      double x = train_case[i][col];
      double y = train_case[i][IQ_col];
      if (x == NA) continue;
      n++;
      double sa = y - A[col] * x - B[col];
      // cerr << sa << endl;
      zansa[col] += sa * sa;
    }
    zansa[col] /= n;
    cerr << col << ": " << (long long)zansa[col] << endl;
  }
  void calc_all() {
    for (auto i=0; i<=scenario; i++) {
      for (auto j=0; j<used_col[i].size(); j++) {
        int c = used_col[i][j];
        calc_AB(c);
        zansa_2jouwa(c);
        ranking.push_back(make_pair(zansa[c], c));
      }
    }
    sort(ranking.begin(), ranking.end());
  }
  void make_training() {
    make_train_case_st();
    convert_double();
    fill_iq();
    calc_all();
  }
  void make_test_case_st() { // testingをsplit
    int N = testing.size();
    for (auto i=0; i<N; i++) {
      vector<string> sps = split(testing[i]);
      test_case_st.push_back(sps);
    }
    cerr << "split completed" << endl;
  }
  void convert_double_test() { // doubleにする
    test_case = vector< vector<double> >(test_case_st.size(),
                                          vector<double>());
    for (auto i=0; i<test_case_st.size(); i++) {
      test_case[i] = vector<double>(test_case_st[i].size());
      for (auto j=0; j<test_case_st[i].size(); j++) {
        if (test_case_st[i][j][0] == 'N') {
          test_case[i][j] = NA;
        } else {
          // cerr << test_case_st[i][j] << endl;
          test_case[i][j] = stof(test_case_st[i][j]);
        }
      }
    }
    cerr << "convert completed" << endl;
  }
  void make_X() {
    vector< pair<int, int> > temp; // id, iq 
    for (auto i=0; i<test_case.size(); i++) {
      bool again = false;
      if (temp.size() > 0 &&
          test_case[i][0] == temp[temp.size()-1].first) again = true;
      for (auto j=0; j<ranking.size(); j++) {
        int u = ranking[j].second;
        // cerr << "u = " << u << endl;
        double x = test_case[i][u];
        if (x == NA) continue;
        double y = A[u] * x + B[u];
        // cerr << "y = " << y << endl;
        int predict = 100;
        if (y < 85) {
          predict = 85;
        } else if (y > 115) {
          predict = 115;
        } else predict = (int)y;
        if (again) {
          temp[temp.size()-1].second = predict;
        } else {
          temp.push_back(make_pair(test_case[i][0], predict));
        }
        break;
      }
    }
    for (auto i=0; i<temp.size(); i++) {
      X.push_back(temp[i].second);
    }
  }
  void make_testing() {
    make_test_case_st();
    convert_double_test();
    make_X();
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
    make_testing();
    return X;
  }
};
