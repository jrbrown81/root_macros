#include "TF1.h"
#include "TH1.h"

//void fitPeak(double peakPos, double fitMin, double fitMax) {
//void fitPeak(double fitMin, double fitMax) {
//void findAndFit2(char hist[], double fitMin, double fitMax) {
void findAndFit2(TString hist, double fitMin, double fitMax) {

    TCanvas* c1;
    if(_file0->GetListOfKeys()->Contains("c1")) {
        c1=(TCanvas*)_file0->Get("c1");
        c1->Clear();
    }
    else TCanvas* c1 = new TCanvas("c1","c1");
	c1->Divide(1,2);
	c1->cd(1);
	
	ofstream out("output.dat");

	double peakPos=(fitMax+fitMin)/2;
    TF1* myFunc = new TF1("myFunc","[0]+[1]*x+[2]*exp(-(x-[3])**2/(2*[4]**2))",0,4095);
    myFunc->SetParNames("Offset","Slope","Constant","Mean","Sigma");
    myFunc->SetParLimits(0,0,10000);
    myFunc->SetParLimits(1,-100,-0.05);
    myFunc->SetParLimits(2,50,1e7);
    myFunc->SetParLimits(3,fitMin,fitMax);
    myFunc->SetParLimits(4,5,150);
    myFunc->SetParameters(100,-1,5000,peakPos,100);

/*  int test=0;
    TH1F* histo;
    TString hist_str="hist";
    int i=1;
    
    while(test==0) {
        if(_file0->GetListOfKeys()->Contains(Form("hist%i",i))) i++;
        else {
            i--;
            test=1;
        }
    }
    histo=(TH1F*)_file0->Get(Form("hist%i",i));*/

    TH1F* histo=(TH1F*)_file0->Get(hist);
	
    histo->Fit(myFunc,"","",fitMin,fitMax);
    Double_t sigma=myFunc->GetParameter(4);
    cout << "Centroid: " << myFunc->GetParameter(3) << " FWHM: " << 2.35*sigma << endl;

	histo->Draw();

/*    TCanvas* c1;
    if(_file0->GetListOfKeys()->Contains("c1")) {
        c1=(TCanvas*)_file0->Get("c1");
        c1->Clear();
    }
    else TCanvas* c1 = new TCanvas("c1","c1");
*/
/*	c1->Clear();
	c1->Divide(1,2);
	c1->cd(1);
	histo->Draw();
	c1->cd(2);*/
	/*TSpectrum* spec=new TSpectrum();
	histo->GetXaxis()->UnZoom();
	Int_t nFound=spec->Search(histo,sigma,"",0.0001);
    //Int_t nFound=spec->Search(histo,10,"",0.002);

	Double_t* xPos=spec->GetPositionX();
	Double_t xMax=0;
	Double_t low=0, high=8192;
	Int_t counter=0;
	Double_t ordered[nFound];

	// Loop to order peaks by energy
	while(counter<nFound) {
		for(int i=0; i<nFound; i++) if(xPos[i]>low && xPos[i]<high) high=xPos[i];
		ordered[counter]=high;
		cout << high << endl;
		out << high << endl;
		counter++;
		low=high;
		high=8192;
	}

	histo->GetXaxis()->SetRangeUser(0,ordered[nFound-1]*1.1);
	c1->GetPad(2)->SetLogy(1);
	histo->DrawCopy();

	c1->cd(1);
	histo->GetXaxis()->SetRangeUser(fitMin*0.75,fitMax*1.25);
	histo->Draw();
*/
}
