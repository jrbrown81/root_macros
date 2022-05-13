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
#include <string>
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"

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
//	TFile *hfile=new TFile(outfile,"recreate");
    TFile *hfile=new TFile("spectra.root","update");
//    TFile *hfile=new TFile("test.root","recreate");

//	TString path="../../data/25-10-16/rewrap/";
//	TString infile="28.91V_no_aircon2.Spe";

	cout << "Plotting histogram from file: " << infile << endl << endl;

//	ifstream in(path + infile);
	ifstream in(infile);
	std::string data;

//	int headLines=12, tailLines=16, dataLines;
//	int headLines=12, tailLines=16, dataLines=16384;
	int headLines=12, tailLines=16, dataLines=8192;
//	int headLines=12, tailLines=16, dataLines=4096;
//	int headLines=12, tailLines=16, dataLines=2048;
//	int headLines=12, tailLines=16, dataLines=1024;
//	int totalLines=headLines + dataLines + tailLines;
	int lineNo=0, chnNo=0;

	int i_data;

    TH1F* histo;
    TString hist_str="hist";
    int i=1;

    infile.ReplaceAll(".Spe","");
    // infile.ReplaceAll("_"," ");
    int test=0;
    while(test==0) {
        if(!hfile->GetListOfKeys()->Contains(Form("hist%i",i))) {
            histo=new TH1F(Form("hist%i",i),infile,dataLines,0,dataLines-1);
            test=1;
        } else i++;
    }
	histo->GetXaxis()->SetTitle("Channel");

	while(!in.eof()) {
		getline(in,data);
		lineNo++;
/*		if(lineNo==headLines) {
			std::istringstream iss(data);
			std::string sub;
			iss >> sub;
			iss >> sub;
			dataLines=std::stoi(sub) +1 ;
			cout << dataLines << " lines of data to be read" << endl;
		}*/
		if(lineNo>headLines && lineNo<=headLines+dataLines) {
			i_data = std::stoi (data);
			//if(chnNo<50) {
			//	cout << chnNo << data << endl;
			//	cout << i_data << endl;
			//}
			histo->SetBinContent(chnNo,i_data);
			chnNo++;
		}
	}

//	TCanvas* c1 = new TCanvas("c1","c1");
//	c1->cd();
////	histo->GetXaxis()->SetRangeUser(0,1500);
//	histo->Draw();

    //TString myString=histo->GetTitle();
    //myString.ReplaceAll(".","p");
    //myString.ReplaceAll("pSpe",".C");
    //myString.Prepend("histo_");
    //c1->SaveAs(myString);

    hfile->Write();
//    delete c1;
    hfile->Close();

}
