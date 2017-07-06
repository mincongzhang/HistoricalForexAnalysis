#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/unordered_map.hpp>

#define log(msg) do{std::cout<<msg<<std::endl;}while(0)

const int SIZE       = 6;
const int DATE_TIME  = 0;
const int OPEN       = 1;
const int HIGH       = 2;
const int LOW        = 3;
const int CLOSE      = 4;

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

struct FxData{
  int date;
  int time;
  double open;
  double close;
  double change;
  double rate;
  FxData(std::string & date_time, const std::string & o, const std::string & c){
    std::vector<std::string> strs;
    boost::split(strs,date_time,boost::is_any_of(" "));
    if(strs.size()!=2){
      log("Unhandled date time["<<date_time<<"]");
      return;
    }

    lexCast(strs[0],date);
    lexCast(strs[1],time);
    lexCast(o,open);
    lexCast(c,close);
    change = 0.0;
  }
};

typedef std::map< int,std::vector<FxData> >  DataMap;
typedef std::vector<FxData> FxDataVec;

int main(int argc, char** argv){

  std::string filename;
  if(argc>1){
    filename = argv[1];
  } else {
    filename = "data.txt";
    log("filename not provided! using default file ["<<filename<<"]");
  }

  ///////////////////////////////////////
  // Load Rate Map
  ///////////////////////////////////////

  log("loading ["<<filename<<"]");
  DataMap data_map;

  std::ifstream file(filename.c_str());
  if (file.is_open()){
    std::string line;
    while (getline(file,line)){
      std::vector<std::string> strs;
      boost::split(strs,line,boost::is_any_of(";"));
      if(strs.size()!=SIZE){
        log("ERROR: unsupported data ["<<line<<"]");
        continue;
      }

      std::string date_time  = strs[DATE_TIME];
      std::string open       = strs[OPEN];
      std::string close      = strs[CLOSE];
      FxData data(date_time,open,close);

      //hourly data
      if(data.time%10000==0){
        data_map[data.date].push_back(data);
      }
      //log("DATE:"<<date<<" TIME:"<<time<<" RATE:"<<rate<<" (rate size["<<rate.size()<<"])");
    }
  }
  file.close();

  ///////////////////////////////////////
  // Analysis
  ///////////////////////////////////////

  log("calculate price change");
  for(DataMap::iterator it=data_map.begin(); it!=data_map.end(); ++it){
    //std::cout<<it->first<<std::endl;
    FxDataVec & v = it->second;

    for(int i=1; i<v.size();++i){
      v[i].change = v[i].close - v[i-1].close;
      v[i].rate   = (v[i].close - v[i-1].close)/v[i-1].close;
    }

    //for(FxDataVec::const_iterator v_it=it->second.begin(); v_it!=it->second.end(); ++v_it){
    //  std::cout<<" "<<v_it->date<<" "<<v_it->time<<" "<<v_it->open<<" "<<v_it->close<<" "<<v_it->change<<" "<<v_it->rate<<std::endl;
    //}
  }

  log("count gain");
  const double base = 10.0;
  double count = 10.0;
  double borrow = 0.0;
  double leverage = 100;
  int trade_count = 0;
  for(DataMap::iterator it=data_map.begin(); it!=data_map.end(); ++it){
    log(it->first);
    double eu_change = 0.0;

    for(FxDataVec::const_iterator v_it=it->second.begin(); v_it!=it->second.end(); ++v_it){

      if(v_it->time==50000||v_it->time==60000){
        eu_change += v_it->change;
      }

      if(count<base){
        log("count:"<<count<<" borrowing...");
        count+=base;
        borrow+=base;
      }

      double curr = 0.0;
      if(count>base*2){
        count-=base*2;
        curr = base*2;
      } else {
        count-=base;
        curr = base;
      }

      if(v_it->time==100000||v_it->time==110000){

        double interest = v_it->rate*curr*leverage;

        //buy
        if(eu_change>0){
          trade_count++;
          if(interest+curr<0){
            curr=0;
            log("boom!"<<interest);
          }else {
            curr += interest;
          }
        }

        //sell
        if(eu_change<0){
          trade_count++;
          if(curr-interest<0){
            curr = 0;
            log("boom!"<<interest);
          } else {
            curr -= interest;
          }
        }
      }
      count+=curr;
    }//for(FxDataVec
  log("count:"<<count);

  }//for(DataMap::iterator
  log("final count:"<<count);
  log("borrow:"<<borrow);
  log("trade days:"<<trade_count/2);
}
