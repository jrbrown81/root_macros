// Plots and fits dPhi distributions from pairAnalysis results file ("histo.root").

#include "GetEnhancement.C"

void plotDphi(TString fitOpt="", TString drawOpt="", Bool_t phaseShift=0)
{

// dPhiAngle
	TCanvas* dPhi_c=new TCanvas("dPhi_c","dPhiAngle");
	dPhi_c->Divide(2,4);
	
	TH1F* dPhiAngle_rebin=(TH1F*)dPhiAngle->Clone("dPhiAngle_rebin");
	TH1F* dPhiAngle_rebin2=(TH1F*)dPhiAngle->Clone("dPhiAngle_rebin2");
	TH1F* dPhiAngle_rebin3=(TH1F*)dPhiAngle->Clone("dPhiAngle_rebin3");
	dPhiAngle_rebin->Rebin(2);
   dPhiAngle_rebin2->Rebin(4);
   dPhiAngle_rebin3->Rebin(6);
	dPhi_c->cd(1);
	GetEnhancement(dPhiAngle,fitOpt,drawOpt,phaseShift);
	dPhi_c->cd(3);
	GetEnhancement(dPhiAngle_rebin,fitOpt,drawOpt,phaseShift);
   dPhi_c->cd(5);
	GetEnhancement(dPhiAngle_rebin2,fitOpt,drawOpt,phaseShift);
   dPhi_c->cd(7);
	GetEnhancement(dPhiAngle_rebin3,fitOpt,drawOpt,phaseShift);

	TH1F* dPhiAngle_Ewin_rebin=(TH1F*)dPhiAngle_Ewin->Clone("dPhiAngle_Ewin_rebin");
	TH1F* dPhiAngle_Ewin_rebin2=(TH1F*)dPhiAngle_Ewin->Clone("dPhiAngle_Ewin_rebin2");
	TH1F* dPhiAngle_Ewin_rebin3=(TH1F*)dPhiAngle_Ewin->Clone("dPhiAngle_Ewin_rebin3");
	dPhiAngle_Ewin_rebin->Rebin(2);
	dPhiAngle_Ewin_rebin2->Rebin(4);
	dPhiAngle_Ewin_rebin3->Rebin(6);
	dPhi_c->cd(2);
	GetEnhancement(dPhiAngle_Ewin,fitOpt,drawOpt,phaseShift);
	dPhi_c->cd(4);
	GetEnhancement(dPhiAngle_Ewin_rebin,fitOpt,drawOpt,phaseShift);
	dPhi_c->cd(6);
	GetEnhancement(dPhiAngle_Ewin_rebin2,fitOpt,drawOpt,phaseShift);
	dPhi_c->cd(8);
	GetEnhancement(dPhiAngle_Ewin_rebin3,fitOpt,drawOpt,phaseShift);
 
	if(phaseShift==0) dPhi_c->SaveAs("dPhiPlots" + fitOpt + ".pdf");
	if(phaseShift==1) dPhi_c->SaveAs("dPhiPlots" + fitOpt + "ps.pdf");

// dPhiAngle1
   TCanvas* dPhi1_c=new TCanvas("dPhi1_c","dPhiAngle1");
   dPhi1_c->Divide(2,4);

   TH1F* dPhiAngle1_rebin=(TH1F*)dPhiAngle1->Clone("dPhiAngle1_rebin");
   TH1F* dPhiAngle1_rebin2=(TH1F*)dPhiAngle1->Clone("dPhiAngle1_rebin2");
   TH1F* dPhiAngle1_rebin3=(TH1F*)dPhiAngle1->Clone("dPhiAngle1_rebin3");
   dPhiAngle1_rebin->Rebin(2);
   dPhiAngle1_rebin2->Rebin(4);
   dPhiAngle1_rebin3->Rebin(6);
   dPhi1_c->cd(1);
   GetEnhancement(dPhiAngle1,fitOpt,drawOpt,phaseShift);
   dPhi1_c->cd(3);
   GetEnhancement(dPhiAngle1_rebin,fitOpt,drawOpt,phaseShift);
   dPhi1_c->cd(5);
   GetEnhancement(dPhiAngle1_rebin2,fitOpt,drawOpt,phaseShift);
   dPhi1_c->cd(7);
   GetEnhancement(dPhiAngle1_rebin3,fitOpt,drawOpt,phaseShift);
   
   TH1F* dPhiAngle1_Ewin_rebin=(TH1F*)dPhiAngle1_Ewin->Clone("dPhiAngle1_Ewin_rebin");
   TH1F* dPhiAngle1_Ewin_rebin2=(TH1F*)dPhiAngle1_Ewin->Clone("dPhiAngle1_Ewin_rebin2");
   TH1F* dPhiAngle1_Ewin_rebin3=(TH1F*)dPhiAngle1_Ewin->Clone("dPhiAngle1_Ewin_rebin3");
   dPhiAngle1_Ewin_rebin->Rebin(2);
   dPhiAngle1_Ewin_rebin2->Rebin(4);
   dPhiAngle1_Ewin_rebin3->Rebin(6);
   dPhi1_c->cd(2);
   GetEnhancement(dPhiAngle1_Ewin,fitOpt,drawOpt,phaseShift);
	dPhi1_c->cd(4);
   GetEnhancement(dPhiAngle1_Ewin_rebin,fitOpt,drawOpt,phaseShift);
   dPhi1_c->cd(6);
   GetEnhancement(dPhiAngle1_Ewin_rebin2,fitOpt,drawOpt,phaseShift);
   dPhi1_c->cd(8);
   GetEnhancement(dPhiAngle1_Ewin_rebin3,fitOpt,drawOpt,phaseShift);

   if(phaseShift==0) dPhi1_c->SaveAs("dPhiPlots1" + fitOpt + ".pdf");
   if(phaseShift==1) dPhi1_c->SaveAs("dPhiPlots1" + fitOpt + "ps.pdf");
   
   return;
}
