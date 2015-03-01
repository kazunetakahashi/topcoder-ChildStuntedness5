#include <chrono>
#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <valarray>
#define NDEBUG
#include <cassert>
using namespace std;

// 行列
struct matrix {
  int row, col;
  valarray<double> a;
  matrix(int N, int M) {
    a = valarray<double>(N * M);
    row = N;
    col = M;
  }
  bool operator<( const matrix& right ) const { // 使わないけどtupleに必要
    return row < right.row;
  }
  bool operator>( const matrix& right ) const { // 使わないけどtupleに必要
    return row > right.row;
  }
};

matrix multiply(matrix A, matrix B) {
  assert(A.col == B.row);
  int N = A.col;
  matrix C(A.row, B.col);
  for (auto i=0; i<C.row; i++) {
    for (auto j=0; j<C.col; j++) {
      C.a[i*C.col + j] = ((valarray<double>)A.a[slice(i*A.col, N, 1)] *
                          (valarray<double>)B.a[slice(j, N, B.col)]).sum();
    }
  }
  return C;
}

matrix inverse(matrix A, matrix B) { // A^{-1} B を出力
  assert(A.row == A.col);
  assert(A.col == B.row);
  int N = A.row;
  int M = B.col;
  for (auto i=0; i<N; i++) {
    double taikaku = A.a[i*N+i];
    for (auto k=0; k<N; k++) {
      if (i == k) continue;
      double keisu = A.a[k*N+i]/taikaku;
      // A.a[k*N+i] = 0;
      for (auto j=i+1; j<N; j++) {
        A.a[k*N+j] = A.a[k*N+j] - keisu * A.a[i*N+j];
      }
      for (auto j=0; j<M; j++) {
        B.a[k*M+j] = B.a[k*M+j] - keisu * B.a[i*M+j];
      }
    }
  }
  for (auto i=0; i<N; i++) {
    double taikaku = A.a[i*N+i];
    for (auto j=0; j<M; j++) {
      B.a[i*M+j] = B.a[i*M+j]/taikaku;
    }
  }
  return B;
}

matrix transposed(matrix A) {
  matrix B = matrix(A.col, A.row);
  for (auto i=0; i<B.row; i++) {
    for (auto j=0; j<B.col; j++) {
      B.a[i*B.col + j] = A.a[j*A.col + i];
    }
  }
  return B;
}

class ChildStuntedness5 {
private:
  // 普通の
  int testType;
  int scenario;
  vector<string> training;
  vector<string> testing;
  // 定数
  const double NA = -1000000;
  const vector< vector<int> > used_col =
    { {11, 13, 14, 15, 16, 17},
      {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {10, 12, 18, 19, 20, 21, 22, 23, 24, 25}
    };
  const vector<vector< vector<int> > > mult_col =
    {
      { {11, 13, 14, 15, 16, 17},
        {2, 3, 5, 11, 13, 14, 15, 16, 17},
        // {10, 12, 18, 19, 20, 21, 22, 23, 24, 25}
        {10, 12, 14, 15, 17, 18, 19, 20, 21, 22, 23, 24, 25}
      },
      { {11, 13, 14, 15, 16, 17},
        {2, 3, 5, 11, 13, 14, 15, 16, 17},
        {10, 12, 14, 15, 17, 18, 19, 20, 21, 22, 23, 24, 25}
      },
      { {11, 13, 14, 15, 16, 17},
        {2, 3, 5, 11, 13, 14, 15, 16, 17},
        {10, 12, 14, 15, 17, 18, 19, 20, 21, 22, 23, 24, 25}
      }
    };
  const int IQ_col = 26;
  const int inf_iq = 60;
  const int sup_iq = 140;
  const int mari_col = 20;
  const int feed_col = 12;
  const int feed_NA = 90;
  const int feed_transform[4] = {0, 2, 0, 1};
  const int demo1_col = 19;
  const int demo1_max = 2;
  const int demo2_col = 25;
  const int demo2_max = 5;
  const int born_w_col = 14;
  const int region_col = 10;
  // 自分の
  vector< vector<string> > train_case_st;
  vector< vector<double> > train_case;
  vector< vector<double> > train_case_id;  
  vector< vector<string> > test_case_st;
  vector< vector<double> > test_case;
  vector< vector<double> > test_case_id;
  int num_col = 0; // mult_col[testType][scenario].size()
  double reg_transform[100]; // reg
  double mari_transform[7];
  double A[100]; // 係数部分
  double B[100]; // 定数部分 IQ = Ax + B
  double zansa[100];
  vector< tuple<double, bool, int, matrix> > ranking_mult; // zansa, mult?, id, beta 
  vector<double> X; // answer
  
  // lemmas
  void init() {
    num_col = mult_col[testType][scenario].size();
  }
  
  vector<string> split(string S) { // コンマでsprit
    vector<string> ans;
    stringstream ss(S);
    string s;
    while(getline(ss, s, ',')) {
      ans.push_back(s);
    }
    return ans;
  }
  
  bool issameline_mult(int row, vector< vector<double> >::iterator it) {
    return (row > 1 && it[row-1][0] == it[row][0]);
  }
  
  bool issameline(int row, int col, vector< vector<double> >::iterator it) {
    return (row > 1 && it[row-1][0] == it[row][0]
            && it[row-1][col] == it[row][col]);
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
  
  void make_train_case() {
    int N = training.size();
    for (auto i=0; i<N; i++) {
      vector<string> sps = split(training[i]);
      train_case_st.push_back(sps);
    }
    cerr << "split completed" << endl;
    train_case = vector< vector<double> >(train_case_st.size(),
                                          vector<double>());
    for (auto i=0; i<train_case_st.size(); i++) {
      train_case[i] = vector<double>(train_case_st[i].size());
      for (auto j=0; j<train_case_st[i].size(); j++) {
        if (train_case_st[i][j][0] == 'N') {
          train_case[i][j] = NA;
        } else {
          train_case[i][j] = stof(train_case_st[i][j]);
        }
      }
    }
    cerr << "convert completed" << endl;
    fill_iq(); // ここだけtrainに必要！！
  }
  
  void make_train_case_id() {
    int N = training.size();
    for (auto i=0; i<N; i++) {
      if (!(issameline_mult(i, train_case.begin()))) {
        train_case_id.push_back(train_case[i]);
      }
    }
    cerr << "id sorted" << endl;
  }
  
  void make_test_case() {
    int N = testing.size();
    for (auto i=0; i<N; i++) {
      vector<string> sps = split(testing[i]);
      test_case_st.push_back(sps);
    }
    cerr << "split completed" << endl;
    test_case = vector< vector<double> >(test_case_st.size(),
                                          vector<double>());
    for (auto i=0; i<test_case_st.size(); i++) {
      test_case[i] = vector<double>(test_case_st[i].size());
      for (auto j=0; j<test_case_st[i].size(); j++) {
        if (test_case_st[i][j][0] == 'N') {
          test_case[i][j] = NA;
        } else {
          test_case[i][j] = stof(test_case_st[i][j]);
        }
      }
    }
    cerr << "convert completed" << endl;
  }
  
  void make_test_case_id() {
    int N = testing.size();
    for (auto i=0; i<N; i++) {
      if (!(issameline_mult(i, test_case.begin()))) {
        test_case_id.push_back(test_case[i]);
      }
    }
    cerr << "id sorted" << endl;
  }
  
  void average_col(int col, double* tra, int siz) {
    double sum[100];
    int cou[100];
    fill(sum, sum+100, 0);
    fill(cou, cou+100, 0);
    for (auto i=0; i<train_case.size(); i++) {
      if (issameline(i, col, train_case.begin())) {
        continue;
      }
      double reg = train_case[i][col];
      if (reg == NA) {
        continue;
      }
      int reg_i = (int)reg;
      cou[reg_i]++;
      sum[reg_i] = sum[reg_i] + train_case[i][IQ_col];
    }
    vector< pair<double, int> > temp;
    for (auto i=0; i<siz; i++) {
      if (cou[i] < 10) {
        tra[i] = NA;
      } else {
        // cerr << sum[i] << " " << cou[i] << endl;
        temp.push_back(make_pair(sum[i]/cou[i], i));
      }
    }
    sort(temp.begin(), temp.end());
    for (auto i=0; i<temp.size(); i++) {
      // cerr << "region " << temp[i].second << ": " << temp[i].first << endl;
      tra[temp[i].second] = i;
    }
  }
  
  void transform(vector< vector<double> >::iterator b,
                 vector< vector<double> >::iterator e) {
    // 結婚状態の数値を適正化。
    vector< vector<double> >::iterator t = b;
    while (t != e) {
      double a = (*t)[mari_col];
      if (a == NA) {
        t++;
        continue;
      }
      (*t)[mari_col] = mari_transform[(int)a];
      t++;
    }
    // 母乳 or not の数値を適正化。
    t = b;
    while (t != e) {
      double a = (*t)[feed_col];
      if (a == NA || a == feed_NA) {
        (*t)[mari_col] = NA;
        t++;
        continue;
      }
      (*t)[mari_col] = feed_transform[(int)a];
      t++;
    }
    // 謎の項目1・2の数値を適正化。
    // 特に2のほうは、相関性が高いので大事。
    t = b;
    while (t != e) {
      double a = (*t)[demo1_col];
      if (a == NA || a <= 0 || a > demo1_max) {
        (*t)[demo1_col] = NA;
        t++;
        continue;
      }
      t++;      
    }
    t = b;
    while (t != e) {
      double a = (*t)[demo2_col];
      if (a == NA || a <= 0 || a > demo2_max) {
        (*t)[demo1_col] = NA;
        t++;
        continue;
      }
      t++;      
    }
    // 出生時体重を1000で割る
    t = b;
    while (t != e) {
      double a = (*t)[born_w_col];
      if (a == NA) {
        t++;
        continue;
      }
      (*t)[born_w_col] = a/1000;
      t++;      
    }
    // 横長(4)を身長(3)に統合
    t = b;
    while (t != e) {
      double a = (*t)[4];
      if (a == NA) {
        t++;
        continue;
      }
      (*t)[3] = a;
      t++;
    }
    // 体重・身長・横長を「傾き」で置き換える。出生時は+12で求まる。
    for (auto col=2; col<=3; col++) {
      int col_b = col+12;
      t = b;
      while (t != e) {
        double a = (*t)[col];
        if (a == NA) {
          t++;
          continue;
        }
        double b = (*t)[1];
        if (b < 10) {
          (*t)[col] = NA;
          t++;
          continue;
        }
        (*t)[col] = (a-(*t)[col_b])/b;
        t++;
      }
    }
    // 地域をtransformする。
    t = b;
    while (t != e) {
      double a = (*t)[region_col];
      if (a == NA) {
        t++;
        continue;
      }
      (*t)[region_col] = reg_transform[(int)a];
      t++;
      // if ((*t)[region_col] != NA) cerr << (*t)[region_col] << endl;
    }
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
      zansa[col] = zansa[col] + sa * sa;
    }
    zansa[col] = zansa[col]/n;
    // cerr << col << ": " << (long long)zansa[col] << endl;
    // cerr << A[col] << " " << B[col] << endl;
  }
  
  void calc_mult_zansa(int id) {
    int n = 0;
    bool useid = (scenario != 1);
    // cerr << "num_col = " << num_col << endl; 
    for (auto i=0; i<num_col; i++) {
      if( (id >> i & 1) == 1) n++;
    }
    if (n < 2) return;
    int r = (useid ? train_case_id.size() : train_case.size());
    // cerr << "r = " << r << endl;
    vector< vector<double> >::iterator it =
      (useid ? train_case_id.begin() : train_case.begin());
    vector<int> valid;
    for (auto j=0; j<r; j++) {
      for (auto i=0; i<num_col; i++) {
        if ( (id >> i & 1) == 1 && it[j][mult_col[testType][scenario][i]] == NA ) {
          goto EXIT;
        }
      }
      valid.push_back(j);
    EXIT:
      continue;
    }
    int row_n = (int)valid.size();
    if (100 * row_n < r) { // 小さすぎるので棄却
      return;
    }
    // cerr << "row_n = " << row_n << ", n+1 = " << n+1 << endl;
    matrix X(row_n, n+1);
    for (auto j=0; j<row_n; j++) {
      int m = 0;
      for (auto i=0; i<num_col; i++) {
        if ( (id >> i & 1) == 1 ) {
          // cerr << "j=" << j << " " << "m=" << m << endl;
          X.a[j * X.col + m] = it[valid[j]][mult_col[testType][scenario][i]];
          m++;
        }
      }
      // cerr << "j=" << j << " " << "m=" << m << endl;
      X.a[j * X.col + m] = 1;
    }
    matrix y(row_n, 1);
    for (auto j=0; j<row_n; j++) {
      // cerr << "j=" << j << endl;
      y.a[j] = it[valid[j]][IQ_col];
    }
    // cerr << "y" << endl;
    matrix Xt = transposed(X);
    // cerr << "Xt" << endl;
    matrix beta = inverse(multiply(Xt, X), multiply(Xt, y));
    /* cerr << "beta" << endl;
    for (auto i=0; i<beta.a.size(); i++) {
      cerr << beta.a[i] << " ";
    }
    cerr << endl; */
    double zansa_t = 0;
    valarray<double> eps = y.a - multiply(X, beta).a;
    for (auto i=0; i<eps.size(); i++) {
      zansa_t = zansa_t + eps[i] * eps[i];
    }
    zansa_t = zansa_t/row_n;
    // cerr << "id = " << id << ", zansa_t = " << (long long)zansa_t << endl;
    ranking_mult.push_back(make_tuple(zansa_t, true, id, beta));
  }

  void calc_all_single() {
    for (auto i=0; i<=scenario; i++) {
      for (auto j=0; j<used_col[i].size(); j++) {
        int c = used_col[i][j];
        calc_AB(c);
        zansa_2jouwa(c);
        matrix temp(0, 0);
        ranking_mult.push_back(make_tuple(zansa[c], false, c, temp));
      }
    }
  }

  void calc_all_mult() {
    calc_all_single();
    cerr << "single completed" << endl;
    int tot = (1 << num_col);
    for (auto id=0; id<tot; id++) {
      // cerr << "id = " << id << endl;
      calc_mult_zansa(id);
    }
    sort(ranking_mult.begin(), ranking_mult.end());
    for (auto i=0; i<10; i++) {
      if (get<1>(ranking_mult[i]))
        cerr << "i = " << i
             << ", zansa = " << get<0>(ranking_mult[i])
             << ", id = " << get<2>(ranking_mult[i]) << endl;
    }
  }
  
  void make_X() {
    // bool useid = (scenario != 1);
    int r = test_case.size();
    vector< pair<int, int> > temp; // id, iq 
    for (auto i=0; i<r; i++) {
      // cerr << "i = " << i << endl;
      bool again = false;
      if (temp.size() > 0 &&
          test_case[i][0] == temp[temp.size()-1].first) again = true;
      for (auto j=0; j<ranking_mult.size(); j++) {
        double y = 100;
        bool no_na = true;
        if (!(get<1>(ranking_mult[j]))) {
          int u = get<2>(ranking_mult[j]);
          // cerr << "u = " << u << endl;
          double x = test_case[i][u];
          if (x == NA) continue;
          y = A[u] * x + B[u];
          // cerr << "y = " << y << endl;
        } else {
          int id = get<2>(ranking_mult[j]);
          matrix beta = get<3>(ranking_mult[j]);
          int m = 0;
          y = 0;
          for (auto k=0; k<num_col; k++) {
            if( (id >> k & 1) == 1) {
              double x = test_case[i][mult_col[testType][scenario][k]];
              if (x == NA) {
                no_na = false;
                break;
              }
              y = y + x * beta.a[m++];
            }
          }
          if (!no_na) continue;
          y = y + beta.a[m];
        }
        int predict = 100;
        if (y < inf_iq) {
          predict = inf_iq;
        } else if (y > sup_iq) {
          predict = sup_iq;
        } else predict = (int)y;
        if (again) {
          temp[temp.size()-1].second 
            = (predict + temp[temp.size()-1].second)/2;
        } else {
          temp.push_back(make_pair(test_case[i][0], predict));
        }
        break;
      }
    }
    cerr << "size: " << temp.size() << endl;
    for (auto i=0; i<temp.size(); i++) {
      X.push_back(temp[i].second);
    }
  }
  
  void make_training() {
    make_train_case();
    average_col(mari_col, mari_transform, 7);
    average_col(region_col, reg_transform, 100);
    transform(train_case.begin(), train_case.end());
    make_train_case_id();
    calc_all_mult();
  }
  
  void make_testing() {
    make_test_case();
    transform(test_case.begin(), test_case.end());
    make_test_case_id();
    make_X();
  }
  
  //  main theorems
public:
  vector<double> predict(int _testType, int _scenario, 
                         vector<string> _training, vector<string> _testing) {
    // const auto start = chrono::system_clock::now();
    testType = _testType;
    scenario = _scenario;
    training = _training;
    testing = _testing;
    init();
    make_training();
    make_testing();
    /* const auto end = chrono::system_clock::now();
    const auto t = end - start;
    cerr << chrono::duration_cast<chrono::milliseconds>(t).count()/1000 << endl; */
    return X;
  }
};
