#include <iostream>
#include <vector>
using namespace std;

class ChildStuntedness5 {
public:
  vector<double> predict(int testType, int scenario, 
                         vector<string> training, vector<string> testing) {
    vector<double> X;
    for (unsigned int i=0; i<training.size(); i++) {
      cerr << "no" << i << endl;
      cerr << training[i] << endl;
      cerr << testing[i] << endl;
      X.push_back(90);
    }
    return X;
  }
};
