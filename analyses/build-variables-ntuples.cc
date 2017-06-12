///////////////////////////////////////////////////////////////////////////////////////////////////
//
/// Description to be added...
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <fstream>                        //< file I/O
#include "zfstream.h"                     //< gzipepd file I/O
#include "CmdLine.hh"                     //< arguments from the command line
#include "EventMixer.hh"                  //< event interface
#include "PU14.hh"                        //< adidtional info for particles
#include "fastjet/ClusterSequence.hh"     //< clustering
#include "SubstructureVariables.hh"       //< all the measures sustructure variables
#include "Detector.hh"                    //< Peter's detector simulatiom
#include <fastjet/contrib/SoftKiller.hh>  //< for pileup subtraction

using namespace std;
using namespace fastjet;

int main (int argc, char ** argv) {
  //========================================================================
  // Parse th ecommand line
  //========================================================================
  CmdLine cmdline(argc,argv);

  //----------------------------------------------------------------------
  // basic input
  int nev = cmdline.value<int>("-nev",1);  // first argument: command line option; second argument: default value
  string outname = cmdline.value<string>("-out");

  //----------------------------------------------------------------------
  // various physical quantities and cuts
  double R = cmdline.value<double>("-R",1.0);
  double particle_rapmax = cmdline.value("-particle.rapmax",  -1.0); // ineffective when -ve
  double jet_rapmax = cmdline.value("-jet.rapmax",  2.5);
  double jet_ptmin  = cmdline.value("-jet.ptmin", 500.0);

  //------------------------------------------------------------------------
  // create mixer that will construct events by mixing hard and pileup
  // events read from files given from command line using 
  // -hard hard_events_file(.gz) -pileup pileup_events_file(.gz)
  EventMixer mixer(&cmdline);

  //----------------------------------------------------------------------
  // optional pileup subtraction (uses the SoftKiller)
  bool pu_subtraction = cmdline.present("-pusub");
  double sk_grid_size = cmdline.value("-sk.grid", 0.4);
  if (pu_subtraction){
    if (particle_rapmax < 0){
      cerr << "ERROR: pileup subtraction requires a maximal rapidity for particles." << endl
           << "       Set it using -particle.rapmax <value> option." << endl;
      return 1;
    }
  }

  //----------------------------------------------------------------------
  // optional detector simulation
  bool buildATLAS = cmdline.present("-ATLAS");
  bool buildCMS   = cmdline.present("-CMS");
  if ((buildATLAS) && (buildCMS)) {
    cout << "ERROR: One is no tallowed to specify both the ATLAS and CMS detector in a single run" << endl;
    return 1;
  }

  assert(cmdline.all_options_used());

  //========================================================================
  // construct the objects needed for the analysis
  //========================================================================

  // jet reconstruction
  JetDefinition jet_def(antikt_algorithm,R);       // the jet definition....

  // selection of the particles and jets
  Selector sel_particles = SelectorIdentity();
  if (particle_rapmax>0) sel_particles = SelectorAbsRapMax(particle_rapmax);
  Selector sel_hard_jets = SelectorNHardest(2) * SelectorPtMin(jet_ptmin) * SelectorAbsRapMax(jet_rapmax);
  
  //-------------------------------------------------------------------------
  // set up substructure tools
  SubstructureVariables subvars(cmdline);

  //------------------------------------------------------------------------
  // optional pileup subtraction
  SharedPtr<contrib::SoftKiller> sk;
  if (pu_subtraction)
    sk.reset(new contrib::SoftKiller(particle_rapmax, sk_grid_size));

  //----------------------------------------------------------------------
  // optional detector simulation
  Detector::Experiment* detector_model_ptr = 0; 
  if (buildATLAS) detector_model_ptr = Detector::Build::ATLAS();
  if (buildCMS  ) detector_model_ptr = Detector::Build::CMS();

  // store as a shared ptr for easier memory management and get a
  // signal processor - the processor takes ownership of the detector
  // model object
  SharedPtr<Detector::Experiment> shared_detector_model_ptr;
  SharedPtr<Detector::Signals> signal_processor;

  if (detector_model_ptr){
    shared_detector_model_ptr.reset(detector_model_ptr);
    signal_processor.reset(new Detector::Signals(detector_model_ptr));
  }

  //----------------------------------------------------------------------
  // create a header with run info
  ostringstream header;
  header << "# Ran: " << cmdline.command_line() << endl;
  header << "# Used jet definition: " << jet_def.description() << endl;
  header << "# EventGeneration: " << mixer.description() << endl;
  header << subvars.description() << "#" << endl;
  header << "#" << endl;
  if (pu_subtraction)
    header << "# Pileup suibtraction using: " << sk->description() << endl;
  if (detector_model_ptr)
    header << detector_model_ptr->description();
  header << "#------------------------------------------------------------------------" << endl;
  
  //========================================================================
  // prepare the output
  //========================================================================
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
    
  
  //========================================================================
  // event loop
  //========================================================================
  int iev = 0;
  int periodic_iev_output=10;
  int nentries = 0;
  while ( mixer.next_event() && iev < nev ) {
    //----------------------------------------------------------------------
    // increment event number    
    iev++;
    if (iev % periodic_iev_output == 0){
      cout << "Event " << iev << endl;
      if (iev == 15 * periodic_iev_output) periodic_iev_output*=10;
    }
     
    //----------------------------------------------------------------------
    // extract particles from event 
    vector<PseudoJet> full_event = sel_particles(mixer.particles()) ;

    //----------------------------------------------------------------------
    // apply the optional Detector simulation
    if ( !signal_processor->fill(full_event) ) {
      cout << "WARNING: detector simulation failed (likely an empty event or a disabled signal processor). Discarding event " << iev << endl;
       continue;
     }
    full_event = signal_processor->get();
    
    //----------------------------------------------------------------------
    // apply the optional Soft Killer
    if (sk) full_event = (*sk)(full_event);
     
    ClusterSequence cs_full(full_event,jet_def);
    vector<PseudoJet> jets = sel_hard_jets(cs_full.inclusive_jets());

    //----------------------------------------------------------------------
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
