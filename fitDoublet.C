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
#include "TLatex.h"

double fitDoublet(TH1F* histo, double fitMin, double fitMax, TString myOpt="", TString opt="", TString gopt="", double secondary=0) {

//	ofstream out("output.dat");

  if(myOpt.Contains("Q")) opt+="Q";

  // Centroid
  double centroid=(fitMax+fitMin)/2;
  Int_t minFitBin=(Int_t)histo->GetXaxis()->FindBin(fitMin);
  Int_t maxFitBin=(Int_t)histo->GetXaxis()->FindBin(fitMax);
  // Constant
  double integral=(double)histo->Integral(minFitBin,maxFitBin);
  double maxConst=integral*2;
  double minConst=integral/1000;
  double area, area2;
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
  TF1* myFunc;
  if(myOpt.Contains("D")) {
    myFunc = new TF1("myFunc","[0]+[1]*x+[2]*exp(-(x-[3])**2/(2*[4]**2))+[1]*x+[5]*exp(-(x-[6])**2/(2*[4]**2))",0,4095);
    myFunc->SetParNames("Offset","Slope","Constant","Mean","Sigma","Constant2","Mean2");
  } else {
    myFunc = new TF1("myFunc","[0]+[1]*x+[2]*exp(-(x-[3])**2/(2*[4]**2))",0,4095);
    myFunc->SetParNames("Offset","Slope","Constant","Mean","Sigma");
  }
  if(myOpt.Contains("B")) {
    offset=0;
    slope=0;
    myFunc->FixParameter(0,offset);
    myFunc->FixParameter(1,slope);
  } else {
    myFunc->SetParLimits(0,offMin,offMax);
    myFunc->SetParLimits(1,slopeMin,slopeMax);
  }
  myFunc->SetParLimits(2,minConst,maxConst);
  myFunc->SetParLimits(3,fitMin,fitMax);
  myFunc->SetParLimits(4,minWidth,maxWidth);
  if(myOpt.Contains("D")) {
    myFunc->SetParLimits(5,0,maxConst);
    myFunc->SetParLimits(6,fitMin,fitMax);
    if(secondary!=0) myFunc->SetParameters(offset,slope,integral/4,centroid,fwhm,integral/4,secondary);
    else myFunc->SetParameters(offset,slope,integral/4,(4*fitMin+fitMax)/3,fwhm,integral/4,(5*fitMin+2*fitMax)/3);
  }
  else myFunc->SetParameters(offset,slope,integral/2,centroid,fwhm);


  if(!myOpt.Contains("Q")) {
    cout << endl << "//////////////////////////////////////////////////////" << endl;
    cout << "//////////////// Fit Parameter Limits ////////////////" << endl;
    cout << "Parameter	Min	 	Max     	 Guess" << endl;
  	cout << "Offset:		" << offMin << "		"	<< offMax << "     " << offset << endl;
    cout << "Slope:      	" << slopeMin << "   	" << slopeMax	<< "		" << slope << endl;
    cout << "Constant:       " << minConst << "          " << maxConst << "	       " << myFunc->GetParameter(2) << endl;
    // cout << "Constant:       " << minConst << "          " << maxConst << "	       " << integral/2 << endl;
    cout << "Mean:		" << fitMin << "		" << fitMax << "   		" << myFunc->GetParameter(3) << endl;
    // cout << "Mean:		" << fitMin << "		" << fitMax << "   		" << centroid << endl;
    cout << "Sigma:		" << minWidth << "	 	" << maxWidth << "	 	    " << fwhm << endl;
    if(myOpt.Contains("D")) {
      cout << "Constant2:       " << minConst << "          " << maxConst << "	       " << myFunc->GetParameter(5) << endl;
      cout << "Mean2:		" << fitMin << "		" << fitMax << "   		" << myFunc->GetParameter(6) << endl;
    }
    cout << "//////////////////////////////////////////////////////" << endl << endl;;
  }

  histo->Fit(myFunc,opt,gopt,fitMin,fitMax);
  centroid=myFunc->GetParameter(3);
  fwhm=2.35*myFunc->GetParameter(4);
  area=myFunc->GetParameter(2)*myFunc->GetParameter(4)/histo->GetBinWidth(1)*pow(2*TMath::Pi(),0.5);
  if(myOpt.Contains("D")) {
    secondary=myFunc->GetParameter(6);
    area2=myFunc->GetParameter(5)*myFunc->GetParameter(4)/histo->GetBinWidth(1)*pow(2*TMath::Pi(),0.5);
  }
  if(!myOpt.Contains("Q")) {
    if(myOpt.Contains("D")) {
        cout << "Centroid 1: " << centroid << ", FWHM: " << fwhm << " (" << fwhm/centroid*100 << "%), " << "Peak Area1: " << area << endl;
        cout << "Centroid 2: " << secondary << ", FWHM: " << fwhm << " (" << fwhm/secondary*100 << "%), " << "Peak Area 2: " << area2 << endl;
      }
    else cout << "Centroid: " << centroid << ", FWHM: " << fwhm << " (" << fwhm/centroid*100 << "%), " << "Peak Area: " << area << endl;
  }

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

  if(myOpt.Contains("D")) {
    TF1* f3=new TF1("f3","gaus",fitMin,fitMax);
  	f3->SetParameters(myFunc->GetParameter(5),myFunc->GetParameter(6),myFunc->GetParameter(4));
  	f3->SetLineColor(7);
  	f3->Draw("same");
  }

if(!myOpt.Contains("N")) {
    TLatex *tl=new TLatex;
    tl->SetTextSize(0.05);
    tl->SetTextColor(2);
    tl->SetNDC();
    if(myOpt.Contains("D")) {
      tl->DrawLatex(0.6,0.73,Form("Centroid 1= %.3f",centroid));
      tl->DrawLatex(0.6,0.68,Form("Centroid 2= %.3f",secondary));
    } else tl->DrawLatex(0.6,0.68,Form("Centroid = %.3f",centroid));
    tl->DrawLatex(0.6,0.63,Form("Sigma = %.3f",fwhm/2.35));
    tl->DrawLatex(0.6,0.58,Form("FWHM = %.3f %%",fwhm/centroid*100));
    tl->DrawLatex(0.6,0.53,Form("Area= %.3f",area));
    if(myOpt.Contains("D")) tl->DrawLatex(0.6,0.48,Form("Area 2= %.3f",area2));
  }

   return centroid;
}

double fitDoublet(TH1I* histo, double fitMin, double fitMax, TString myOpt="", TString opt="", TString gopt="", double secondary=0) {

   if(!myOpt.Contains("Q")) cout << endl << "Received TH1I. Cloning to a TH1F for fitting." << endl;

   TString str=histo->GetName();
   str+="_th1f";
   TH1F* hist_th1f=(TH1F*)histo->Clone(str);

   return fitDoublet(hist_th1f,fitMin,fitMax,myOpt,opt,gopt,secondary);
}

double fitDoublet(TH1D* histo, double fitMin, double fitMax, TString myOpt="", TString opt="", TString gopt="", double secondary=0) {

   if(!myOpt.Contains("Q")) cout << endl << "Received TH1D. Cloning to a TH1F for fitting." << endl;

   TString str=histo->GetName();
   str+="_th1f";
   TH1F* hist_th1f=(TH1F*)histo->Clone(str);

   return fitDoublet(hist_th1f,fitMin,fitMax,myOpt,opt,gopt,secondary);
}

void help() {

   cout << endl << "Fit a Gaussian plus linear background to a defined range in a histogram." << endl
        << "Will accept either TH1F or TH1I." << endl
        << endl << "	fitDoublet(TH1I* histo, double fitMin, double fitMax, TString myOpt, TString opt, TString gopt, double secondary)" << endl
	<< endl << "where 'histo' is your histogram to fit and 'fitMin' and 'fitMax' define the range over which to fit," << endl
  << "and myOpt is an option string." << endl
  << "'N' - prevents printing of fit results on the canvas," << endl
  << "'Q' - suppresses printing fit results to the command line," << endl
  << "'B' - forces no background under the Gaussian." << endl
  << "'D' - fits a doublet rather than a single Gaussian, with the approximate centroid (optionally) specifed with 'secondary'." << endl
  << "'opt' and 'gopt' are passed directly to the 'Fit' function." << endl;

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
