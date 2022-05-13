// Code to fit peaks to Cs137 + Eu152 spectra
// To run:
//   - create/or load a histogram,
//   - load the code with .L findAndFit.C
//   - execute findAndFit(histoName,fitMin,fitMax), where fitMin/Max and the range with the 662 keV peak in

#include <TF1.h>
#include <TH1.h>
#include <TMath.h>

void linearityFits(TH1F* histo, double fitMin, double fitMax, int nToFit=10, double thresh=0.001)
{
	if(nToFit>10) nToFit=10;

	TCanvas* lin_c=new TCanvas("lin_c","Linearity fits",0,0,800,800);

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

	lin_c->cd(1);
	gPad->SetLogy(1);
	histo->Draw();
	TSpectrum* spec=new TSpectrum();
	histo->GetXaxis()->UnZoom();
    Int_t nFound=spec->Search(histo,sigma,"",thresh);

 	Double_t* xPos=spec->GetPositionX();	// array of peaks found, ordered by amplitude
	Double_t energy[10]={1408,1100.87,964.1,778.9,662.1,344.2,244.7,121.7,41.075,32.061};
	Double_t chn[10];
	Double_t xMax=0;
	Double_t low=0, high=8192;
	Int_t counter=0;
	Double_t ordered[nFound];	// array of peaks found re-ordered by energy (largest first)
	Int_t index[nFound];

	Double_t toUse[nFound];		// array of peaks to use for calibration and linearity correction
	Double_t EtoUse[nFound];
	char answer;
	int	fit=0;
	int nUsed=0;

	TMath::Sort(nFound,xPos,index);

	for(int i=0;i<nFound;i++) ordered[i]=xPos[index[i]];
	cout << endl << "First guess at peaks to use for calibration: " << endl;
	cout << "Energy (keV)	Channel" << endl;
	for(int i=0;i<10;i++) cout << energy[i] << "		" << ordered[i] << endl;

	histo->GetXaxis()->SetRangeUser(0,ordered[0]*1.1);
	gPad->Update();
	cout << gPad->GetUymax() << " " << pow(10,gPad->GetUymax()) << endl;
	double min=pow(10,gPad->GetUymin());
	double max=pow(10,gPad->GetUymax());
	gPad->Update();
	TLine* tl=new TLine(0,0,1,1);

	cout << "Accept these peaks (y/n/q): ";
	cin >> answer;

	while(answer!='q') {
		if(answer=='y') {
			cout << "Using these peaks..." << endl;
			answer='q';
		}
		else if(answer=='n') {
			cout << "Which peaks to use..." << endl;
			tl->SetLineColor(2);
			tl->SetLineStyle(2);
			tl->SetLineWidth(2);

			for(int i=0; i<10; i++)
			{
				cout << "Use " << energy[i] << " keV peak? (y/n): ";
				cin >> answer;
				if(answer=='n') continue;
				else if(answer=='y') {
					if(fit>=nFound) {
						cout << "No peaks left. Exiting." << endl;
						break;
					}
					while(answer!='q') {
						tl->DrawLine(ordered[fit],min,ordered[fit],max);
						gPad->Update();
						cout << "	Use: " << ordered[fit] <<" ? (y/n): ";
						cin >> answer;
						if(answer=='y') {
							toUse[nUsed]=ordered[fit];
							EtoUse[nUsed]=energy[i];
							fit++;
							nUsed++;
							break;
						}
						else if(answer=='n') {
							fit++;
							continue;
						}
						else if(answer=='q') break;
						else cout << "try again" << endl;
					}
					if(answer=='q') break;
					else continue;
				}
				else if(answer=='q') break;
			}

			cout << endl << "New peaks to use for calibration: " << endl;
			cout << "Energy (keV)	Channel" << endl;
			for(int i=0;i<nUsed;i++) cout << EtoUse[i] << "		" << toUse[i] << endl;
			break;
		}
		else {
			cout << "Try again (y/n): ";
			cin >> answer;
		}
	}
	if(answer=='q') return;

	lin_c->Clear();
	lin_c->Divide(1,3);

 	lin_c->cd(1);
	histo->GetXaxis()->SetRangeUser(fitMin*0.75,fitMax*1.25);
	histo->DrawCopy();

	lin_c->cd(2);
	gPad->SetLogy(1);
	histo->GetXaxis()->SetRangeUser(0,ordered[0]*1.1);
	histo->DrawCopy();
	for(int i=0;i<nUsed;i++) tl->DrawLine(toUse[i],min,toUse[i],max);

	TGraph *gr;
	if(nUsed==0) gr=new TGraph(nToFit,ordered,energy);
	else gr=new TGraph(nUsed,toUse,EtoUse);
	lin_c->cd(3);
	if(nUsed==0) gr->GetXaxis()->SetLimits(0,ordered[0]*1.1);
	else gr->GetXaxis()->SetLimits(0,toUse[0]*1.1);
	gr->Draw("a*");
	if(nUsed==0) gr->GetXaxis()->SetRangeUser(0,ordered[0]*1.1);
	else gr->GetXaxis()->SetRangeUser(0,toUse[0]*1.1);
	gr->GetYaxis()->SetRangeUser(0,1500);
	gr->SetTitle("Calibration");
	gr->GetXaxis()->SetTitle("channel");
	gr->GetYaxis()->SetTitle("energy (keV)");
	TF1* linFunc = new TF1("linFunc","pol2",0,8192);
	gr->Fit("linFunc","q");
	gr->Draw("a*");

	gPad->Update();

	Double_t FWHM=2.35*sigma*(linFunc->GetParameter(1)+2*linFunc->GetParameter(2)*cent)*100/energy[4];
	cout << endl << "Linearity corrected resolution: " << FWHM << " %" << endl;

	// TString name=histo->GetName();
	TString name=histo->GetTitle();

	cout << "Save result to file '" << name << "_linearity.dat' and " << name << "_linearity.pdf'? (y/n): ";
	answer=0;
	while(answer!='n') {
		cin >> answer;
		if(answer=='y') {
			ofstream out(name+"_linearity.dat");
			cout << endl << "Writing results to file '" << name << "_linearity.dat'" << endl;
			out << "FWHM (% of 662keV)" << endl << FWHM << endl;
			out << "Energy (keV)	Channel" << endl;
			if(nUsed==0) for(int i=0;i<nToFit;i++) out << energy[i] << "		" << ordered[i] << endl;
			else for(int i=0;i<nUsed;i++) out << EtoUse[i] << "		" << toUse[i] << endl;
			out.close();

			lin_c->SaveAs(name+"_linearity.pdf");
			break;
		}
		cout << "Try again. Save results? (y/n): ";
	}
}


void linearityFits(TH1I* histo, double fitMin, double fitMax, int nToFit=10, double thresh=0.001)
{
	cout << endl << "Received TH1I. Cloning to a TH1F for fitting." << endl;

	TString str=histo->GetName();
	str+="_th1f";
	TH1F* hist_th1f=(TH1F*)histo->Clone(str);

	return linearityFits(hist_th1f,fitMin,fitMax,nToFit,thresh);
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
