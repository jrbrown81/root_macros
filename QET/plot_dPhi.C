// Calculates and plots dPhi from simulation root tree files.
{
   TCanvas* dPhi_c=new TCanvas("dPhi_c","dPhi_c");
   TH1F *h1=new TH1F("h1","",45,-180,180);
   TH1F *h2=new TH1F("h2","",45,-180,180);
   TH1F *h3=new TH1F("h3","",45,-180,180);
   TH1F *h4=new TH1F("h4","",45,-180,180);

   Double_t pi=TMath::Pi();

   tree->Draw("(phi2+phi1)*180/pi>>h1","(phi1+phi2)<pi && (phi1+phi2)>-pi && X1_a>-2 && X1_a<2 && Y1_a>-2 && Y1_a<2 && X2_a>-2 && X2_a<2 && Y2_a>-2 && Y2_a<2","");
   tree->Draw("(phi2+phi1)*180/pi+360>>h2","(phi1+phi2)<-pi && X1_a>-2 && X1_a<2 && Y1_a>-2 && Y1_a<2 && X2_a>-2 && X2_a<2 && Y2_a>-2 && Y2_a<2","");
   tree->Draw("(phi2+phi1)*180/pi-360>>h3","(phi1+phi2)>pi && X1_a>-2 && X1_a<2 && Y1_a>-2 && Y1_a<2 && X2_a>-2 && X2_a<2 && Y2_a>-2 && Y2_a<2","");
   
//   tree->Draw("(phi2+phi1)*180/pi>>h1","(phi1+phi2)<pi && (phi1+phi2)>-pi","");
//   tree->Draw("(phi2+phi1)*180/pi+360>>h2","(phi1+phi2)<-pi","");
//   tree->Draw("(phi2+phi1)*180/pi-360>>h3","(phi1+phi2)>pi","");
   
//   tree->Draw("-(phi2+phi1)*180/pi-360>>h2","(phi1+phi2)<-pi","");
//   tree->Draw("-(phi2+phi1)*180/pi+360>>h3","(phi1+phi2)>pi","");
   
   h4->Add(h1);
   h4->Add(h2);
   h4->Add(h3);
   
//   h1->Add(h2);
//   h1->Add(h3);

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
   
   TCanvas* dPhi2_c=new TCanvas("dPhi2_c","dPhi2_c");
   TH1F *h5=new TH1F("h5","",45,-180,180);
   TH1F *h6=new TH1F("h6","",45,-180,180);
   TH1F *h7=new TH1F("h7","",45,-180,180);
   TH1F *h8=new TH1F("h8","",45,-180,180);
  
   tree->Draw("(phi2+phi1)*180/pi>>h5","(phi1+phi2)<pi && (phi1+phi2)>-pi","");
   tree->Draw("(phi2+phi1)*180/pi+360>>h6","(phi1+phi2)<-pi","");
   tree->Draw("(phi2+phi1)*180/pi-360>>h7","(phi1+phi2)>pi","");

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
}
