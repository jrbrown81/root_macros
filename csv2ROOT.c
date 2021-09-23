//
//  csv2ROOT.c
//  
//
//  Created by Jamie Brown on 19/08/2019.
//

#include <stdio.h>

void csv2ROOT(TString inFile){

	TString outFile=inFile;
	outFile.ReplaceAll(".csv",".root");

	TFile* file=new TFile(outFile,"recreate");
	TTree* tree=new TTree("tree","");
	Long64_t nLines=tree->ReadFile(inFile);
	tree->Write();
	file->Close();
	
	return;
}
