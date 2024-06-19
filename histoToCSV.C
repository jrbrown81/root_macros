#include <iostream>

void histoToCSV(TH1F* hist, TString output="output.csv", Int_t columns=1, Bool_t overwrite=0)
{
	ofstream outfile;

	if(!(columns==1 || columns==2)) {
		cout << "Output must be to 1 or 2 columns. \nExiting." << endl;
		return;
	}
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
	if(columns==1) {
		cout << "Writing output to file: '" << output << "' as a single column..." << endl;
		for(int i=1; i<=nBins; i++) outfile << hist->GetBinContent(i) << endl;
	}
	else if(columns==2) {
		cout << "Writing output to file: '" << output << "' as two columns..." << endl;
		for(int i=1; i<=nBins; i++) outfile << hist->GetBinLowEdge(i) << "," << hist->GetBinContent(i) << endl;
	}

	outfile.close();

	return;
}
