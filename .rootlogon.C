{
// Place this in home area, to be executed every time root is loaded.
// To prevent this from executing when ROOT is started do:$ root -n
// Will be superceded by a local .rootlogon.C (I think)
// J. Brown, 16/04/25

	cout << "~/.rootlogon.C executed." << endl;
	cout << "(To disable this, launch ROOT with: root -n)" << endl;
	if(!gROOT->ProcessLine(".L myUsefulMacros.C")) cout << "myUsefulMacros.C has been loaded." << endl;

}	
