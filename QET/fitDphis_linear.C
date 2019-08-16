// Plots and fits dPhi distributions from pairAnalysis results file ("histo.root").

#include "GetLinear.C"
//#include "GetEnhancement.C"

void fitDphis_linear(TString fitOpt="", TString drawOpt="")
{

// dPhiAngle
	TCanvas* dPhi_lin_c=new TCanvas("dPhi_lin_c","dPhiAngle Linear");
	dPhi_lin_c->Divide(2,4);

	TH1F* dPhiAngle_lin=(TH1F*)dPhiAngle->Clone("dPhiAngle_lin");
	TH1F* dPhiAngle_lin_rebin=(TH1F*)dPhiAngle->Clone("dPhiAngle_lin_rebin");
	TH1F* dPhiAngle_lin_rebin2=(TH1F*)dPhiAngle->Clone("dPhiAngle_lin_rebin2");
	TH1F* dPhiAngle_lin_rebin3=(TH1F*)dPhiAngle->Clone("dPhiAngle_lin_rebin3");
	dPhiAngle_lin_rebin->Rebin(2);
   dPhiAngle_lin_rebin2->Rebin(4);
   dPhiAngle_lin_rebin3->Rebin(6);
	dPhi_lin_c->cd(1);
	GetLinear(dPhiAngle_lin,fitOpt,drawOpt);
	dPhi_lin_c->cd(3);
	GetLinear(dPhiAngle_lin_rebin,fitOpt,drawOpt);
   dPhi_lin_c->cd(5);
	GetLinear(dPhiAngle_lin_rebin2,fitOpt,drawOpt);
   dPhi_lin_c->cd(7);
	GetLinear(dPhiAngle_lin_rebin3,fitOpt,drawOpt);

	TH1F* dPhiAngle_Ewin_lin=(TH1F*)dPhiAngle_Ewin->Clone("dPhiAngle_Ewin_lin");
	TH1F* dPhiAngle_Ewin_lin_rebin=(TH1F*)dPhiAngle_Ewin->Clone("dPhiAngle_Ewin_lin_rebin");
	TH1F* dPhiAngle_Ewin_lin_rebin2=(TH1F*)dPhiAngle_Ewin->Clone("dPhiAngle_Ewin_lin_rebin2");
	TH1F* dPhiAngle_Ewin_lin_rebin3=(TH1F*)dPhiAngle_Ewin->Clone("dPhiAngle_Ewin_lin_rebin3");
	dPhiAngle_Ewin_lin_rebin->Rebin(2);
	dPhiAngle_Ewin_lin_rebin2->Rebin(4);
	dPhiAngle_Ewin_lin_rebin3->Rebin(6);
	dPhi_lin_c->cd(2);
	GetLinear(dPhiAngle_Ewin_lin,fitOpt,drawOpt);
	dPhi_lin_c->cd(4);
	GetLinear(dPhiAngle_Ewin_lin_rebin,fitOpt,drawOpt);
	dPhi_lin_c->cd(6);
	GetLinear(dPhiAngle_Ewin_lin_rebin2,fitOpt,drawOpt);
	dPhi_lin_c->cd(8);
	GetLinear(dPhiAngle_Ewin_lin_rebin3,fitOpt,drawOpt);

	dPhi_lin_c->SaveAs("dPhiPlots" + fitOpt + "_lin.pdf");

// dPhiAngle1
   TCanvas* dPhi1_lin_c=new TCanvas("dPhi1_lin_c","dPhiAngle1 Linear");
   dPhi1_lin_c->Divide(2,4);

   TH1F* dPhiAngle1_rebin=(TH1F*)dPhiAngle1->Clone("dPhiAngle1_rebin");
   TH1F* dPhiAngle1_rebin2=(TH1F*)dPhiAngle1->Clone("dPhiAngle1_rebin2");
   TH1F* dPhiAngle1_rebin3=(TH1F*)dPhiAngle1->Clone("dPhiAngle1_rebin3");
   dPhiAngle1_rebin->Rebin(2);
   dPhiAngle1_rebin2->Rebin(4);
   dPhiAngle1_rebin3->Rebin(6);
   dPhi1_lin_c->cd(1);
   GetLinear(dPhiAngle1,fitOpt,drawOpt);
   dPhi1_lin_c->cd(3);
   GetLinear(dPhiAngle1_rebin,fitOpt,drawOpt);
   dPhi1_lin_c->cd(5);
   GetLinear(dPhiAngle1_rebin2,fitOpt,drawOpt);
   dPhi1_lin_c->cd(7);
   GetLinear(dPhiAngle1_rebin3,fitOpt,drawOpt);

   TH1F* dPhiAngle1_Ewin_rebin=(TH1F*)dPhiAngle1_Ewin->Clone("dPhiAngle1_Ewin_rebin");
   TH1F* dPhiAngle1_Ewin_rebin2=(TH1F*)dPhiAngle1_Ewin->Clone("dPhiAngle1_Ewin_rebin2");
   TH1F* dPhiAngle1_Ewin_rebin3=(TH1F*)dPhiAngle1_Ewin->Clone("dPhiAngle1_Ewin_rebin3");
   dPhiAngle1_Ewin_rebin->Rebin(2);
   dPhiAngle1_Ewin_rebin2->Rebin(4);
   dPhiAngle1_Ewin_rebin3->Rebin(6);
   dPhi1_lin_c->cd(2);
   GetLinear(dPhiAngle1_Ewin,fitOpt,drawOpt);
	dPhi1_lin_c->cd(4);
   GetLinear(dPhiAngle1_Ewin_rebin,fitOpt,drawOpt);
   dPhi1_lin_c->cd(6);
   GetLinear(dPhiAngle1_Ewin_rebin2,fitOpt,drawOpt);
   dPhi1_lin_c->cd(8);
   GetLinear(dPhiAngle1_Ewin_rebin3,fitOpt,drawOpt);

   dPhi1_lin_c->SaveAs("dPhiPlots1" + fitOpt + "_lin.pdf");
//
   return;
}
