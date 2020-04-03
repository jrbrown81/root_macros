// Calculates and plots dPhi from simulation root tree files.
void plotDphi(Long64_t nEvents=0,Float_t thetaMin=70, Float_t thetaMax=110, Bool_t radianData=0)
{
   if(nEvents==0) nEvents=tree->GetEntriesFast();

   Double_t pi=TMath::Pi();
   Double_t th_min;
   Double_t th_max;
   if(radianData) {
      th_min=thetaMin*TMath::DegToRad();
      th_max=thetaMax*TMath::DegToRad();
      tree->SetAlias("dPhi","(phi1+phi2)*TMath::RadToDeg()");
   } else {
      th_min=thetaMin;
      th_max=thetaMax;
      tree->SetAlias("dPhi","(phi1+phi2)");
   }

   TCut thetaCut(Form("theta2>%f && theta2<%f && theta1>%f && theta1<%f",th_min,th_max,th_min,th_max));
   TCut energyCut("(E1_a+E1_b)>480 && (E2_a+E2_b)>480");
   TCut phiCut1("dPhi!=0 && dPhi<180 && dPhi>-180");
   TCut phiCut2("dPhi!=0 && dPhi<-180");
   TCut phiCut3("dPhi!=0 && dPhi>180");
   TCut XYcut1("(X1_a*X1_a+Y1_a*Y1_a)<1 && (X2_a*X2_a+Y2_a*Y2_a)<1");
   TCut XYcut2("(X1_a*X1_a+Y1_a*Y1_a)<2 && (X2_a*X2_a+Y2_a*Y2_a)<2");
   TCut XYcut4("(X1_a*X1_a+Y1_a*Y1_a)<4 && (X2_a*X2_a+Y2_a*Y2_a)<4");

   cout << "Plotting cos(2dPhi) for " << nEvents << " events for " << thetaMin << "<theta<" << thetaMax << " (" << th_min << "<th<" << th_max << ")." << endl;
   cout << "Using cuts:" << endl 
	<< thetaCut << endl
	<< energyCut << endl;

   TCanvas* dPhi_c=new TCanvas("dPhi_c","-4<X(Y)<4");
   TH1F *h1=new TH1F("h1","",500,-180,180);
   TH1F *h2=new TH1F("h2","",500,-180,180);
   TH1F *h3=new TH1F("h3","",500,-180,180);
   TH1F *h4=new TH1F("h4","",500,-180,180);

   tree->Draw("dPhi>>h1",phiCut1 && XYcut4 && energyCut && thetaCut,"",nEvents);
   tree->Draw("dPhi+360>>h2",phiCut2 && XYcut4 && energyCut && thetaCut,"",nEvents);
   tree->Draw("dPhi-360>>h3",phiCut3 && XYcut4 && energyCut && thetaCut,"",nEvents);
   
   h4->Add(h1);
   h4->Add(h2);
   h4->Add(h3);
   
   dPhi_c->Clear();
   dPhi_c->Divide(2,2);
   dPhi_c->cd(1);
   h1->Draw();
   dPhi_c->cd(2);
   h2->Draw();
   dPhi_c->cd(3);
   h3->Draw();
   dPhi_c->cd(4);
   h4->Draw();
   
   TCanvas* dPhi2_c=new TCanvas("dPhi2_c","all X(Y)");
   TH1F *h5=new TH1F("h5","",500,-180,180);
   TH1F *h6=new TH1F("h6","",500,-180,180);
   TH1F *h7=new TH1F("h7","",500,-180,180);
   TH1F *h8=new TH1F("h8","",500,-180,180);
 
   tree->Draw("dPhi>>h5",phiCut1 && energyCut && thetaCut,"",nEvents); 
   tree->Draw("dPhi+360>>h6",phiCut2 && energyCut && thetaCut,"",nEvents); 
   tree->Draw("dPhi-360>>h7",phiCut3 && energyCut && thetaCut,"",nEvents); 

   h8->Add(h5);
   h8->Add(h6);
   h8->Add(h7);

   dPhi2_c->Clear();
   dPhi2_c->Divide(2,2);
   dPhi2_c->cd(1);
   h5->Draw();
   dPhi2_c->cd(2);
   h6->Draw();
   dPhi2_c->cd(3);
   h7->Draw();
   dPhi2_c->cd(4);
   h8->Draw();

   TCanvas* dPhi3_c=new TCanvas("dPhi3_c","-1<X(Y)<1");
   TH1F *h9=new TH1F("h9","",500,-180,180);
   TH1F *h10=new TH1F("h10","",500,-180,180);
   TH1F *h11=new TH1F("h11","",500,-180,180);
   TH1F *h12=new TH1F("h12","",500,-180,180);

   tree->Draw("dPhi>>h9",phiCut1 && XYcut1 && energyCut && thetaCut,"",nEvents);
   tree->Draw("dPhi+360>>h10",phiCut2 && XYcut1 && energyCut && thetaCut,"",nEvents);
   tree->Draw("dPhi-360>>h11",phiCut3 && XYcut1 && energyCut && thetaCut,"",nEvents);

   h12->Add(h9);
   h12->Add(h10);
   h12->Add(h11);
   
   dPhi3_c->Clear();
   dPhi3_c->Divide(2,2);
   dPhi3_c->cd(1);
   h9->Draw();
   dPhi3_c->cd(2);
   h10->Draw();
   dPhi3_c->cd(3);
   h11->Draw();
   dPhi3_c->cd(4);
   h12->Draw();

   return;
}
