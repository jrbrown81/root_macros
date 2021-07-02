////////////////////////////////////////////////////
//                                                //
// Code to fit a Gausian plus a linear background //
// to a histogram, over the specified range.      //
//                                                //
// J. Brown                                       //
// 05/01/17                                       //
//                                                //
////////////////////////////////////////////////////

#include "TF1.h"
#include "TH1.h"

//void fitPeak(double peakPos, double fitMin, double fitMax) {
double fitPeak(TH1F* histo, double fitMin, double fitMax) {
//void fitPeak(double fitMin, double fitMax) {
//void findAndFit(double fitMin, double fitMax) {

//	ofstream out("output.dat");

//        double entries=(double)histo->GetEntries();
        double entries=(double)histo->Integral();
// Centroid
        double centroid=(fitMax+fitMin)/2;
        Int_t minFitBin=(Int_t)histo->GetXaxis()->FindBin(fitMin);
        Int_t maxFitBin=(Int_t)histo->GetXaxis()->FindBin(fitMax);
// Constant
        double integral=(double)histo->Integral(fitMin,fitMax);
//        double integral=(double)histo->Integral(minFitBin,maxFitBin);
        double maxConst=integral*2;
        double minConst=integral/1000;
        double constant=integral;
// Width
        double range=fitMax-fitMin;
        double fwhm=range/4;
        double maxWidth=range;
        double minWidth=range/100;

// Slope
				double slope = (histo->GetBinContent(maxFitBin)-histo->GetBinContent(minFitBin))/(fitMax-fitMin);
        double slopeMin = -2*abs(slope);
				double slopeMax = 2*abs(slope);;
// Offset
        double offset = histo->GetBinContent(maxFitBin) - slope*fitMax;
        double offMin=offset*-0.5;
        double offMax=offset*2;
				
// Fit Function
        TF1* myFunc = new TF1("myFunc","[0]+[1]*x+[2]*exp(-(x-[3])**2/(2*[4]**2))",0,4095);
        myFunc->SetParNames("Offset","Slope","Constant","Mean","Sigma");
        myFunc->SetParLimits(0,offMin,offMax);
//        myFunc->SetParLimits(0,0,entries/2);
        //myFunc->SetParLimits(0,0,10000);
        myFunc->SetParLimits(1,slopeMin,slopeMax);
        //myFunc->SetParLimits(1,-100,2);
//        myFunc->SetParLimits(2,5,entries);
        myFunc->SetParLimits(2,minConst,maxConst);
        myFunc->SetParLimits(3,fitMin,fitMax);
        //myFunc->SetParLimits(4,5,(fitMax-fitMin));
        myFunc->SetParLimits(4,minWidth,maxWidth);
        //myFunc->SetParLimits(4,5,15000);
        //myFunc->SetParameters(1,-1,5000,peakPos,(fitMax-fitMin)/4);
//        myFunc->SetParameters(1,-1,entries/2,centroid,fwhm);
        myFunc->SetParameters(offset,slope,integral,centroid,fwhm);
//        myFunc->SetParameters(1,0,integral,centroid,fwhm);

        cout << endl << "//////////////////////////////////////////////////////" << endl;
        cout << "//////////////// Fit Parameter Limits ////////////////" << endl;
        cout << "Parameter	Min	 	Max     	 Guess" << endl;
				cout << "Offset:		" << offMin << "		"	<< offMax << "     " << offset << endl;
        cout << "Slope:      	" << slopeMin << "   	" << slopeMax	<< "		" << slope << endl;
        cout << "Constant:       " << minConst << "          " << maxConst << "	       " << integral << endl;
        cout << "Mean:		" << fitMin << "		" << fitMax << "   		" << centroid << endl;
        cout << "Sigma:		" << minWidth << "	 	" << maxWidth << "	 	    " << fwhm << endl;
        cout << "//////////////////////////////////////////////////////" << endl << endl;;
        
        histo->Fit(myFunc,"","",fitMin,fitMax);
        centroid=myFunc->GetParameter(3);
        fwhm=2.35*myFunc->GetParameter(4);
        //cout << "Centroid: " << myFunc->GetParameter(3) << ", FWHM: " << 2.35*myFunc->GetParameter(4) << endl;
        cout << "Centroid: " << centroid << ", FWHM: " << fwhm << " (" << fwhm/centroid*100 << "%)" << endl;

/*	c1->Clear();
	c1->Divide(1,2);
	c1->cd(1);
	hist1->Draw();
	c1->cd(2);
	TSpectrum* spec=new TSpectrum();
	hist1->GetXaxis()->UnZoom();
	Int_t nFound=spec->Search(hist1,10,"",0.002);

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

	hist1->GetXaxis()->SetRangeUser(0,ordered[nFound-1]*1.1);
	c1->GetPad(2)->SetLogy(1);
	hist1->DrawCopy();
*/
//	c1->cd(1);
	histo->GetXaxis()->SetRangeUser(fitMin*0.75,fitMax*1.25);
	histo->Draw();
	
	TF1* f1=new TF1("f1","pol1",fitMin,fitMax);
	f1->SetParameters(myFunc->GetParameter(0),myFunc->GetParameter(1));
	f1->SetLineColor(3);
	f1->Draw("same");
	
	TF1* f2=new TF1("f2","gaus",fitMin,fitMax);
	f2->SetParameters(myFunc->GetParameter(2),myFunc->GetParameter(3),myFunc->GetParameter(4));
	f2->SetLineColor(6);
	f2->Draw("same");


/*	TString myString=histo->GetTitle();
	myString.ReplaceAll(".","p");
	myString.ReplaceAll("pSpe",".C");
	myString.Prepend("histo_");
	c1->SaveAs(myString);
*/
   return centroid;
}

double fitPeak(TH1I* histo, double fitMin, double fitMax) {

   cout << endl << "Received TH1I. Cloning to a TH1F for fitting." << endl;
   
   TString str=histo->GetName();
   str+="_th1f";
   TH1F* hist_th1f=(TH1F*)histo->Clone(str);
   
   return fitPeak(hist_th1f,fitMin,fitMax);
}

void help() {
   
   cout << endl << "Fit a Gaussian plus linear background to a defined range in a histogram." << endl
        << "Will accept either TH1F or TH1I." << endl
        << endl << "	fitPeak(TH1I* histo, double fitMin, double fitMax)" << endl
	<< endl << "where 'histo' is you histogram to fit and 'fitMin' and 'fitMax' define the range over which to fit." << endl;

}

void Help() {
	help();
}
void usage() {
	help();
}
void Usage() {
	help();
}
