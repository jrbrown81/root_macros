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
	Bool_t error=0;

	Float_t eRestMass=511;
  Float_t outgoingEnergy = incomingEnergy/(1.+incomingEnergy/eRestMass*(1.-TMath::Cos(angle*TMath::DegToRad())));

//   Float_t incomingEnergy=511;
//   Float_t outgoingEnergy = incomingEnergy - incomingEnergy*(1. - TMath::Cos(angle*TMath::DegToRad()))/(2. - TMath::Cos(angle*TMath::DegToRad()));
  Float_t depositedEnergy = incomingEnergy - outgoingEnergy;

	if(incomingEnergy<=0) {
		error=1;
		if(!opt.Contains("Q")) {
			cout << "!!!!!!!!!!!! Error !!!!!!!!!!!!" << endl
				  	<< "Incoming Energy must be > 0." << endl
				 	  << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		}
	} else if(angle<0 || angle >180) {
		error=1;
		if(!opt.Contains("Q")) {
			cout << "!!!!!!!!!!!! Error !!!!!!!!!!!!" << endl
					 << "Angle must be between 0 and 180 degrees." << endl
				 	 << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		}
	} else {
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
	}

  if(error) return 0;
	else return outgoingEnergy;
}

Float_t GetAngleFromEnergy(const Float_t incomingEnergy, const Float_t depositedEnergy, TString opt="")
//void GetAngleFromEnergy(const Float_t incomingEnergy, const Float_t depositedEnergy)
//void GetAngleFromEnergy(const Float_t depositedEnergy)
{
	Bool_t error=0;

  Float_t eRestMass=511;
	Float_t arg=1.+eRestMass/incomingEnergy-eRestMass/(incomingEnergy-depositedEnergy);
	Float_t angle = TMath::ACos(arg)*TMath::RadToDeg();
	// Float_t angle = TMath::ACos(1.+eRestMass/incomingEnergy-eRestMass/(incomingEnergy-depositedEnergy))*TMath::RadToDeg();

//   Float_t incomingEnergy=511;
//   Float_t angle = TMath::ACos(1.-depositedEnergy/(incomingEnergy-depositedEnergy))*TMath::RadToDeg();
  Float_t outgoingEnergy = incomingEnergy - depositedEnergy;

	if(incomingEnergy<=0) {
		error=1;
		if(!opt.Contains("Q")) {
			cout << "!!!!!!!!!!!! Error !!!!!!!!!!!!" << endl
				  	<< "Incoming Energy must be > 0." << endl
				 	  << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		}
	} else if(depositedEnergy<0) {
			error=1;
			if(!opt.Contains("Q")) {
				cout << "!!!!!!!!!!!! Error !!!!!!!!!!!!" << endl
				  	 << "Deposited Energy must be >= 0." << endl
				 	   << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			}
	} else {
		if(arg<-1) {
			error=1;
			if(!opt.Contains("Q")) {
				cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
					 	 << "!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!" << endl
					 	 << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
					 	 << "For an incoming energy of " << incomingEnergy << " keV, the" << endl
					 	 << "maximum energy deposit is " << incomingEnergy-GetEnergyFromAngle(incomingEnergy,180,"Q") << " keV." << endl
					 	 << "The following output will be incorrect!"	<< endl
           	 << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
			}
		}

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
	}

  if(error) return 0;
	else return angle;
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
