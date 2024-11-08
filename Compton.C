void Help();
void Usage();

void Compton()
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
		<< "!!!! Now works for any incoming energy !!!!" << endl
		<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
//		<< "!!!! ONLY VALID FOR E_i=511keV !!!!" << endl
//		<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

	Help();
}

Float_t GetEnergyFromAngle(const Float_t incomingEnergy, const Float_t angle, TString opt="")
//void GetEnergyFromAngle(const Float_t angle)
{
	Float_t eRestMass=511;
   Float_t outgoingEnergy = incomingEnergy/(1.+incomingEnergy/eRestMass*(1.-TMath::Cos(angle*TMath::DegToRad())));

//   Float_t incomingEnergy=511;
//   Float_t outgoingEnergy = incomingEnergy - incomingEnergy*(1. - TMath::Cos(angle*TMath::DegToRad()))/(2. - TMath::Cos(angle*TMath::DegToRad()));
   Float_t depositedEnergy = incomingEnergy - outgoingEnergy;

   if(!opt.Contains("Q")) {
     cout << "--------------------------------------" << endl
          << "<<< Compton Scattering Calculation >>>" << endl
          << "--------------------------------------" << endl;

     cout << "Incoming Energy:  " << incomingEnergy << " keV" << endl
          << "Scattering angle: " << angle << " degrees" << endl
          << "Outgoing Energy:  " << outgoingEnergy << " keV" << endl
          << "Energy deposited: " << depositedEnergy << " keV" << endl;

     cout << "--------------------------------------" << endl;
   }

   return outgoingEnergy;
}

Float_t GetAngleFromEnergy(const Float_t incomingEnergy, const Float_t depositedEnergy, TString opt="")
//void GetAngleFromEnergy(const Float_t incomingEnergy, const Float_t depositedEnergy)
//void GetAngleFromEnergy(const Float_t depositedEnergy)
{
  Float_t eRestMass=511;
	Float_t arg=1.+eRestMass/incomingEnergy-eRestMass/(incomingEnergy-depositedEnergy);
	Float_t angle = TMath::ACos(arg)*TMath::RadToDeg();
	// Float_t angle = TMath::ACos(1.+eRestMass/incomingEnergy-eRestMass/(incomingEnergy-depositedEnergy))*TMath::RadToDeg();

//   Float_t incomingEnergy=511;
//   Float_t angle = TMath::ACos(1.-depositedEnergy/(incomingEnergy-depositedEnergy))*TMath::RadToDeg();
  Float_t outgoingEnergy = incomingEnergy - depositedEnergy;

   if(!opt.Contains("Q")) {
     cout << "--------------------------------------" << endl
          << "<<< Compton Scattering Calculation >>>" << endl
          << "--------------------------------------" << endl;

		if(arg<-1) {
			cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
					 << "!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!" << endl
					 << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl
					 << "For an incoming energy of " << incomingEnergy << " keV, the" << endl
					 << "maximum energy deposit is " << incomingEnergy-GetEnergyFromAngle(incomingEnergy,180,"Q") << " keV." << endl
					 << "The following output will be incorrect!"	<< endl
           << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
		}

     cout << "Incoming Energy:  " << incomingEnergy << " keV" << endl
          << "Scattering angle: " << angle << " degrees" << endl
          << "Outgoing Energy:  " << outgoingEnergy << " keV" << endl
          << "Energy deposited: " << depositedEnergy << " keV" << endl;

     cout << "--------------------------------------" << endl;
   }

   return angle;
}

void Help()
{
   cout << "GetAngleFromEnergy(const Float_t incomingEnergy, const Float_t depositedEnergy, TString opt=\"\"), or" << endl
	<< "GetEnergyFromAngle(const Float_t incomingEnergy, const Float_t angle, TString opt=\"\")" << endl;
//   cout << "GetAngleFromEnergy(const Float_t depositedEnergy), or" << endl
//	<< "GetEnergyFromAngle(const Float_t angle)" << endl;
}

void Usage() {Help();}
void usage() {Help();}
void help() {Help();}
