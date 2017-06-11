///////////////////////////////////////////////////////////////////////////////////////////////////
//
/// Description to be added...
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "EventMixer.hh"
#include "CmdLine.hh"
#include "PU14.hh"
#include "zfstream.h"
#include "fastjet/ClusterSequence.hh"
#include "SubstructureVariables.hh"
#include <fstream>

#include "Detector.hh"

using namespace std;
using namespace fastjet;

int main (int argc, char ** argv) {
  CmdLine cmdline(argc,argv);
  // inputs read from command line
  int nev = cmdline.value<int>("-nev",1);  // first argument: command line option; second argument: default value
  double R = cmdline.value<double>("-R",1.0);

  string outname = cmdline.value<string>("-out");
  
  // some definitions
  JetDefinition jet_def(antikt_algorithm,R);       // the jet definition....

  double jet_rapmax = cmdline.value("-jet.rapmax",  2.5);
  double jet_ptmin  = cmdline.value("-jet.ptmin", 500.0);
  
  //-------------------------------------------------------------------------
  // build a detector if requested
  bool buildATLAS = cmdline.present("-ATLAS");
  bool buildCMS   = cmdline.present("-CMS");
   
  Detector::Experiment* detector_model_ptr = 0; 
  if ( buildATLAS ) {
    if ( buildCMS ) {
      cout << "Found both the -ATLAS and -CMS option, can only build one detector" << endl;
      return 0;
    }
    detector_model_ptr = Detector::Build::ATLAS();
  } else {
    if ( buildCMS ) { 
      detector_model_ptr = Detector::Build::CMS();
    } else  {
      cout << "No detector smearing configured, nothing to do" << endl;
      return 0; 
    }
  }
  SharedPtr<Detector::Experiment> shared_detector_model_ptr(detector_model_ptr);

  // get a signal processor - the processor takes ownership of the detector model object
  SharedPtr<Detector::Signals> signal_processor(new Detector::Signals(detector_model_ptr)); 

  // selection of the hardest jets
  Selector sel_hard_jets = SelectorNHardest(2) * SelectorPtMin(jet_ptmin) * SelectorAbsRapMax(jet_rapmax);

  ostringstream header;
  header << "# Ran: " << cmdline.command_line() << endl;
  header << "# Used jet definition: " << jet_def.description() << endl;

  //-------------------------------------------------------------------------
  // set up substructure tools
  SubstructureVariables subvars(cmdline);
  //TODO: add this once Fredericc has finished playing w the file
  header << subvars.description() << "#" << endl;
  
  //------------------------------------------------------------------------
  // create mixer that will construct events by mixing hard and pileup
  // events read from files given from command line using 
  // -hard hard_events_file(.gz) -pileup pileup_events_file(.gz)
  EventMixer mixer(&cmdline);
  header << "# EventGeneration: " << mixer.description() << endl;
  header << "#" << endl;
  
  //assert(cmdline.all_options_used());

  //------------------------------------------------------------------------
  // prepare the output
  ostream *ostr;  
  if (outname.length() > 3 && outname.find(std::string(".gz")) +3 == outname.length()) {
    ostr = new gzofstream(outname.c_str());
  } else {
    ostr = new ofstream(outname.c_str());
  }
  SharedPtr<ostream> ostr_ptr(ostr);
  
  (*ostr) << header.str();
  (*ostr) << "#columns: ";
  (*ostr) << "scalarpt_loose scalarpt_tight scalarpt_plain ";
  (*ostr) << "mass_loose mass_tight mass_plain mass_trim ";
  (*ostr) << "zg_loose zg_tight ";
  (*ostr) << "thetag_loose thetag_tight ";
  for (const auto & name : {"tau1", "tau2", "1e2", "1e3", "2e3", "3e3"}){
    for (int beta=1; beta<=2; ++beta){
      for (const auto & level : {"loose", "tight", "plain"}){
        (*ostr) << name << "_beta" << beta << "_" << level << " ";
      }
    }
  }
  for (const auto & name : {"tau21", "D2", "M2", "N2"}){
    for (int beta=1; beta<=2; ++beta){
      (*ostr) << name << "_beta" << beta << "_trimmed ";
    }
  }  
  (*ostr) << endl;
  
  //------------------------------------------------------------------------
  // loop over events
  int iev = 0;
  int periodic_iev_output=10;
  int nentries = 0;
  cout << "Start running for " << nev << " events" << endl; 
  while ( mixer.next_event() && iev < nev ) {
     // increment event number    
     iev++;
     
     // extract particles from event 
     vector<PseudoJet> full_event = mixer.particles() ;
     if (iev % periodic_iev_output == 0){
       cout << "Event " << iev << endl;
       if (iev == 15 * periodic_iev_output) periodic_iev_output*=10;
     }

     // collect detector signals
     if ( !signal_processor->fill(full_event) ) {
       cout << "Empty event or signal processor is disabled" << endl;
       continue;
     }

          
     vector<PseudoJet> jets = sel_hard_jets(jet_def(signal_processor->get()));

     // loop over the jets
     for (const PseudoJet &jet : jets){
       subvars.set_jet(jet);
       ++nentries;

       // handle scalar pt sums
       for (int igroom = 0; igroom<3; ++igroom){
         (*ostr) << subvars.scalarsum_pt((SubstructureVariables::groom) igroom) << " ";
       }       
         
       // handle masses
       for (int igroom = 0; igroom<4; ++igroom){
         (*ostr) << subvars.m((SubstructureVariables::groom) igroom) << " ";
       }
       
       (*ostr) << subvars.zg(SubstructureVariables::loose) << " ";
       (*ostr) << subvars.zg(SubstructureVariables::tight) << " ";
       (*ostr) << subvars.thetag(SubstructureVariables::loose) << " ";
       (*ostr) << subvars.thetag(SubstructureVariables::tight) << " ";

       // tau1
       for (int ibeta = 0; ibeta<2; ++ibeta){
         for (int igroom = 0; igroom<3; ++igroom){
           (*ostr) << subvars.tau1((SubstructureVariables::groom) igroom,
                                   (SubstructureVariables::beta) ibeta) << " ";
         }
       }

       // tau2
       for (int ibeta = 0; ibeta<2; ++ibeta){
         for (int igroom = 0; igroom<3; ++igroom){
           (*ostr) << subvars.tau2((SubstructureVariables::groom) igroom,
                                   (SubstructureVariables::beta) ibeta) << " ";
         }
       }

       // 1e2
       for (int ibeta = 0; ibeta<2; ++ibeta){
         for (int igroom = 0; igroom<3; ++igroom){
           (*ostr) << subvars.ecfg_v1_N2((SubstructureVariables::groom) igroom,
                                         (SubstructureVariables::beta) ibeta) << " ";
         }
       }

       // 1e3
       for (int ibeta = 0; ibeta<2; ++ibeta){
         for (int igroom = 0; igroom<3; ++igroom){
           (*ostr) << subvars.ecfg_v1_N3((SubstructureVariables::groom) igroom,
                                         (SubstructureVariables::beta) ibeta) << " ";
         }
       }

       // 2e3
       for (int ibeta = 0; ibeta<2; ++ibeta){
         for (int igroom = 0; igroom<3; ++igroom){
           (*ostr) << subvars.ecfg_v2_N3((SubstructureVariables::groom) igroom,
                                         (SubstructureVariables::beta) ibeta) << " ";
         }
       }
       
       // 3e3
       for (int ibeta = 0; ibeta<2; ++ibeta){
         for (int igroom = 0; igroom<3; ++igroom){
           (*ostr) << subvars.ecfg_v3_N3((SubstructureVariables::groom) igroom,
                                         (SubstructureVariables::beta) ibeta) << " ";
         }
       }

       // trimmed versions for ATLAS
       for (int ibeta = 0; ibeta<2; ++ibeta)
         (*ostr) << subvars.tau21(SubstructureVariables::trim, 
                                  (SubstructureVariables::beta) ibeta) << " ";
       for (int ibeta = 0; ibeta<2; ++ibeta)
         (*ostr) << subvars.D2(SubstructureVariables::trim, 
                               (SubstructureVariables::beta) ibeta) << " ";
       for (int ibeta = 0; ibeta<2; ++ibeta)
         (*ostr) << subvars.M2(SubstructureVariables::trim, 
                               (SubstructureVariables::beta) ibeta) << " ";
       for (int ibeta = 0; ibeta<2; ++ibeta)
         (*ostr) << subvars.N2(SubstructureVariables::trim, 
                               (SubstructureVariables::beta) ibeta) << " ";

       (*ostr) << endl;


     } // end of loop over jets
  } // end of loop over events
  (*ostr) << "#Nentries=" << nentries << endl;
  
  return 0;
}  
