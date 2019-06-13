/*Double_t GetEnhancement(TH1F* hist,Double_t fitMin,Double_t fitMax)
{
	TF1* cos2phi=new TF1("cos2phi","[0]*cos(2*x*TMath::Pi()/180)+[1]",-180,180);
	Double_t amp;
	Double_t off;
	Double_t enh;

	hist->Fit(cos2phi,"L","",fitMin,fitMax);
	amp=cos2phi->GetParameter(0);
	off=cos2phi->GetParameter(1);

	enh=(-amp+off)/(amp+off);

	return enh;
}*/

Double_t GetEnhancement(TH1F* hist)
{
	TF1* cos2phi=new TF1("cos2phi","[0]*cos(2*x*TMath::Pi()/180)+[1]",-180,180);
	Double_t amp;
	Double_t off;
	Double_t enh;

	hist->Fit(cos2phi,"L","");
	amp=cos2phi->GetParameter(0);
	off=cos2phi->GetParameter(1);

	enh=(-amp+off)/(amp+off);

	Double_t amp_err;
	Double_t off_err;
	Double_t enh_err;
   
	Double_t damp;
   
	damp=hist->GetXaxis()->GetBinWidth(1);
	damp=damp*TMath::Pi()/180;
	damp=TMath::Sin(damp)/damp;

	amp_err = cos2phi->GetParError(0);
	off_err=cos2phi->GetParError(1);
 
	enh_err = 2/pow((off+amp),2)*sqrt(amp*amp*off_err*off_err+off*off*amp_err*amp_err);

	cout << "Amplitde: " << amp << " +/- " << amp_err << endl
		<< "Offset: " << off << "  +/- " << off_err << endl
		<< "Enhancement: " << enh << " +/- " << enh_err << endl
		<< "Damping factor: " << damp << endl
		<< "Corrected Enhancement: " << enh/damp << " +/- " << enh_err/damp << endl;

	return enh;
}
