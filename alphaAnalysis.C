///////////////////////////////////////////////////////////
//                                                       //
// Some useful analysis functions stolen from Chris Page //
// and modified by me (added fitAm241 function).         //
//                                                       //
// J. Brown                                              //
// 01/04/25                                              //
//                                                       //
///////////////////////////////////////////////////////////

TRandom3 rng(0);
vector<double> Am241Energies={5388,5442.80,5485.56};
vector<double> Am241Intensities={1.660,13.1,84.8};

TH1F *autoGetHist1D()
{

    TPad *current_pad = (TPad *)gROOT->GetSelectedPad();

    TIter next(current_pad->GetListOfPrimitives());
    TObject *obj;
    int iterCount = 0;
    while ((obj = next()) && iterCount < 30)
    {
        if (obj->IsA()->InheritsFrom(TH1::Class()))
        {
            return (TH1F *)obj;
        }
    }

    cout << "couldn't find a 1D histo" << endl;
    return NULL;
}


TH1F *agh()
{

    return autoGetHist1D();

}

TGraph *autoGetGraph()
{

    TPad *current_pad = (TPad *)gROOT->GetSelectedPad();

    TIter next(current_pad->GetListOfPrimitives());
    TObject *obj;
    int iterCount = 0;
    while ((obj = next()) && iterCount < 30)
    {
        if (obj->IsA()->InheritsFrom(TGraph::Class()))
        {
            return (TGraph *)obj;
        }
    }

    cout << "couldn't find a 1D histo" << endl;
    return NULL;
}

TPad *autoGetPad()
{
    return (TPad *)gROOT->GetSelectedPad();
}

TCanvas *autoGetCanvas()
{
    TPad *current_pad = (TPad *)gROOT->GetSelectedPad();
    return  (TCanvas *)current_pad->GetCanvas();


}


void logy()
{
    TPad *current_pad = (TPad *)gROOT->GetSelectedPad();
    if (current_pad != NULL)
        current_pad->SetLogy(!current_pad->GetLogy());
    else
        cout << "No current pad - skipping!" << endl;
}

void prettyH(TH1F *h = NULL)
{
    if (h == NULL)
        h = autoGetHist1D();

    h->GetXaxis()->SetLabelOffset(0.043);
    h->GetYaxis()->SetTickLength(-0.015);
    h->GetXaxis()->SetTickLength(-0.05);
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetXaxis()->SetTitleOffset(1.6);
    h->SetLineColor(kBlack);
    h->Draw();
}


void aLab(TH1F *h = NULL)
{

    if (h == NULL)
        h = autoGetHist1D();
    h->GetXaxis()->SetTitle("E#alpha [keV]");

    h->GetYaxis()->SetTitle(Form("Counts / (%.2f keV)", h->GetBinWidth(1)));
}

TF1* fitAlphas(double rangeMin, double rangeMax, std::vector<double>peaks,bool verbose = true,TH1F *h = autoGetHist1D(),bool effCorrection = false, TString fitOpt = "RN",bool noBG = false)
{
    const int noPeaks = peaks.size();
    if (noPeaks==0)
    {
        cout<<"Give me some peaks!"<<endl;
        return NULL;
    }
    double binFactor;
    double binF;

    if(h==NULL)
    {
        cout<<"Give me a histo!"<<endl;
        return NULL;
    }

    double maxArea = h->GetEntries();

    double lim_Area[2]  = {1, maxArea};            // Area
    double lim_sigma[3] = {10, 2, 200};         // sigma
    // double lim_sigma[3] = {10, 2, 20};         // sigma

    int x1 = h->GetBin(rangeMin);
    int x2 = h->GetBin(rangeMax);
    double y1 = h->GetBinContent(x1);
    double y2 = h->GetBinContent(x2);

    binFactor = h->GetBinWidth(1);

    double grad = (y2-y1)/(x2-x1);
    double offs = y2 - grad*x2;

    // grad = 0.0;
    // offs = 5.;

    TString fitFuncStr = " ";
    for(int i =0;i<noPeaks;i++)
        fitFuncStr+=Form("crystalball(x,[A%i],[M%i],[sigma],[alpha],[N]) + ",i,i);

    fitFuncStr+="[offset] ";
    TF1 *f1 = new TF1("f1", (const char*)fitFuncStr,rangeMin,rangeMax);
    f1->SetNpx(1000);

    for(int i =0;i<noPeaks;i++)
    {
        f1->SetParameter(f1->GetParNumber(Form("A%i",i)),h->GetBinContent(h->FindBin(peaks[2]))*Am241Intensities[i]/100);
        // f1->SetParameter(f1->GetParNumber(Form("A%i",i)),h->GetBinContent(h->FindBin(peaks[i])));
        f1->SetParLimits(f1->GetParNumber(Form("A%i",i)),lim_Area[0],2.*lim_Area[1]);
        f1->SetParameter(f1->GetParNumber(Form("M%i",i)),peaks[i]);
        f1->SetParLimits(f1->GetParNumber(Form("M%i",i)),peaks[i]-5,peaks[i]+5);
    }

    f1->SetParameter(f1->GetParNumber("offset"),offs);
    f1->SetParameter(f1->GetParNumber("sigma"),lim_sigma[0]);
    f1->SetParLimits(f1->GetParNumber("sigma"),lim_sigma[1],lim_sigma[2]);
    f1->SetParameter(f1->GetParNumber("alpha"),1.);
    f1->SetParLimits(f1->GetParNumber("alpha"),0.01,10.);
    f1->SetParameter(f1->GetParNumber("N"),20.);
    f1->SetParLimits(f1->GetParNumber("N"),10,100.);
    f1->SetParameter(f1->GetParNumber("offset"),0.);

    if(!verbose) fitOpt = "QRN";
    h->Fit(f1,fitOpt);

    for(int i = 0; i<noPeaks;i++)
    {
        TF1 *subFit = new TF1(Form("cb_%i",(int)f1->GetParameter(Form("M%i",i))),
                              "crystalball",rangeMin,rangeMax);
        subFit->SetParameters(f1->GetParameter(Form("A%i",i)),
                              f1->GetParameter(Form("M%i",i)),
                              f1->GetParameter("sigma"),
                              f1->GetParameter("alpha"),
                              f1->GetParameter("N"));

        subFit->SetParError(0,f1->GetParError(f1->GetParNumber(Form("A%i",i))));
        subFit->SetParError(1,f1->GetParError(f1->GetParNumber(Form("M%i",i))));
        subFit->SetParError(2,f1->GetParError(f1->GetParNumber("sigma")));
        subFit->SetParError(3,f1->GetParError(f1->GetParNumber("alpha")));
        subFit->SetParError(4,f1->GetParError(f1->GetParNumber("N")));
        subFit->SetFillStyle(0);
        subFit->SetLineColor(7);
        subFit->SetNpx(rangeMax-rangeMin);
        subFit->Draw("same");
        double efficiency = 100;
        if(effCorrection)
            efficiency = 4.3;
        if(verbose)
        cout<<i<<":  E = "<<
                f1->GetParameter(Form("M%i",i))<<" +/- "<<
                f1->GetParError(f1->GetParNumber(Form("M%i",i)))<< " keV. Area = "<<
                subFit->Integral(rangeMin,rangeMax) * 100/(efficiency*binFactor)<<" +/- "<<
                subFit->Integral(rangeMin,rangeMax) * f1->GetParError(f1->GetParNumber(Form("A%i",i))) / f1->GetParameter(Form("A%i",i))*
                100/(efficiency*binFactor)<<"\n";
    }
    f1->Draw("same");

		// cout << "Resolution: " << f1->GetParameter("sigma")*2.35 << endl;
    return f1;
}

TF1* fitAm241(double rangeMin, double rangeMax, TH1F *h = autoGetHist1D(), TString fitOpt = "RN",bool noBG = false)
{
	double rough5845 = h->GetMaximumBin();
	double roughGain = Am241Energies[2] / rough5845;
	double guess5442 = Am241Energies[1] / roughGain;
	double guess5388 = Am241Energies[0] / roughGain;

	return fitAlphas(rangeMin,rangeMax,{guess5388,guess5442,rough5845},true,h,false,"RN",false);
}

vector<double> calibrate(TH1F* h=autoGetHist1D(),double xmin=1000, double xmax=1200)
{
// vector<double> calibrate(TH1F* h, TFile *out, TDirectory *dir,int runNo) {
	// Rough estimates
	double rough5845 = h->GetMaximumBin();
	double roughGain = Am241Energies[2] / rough5845;
	double guess5442 = Am241Energies[1] / roughGain;
	double guess5388 = Am241Energies[0] / roughGain;
	cout << guess5388 << " " << guess5442 << " " << rough5845 << endl;

	TCanvas *c = new TCanvas(Form("uncal_Fitted_alpha_peaks"),Form("uncal_Fitted_alpha_peaks"));
	// TCanvas *c = new TCanvas(Form("uncal_Fitted_alpha_peaks_run_%i",runNo),Form("uncal_Fitted_alpha_peaks_run_%i",runNo));
	c->cd();
	h->Draw("hist");
	TF1* f = fitAlphas(xmin, xmax, {guess5388, guess5442, rough5845},0,h);
	// TF1* f = fitAlphas(1010, 1090, {guess5388, guess5442, rough5845},0,h);

	// Extract fit values and errors
	double fit5845 = f->GetParameter(f->GetParNumber("M2"));
	double fit5442 = f->GetParameter(f->GetParNumber("M1"));
	double fit5388 = f->GetParameter(f->GetParNumber("M0"));

	double err5845 = f->GetParError(f->GetParNumber("M0"));
	double err5442 = f->GetParError(f->GetParNumber("M1"));
	double err5388 = f->GetParError(f->GetParNumber("M2"));

	double alphaEnergies[2] = {Am241Energies[2],Am241Energies[1]};
	double fittedPositions[2] = {fit5845, fit5442};
	double positionErrors[2] = {err5845, err5442};
	// double alphaEnergies[3] = {Am241Energies[2],Am241Energies[1],Am241Energies[0]};
	// double fittedPositions[3] = {fit5845, fit5442, fit5388};
	// double positionErrors[3] = {err5845, err5442, err5388};
	// double alphaEnergies[2] = {5485.56,  5388.0};
	// double fittedPositions[2] = {fit5845,  fit5388};
	// double positionErrors[2] = {err5845,  err5388};

	TGraphErrors* calibrationGraph = new TGraphErrors(2, fittedPositions, alphaEnergies, nullptr, positionErrors);
	// TGraphErrors* calibrationGraph = new TGraphErrors(2, fittedPositions, alphaEnergies, nullptr, positionErrors);
	calibrationGraph->SetTitle("Energy Calibration;Channel;Energy (keV)");
	calibrationGraph->SetMarkerStyle(21);
	calibrationGraph->SetMarkerSize(1.0);
	calibrationGraph->SetMarkerColor(kBlue);

	TCanvas *c2 = new TCanvas(Form("calibration_canv"), Form("calibration_canv"));
	// TCanvas *c2 = new TCanvas(Form("calibration_canv_%i",runNo), Form("calibration_canv_%i",runNo));
	calibrationGraph->Draw("AP");

	TF1* linearFit = new TF1("linearFit", "pol1");
	calibrationGraph->Fit(linearFit, "");
	// calibrationGraph->Fit(linearFit, "Q");

	linearFit->SetLineColor(kRed);
	linearFit->Draw("same");
	// dir->cd();
	// out->cd();
	// dir->cd();
	// calibrationGraph->Write();
	// c->Write();
	// c2->Write();
	// cout<<linearFit->GetParameter(0)<<", "<<linearFit->GetParameter(1)<<endl;

	// cout <<
	return {linearFit->GetParameter(0), linearFit->GetParameter(1)};
}

// vector<double> calibrate(TH1F* h=autoGetHist1D()) {
// 	return calibrate(h);
// }

TH1F* ApplyCalibration(TH1F* uncalibratedHist, const vector<double>& calibrationParams) {
	if (!uncalibratedHist) {
	    std::cerr << "Error: Invalid histogram provided for calibration." << std::endl;
	    return nullptr;
	}

	if (calibrationParams.size() != 2) {
	    std::cerr << "Error: Calibration parameters must have exactly 2 values (intercept and slope)." << std::endl;
	    return nullptr;
	}

	// Extract calibration parameters
	double intercept = calibrationParams[0];
	double slope = calibrationParams[1];

	// Create a new histogram for the calibrated data
	int nBins = uncalibratedHist->GetNbinsX();
	double xMin = uncalibratedHist->GetXaxis()->GetXmin();
	double xMax = uncalibratedHist->GetXaxis()->GetXmax();

	// Calibrate x-axis limits
	double calibratedXMin = slope * xMin + intercept;
	double calibratedXMax = slope * xMax + intercept;

	TH1F* calibratedHist = new TH1F(Form("%s_calibrated", uncalibratedHist->GetName()),
	                                uncalibratedHist->GetTitle(),
	                                8000,0,8000);

	// Random number generator


	// Loop through bins of the uncalibrated histogram
	for (int bin = 1; bin <= nBins; ++bin) {
	    int nEvents = uncalibratedHist->GetBinContent(bin);
	    double binError = uncalibratedHist->GetBinError(bin);
	    double binCenter = uncalibratedHist->GetBinCenter(bin);

	    for (int i = 0; i < nEvents; ++i) {
	        double randomShift = rng.Uniform(-0.5, 0.5);
	        double calibratedValue = slope * (binCenter + randomShift) + intercept;


	        calibratedHist->Fill(calibratedValue);
	    }

	}

	prettyH(calibratedHist);
	aLab(calibratedHist);

	calibratedHist->GetXaxis()->SetRangeUser(5000, 6000);

	return calibratedHist;
}

TH1F* ApplyCalibration(TH1F* uncalibratedHist=autoGetHist1D()) {
	return ApplyCalibration(uncalibratedHist,calibrate(uncalibratedHist));
}

TF1 *extractCb(TF1 *f1,vector<double> peaksToFit = {5280, 5388, 5442, 5485.56})
{

	int noPeaks = peaksToFit.size();
	int i = noPeaks-1;
	TF1 *subFit = new TF1(Form("cb_%i",(int)f1->GetParameter(Form("M%i",i))),
	                        "crystalball",5000,6000);
	subFit->SetParameters(f1->GetParameter(Form("A%i",i)),
	                        f1->GetParameter(Form("M%i",i)),
	                        f1->GetParameter("sigma"),
	                        f1->GetParameter("alpha"),
	                        f1->GetParameter("N"));

	subFit->SetParError(0,f1->GetParError(f1->GetParNumber(Form("A%i",i))));
	subFit->SetParError(1,f1->GetParError(f1->GetParNumber(Form("M%i",i))));
	subFit->SetParError(2,f1->GetParError(f1->GetParNumber("sigma")));
	subFit->SetParError(3,f1->GetParError(f1->GetParNumber("alpha")));
	subFit->SetParError(4,f1->GetParError(f1->GetParNumber("N")));

	return subFit;

}

double FindFWHM(TF1* fit, double xmin, double mean, double xmax)
{
   // Get the maximum value of the function within the range
   double maxVal = fit->GetMaximum();
   double halfMax = maxVal / 2.0;
   // Find the roots where the function crosses half of the maximum
   double leftRoot = fit->GetX(halfMax, xmin, mean);
   double rightRoot = fit->GetX(halfMax, mean, xmax);
   // Calculate FWHM (distance between the two roots)
   return rightRoot - leftRoot;
}

double FindFWHM(double xmin=5000, double xmax=6000, TH1F* hist=autoGetHist1D())
{
   // Get the fit function within the range
	 TF1* fit=fitAm241(xmin,xmax,hist);
   // Call the real FindFWHM function
	 return FindFWHM(fit,xmin,hist->GetMaximumBin(),xmax);
}

double FindFWHMuncalibrated(double xmin, double xmax, bool useLinFit=0, TH1F* hist=autoGetHist1D())
{
	double rough5845 = hist->GetMaximumBin();
	double roughGain = Am241Energies[2] / rough5845;
	double guess5442 = Am241Energies[1] / roughGain;
	double guess5388 = Am241Energies[0] / roughGain;

	TF1* fit=fitAlphas(xmin,xmax,{guess5388,guess5442,rough5845},true,hist,false,"RN",false);
	vector<double> linFit=calibrate(hist);
	double gain=Am241Energies[2]/ fit->GetParameter(fit->GetParNumber("M2"));
	double offset=0;
	if(useLinFit) gain=linFit[1];

	return FindFWHM(fit,xmin,hist->GetMaximumBin(),xmax)*gain;
}
