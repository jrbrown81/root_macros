void plotCSVhistogram(TString filename)
{
	gStyle->SetOptStat(0);
	TCanvas* c1=new TCanvas("c1","c1");
	// TFile *f0 = TFile::Open(filename);
	// f0->cd();

	// TString outname=(TString)filename.ReplaceAll("csv","root");
	// TFile* f=new TFile(outname,"recreate");

	TTree* tree=new TTree("tree","");
	Long64_t nLines=tree->ReadFile(filename,"x:y");
	tree->Draw("y:x");
	Double_t* chn=tree->GetV2();
	Double_t* counts=tree->GetV1();
	TH1F* h1=new TH1F("h1",filename,4096,0,4096);
	for(int i=0;i<4096;i++) h1->Fill(i,counts[i]);
	h1->Draw("hist");
	// h1->Write();
	// f->Close();
}
