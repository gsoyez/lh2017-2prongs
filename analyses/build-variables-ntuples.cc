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
#include <fastjet/tools/GridMedianBackgroundEstimator.hh>  //< for pileup subtraction
#include <fastjet/tools/Subtractor.hh>    //< for pileup subtraction
#include <fastjet/contrib/SoftKiller.hh>  //< for pileup subtraction
#include <fastjet/contrib/SoftKiller.hh>  //< for pileup subtraction
#include <iomanip>

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
  bool debug = cmdline.present("-debug");
  
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
  double sk_grid_size = cmdline.value("-sk.grid", 0.5);
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
  bool no_magnetic_field = cmdline.present("-noB");

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
  SharedPtr<GridMedianBackgroundEstimator> gmbge_emc;
  SharedPtr<GridMedianBackgroundEstimator> gmbge_hac;
  SharedPtr<BackgroundRescalingYPolynomial> rescaling;
  SharedPtr<Subtractor> area_median_subtractor;

  if (detector_model_ptr){
    // option to switch off the magnetic field
    if (no_magnetic_field)
      detector_model_ptr->turnMagneticFieldOff();

    // with pileup subtraction we'll need an area-median (before we run the Soft Killer)
    if (pu_subtraction){
      gmbge_emc.reset(new GridMedianBackgroundEstimator(detector_model_ptr->getEMCTowerGrid().etaMax(), 0.55));
      gmbge_hac.reset(new GridMedianBackgroundEstimator(particle_rapmax, 0.55));
      //if (buildATLAS)
      // set for ATLAS with mupu=50.0
      rescaling.reset(new BackgroundRescalingYPolynomial(1.0, 0, -0.0276383, 0, 0.000237878));
      gmbge_emc->set_rescaling_class(rescaling.get());
      gmbge_hac->set_rescaling_class(rescaling.get());
    }
    
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
    if (detector_model_ptr){
      if ( !signal_processor->fill(full_event) ) {
        cout << "WARNING: detector simulation failed (likely an empty event or a disabled signal processor). Discarding event " << iev << endl;
        continue;
      }

      // with pileup subtraction, we first apply an area--median subtraction
      //TODO: subtract EMC and HAC separately
      if (pu_subtraction){
        Selector sel_hard = SelectorIsHard();

        // get the tracks (and muons) (and apply CHS)
        vector<PseudoJet> tracks = sel_hard(signal_processor->getTracks());
        vector<PseudoJet> muons  = sel_hard(signal_processor->getMuons());

        full_event = tracks;
        if (debug) cout << "found " << setw(5) << tracks.size() << " tracks (event: " << full_event.size() << ")" << endl;
        full_event.insert(full_event.end(), muons.begin(), muons.end());
        if (debug) cout << "found " << setw(5) << muons.size() << " muons  (event: " << full_event.size() << ")" << endl;
        
        // now get and subtract the towers
        //vector<PseudoJet> all_towers = signal_processor->getTowers();
        // we separate EMC and HAC colours
        vector<PseudoJet> emc_towers = signal_processor->getEMCTowers();
        double emc_cell_area = detector_model_ptr->getEMCTowerGrid().etaWidth() * detector_model_ptr->getEMCTowerGrid().phiWidth();
        gmbge_emc->set_particles(emc_towers);
        for (const auto &tower : emc_towers){
          double ptsub = tower.pt() - gmbge_emc->rho(tower)*emc_cell_area;
          if (ptsub>0) full_event.push_back(PtYPhiM(ptsub, tower.rap(), tower.phi()));
        }
        if (debug) cout << "found " << setw(5) << emc_towers.size() << " EMC tw (event: " << full_event.size() << ")"
                        << ") [rho = " << gmbge_emc->rho(PtYPhiM(1.0,0.0,0.0)) << "]" << endl;
        
        vector<PseudoJet> hac_towers = signal_processor->getHACTowers();
        double hac_cell_area = detector_model_ptr->getHACTowerGrid().etaWidth() * detector_model_ptr->getHACTowerGrid().phiWidth();
        gmbge_hac->set_particles(hac_towers);
        for (const auto &tower : hac_towers){
          double ptsub = tower.pt() - gmbge_hac->rho(tower)*hac_cell_area;
          if (ptsub>0) full_event.push_back(PtYPhiM(ptsub, tower.rap(), tower.phi()));
        }        
        if (debug) cout << "found " << setw(5) << hac_towers.size() << " HAC tw (event: " << full_event.size()
                        << ") [rho = " << gmbge_hac->rho(PtYPhiM(1.0,0.0,0.0)) << "]" << endl;
      } else {
        // we can directly get the full event
        full_event = signal_processor->get();
      }
      
    }
    
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
