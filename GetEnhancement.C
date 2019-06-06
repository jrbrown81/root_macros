Double_t GetEnhancement(TH1F* hist,Double_t fitMin,Double_t fitMax)
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
}

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

	return enh;
}
