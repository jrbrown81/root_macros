#include <iostream>

void histoToCSV(TH1F* hist, TString output="output.csv", Int_t columns=1, Bool_t overwrite=0, Int_t useBinEdges=0)
{
	ofstream outfile;

	if(!(columns==1 || columns==2)) {
		cout << "Aborting: Output must be to 1 or 2 columns." << endl;
		return;
	}
	if(gSystem->AccessPathName(output)) outfile.open(output);
	else {
		cout << "WARNING! File: " << output << " already exists." << endl;
		if(overwrite) {
			cout << "Overwrite mode is enabled so overwriting file." << endl;
			outfile.open(output);
		} else {
			cout << "Aborting. If you want to overwrite file use:" << endl
					<< "histoToCSV(" << hist->GetName() << ",\"" << output << "\", N columns, 1)" << endl;
			return;
		}
	}

	Int_t nBins=hist->GetNbinsX();
	if(columns==1) {
		cout << "Writing output to file: '" << output << "' as a single column..." << endl;
		for(int i=1; i<=nBins; i++) outfile << hist->GetBinContent(i) << endl;
	}
	else if(columns==2) {
		cout << "Writing output to file: '" << output << "' as two columns" << endl;
		if(useBinEdges==0) {
			cout << "using simple bin numbering..." << endl;
			for(int i=1; i<=nBins; i++) outfile << i-1 << "," << hist->GetBinContent(i) << endl;
		} else if(useBinEdges==1) {
			cout << "using low edges of bins for bin numbering..." << endl;
			for(int i=1; i<=nBins; i++) outfile << hist->GetBinLowEdge(i) << "," << hist->GetBinContent(i) << endl;
		} else if(useBinEdges==2) {
			cout << "using centre of bins for bin numbering..." << endl;
			for(int i=1; i<=nBins; i++) outfile << hist->GetBinCenter(i) << "," << hist->GetBinContent(i) << endl;
		}
	}

	outfile.close();
	return;
}
