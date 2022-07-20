#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void readSpe2TH1F(TString infile) {

	TString outfile=infile;
	outfile.ReplaceAll(".Spe",".root");
  TFile *hfile=new TFile("spectra.root","update");

	cout << "Plotting histogram from file: " << infile << endl << endl;

	ifstream in(infile);
	std::string data;

	int headLines=12, dataLines=0;
	int lineNo=0, chnNo=0;

	int i_data;

    TH1F* histo;
    TString hist_str="hist";
    int i=1;

    infile.ReplaceAll(".Spe","");

	while(!in.eof()) {
		getline(in,data);
		lineNo++;
		if(lineNo==headLines) {
			std::istringstream iss(data);
			std::string sub;
			iss >> sub >> dataLines;
			dataLines++;
			cout << dataLines << " lines of data to be read" << endl;
      int test=0;
      while(test==0) {
          if(!hfile->GetListOfKeys()->Contains(Form("hist%i",i))) {
              histo=new TH1F(Form("hist%i",i),infile,dataLines,0,dataLines-1);
              test=1;
          } else i++;
      }
  	histo->GetXaxis()->SetTitle("Channel");
		}
		if(lineNo>=headLines && lineNo<=headLines+dataLines) {
			i_data = std::stoi (data);
			histo->SetBinContent(chnNo,i_data);
			chnNo++;
		}
	}

    hfile->Write();
    hfile->Close();

}
