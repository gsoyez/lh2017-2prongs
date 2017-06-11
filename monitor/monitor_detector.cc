///////////////////////////////////////////////////////////////////////////////////////////////////
//
/// Description to be added...
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "EventMixer.hh"
#include "CmdLine.hh"

#include "Detector.hh"

using namespace std;
using namespace fastjet;

int main (int argc, char ** argv) {
  CmdLine cmdline(argc,argv);
  // inputs read from command line
  int nev = cmdline.value<int>("-nev",1);                        // first argument: command line option; second argument: default value
#ifdef _MONITOR_WITH_ROOT
  string outname = cmdline.value<string>("-out","monitor.root"); // 
#else 
  string outname = cmdline.value<string>("-out"); // 
#endif
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
  
  //------------------------------------------------------------------------
  // create mixer that will construct events by mixing hard and pileup
  // events read from files given from command line using 
  // -hard hard_events_file(.gz) -pileup pileup_events_file(.gz)
  EventMixer mixer(&cmdline);
  
  //------------------------------------------------------------------------
  // loop over events
  int iev = 0;
  int periodic_iev_output=10;
  cout << "Start running for " << nev << " events" << endl; 
  while ( mixer.next_event() && iev < nev ) {
     // increment event number    
     iev++;
     cout << "Event " << iev << endl;
     
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
  } // end of loop over events

#ifdef _MONITOR_WITH_ROOT
  signal_processor->finalize(outname);
#else
  //  signal_processor->finalize();
#endif
  return 0;
}  
