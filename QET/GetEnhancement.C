#include "TFitResult.h"
#include "TFitResultPtr.h"

/*Double_t GetEnhancement(TH1F* hist,Double_t fitMin,Double_t fitMax)
{
	TF1* cos2phi=new TF1("cos2phi","[0]*cos(2*x*TMath::DegToRad())+[1]",-180,180);
	Double_t amp;
	Double_t off;
	Double_t enh;

	hist->Fit(cos2phi,"L","",fitMin,fitMax);
	amp=cos2phi->GetParameter(0);
	off=cos2phi->GetParameter(1);

	enh=(-amp+off)/(amp+off);

	return enh;
}*/
// Bool_t reject=1;
// Double_t cos2phiReject(double* x, double* par)
// {
// 	if(reject && x[0]>-30 && x[0]<30) {
// 		TF1::RejectPoint();
// 		return 0;
// 	}
// 	Double_t f = par[0]*cos(2*(x[0]+par[2])*TMath::DegToRad())+par[1];
// 	return f;
//
// }

TFitResultPtr GetEnhancement(TH1F* hist, TString opt="", TString fitOpt="WLIR", TString drawOpt="", Bool_t phaseShift=0, Double_t xmin=0, Double_t xmax=0)
{
	if(!opt.Contains("Q")) {
		cout << "------------------------------" << endl;
		cout << "Fitting cos(2phi) to " << hist->GetName() << endl;
		cout << "------------------------------" << endl;
	}

//	if(!fitOpt.Contains("0")) fitOpt+="0";
	if(!fitOpt.Contains("S")) fitOpt+="S"; // need this so can return the FitResultPtr
	if(xmin==xmax && !fitOpt.Contains("R")) fitOpt+="R"; // need this so the fit is only performed on the range of the function, i.e. -180 to 180 degrees
	if(opt.Contains("Q")) fitOpt+="Q";
//	if(!fitOpt.Contains("+")) fitOpt+="+";

	TF1* cos2phi=new TF1("cos2phi","[0]*cos(2*(x+[2])*TMath::DegToRad())+[1]",-180,180);
	// TF1* cos2phi=new TF1("cos2phi",cos2phiReject,-180,180,3);
   cos2phi->SetParName(0,"Amplitude");
   cos2phi->SetParName(1,"Offset");
   cos2phi->SetParName(2,"Phase Shift");

	Double_t amp;
	Double_t off;
	// Double_t amp_c;
	// Double_t enh_c;
	Double_t phase;
	Double_t enh;
	Double_t chi2;
	Int_t ndf;

   Double_t mean=hist->Integral()/hist->GetNbinsX();

   cos2phi->SetParameter(0,0);
   cos2phi->SetParameter(1,mean);
   cos2phi->SetParameter(2,0);
	if(phaseShift==1) cos2phi->SetParLimits(0,-1e6,0);
   if(phaseShift==0) cos2phi->FixParameter(2,0);
	TFitResultPtr fitResPtr;
	fitResPtr=hist->Fit(cos2phi,fitOpt,"",xmin,xmax);
	amp=cos2phi->GetParameter(0);
	off=cos2phi->GetParameter(1);
	phase=cos2phi->GetParameter(2);
	chi2=cos2phi->GetChisquare();
	ndf=fitResPtr->Ndf();

	enh=(-amp+off)/(amp+off);

	Double_t amp_err;
	Double_t off_err;
	Double_t phase_err;
	Double_t enh_err;
	// Double_t amp_c_err;
	// Double_t enh_c_err;

	// Double_t damp;
	// damp=hist->GetXaxis()->GetBinWidth(1);
	// damp=damp*TMath::DegToRad();
	// damp=TMath::Sin(damp)/damp;
	// amp_c=amp/damp;
	// enh_c=(-amp_c+off)/(amp_c+off);

	amp_err = cos2phi->GetParError(0);
	// amp_c_err = amp_err/damp;
	off_err=cos2phi->GetParError(1);
	phase_err=cos2phi->GetParError(2);

	enh_err = 2/pow((off+amp),2)*sqrt(amp*amp*off_err*off_err+off*off*amp_err*amp_err);
	// enh_c_err = 2/pow((off+amp_c),2)*sqrt(amp_c*amp_c*off_err*off_err+off*off*amp_c_err*amp_c_err);

	if(!opt.Contains("Q")) {
		cout << "Amplitde: " << amp << " +/- " << amp_err << endl
			<< "Offset: " << off << "  +/- " << off_err << endl;
		if(phaseShift==1) cout << "Phase Shift: " << phase << " +/- " << phase_err << endl;
		cout << "Enhancement: " << enh << " +/- " << enh_err << endl
			// << "Damping factor: " << damp << endl
			// << "Corrected Enhancement: " << enh_c << " +/- " << enh_c_err << endl
			// << "Corrected Enhancement: " << enh/damp << " +/- " << enh_err/damp << endl
			<< "Chi squared / Ndf: " << chi2 << " / " << ndf << endl << endl;
	}
	hist->Draw(drawOpt);

	TLatex *tl=new TLatex;
	tl->SetTextSize(0.05);
//	tl->SetTextSize(0.1);
	tl->SetTextColor(2);
	tl->SetNDC();
//	tl->DrawLatex(0.55,0.8,Form("Enh. = %.3f #pm %.3f",enh,enh_err));
	// tl->DrawLatex(0.55,0.8,Form("Enh._{corr.} = %.3f #pm %.3f",enh/damp,enh_err/damp));
	// tl->DrawLatex(0.55,0.8,Form("Enh._{corr.} = %.3f #pm %.3f",enh_c,enh_c_err));
	// tl->DrawLatex(0.55,0.8,Form("R_{corr.} = %.3f #pm %.3f",enh_c,enh_c_err));
	tl->DrawLatex(0.55,0.8,Form("R = %.3f #pm %.3f",enh,enh_err));
	tl->DrawLatex(0.55,0.7,Form("#chi^{2} / Ndf = %.3f / %i",chi2,ndf));
   if(phaseShift==1) tl->DrawLatex(0.55,0.6,Form("Ph. shift = %.3f #pm %.3f",phase,phase_err));

//	return enh;
	return fitResPtr;
}
