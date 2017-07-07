#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/unordered_map.hpp>

#define log(msg) do{std::cout<<msg<<std::endl;}while(0)

const int SIZE = 5;
const int MONTH = 0;
const int DAY = 1;
const int YEAR = 2;
const int TIME = 3;
const int RATE = 4;

template<typename INPUT,typename OUTPUT>
bool lexCast(const INPUT & in, OUTPUT & out) {
  try {
    out = boost::lexical_cast<OUTPUT>( in );
  }
  catch( boost::bad_lexical_cast& e ) {
    log( "Exception thrown: [" << e.what() << "] for INPUT[" << in << "]" );
    return false;
  }
  return true;
}

struct HourlyRate{
  std::string time;
  double rate;
  HourlyRate(const std::string & t, const std::string & r){
    time = t;
    lexCast(r,rate);
  }
};

typedef std::map< std::string,std::vector<HourlyRate> >  RateMap;
typedef std::vector<HourlyRate> HourlyRateVec;

int main(int argc, char** argv){

  std::string filename;
  if(argc>1){
    filename = argv[1];
  } else {
    filename = "data.txt";
    log("filename not provided! using default file ["<<filename<<"]");
  }

  log("loading ["<<filename<<"]");
  RateMap rate_map;

  std::ifstream file(filename.c_str());
  if (file.is_open()){
    std::string line;
    while (getline(file,line)){
      std::vector<std::string> strs;
      boost::split(strs,line,boost::is_any_of(" "));

      std::string date = strs[MONTH]+" "+strs[DAY]+" "+strs[YEAR];
      std::string time = strs[TIME];
      std::string rate = strs[RATE];
      rate.erase(rate.end()-1);

      rate_map[date].push_back(HourlyRate(time,rate));
      log("DATE:"<<date<<" TIME:"<<time<<" RATE:"<<rate<<" (rate size["<<rate.size()<<"])");
    }
  }
  file.close();

  log("dumping rate map");
  double count;
  for(RateMap::const_iterator it=rate_map.begin(); it!=rate_map.end(); ++it){
    std::cout<<it->first<<std::endl;
    HourlyRateVec v = it->second;
    if(it->second.size()<6){
      continue;
    }

    double asia_rate = v[0].rate+v[1].rate;
    double eur_rate = v[2].rate+v[3].rate;
    double us_rate = v[4].rate+v[5].rate;
    //log("asia:"<<asia_rate<<" EU:"<<eur_rate<<" US:"<<us_rate);

    /*
    //method 1
    if(asia_rate<0 && eur_rate<0) {
    count-=us_rate;
    }
    if(asia_rate>0 && eur_rate>0) {
    count+=us_rate;
    }
    */

    //method 2
    if(eur_rate<0) {
      count-=us_rate;
    }
    if(eur_rate>0) {
      count+=us_rate;
    }

    log(count);

    //for(HourlyRateVec::const_iterator v_it=it->second.begin(); v_it!=it->second.end(); ++v_it){
    //    std::cout<<" "<<v_it->time<<" "<<v_it->rate<<std::endl;
    //  }
  }
  log("Final: "<<count);

}
