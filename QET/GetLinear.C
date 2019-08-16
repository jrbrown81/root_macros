void GetLinear(TH1F* hist, TString fitOpt="", TString drawOpt="")
//Double_t GetEnhancement(TH1F* hist, TString fitOpt="", TString drawOpt="", Bool_t phaseShift=0)
{
	cout << "------------------------------" << endl;
	cout << "Fitting " << hist->GetName() << endl;
	cout << "------------------------------" << endl;
	
//	if(!fitOpt.Contains("0")) fitOpt+="0";
	if(!fitOpt.Contains("S")) fitOpt+="S";
//	if(!fitOpt.Contains("+")) fitOpt+="+";

	Double_t slope;
	Double_t intercept;
	Double_t slope_err;
	Double_t intercept_err;
	Double_t chi2;
	Int_t ndf;

//	TFitResultPtr fitResPtr=hist->Fit("pol1",fitOpt,"");
//	hist->Fit("pol1",fitOpt,"");
//	intercept=pol1->GetParameter(0);
//	slope=pol1->GetParameter(1);
//	chi2=pol1->GetChisquare();

//	intercept_err = pol1->GetParError(0);
//	slope_err=pol1->GetParError(1);

	TFitResultPtr fitResPtr=hist->Fit("pol0",fitOpt,"");
	intercept=fitResPtr->Parameter(0);
//	hist->Fit("pol0",fitOpt,"");
//	intercept=pol0->GetParameter(0);
	intercept_err = fitResPtr->ParError(0);
	chi2=fitResPtr->Chi2();
	ndf=fitResPtr->Ndf();


//	cout << "Slope: " << slope << " +/- " << slope_err << endl;
	cout << "Intercept: " << intercept << "  +/- " << intercept_err << endl;
	cout << "Chi squared / Ndf: " << chi2 << " / " << ndf << endl << endl;
	
	hist->Draw(drawOpt);
   
	TLatex *tl=new TLatex;
//	tl->SetTextSize(0.05);
	tl->SetTextSize(0.1);
	tl->SetTextColor(2);
	tl->SetNDC();
//	tl->DrawLatex(0.55,0.8,Form("Slope = %.3f #pm %.3f",slope,slope_err));
	tl->DrawLatex(0.55,0.8,Form("Intercept = %.3f #pm %.3f",intercept,intercept_err));
	tl->DrawLatex(0.55,0.7,Form("#chi^{2} / Ndf = %.3f / %i",chi2,ndf));
	
//	return fitResPtr;
	return;
}
