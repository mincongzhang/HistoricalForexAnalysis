/*
Dump the data with 15 mins avg and standard deviation
*/

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <deque>
#include <math.h> /* sqrt */

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

template<typename T>
std::string toString ( T num ){
  std::stringstream ss;
  ss << num;
  return ss.str();
}

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
  double avg;
  double std_deviation;
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
  // Load
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

      data_map[data.date].push_back(data);
      //log("DATE:"<<date<<" TIME:"<<time<<" RATE:"<<rate<<" (rate size["<<rate.size()<<"])");
    }
  }
  file.close();

  ///////////////////////////////////////
  // Dump
  ///////////////////////////////////////

  int file_id=0;
  for(DataMap::iterator it=data_map.begin(); it!=data_map.end(); ++it){
    file_id++;

    std::string filename = toString(file_id)+".csv";
    std::ofstream myfile;
    myfile.open(filename.c_str());

    std::cout<<filename<<std::endl;

    std::deque<double> period_data_dq;
    double period_sum(0);
    for(FxDataVec::iterator v_it=it->second.begin(); v_it!=it->second.end(); ++v_it){
      FxData & data = *v_it;


      //calculate 15 min avg
      int period = 15;
      period_sum+=data.close;
      period_data_dq.push_back(data.close);
      if(period_data_dq.size()>period){
        period_sum-=period_data_dq.front();
        period_data_dq.pop_front();
      }
      data.avg = period_sum/period_data_dq.size();

      //calculate standard deviation
      double sd_sum=0.0;
      for(unsigned int i=0; i<period_data_dq.size();++i){
        double diff = (period_data_dq[i]-data.avg);
        sd_sum+=diff*diff;
      }
      data.std_deviation = std::sqrt(sd_sum/period_data_dq.size());

      //dump
      myfile<<data.time<<","<<data.close<<","<<data.avg<<","<<data.std_deviation<<"\n";
    }

    myfile.close();
  }
}
