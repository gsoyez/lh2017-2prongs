// find the list of methods (for a given pt) that are on the overall
// line of best perf and robustness

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <list>

using namespace std;

//------------------------------------------------------------------------
class Info{
public:
  Info(const string &i_method, double sigeff, double Scorr, double Bcorr){
    method = i_method;
    perf = sigeff;
    resi = 1.0/sqrt(Scorr*Scorr+Bcorr*Bcorr);
  }

  string method;
  double perf, resi;
};

bool operator<(const Info &i1, const Info &i2){
  return i1.resi<i2.resi;
}

std::ostream & operator<<(std::ostream & ostr, const Info &info) {
  ostr << info.method << " " << info.resi << " " << info.perf;
  return ostr;
}


//------------------------------------------------------------------------
int main(){
  // at the moment, we'll just read from stdin
  vector<Info> vector_infos;
  string method;
  double sigeff,Scorr,Bcorr;
  
  string line;
  while (getline(cin, line)){
    istringstream iss(line);
    iss >> method >> sigeff >> Scorr >> Bcorr;

    if (sigeff<0.0001) continue;
    
    vector_infos.push_back(Info(method,sigeff,Scorr,Bcorr));
  }

  // now sort by increasing resilience
  sort(vector_infos.begin(), vector_infos.end());

  list<Info> infos;
  for (auto &info : vector_infos){
    infos.push_back(info);
    cout << info << endl;
  }
  cout << endl << endl;

  //------------------------------------------------------------------------
  // loop and removve whatever is not on the optimal line
  list<Info>::iterator start=infos.begin();
  while (start != infos.end()){

    // find the largest amongst the remaining 
    list<Info>::iterator finish=start;

    list<Info>::iterator largest=start;
    double v = start->perf;
    while (++finish != infos.end()){
      if (finish->perf>v){
        v = finish->perf;
        largest = finish;
      }
    }

    // we have 2 choices: either the largest is "start" and we keep it
    // or the largest is not and we remove everything between start and largest(-1)
    if (largest != start) infos.erase(start,largest);

    start=largest;
    start++;
  }

  cout << "no_resiliency 0.0 " << infos.begin()->perf << endl;
  for (auto &info : infos){
    cout << info << endl;
  }
  cout << "no_resiliency " << infos.rbegin()->resi << " 0.0" << endl;
   
    
  
  return 0;
}
