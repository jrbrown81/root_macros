// Code to read files in a directory, find all .Spe files,
// read each one into a histogram and save all into a .root file
//
// Jamie Brown, 08/12/16
//
// To execute navigate to target directory, load root, and type:
// ".x processSpeDir.C"
//
// Note: this includes a copy of "readSpe2TH1F.C". Would be better to link to this incase improvemnts are
// made to that code.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <TH1.h>
#include <TFile.h>
#include <TSystemDirectory.h>
// #include "TF1.h"
// #include "TMath.h"

using namespace std;
void readSpe2TH1F(TString infile);

void processSpeDir(const char *dirname="./", const char *ext=".Spe")
{
   TSystemDirectory dir(dirname, dirname);
   TList *files = dir.GetListOfFiles();
   if (files) {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) {
         fname = file->GetName();
         if (!file->IsDirectory() && fname.EndsWith(ext)) {
            cout << fname.Data() << endl;
						readSpe2TH1F(fname.Data());
         }
      }
   }
}

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
			// loop over objects in the spectr.root file and find an unused histogram name
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
