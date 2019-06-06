//
//  maestrospe2root.C
//  
//
//  Created by Paul Davies on 14/09/2015.
//  Nuclear Physics Group, Department of Physics, University of York
//

/*

To compile this code needs to be linked to ROOT libraries, use:

g++ `root-config --cflags --libs` -Wl,--no-as-needed -lTree -lRIO -lCore -lHist maestrospe2root.C -o maestrospe2root
g++ `root-config --cflags --libs` maestrospe2root.C -o maestrospe2root

--no-as-needed, -lTree -lRIO -lCore -lHist is used to work around some linking issues in Ubuntu. Probably not needed on less pedantic systems

data is saved into a tree, 1 histogram per pulse. To look at them in root use:

tree->Draw("hist.Draw()","","goff",1,12) which will plot one event starting at event 12

*/


#include "TMath.h"	
#include "TF1.h"
#include "TH1.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TTree.h"
#include "TFile.h"
#include "TPad.h"
#include "TCanvas.h"

#include <fstream>
#include <cstdlib>


using namespace std;

struct headerdata {
/*
$SPEC_ID:
No sample description was entered.
$SPEC_REM:
DET# 2
DETDESC# NP72 MCB 131
AP# Maestro Version 7.01
$DATE_MEA:
10/19/2015 14:46:51
$MEAS_TIM:
1774 1827
*/

  string spec_id;
  string spec_rem[3];
  string date_time;
  int start_time;
  int end_time;
  int first_channel;
  int last_channel;

} ;





int maestrospe2root(char * infile){

int debug = 0;

TString infilename(infile);
TString outfilename(infile);
outfilename.ReplaceAll(".Spe",".C");

if(infilename == outfilename){
	cout << "Infilename same as outfile name" << endl;
	return 9999;
	}

ifstream in(infile);

std::string data;

size_t string_start;
size_t string_sep;
size_t string_end;

headerdata hdrdata;

TH1I * hist;

//number of channels read in from a spectrum
int datapointcounter = 0;

//for info this is where we will store the header info, this is declared at the start of the file
/*
struct headerdata {
//for info Wavedump txt files have the following header info


//$SPEC_ID:
//No sample description was entered.
//$SPEC_REM:
//DET# 2
//DETDESC# NP72 MCB 131
//AP# Maestro Version 7.01
//$DATE_MEA:
//10/19/2015 14:46:51
//$MEAS_TIM:
//1774 1827


  string spec_id;
  string spec_rem[3];
  string date_time;
  int start_time;
  int end_time;
  int first_channel;
  int last_channel;

} ;
*/


//lets start with a clean header data structure
clearheaderdata(hdrdata);

while( !in.eof() ){
	
	//get the data and save as a string
	getline(in,data);	
	//reading in as a string so lets use the first item in the header to denote a new event
	//then check for each of the header titles, if it doesn't contain a header title it must be data
	if( data.find("$SPEC_ID:") != std::string::npos ) {
		getline(in,data);
		hdrdata.spec_id = data;
		if(debug) cout << hdrdata.spec_id << endl;
	}
	else if( data.find("$SPEC_REM:") != std::string::npos ) {
		
		for(int i = 0; i < 3; i++){
		  getline(in,data);
		  hdrdata.spec_rem[i].append(data);
		  if(debug) cout <<  hdrdata.spec_rem[i] << endl;
		  }
	}
	else if( data.find("$DATE_MEA:") != std::string::npos ) {
		
		getline(in,data);
		hdrdata.date_time.append(data);
		if(debug) cout <<  hdrdata.date_time << endl;
	}
	else if( data.find("$MEAS_TIM:") != std::string::npos ) {
		
		string start_time, end_time;
		getline(in,data);
		
		string_start = 0;
		string_sep = data.find(" ");
		string_end = data.length();
		
		start_time.append(data,string_start,string_sep);
		end_time.append(data,string_sep + 1,string_end);
		hdrdata.start_time = atoi(start_time.data());
		hdrdata.end_time = atoi(end_time.data());
		
		if(debug) cout <<  hdrdata.start_time << endl;
		if(debug) cout <<  hdrdata.end_time << endl;

	}
	else if( data.find("$DATA:") != std::string::npos ) {
		
		string first_channel, last_channel;
		
		getline(in,data);
		string_start = 0;
		string_sep = data.find(" ");
		string_end = data.length();

		first_channel.append(data,string_start,string_sep);
		last_channel.append(data,string_sep + 1,string_end);
		hdrdata.first_channel = atoi(first_channel.data());
		hdrdata.last_channel = atoi(last_channel.data());
		
		if(debug) cout <<  hdrdata.first_channel << endl;
		if(debug) cout <<  hdrdata.last_channel << endl;		
			
		hist = new TH1I("hist",infile,hdrdata.last_channel+1,hdrdata.first_channel,hdrdata.last_channel);
		hist->Sumw2();
		cout << hist->GetNbinsX() << endl;
		cout <<  hdrdata.first_channel << endl;
		cout <<  hdrdata.last_channel << endl;


		
	}
	else {
		//getline(in,data);
		if(datapointcounter < hdrdata.last_channel+1){
			datapointcounter++;
			if(debug) cout << data.data() << endl;
			//			hist->Fill(datapointcounter,atoi(data.data()));
			hist->SetBinContent(datapointcounter,(Int_t)atoi(data.data()));
			hist->SetBinError(datapointcounter,sqrt((Int_t)atoi(data.data())));
			}


		//cout << data << endl;
		//datapointcounter++;
		//double tmpdata = atof(data.data());
		//if(debug == 8) std::cout << tmpdata << std::endl;
		//hist->Fill(datapointcounter, tmpdata);
		}
	

}


hist->SaveAs(outfilename.Data());
delete hist;
return 1;

}

//some helper functions

void Usage(){

cout << "Usage: \n" << "\t ./maestrospe2root <inputfile> <outputfile>" << endl;

}

/*
struct headerdata {
//for info Wavedump txt files have the following header info
  string spec_id;
  string spec_rem;
  string date_time;
  int start_time;
  int end_time;
  int first_channel;
  int last_channel;

} ;
*/

void printheaderdata(headerdata hdrdata){

std::cout << "Spec ID: " << hdrdata.spec_id <<std::endl;
for(int i = 0; i < 3; i++) 
  std::cout << "Spec REM: " << hdrdata.spec_rem[i] << std::endl;
std::cout << "Date/Time: " << hdrdata.date_time << std::endl;
std::cout << "Start Clock Time: " << hdrdata.start_time << std::endl;
std::cout << "Finish Clock Time: " << hdrdata.end_time << std::endl;
std::cout << "First Channel: " << hdrdata.first_channel << std::endl;
std::cout << "Last Channel: " << hdrdata.last_channel << std::endl;

}

void clearheaderdata(headerdata hdrdata){

hdrdata.spec_id = "";
for(int i = 0; i < 3; i++) hdrdata.spec_rem[i] = "";
hdrdata.date_time = "";
hdrdata.start_time = 0;
hdrdata.end_time = 0;
hdrdata.first_channel = 0;
hdrdata.last_channel = 0;

}
