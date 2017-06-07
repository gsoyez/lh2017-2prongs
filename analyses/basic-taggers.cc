///////////////////////////////////////////////////////////////////////////////////////////////////
//
/// Description to be added...
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "EventMixer.hh"
#include "CmdLine.hh"
#include "PU14.hh"
#include "fastjet/ClusterSequence.hh"

#include <fastjet/tools/Filter.hh>
#include <fastjet/contrib/EnergyCorrelator.hh>

#include "SimpleHist.hh"
#include <fstream>

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
  
  // selection of the hardest jets
  Selector sel_hard_jets = SelectorNHardest(2) * SelectorPtMin(jet_ptmin) * SelectorAbsRapMax(jet_rapmax);

  ostringstream header;
  header << "# Used jet definition: " << jet_def.description() << endl;
  
  //-------------------------------------------------------------------------
  // set up substructure tools

  // ATLAS setup
  double atlas_Rtrim = cmdline.value("-atlas.Rtrim", 0.2);
  double atlas_fcut  = cmdline.value("-atlas.fcut", 0.05);
  Filter atlas_trimmer(JetDefinition(kt_algorithm, atlas_Rtrim), SelectorPtFractionMin(atlas_fcut));

  double atlas_D2beta = cmdline.value("-atlas.D2beta", 1.0);
  contrib::EnergyCorrelatorD2 atlas_D2(atlas_D2beta);
  
  //-------------------------------------------------------------------------
  // set up structures for the results/distributions
  SimpleHist atlas_groomed_mass_distrib(0.0, 300.0, 5.0);
  SimpleHist atlas_D2_distrib(0.0, 5.0, 0.1);
  
  //------------------------------------------------------------------------
  // create mixer that will construct events by mixing hard and pileup
  // events read from files given from command line using 
  // -hard hard_events_file(.gz) -pileup pileup_events_file(.gz)
  EventMixer mixer(&cmdline);  
  
  assert(cmdline.all_options_used());

  // loop over events
  int iev = 0;
  while ( mixer.next_event() && iev < nev ) {
     // increment event number    
     iev++;
     
     // extract particles from event 
     vector<PseudoJet> full_event = mixer.particles() ;
     cout << "\nEvent " << iev << endl;
     cout << "nPU = " << mixer.npu() << endl;
     
     // cluster hard event only
     vector<PseudoJet> hard_event, pileup_event;
     SelectorIsHard().sift(full_event, hard_event, pileup_event); // this sifts the full event into two vectors
                                                                  // of PseudoJet, one for the hard event, one for the pileup
     ClusterSequence cs_hard(hard_event,jet_def);
     vector<PseudoJet> jets = sel_hard_jets(cs_hard.inclusive_jets());

     // loop over the jets
     cout << "jet_mass groomed_mass  D2" << endl;
     for (const PseudoJet &jet : jets){
       // apply trimming to get the groomed jet
       PseudoJet atlas_groomed_jet = atlas_trimmer(jet);
       atlas_groomed_mass_distrib.add_entry(atlas_groomed_jet.m());
       
       // compute D2 on the GROOMED jet
       // TODO: check the behaviour for 1-particle jets
       double atlas_D2_value = atlas_D2(atlas_groomed_jet);
       atlas_D2_distrib.add_entry(atlas_D2_value);

       cout << jet.m() << " " << atlas_groomed_jet.m() << " " << atlas_D2_value << endl;
     } // end of loop over jets
  } // end of loop over events

  // save results
  ofstream ostr(outname.c_str());
  ostr << "# Ran: " << cmdline.command_line() << endl;
  ostr << header.str();
  ostr << "#" << endl;
  ostr << "# ATLAS_groomed_mass" << endl;
  output(atlas_groomed_mass_distrib, &ostr);
  ostr << endl << endl;
  ostr << "# ATLAS_D2" << endl;
  output(atlas_D2_distrib, &ostr);
  ostr << endl << endl;

  return 0;
}  
