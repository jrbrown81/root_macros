////////////////////////////////////////////////////
// Some useful functions borrowed from Chris Page //
// J. Brown                                       //
// 16/04/25                                       //
////////////////////////////////////////////////////

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
		TCanvas* newCanvas;
		if(!current_pad) {
			cout << "couldn't find a convas" << endl;
			return NULL;
		}
    return  (TCanvas *)current_pad->GetCanvas();
}

void logx()
{
    TPad *current_pad = (TPad *)gROOT->GetSelectedPad();
    if (current_pad != NULL)
        current_pad->SetLogx(!current_pad->GetLogy());
    else
        cout << "No current pad - skipping!" << endl;
}

void logy()
{
    TPad *current_pad = (TPad *)gROOT->GetSelectedPad();
    if (current_pad != NULL)
        current_pad->SetLogy(!current_pad->GetLogy());
    else
        cout << "No current pad - skipping!" << endl;
}

void logz()
{
    TPad *current_pad = (TPad *)gROOT->GetSelectedPad();
    if (current_pad != NULL)
        current_pad->SetLogz(!current_pad->GetLogy());
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
