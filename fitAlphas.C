////////////////////////////////////////////////////
//                                                //
// Code to fit multiple Gausians to a histogram   //
// over the specified range.                      //
//                                                //
// Modified from fitPeaks.C                       //
//                                                //
// J. Brown                                       //
// 14/05/24                                       //
//                                                //
// This doesn't work particularly well.           //
// Use alphaAnalysis.C instead.                   //
////////////////////////////////////////////////////

#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"

////////////////////
// Alpha energies //
////////////////////
// Cm244
// 5804 keV 76.9%
// 5763 keV 23.1%
// Th230
// 4687 keV 76.3%
// 4621 keV 23.4%
// Gd148
// 3183 keV 100%
////////////////////

double fitAs(TH1F* histo, double fitMin, double fitMax, int nPeaks, TString myOpt="", TString opt="", TString gopt="")
{

//	ofstream out("output.dat");
	if(nPeaks<1 || nPeaks>3) {
		cout << "nPeaks must be between 1 and 3. Exiting." << endl;
		return 0;
	}

  if(myOpt.Contains("Q")) opt+="Q";

	TCanvas* alphaFits_c = new TCanvas("alphaFits_c","Alpha Fits");
	alphaFits_c->cd();
	histo->SetStats(0);

  // Centroid
  vector<double> centroid;
	// centroid.push_back((fitMax+fitMin)/2);
	for(int i=0;i<3;i++) {
		if(i<nPeaks) {
			float k=1./(2*nPeaks)+static_cast<float>(i)/nPeaks;
			centroid.push_back(fitMin+k*(fitMax-fitMin));
		} else centroid.push_back(fitMin+0.5*(fitMax-fitMin));
	}

  Int_t minFitBin=(Int_t)histo->GetXaxis()->FindBin(fitMin);
  Int_t maxFitBin=(Int_t)histo->GetXaxis()->FindBin(fitMax);
	// Width
	double range=fitMax-fitMin;
	double fwhm=range/4;
	double maxWidth=range;
	double minWidth=range/100;
  // Constant
	double integral=(double)histo->Integral(minFitBin,maxFitBin);
	// vector<double> integral;
	// for(int i=0;i<3;i++) {
	// 	if(i<nPeaks) integral.push_back((double)histo->Integral(minFitBin,maxFitBin));
	// 	else integral.push_back(0);
	// }
  vector<double> maxConst;
  vector<double> minConst;
  vector<double> area;
	double fact=pow(2*TMath::Pi(),0.5);
	for(int i=0;i<3;i++) {
		if(i<nPeaks) {
			// area.push_back(integral/(2*nPeaks));
			// maxConst.push_back(2*integral/pow(10,nPeaks-1-i));
			// minConst.push_back(integral/1000);
			area.push_back(integral/pow(10,nPeaks-1-i)/(fwhm*fact));
			minConst.push_back(area[i]/10);
			maxConst.push_back(area[i]*10);
		}
		else {
			area.push_back(0);
			minConst.push_back(0);
			maxConst.push_back(0);
		}
	}

  // Slope
  double slope = (histo->GetBinContent(maxFitBin)-histo->GetBinContent(minFitBin))/(fitMax-fitMin);
  double slopeMin = -2*abs(slope);
  double slopeMax = 2*abs(slope);;
  // Offset
  double offset = histo->GetBinContent(maxFitBin) - slope*fitMax;
  double offMin=offset*-0.5;
  double offMax=offset*2;

  // Fit Function
  // TF1* myFunc = new TF1("myFunc","[0]+[1]*x+[3]*exp(-(x-[4])**2/(2*[2]**2))",0,4095);
	// myFunc->SetParNames("Offset","Slope","Sigma","Constant1","Mean1");
  // TF1* myFunc = new TF1("myFunc","[0]+[1]*x+[3]*exp(-(x-[4])**2/(2*[2]**2))+[5]*exp(-(x-[6])**2/(2*[2]**2))+[7]*exp(-(x-[8])**2/(2*[2]**2))",0,4095);
  TF1* myFunc = new TF1("myFunc","[0]+[1]*x+[3]*exp(-(x-[4])**2/(2*[2]**2))+[5]*exp(-(x-[6])**2/(2*[2]**2))+[7]*exp(-(x-[8])**2/(2*[2]**2))",fitMin,fitMax);
  myFunc->SetParNames("Offset","Slope","Sigma","Constant1","Mean1","Constant2","Mean2","Constant3","Mean3");
	myFunc->SetTitle("Combined fit");
  if(myOpt.Contains("B")) {
    offset=0;
    slope=0;
    myFunc->FixParameter(0,offset);
    myFunc->FixParameter(1,slope);

		cout << "myOpt 'B' requested so fitting with no linear background" << endl;
  } else {
    myFunc->SetParLimits(0,offMin,offMax);
    myFunc->SetParLimits(1,slopeMin,slopeMax);
  }
	// old
	// myFunc->SetParLimits(2,minWidth,maxWidth);
  // myFunc->SetParLimits(3,minConst[0],maxConst[0]);
  // myFunc->SetParLimits(4,fitMin,fitMax);
	// new
	myFunc->SetParLimits(2,minWidth,maxWidth);
  myFunc->SetParLimits(3,minConst[0],maxConst[0]);
  myFunc->SetParLimits(4,fitMin,fitMax);
  myFunc->SetParLimits(5,minConst[1],maxConst[1]);
  myFunc->SetParLimits(6,fitMin,fitMax);
  myFunc->SetParLimits(7,minConst[2],maxConst[2]);
  myFunc->SetParLimits(8,fitMin,fitMax);
	for(int i=0;i<(3-nPeaks);i++) {
		myFunc->FixParameter(7-2*i,0);
	}
  // myFunc->SetParameters(offset,slope,integral/2,centroid.at(0),fwhm);
  // myFunc->SetParameters(offset,slope,integral/(2*nPeaks),centroid.at(0),fwhm,integral/(2*nPeaks),centroid.at(1),fwhm,integral/(2*nPeaks),centroid.at(2),fwhm);
	// new
  myFunc->SetParameters(offset,slope,fwhm,area[0],centroid.at(0),area[1],centroid.at(1),area[2],centroid.at(2));
	// old
	// myFunc->SetParameters(offset,slope,fwhm,area[0],centroid[0]);

  if(!myOpt.Contains("Q")) {
    cout << endl << "//////////////////////////////////////////////////////" << endl;
    cout << "//////////////// Fit Parameter Limits ////////////////" << endl;
    cout << "Parameter	Min	 	Max     	 Guess" << endl;
  	cout << "Offset:		" << offMin << "		"	<< offMax << "     " << offset << endl;
    cout << "Slope:      	" << slopeMin << "   	" << slopeMax	<< "		" << slope << endl;
		cout << "Sigma:		" << minWidth << "	 	" << maxWidth << "	 	    " << fwhm << endl;
		for(int i=0;i<nPeaks;i++) {
    	cout << "Constant" << i+1 << " :       " << minConst[i] << "          " << maxConst[i] << "	       " << area[i] << endl;
    	cout << "Mean" << i+1 << " :		" << fitMin << "		" << fitMax << "   		" << centroid[i] << endl;
		}
    cout << "//////////////////////////////////////////////////////" << endl << endl;;
  }

  histo->Fit(myFunc,opt,gopt,fitMin,fitMax);
  // centroid=myFunc->GetParameter(3);
  centroid[0]=myFunc->GetParameter(4);
  centroid[1]=myFunc->GetParameter(6);
  centroid[2]=myFunc->GetParameter(8);
  fwhm=2.35*myFunc->GetParameter(2);
  // area=myFunc->GetParameter(2)*myFunc->GetParameter(4)/histo->GetBinWidth(1)*pow(2*TMath::Pi(),0.5);
  // area[0]=myFunc->GetParameter(3)*myFunc->GetParameter(2)/histo->GetBinWidth(1)*pow(2*TMath::Pi(),0.5);
  area[0]=myFunc->GetParameter(3)*myFunc->GetParameter(2)/histo->GetBinWidth(1)*fact;
  area[1]=myFunc->GetParameter(5)*myFunc->GetParameter(2)/histo->GetBinWidth(1)*fact;
  area[2]=myFunc->GetParameter(7)*myFunc->GetParameter(2)/histo->GetBinWidth(1)*fact;
  // if(!myOpt.Contains("Q")) cout << "Centroid: " << centroid.at(0) << ", FWHM: " << fwhm << " (" << fwhm/centroid.at(0)*100 << "%), " << "Peak Area: " << area << endl;

 if(!myOpt.Contains("Q")) cout << "Centroid: " << centroid.at(nPeaks-1) << ", FWHM: " << fwhm << " (" << fwhm/centroid.at(nPeaks-1)*100 << "%), " << "Peak Area: " << area.at(nPeaks-1) << endl;

// Draw the histogram, fit and deconvolution
	// histo->GetXaxis()->SetRangeUser(fitMin*0.75,fitMax*1.25);
	histo->GetXaxis()->SetRangeUser(fitMin*0.90,fitMax*1.10);
	histo->DrawClone(gopt);
	myFunc->Draw("same");

	TF1* f1;
	if(!myOpt.Contains("B")) {
		f1=new TF1("f1","pol1",fitMin,fitMax);
		f1->SetParameters(myFunc->GetParameter(0),myFunc->GetParameter(1));
		f1->SetLineColor(3);
		f1->Draw("same");
	}

	// TF1* f2=new TF1("f2","gaus",fitMin,fitMax);
	// f2->SetParameters(myFunc->GetParameter(2),myFunc->GetParameter(3),myFunc->GetParameter(4));
	// f2->SetLineColor(6);
	// f2->Draw("same");
	TF1* f2[3];
	for(int i=0;i<3;i++) {
		f2[i]=new TF1(Form("f%i",i+2),"gaus",fitMin,fitMax);
		f2[i]->SetTitle(Form("gaus %i",i+1));
		f2[i]->SetParameters(myFunc->GetParameter(3+2*i),myFunc->GetParameter(4+2*i),myFunc->GetParameter(2));
		f2[i]->SetLineColor(i+6);
		if(i<nPeaks) f2[i]->Draw("same");
	}

	alphaFits_c->BuildLegend(0.70,0.60,0.90,0.90);

// Draw fit parameters on canvas
	if(!myOpt.Contains("N")) {
    TLatex *tl=new TLatex;
    tl->SetTextSize(0.05);
    tl->SetTextColor(nPeaks+5);
    tl->SetNDC();
		tl->DrawLatex(0.15,0.85,Form("Gaussian: %i",nPeaks));
    tl->DrawLatex(0.15,0.80,Form("Centroid = %.3f",centroid.at(nPeaks-1)));
    tl->DrawLatex(0.15,0.75,Form("Sigma = %.3f",fwhm/2.35));
    tl->DrawLatex(0.15,0.70,Form("FWHM = %.3f %%",fwhm/centroid.at(nPeaks-1)*100));
    tl->DrawLatex(0.15,0.65,Form("Area= %.3f",area.at(nPeaks-1)));
  }

   return centroid.at(nPeaks-1);
}

double fitAlphas(TH1F* histoIn, double fitMin, double fitMax, int nPeaks, TString myOpt="", TString opt="", TString gopt="") {

   if(!myOpt.Contains("Q")) cout << endl << "Received TH1F. Cloning to a TH1F for fitting." << endl;

   TString str=histoIn->GetName();
   str+="_4fitting";
   TH1F* hist_4fit=(TH1F*)histoIn->Clone(str);

   return fitAs(hist_4fit,fitMin,fitMax,nPeaks,myOpt,opt,gopt);
}

double fitAlphas(TH1I* histoIn, double fitMin, double fitMax, int nPeaks, TString myOpt="", TString opt="", TString gopt="") {

   if(!myOpt.Contains("Q")) cout << endl << "Received TH1I. Cloning to a TH1F for fitting." << endl;

   TString str=histoIn->GetName();
   str+="_4fitting";
   TH1F* hist_4fit=(TH1F*)histoIn->Clone(str);

   return fitAs(hist_4fit,fitMin,fitMax,nPeaks,myOpt,opt,gopt);
}

double fitAlphas(TH1D* histoIn, double fitMin, double fitMax, int nPeaks, TString myOpt="", TString opt="", TString gopt="") {

   if(!myOpt.Contains("Q")) cout << endl << "Received TH1D. Cloning to a TH1F for fitting." << endl;

   TString str=histoIn->GetName();
   str+="_4fitting";
   TH1F* hist_4fit=(TH1F*)histoIn->Clone(str);

   return fitAs(hist_4fit,fitMin,fitMax,nPeaks,myOpt,opt,gopt);
}

void help_fitAlphas() {

	cout << "Coming soon" << endl;
  //  cout << endl << "Fit a Gaussian plus linear background to a defined range in a histogram." << endl
  //       << "Will accept either TH1F or TH1I." << endl
  //       << endl << "	fitPeak(TH1I* histo, double fitMin, double fitMax, TString myOpt, TString opt, TString gopt)" << endl
	// << endl << "where 'histo' is your histogram to fit and 'fitMin' and 'fitMax' define the range over which to fit," << endl
  // << "and myOpt is an option string." << endl
  // << "'N' - prevents printing of fit results on the canvas," << endl
  // << "'Q' - suppresses printing fit results to the command line," << endl
  // << "'B' - forces no background under the Gaussian." << endl
  // << "'opt' and 'gopt' are passed directly to the 'Fit' function." << endl;

}

void Help_fitAlphas() {
	help_fitAlphas();
}
void usage_fitAlphas() {
	help_fitAlphas();
}
void Usage_fitAlphas() {
	help_fitAlphas();
}
