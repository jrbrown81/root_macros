{
   TH1F *h1=new TH1F("h1","",180,-180,180);
   TH1F *h2=new TH1F("h2","",180,-180,180);
   TH1F *h3=new TH1F("h3","",180,-180,180);
   TH1F *h4=new TH1F("h4","",180,-180,180);

   Double_t pi=TMath::Pi();

   tree->Draw("(phi2+phi1)*180/pi>>h1","(phi1+phi2)<pi && (phi1+phi2)>-pi","");
   tree->Draw("(phi2+phi1)*180/pi+360>>h2","(phi1+phi2)<-pi","");
   tree->Draw("(phi2+phi1)*180/pi-360>>h3","(phi1+phi2)>pi","");
//   tree->Draw("-(phi2+phi1)*180/pi-360>>h2","(phi1+phi2)<-pi","");
//   tree->Draw("-(phi2+phi1)*180/pi+360>>h3","(phi1+phi2)>pi","");
   
   h4->Add(h1);
   h4->Add(h2);
   h4->Add(h3);
   
//   h1->Add(h2);
//   h1->Add(h3);

   TCanvas c1;
   c1.Divide(2,2);
   c1.cd(1);
   h1->Draw();
   c1.cd(2);
   h2->Draw();
   c1.cd(3);
   h3->Draw();
   c1.cd(4);
   h4->Draw();
   
   TCanvas c2;
//   TH1F *h5=new TH1F("h5","",180,-180,180);
   TH1F *h6=new TH1F("h6","",180,-180,180);
   TH1F *h7=new TH1F("h7","",180,-180,180);
   TH1F *h8=new TH1F("h8","",180,-180,180);
   tree->Draw("-(phi2+phi1)*180/pi-360>>h6","(phi1+phi2)<-pi","");
   tree->Draw("-(phi2+phi1)*180/pi+360>>h7","(phi1+phi2)>pi","");
   h8->Add(h1);
   h8->Add(h6);
   h8->Add(h7);
   c2.Clear();
   c2.Divide(2,2);
   c2.cd(1);
   h1->Draw();
   c2.cd(2);
   h6->Draw();
   c2.cd(3);
   h7->Draw();
   c2.cd(4);
   h8->Draw();
}
