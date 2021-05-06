// Code to fit peaks to Cs137 + Eu152 spectra
// To run: 
//   - create/or load a histogram, 
//   - load the code with .L findAndFit.C
//   - execute findAndFit(histoName,fitMin,fitMax), where fitMin/Max and the range with the 662 keV peak in

#include "TF1.h"
#include "TH1.h"
#include <TMath.h>

void linearityFits(TH1F* histo, double fitMin, double fitMax, int nToFit=10, double thresh=0.001)
{
	if(nToFit>10) nToFit=10;

	// ofstream out("output.dat");
	
	TCanvas* lin_c=new TCanvas("lin_c","Linearity fits");

	double peakPos=(fitMax+fitMin)/2;
    TF1* myFunc = new TF1("myFunc","[0]+[1]*x+[2]*exp(-(x-[3])**2/(2*[4]**2))",0,4095);
    myFunc->SetParNames("Offset","Slope","Constant","Mean","Sigma");
    myFunc->SetParLimits(0,0,10000);
    myFunc->SetParLimits(1,-100,-0.0005);
    myFunc->SetParLimits(2,20,1e7);
    myFunc->SetParLimits(3,fitMin,fitMax);
    myFunc->SetParLimits(4,5,500);
    myFunc->SetParameters(100,-1,5000,peakPos,100);

    histo->Fit(myFunc,"q","",fitMin,fitMax);
	Double_t cent=myFunc->GetParameter(3);
    Double_t sigma=myFunc->GetParameter(4);
    cout << "Raw fit values" << endl
		<< "Centroid: " << cent << endl 
		<< "FWHM: " << 2.35*sigma << endl
		<< "Raw Resolution: " << 2.35*sigma/cent*100 << " %" << endl;

	lin_c->Clear();
	lin_c->Divide(1,3);
	lin_c->cd(1);
	histo->Draw();
	lin_c->cd(2);
	TSpectrum* spec=new TSpectrum();
	histo->GetXaxis()->UnZoom();
    Int_t nFound=spec->Search(histo,sigma,"",thresh);

	Double_t* xPos=spec->GetPositionX();
	Double_t energy[10]={1408,1100.87,964.1,778.9,662.1,344.2,244.7,121.7,41.075,32.061};
	Double_t chn[10];
	Double_t xMax=0;
	Double_t low=0, high=8192;
	Int_t counter=0;
	Double_t ordered[nFound];
	Int_t index[nFound];

	TMath::Sort(nFound,xPos,index);

	for(int i=0;i<10;i++) ordered[i]=xPos[index[i]];
	cout << endl << "Energy (keV)	Channel" << endl;
	for(int i=0;i<10;i++) cout << energy[i] << "		" << ordered[i] << endl;

	histo->GetXaxis()->SetRangeUser(0,ordered[0]*1.1);
	lin_c->GetPad(2)->SetLogy(1);
	histo->DrawCopy();

	lin_c->cd(1);
	histo->GetXaxis()->SetRangeUser(fitMin*0.75,fitMax*1.25);
	histo->Draw();

	TGraph* gr=new TGraph(nToFit,ordered,energy);
	lin_c->cd(3);
	gr->GetXaxis()->SetLimits(0,ordered[0]*1.1);
	gr->Draw("a*");
	gr->GetXaxis()->SetRangeUser(0,ordered[0]*1.1);
	gr->GetYaxis()->SetRangeUser(0,1500);
	gr->SetTitle("Calibration");
	gr->GetXaxis()->SetTitle("channel");
	gr->GetYaxis()->SetTitle("energy (keV)");
	TF1* linFunc = new TF1("linFunc","pol2",0,8192);
	gr->Fit("linFunc","q");
	gr->Draw("a*");

	Double_t FWHM=2.35*sigma*(linFunc->GetParameter(1)+2*linFunc->GetParameter(2)*cent)*100/energy[4];
	// cout << "Raw FWHM: " << 2.35*sigma/cent*100 << "%" << endl;
	cout << endl << "Linearity corrected resolution: " << FWHM << " %" << endl;
}

void Usage()
{
	cout << endl << "linearityFits(TH1F* histo, double fitMin, double fitMax, int nToFit=10, double thresh=0.001) \n\n"
		<< "Use 'fitMin' and 'fitMax' to set range to fit 662 keV peak in 'histo'. \n"
		<< "Use 'nToFit' to limit number of peaks to use for linearity correction (highest energy peaks will be used). \n"
		<< "'thresh' can be used to adjust minimum amplitude of peaks that will be fitted.\n" << endl;
}

void usage() { Usage();}
void Help() { Usage();}
void help() { Usage();}