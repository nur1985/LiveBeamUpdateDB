#include <stdio.h>
#include "ifbeam.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <regex>
#include <time.h>

using namespace std;

void WritePOT(time_t timecall){
  //Parameters that governs this program
  double checkTime=300;//If there is no data in the DB, check this many seconds back
  double dSpill=60;  //Check the DB every dSpill seconds 
  double delay=300;//Check 5 minutes back

  //Get the current time
  double current_time=(double)timecall;
  current_time=current_time-delay;
  struct tm *timeinfo= localtime(&timecall);

  //Get the correct file based on the date 
  char *outname=new char[100];
  sprintf(outname,"log/%d-%02d-%02d_POTData.log",1900+timeinfo->tm_year,timeinfo->tm_mon+1,timeinfo->tm_mday);//Year since 1900, months start at 0

  std::fstream infile;
  std::fstream outfile;
  infile.open(outname,std::fstream::in);
  bool file_exists=infile.good(); 
 
  //Get last time stamp
  double lastTime=current_time;//Just in case there is nothing in the file
  double lastPOT =0;//Initializing the POT
  std::string junk;

  if(file_exists){
    while(!infile.eof()){
      //infile>>lastTime>>lastPOT>>junk[0]>>junk[1]>>junk[2]>>junk[3]>>junk[4];
      infile>>lastTime>>lastPOT>>junk;
    }
  }
  infile.close();

  //Initialize the beamFolder
  double trtgtd;
  BeamFolder* beamFolder = new BeamFolder("NuMI_Physics_A9","",3600); 

  cout<<"Begin loop "<<lastTime<<endl;
  //Because we have to get from the most recent time, need this to store char
  //Otherwise, we could get POT that was repeats of previous storage
  vector<char*> lines;

  if(!file_exists){
    time_t tmpt=(time_t)current_time;
    struct tm *tmpinfo= localtime(&tmpt);
    
    char nurTimeChar[14];
    strftime(nurTimeChar,14,"%Y%m%d%H%M%S",tmpinfo);
    cout<<nurTimeChar<<endl;

    char * outline=new char[100];
    sprintf(outline,"%9.0f %9.10f %s\n",lastTime,-1.0,nurTimeChar);
    lines.push_back(outline);
  }

  //Get the most recently filled time and grab the info
  //Keep going back until you get all POT until last time found
  while(true){
    if(lastTime>=current_time) break;
    try{
      beamFolder->GetNamedData(current_time,"E:TRTGTD",&trtgtd);
      //printf("%9.0f %9.10f\n",current_time,trtgtd);//Debug

      cout<<endl;
      //Cast current time to time_t
      time_t tmpt=(time_t)current_time;
      struct tm *tmpinfo= localtime(&tmpt);

      char nurTimeChar[14];
      strftime(nurTimeChar,14,"%Y%m%d%H%M%S",tmpinfo);

      char * outline=new char[100];
      sprintf(outline,"%9.0f %9.10f %s\n",current_time,trtgtd,nurTimeChar);
      lines.push_back(outline);
      cout<<outline;
      
      current_time=current_time-dSpill;  
    } catch (WebAPIException we){
      current_time=current_time-checkTime;//Try again
    }
    //printf("%9.0f %9.0f\n",lastTime,current_time);//Debug
  }

  outfile.open(outname,std::fstream::out | std::fstream::app);
  for(int i=lines.size()-1;i>=0;i--){
    outfile<<lines[i];
  }

  outfile.close(); 
}

int main(int argc, char *argv[]){
  //A way of putting in a input time of YYYY MM DD
  //This is ugly.  If you have a better way of doing this, do it
  time_t input_time=time(0);
  if(argc==1){
    struct tm *tmpinput = localtime(&input_time);
    input_time=mktime(tmpinput);
  }
  else if(argc==4){
    struct tm *tmpinput = localtime(&input_time);
    tmpinput->tm_year=atoi(argv[1])-1900;//years since 1900
    tmpinput->tm_mon= atoi(argv[2])-1;//Months start at 0
    tmpinput->tm_mday=atoi(argv[3]);
    tmpinput->tm_hour=0;
    tmpinput->tm_min=0;
    tmpinput->tm_sec=0;
    input_time=mktime(tmpinput);
  }
  else if(argc==7){
    struct tm *tmpinput = localtime(&input_time);
    tmpinput->tm_year=atoi(argv[1])-1900;//years since 1900
    tmpinput->tm_mon= atoi(argv[2])-1;//Months start at 0
    tmpinput->tm_mday=atoi(argv[3]);
    tmpinput->tm_hour=atoi(argv[4]);
    tmpinput->tm_min= atoi(argv[5]);
    tmpinput->tm_sec= atoi(argv[6]);
    input_time=mktime(tmpinput);
  }
  WritePOT(input_time);
  return 0;
}
