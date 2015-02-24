#include <fstream>
#include "main.cpp"

int main() {
  ifstream ifs("exampleData.csv");
  string str;
  vector<string> T;
  while (getline(ifs, str)) {
    T.push_back(str);
  }
  ChildStuntedness5 c;
  vector<double> ans = c.predict(0, 0, T, T);
  for (auto i=0; i<ans.size(); i++) {
    cout << ans[i] << endl;
  }
}
