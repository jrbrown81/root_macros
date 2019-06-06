// Code to fit peaks to Cs137 + Eu152 spectra
// To run: 
//   - create/or load a histogram, 
//   - load the code with .L findAndFit.C
//   - execute findAndFit(histoName,fitMin,fitMax), where fitMin/Max and the range with the 662 keV peak in

#include "TF1.h"
#include "TH1.h"

//void fitPeak(double peakPos, double fitMin, double fitMax) {
//void fitPeak(double fitMin, double fitMax) {
//void findAndFit(double fitMin, double fitMax) {
void findAndFit(TH1F* histo, double fitMin, double fitMax) {

	ofstream out("output.dat");

	double peakPos=(fitMax+fitMin)/2;
    TF1* myFunc = new TF1("myFunc","[0]+[1]*x+[2]*exp(-(x-[3])**2/(2*[4]**2))",0,4095);
    myFunc->SetParNames("Offset","Slope","Constant","Mean","Sigma");
    myFunc->SetParLimits(0,0,10000);
    myFunc->SetParLimits(1,-100,-0.0005);
    myFunc->SetParLimits(2,20,1e7);
    myFunc->SetParLimits(3,fitMin,fitMax);
    myFunc->SetParLimits(4,5,500);
    myFunc->SetParameters(100,-1,5000,peakPos,100);

    histo->Fit(myFunc,"","",fitMin,fitMax);
    Double_t sigma=myFunc->GetParameter(4);
    cout << "Centroid: " << myFunc->GetParameter(3) << " FWHM: " << 2.35*sigma << endl;

	c1->Clear();
	c1->Divide(1,2);
	c1->cd(1);
	histo->Draw();
	c1->cd(2);
	TSpectrum* spec=new TSpectrum();
	histo->GetXaxis()->UnZoom();
//	Int_t nFound=spec->Search(histo,sigma/2,"",0.00001);
	Int_t nFound=spec->Search(histo,sigma/5,"",0.001);
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

}
