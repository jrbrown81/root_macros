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
void fitPeak(TH1F* histo, double fitMin, double fitMax) {
//void fitPeak(double fitMin, double fitMax) {
//void findAndFit(double fitMin, double fitMax) {

//	ofstream out("output.dat");

        double entries=(double)histo->GetEntries();
// Centroid
        double centroid=(fitMax+fitMin)/2;
        Int_t minFitBin=(Int_t)histo->GetXaxis()->FindBin(fitMin);
        Int_t maxFitBin=(Int_t)histo->GetXaxis()->FindBin(fitMax);
// Constant
        double integral=(double)histo->Integral(minFitBin,maxFitBin);
        double maxConst=integral*2;
        double minConst=integral/100;
// Width
        double range=fitMax-fitMin;
        double fwhm=range/4;
        double maxWidth=range;
        double minWidth=range/100;
   
// Fit Function
        TF1* myFunc = new TF1("myFunc","[0]+[1]*x+[2]*exp(-(x-[3])**2/(2*[4]**2))",0,4095);
        myFunc->SetParNames("Offset","Slope","Constant","Mean","Sigma");
        myFunc->SetParLimits(0,0,entries/2);
        //myFunc->SetParLimits(0,0,10000);
        myFunc->SetParLimits(1,-entries/centroid,2);
        //myFunc->SetParLimits(1,-100,2);
//        myFunc->SetParLimits(2,5,entries);
        myFunc->SetParLimits(2,minConst,maxConst);
        myFunc->SetParLimits(3,fitMin,fitMax);
        //myFunc->SetParLimits(4,5,(fitMax-fitMin));
        myFunc->SetParLimits(4,minWidth,maxWidth);
        //myFunc->SetParLimits(4,5,15000);
        //myFunc->SetParameters(1,-1,5000,peakPos,(fitMax-fitMin)/4);
//        myFunc->SetParameters(1,-1,entries/2,centroid,fwhm);
//        myFunc->SetParameters(1,-1,integral,centroid,fwhm);
        myFunc->SetParameters(1,0,integral,centroid,fwhm);

        cout << endl << "//////////////////////////////////////////////////////" << endl;
        cout << "//////////////// Fit Parameter Limits ////////////////" << endl;
        cout << "Parameter	Min	 	Max       Guess" << endl;
        cout << "Offset:		0		" << entries/2 << "     1" << endl;
//        cout << "Slope:      " << -entries/centroid << "   2          -1" << endl;
        cout << "Slope:      " << "    0     " << "   2          -1" << endl;
//        cout << "Constant:   5      " << entries << endl;
        cout << "Constant:       " << minConst << "           " << maxConst << "       " << integral << endl;
        cout << "Mean:		" << fitMin << "		" << fitMax << "   " << centroid << endl;
        cout << "Sigma:		" << minWidth << "	 	" << maxWidth << "     " << fwhm << endl;
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

/*	TString myString=histo->GetTitle();
	myString.ReplaceAll(".","p");
	myString.ReplaceAll("pSpe",".C");
	myString.Prepend("histo_");
	c1->SaveAs(myString);
*/
}
