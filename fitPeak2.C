////////////////////////////////////////////////////
//                                                //
// Code to fit a Gausian plus a linear background //
// to a histogram, over the specified range and   //
// return a pointer to the fit function.          //
// Intended to be called my other macros.         //
//                                                //
// J. Brown                                       //
// 05/01/17                                       //
//                                                //
////////////////////////////////////////////////////

#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"

TFitResultPtr fitPeak2(TH1F* histo, double fitMin, double fitMax, TString myOpt="", TString opt="QS", TString gopt="") {

//	ofstream out("output.dat");

   if(!opt.Contains("S")) opt+="S";
   if(myOpt.Contains("Q")) opt+="Q";

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
    myFunc->SetParLimits(1,slopeMin,slopeMax);
    myFunc->SetParLimits(2,minConst,maxConst);
    myFunc->SetParLimits(3,fitMin,fitMax);
    myFunc->SetParLimits(4,minWidth,maxWidth);
    myFunc->SetParameters(offset,slope,integral,centroid,fwhm);
    myFunc->SetParameters(1,0,integral,centroid,fwhm);


  if(!myOpt.Contains("Q")) {
     cout << endl << "//////////////////////////////////////////////////////" << endl;
     cout << "Fitting: " << histo->GetTitle() << endl;
     cout << "//////////////////////////////////////////////////////" << endl;
  }
  if(myOpt.Contains("V")){
    cout << endl << "//////////////////////////////////////////////////////" << endl;
    cout << "//////////////// Fit Parameter Limits ////////////////" << endl;
    cout << "Parameter	Min	 	Max       Guess" << endl;
    cout << "Offset:		" << offMin << "		"	<< offMax << "     " << offset << endl;
    cout << "Slope:      	" << slopeMin << "   	" << slopeMax	<< "		" << slope << endl;
    cout << "Constant:       " << minConst << "           " << maxConst << "       " << integral << endl;
    cout << "Mean:		" << fitMin << "		" << fitMax << "       " << centroid << endl;
    cout << "Sigma:		" << minWidth << "	 	" << maxWidth << "        " << fwhm << endl;
    cout << "//////////////////////////////////////////////////////" << endl << endl;;
  }

  TFitResultPtr fitPtr = histo->Fit(myFunc,opt,gopt,fitMin,fitMax);
  centroid=fitPtr->Parameter(3);
  fwhm=2.35482*fitPtr->Parameter(4);
  if(myOpt.Contains("Q")) cout << "Centroid: " << centroid << ", FWHM: " << fwhm << " (" << fwhm/centroid*100 << "%)" << endl;

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

  if(!myOpt.Contains("n")) {
      TLatex *tl=new TLatex;
      tl->SetTextSize(0.05);
      tl->SetTextColor(2);
      tl->SetNDC();
      tl->DrawLatex(0.6,0.7,Form("Centroid = %.3f",centroid));
      tl->DrawLatex(0.6,0.65,Form("Sigma = %.3f",fwhm/2.35));
      tl->DrawLatex(0.6,0.6,Form("FWHM = %.3f %%",fwhm/centroid*100));
    }

   return fitPtr;
}

TFitResultPtr fitPeak2(TH1I* histo, double fitMin, double fitMax, TString myOpt="", TString opt="QS", TString gopt="") {

  if(!myOpt.Contains("Q")) cout << endl << "Received TH1I. Cloning to a TH1F for fitting." << endl;

   TString str=histo->GetName();
   str+="_th1f";
   TH1F* hist_th1f=(TH1F*)histo->Clone(str);

   return fitPeak2(hist_th1f,fitMin,fitMax,myOpt,opt,gopt);
}

// Don't use this as this macro will typically be called within other macros and hence lead to conficts
// due to multiple help() functions.
// void help() {
//
//    cout << endl << "Fit a Gaussian plus linear background to a defined range in a histogram." << endl
//         << "Will accept either TH1F or TH1I." << endl
//         << endl << "	fitPeak2(TH1I* histo, double fitMin, double fitMax, TString myOpt, TString opt, TString gopt)" << endl
// 	<< endl << "where 'histo' is you histogram to fit and 'fitMin' and 'fitMax' define the range over which to fit," << endl
//   << "and opt is an option string. Currently myOpt='n' prevents printing of fit results on the canvas." << endl;
//
// }
//
// void Help() {
// 	help();
// }
// void usage() {
// 	help();
// }
// void Usage() {
// 	help();
// }
