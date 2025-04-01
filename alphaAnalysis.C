///////////////////////////////////////////////////////////
//                                                       //
// Some useful analysis functions stolen from Chris Page //
// and modified by me (added fitAm241 function).         //
//                                                       //
// J. Brown                                              //
// 01/04/25                                              //
//                                                       //
///////////////////////////////////////////////////////////

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
    double lim_sigma[3] = {10, 2, 20};         // sigma


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
    // cout<<fitFuncStr<<endl;
    // if(true)
    //     return NULL;
    TF1 *f1 = new TF1("f1", (const char*)fitFuncStr,rangeMin,rangeMax);
    f1->SetNpx(1000);

    for(int i =0;i<noPeaks;i++)
    {
        f1->SetParameter(f1->GetParNumber(Form("A%i",i)),h->GetBinContent(h->FindBin(peaks[i])));
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
    return f1;
}

TF1* fitAm241(double rangeMin, double rangeMax, TH1F *h = autoGetHist1D(), TString fitOpt = "RN",bool noBG = false)
{
	double rough5845 = h->GetMaximumBin();
	double roughGain = 5485.56 / rough5845;
	double guess5442 = 5442.8 / roughGain;
	double guess5388 = 5388.0 / roughGain;

	return fitAlphas(rangeMin,rangeMax,{guess5388,guess5442,rough5845},true,h,false,"RN",false);
}
