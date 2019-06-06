#include <iostream>
#include <fstream>
#include <string>
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"

using namespace std;

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

	int headLines=12, tailLines=16, dataLines=8192;
//	int headLines=12, tailLines=16, dataLines=2048;
	int totalLines=headLines + dataLines + tailLines;
	int lineNo=0, chnNo=0;

	int i_data;

    TH1F* histo;
    TString hist_str="hist";
    int i=1;

    infile.ReplaceAll(".Spe","");
    infile.ReplaceAll("_"," ");
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
		if(lineNo>headLines && lineNo<=headLines+dataLines) {
			i_data = std::stoi (data);
//			if(chnNo<15) {
//				cout << chnNo << data << endl;
//				cout << i_data << endl;
//			}
			histo->SetBinContent(chnNo,i_data);
			chnNo++;
		}
	}

	TCanvas* c1 = new TCanvas("c1","c1");
	c1->cd();
//	histo->GetXaxis()->SetRangeUser(0,1500);
	histo->Draw();
    
    //TString myString=histo->GetTitle();
    //myString.ReplaceAll(".","p");
    //myString.ReplaceAll("pSpe",".C");
    //myString.Prepend("histo_");
    //c1->SaveAs(myString);
    
    hfile->Write();
	hfile->Close();

}
