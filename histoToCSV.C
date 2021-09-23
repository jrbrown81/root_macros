#include <iostream>

void histoToCSV(TH1F* hist, TString output="output.csv", Bool_t overwrite=0)
{
	ofstream outfile;

	if(gSystem->AccessPathName(output)) outfile.open(output);
	else {
		cout << "WARNING! File: " << output << " already exists." << endl;
		if(overwrite) {
			cout << "overwrite mode is enabled so overwriting file." << endl;
			outfile.open(output);
		} else {
			cout << "Exiting." << endl;
			cout << "(if you want to overwrite file use:" << endl
					<< "histoToCSV(" << hist->GetName() << ",\"" << output << "\",1)" << endl;
			return;
		}
	}
	
	Int_t nBins=hist->GetNbinsX();
	for(int i=1; i<=nBins; i++) outfile << hist->GetBinContent(i) << endl;
	
	outfile.close();
	
	return;
}
