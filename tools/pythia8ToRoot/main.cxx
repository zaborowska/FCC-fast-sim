#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC.h"
#include "HepMC/GenEvent.h"
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"

int main(int argc, char* argv[])
{
   if (argc != 3)
   {
      std::cerr << " Usage: ./"<<argv[0]<<" [input file name] [output file name]" << std::endl;
      return 1;
   }
   ifstream is(argv[1]);
   if (!is) {
      std::cerr << " PYTHIA input file " << argv[1] << " was not found. \n"<<std::endl;
      return 1;
   }
   else
   {
      cout << "\n PYTHIA settings will be read from file " << argv[1] << std::endl
           << " and saved to " << argv[2]<< std::endl;
   }
   // Load library to enable saving HepMC to root file
   gSystem->Load("libHepMCdict");
   TFile* fFile = new TFile(argv[2],"RECREATE");

   // Interface for conversion from Pythia8::Event to HepMC event.
   HepMC::Pythia8ToHepMC ToHepMC;

   // Generator.
   Pythia8::Pythia pythia;

   // Read in commands from external file.
   pythia.readFile(argv[1]);

   // Extract settings to be used in the main program.
   int    nEvent    = pythia.mode("Main:numberOfEvents");
   int    nAbort    = pythia.mode("Main:timesAllowErrors");

   // PYTHIA Initialization.
   pythia.init();

  // Begin event loop.
   int iAbort = 0;
   for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

      // Generate event.
      if (!pythia.next()) {

         // If failure because reached end of file then exit event loop.
         if (pythia.info.atEndOfFile()) {
            cout << " Aborted since reached end of Les Houches Event File\n";
            break;
         }

         // First few failures write off as "acceptable" errors, then quit.
         if (++iAbort < nAbort) continue;
         cout << " Event generation aborted prematurely, owing to error!\n";
         break;
      }

      // Construct new empty HepMC event and fill it.
      // Units will be as chosen for HepMC build, but can be changed
      // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
      HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
      ToHepMC.fill_next_event( pythia, hepmcevt );

      // Write the HepMC event to Root file.
      std::cout<<"Saving "<<Form("Event_%d",iEvent)<< " into Root file"<< std::endl;
      fFile->WriteObject(hepmcevt, Form("Event_%d",iEvent));

      delete hepmcevt;

      // End of event loop. Statistics.
   }
   pythia.stat();
   fFile->Print();
   fFile->ls();
   fFile->Close();
   return 0;
}
