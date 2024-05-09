/////////////////////////////////////////////////
//                                             //
// Code to read a single column txt file       //
// containing counts per bin, convert to a     //
// histogram, and save it as a .root file.     //
//                                             //
// Written by J. Brown, 09/05/24							 //
//                                             //
/////////////////////////////////////////////////

#include <stdio.h>
#include <cstdlib>

void txt2ROOT(TString filename,Int_t rebin=0,TString title="")
{
	TH1F* hist;
	vector<Int_t> counts;

	ifstream in(filename);
	std::string data;
	while(getline(in,data)){
		counts.push_back(stoi(data));
	}
	Int_t nbins=counts.size();
	if(title.Length()==0) {
		title=filename;
		title.ReplaceAll(".txt","");
	}
	hist = new TH1F("hist",title,nbins,0,nbins);
	for(int i=0;i<nbins;i++){
		hist->Fill(i,counts.at(i));
	}
	if(rebin!=0) hist->Rebin(rebin);
	hist->Draw("hist");

	TString outfile=filename;
	outfile.ReplaceAll(".txt",".root");
	TFile* file=new TFile(outfile,"recreate");
	hist->Write();
	file->Close();

	return;
}
