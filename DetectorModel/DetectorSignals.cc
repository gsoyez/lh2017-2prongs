
#include "DetectorSignals.hh"

#include "fastjet/Selector.hh"

#include <cstdio>

#ifndef _SIGNAL_CACHE_RESERVE 
#define _SIGNAL_CACHE_RESERVE 50000
#endif

Detector::Signals::Signals(Experiment* exp)
  : _emcTowers(new Tower())
  , _hacTowers(new Tower())
  , _enabled(false)
{ 
  if ( exp != 0 ) { 
    _experiment = std::unique_ptr<Experiment>(exp->clone());
    _emcTowers  = std::unique_ptr<Tower>(new Tower(_experiment->getEMCTowerGrid()));
    _hacTowers  = std::unique_ptr<Tower>(new Tower(_experiment->getHACTowerGrid()));
    _enabled    = true;
    this->_setup();
  } else {
    printf("Detector::Signals::Signals(...) ERROR invalid pointer to Detector::Experiment object\n");
  }
}

Detector::Signals::~Signals()
{ }

bool Detector::Signals::_setup()
{
  _fullevent.reserve(      _SIGNAL_CACHE_RESERVE );
  _calotowers.reserve(     _SIGNAL_CACHE_RESERVE );
  _tracks.reserve(         _SIGNAL_CACHE_RESERVE );
  _muons.reserve(          _SIGNAL_CACHE_RESERVE );
  _noninteracting.reserve( _SIGNAL_CACHE_RESERVE );
  return true;
}

bool Detector::Signals::operator()(const std::vector<fastjet::PseudoJet>& input) { return this->fill(input); }

bool Detector::Signals::fill(const std::vector<fastjet::PseudoJet>& input)
{
  // check enabled
  if ( ! _enabled ) { return false; }
 
  // clear cache
  _cache.clear();

  // clear previous event
  this->reset(); 

  // filter non-interacting particles (and muons) and fill detector input caches
  if ( !this->_filterInput(input) ) { return false; } // empty after filter 

  // process input
  switch ( _experiment->recoMode() )
    {
    case Detector::CaloTower:
      this->_fillTowers();
      this->_collectTowerOutput();
      this->_collectMuonOutput();
      break;
    case Detector::RecoTrack:
      this->_fillTracks();
      this->_collectTrackOutput();
      this->_collectMuonOutput();
      break;
    case Detector::ParticleFlowIncl:
    case Detector::ParticleFlowExcl:
      this->_fillTracks();       // handle the reco track input
      this->_fillTowers();       // catch the rejected input
      this->_collectTrackOutput();
      this->_collectTowerOutput();
      this->_collectMuonOutput();
      break;
    case Detector::RecoMuon:
      printf("\n");
      printf("DetectorSignals::fill(...) #-----------------------------------------------------------------------------#\n");
      printf("DetectorSignals::fill(...) # WARNING reconstruction mode \042RecoMuon\042 not yet implemented, module disabled #\n");
      printf("DetectorSignals::fill(...) #-----------------------------------------------------------------------------#\n");
      printf("\n");
      _enabled = false;
      break;
    case Detector::UnknownMode:
    default:
      printf("\n");
      printf("DetectorSignals::fill(...) #------------------------------------------------------#\n");
      printf("DetectorSignals::fill(...) # WARNING unknown reconstruction mode, module disabled #\n");
      printf("DetectorSignals::fill(...) #------------------------------------------------------#\n");
      printf("\n");
      _enabled = false;
      break;
    }
  return _enabled;
}

bool Detector::Signals::_fillMuons()
{
  // no reco yet, just acceptance emulation
  for ( auto muon : _cache.muonInput ) {
    if ( _experiment->muoAccept(muon,false) ) {
      ParticleInfo::type_t t = muon.user_info<ParticleInfo::base_t>().vertex() == 0 
	? ParticleInfo::HardScatter | ParticleInfo::Muon
	: ParticleInfo::Pileup      | ParticleInfo::Muon;
      _muons.push_back(this->_getInput(muon,t)); 
    }    
  }
  return !_muons.empty();
}

bool Detector::Signals::_fillTracks()
{
  auto fparticle(_cache.particleInput.begin());
  while ( fparticle != _cache.particleInput.end() ) {
    auto particle = *fparticle;
    if ( ParticleInfo::isCharged(particle) ) {
      if ( _experiment->trkAccept(particle,false) ) { 
	fastjet::PseudoJet fj = _experiment->smearedTrack(particle);
	// add signal information
	ParticleInfo::type_t t = particle.user_info<ParticleInfo::base_t>().vertex() == 0 
	  ? ParticleInfo::HardScatter | ParticleInfo::Track 
	  : ParticleInfo::Pileup      | ParticleInfo::Track;
	fj.set_user_info(new ParticleInfo(t,particle.user_info<ParticleInfo::base_t>()));
	_cache.trackInput.push_back(fj);
	_tracks.push_back(fj);
	fparticle = _cache.particleInput.erase(fparticle);
      } else {
	++fparticle;
      } // accepted track
    } else {
      ++fparticle;
    } // charged particle
  } // particle loop
  //
  return !_tracks.empty();
}

bool Detector::Signals::_fillTowers()                  /////////////////////////////////////////////////////////////////////
{                                                      // First, it is tried to add particles with EM shower character to //
  // pick up particles                                 // the EM calorimeter signal. If they are (1) out of coverage or   //
  auto fparticle(_cache.particleInput.begin());        // (2) they are bend away from the EMC in a magnetic field, they   // 
  while ( fparticle != _cache.particleInput.end() ) {  // are handed to the HAC in case (1), because the HAC has larger   //
    auto particle = *fparticle;                        // coverage (eta only). In case (2), they are left in the input    //
    bool isLost(false); bool isLeft(true);             // collection for downstream detectors, if any... Particles with   //           
    if ( ParticleInfo::isEM(particle) ) {              // hadronic showers are handed to the HAC. If they do not reach    //
      // particles measured with the EMC               // it, they are treated like the ones from case (1).               //
      if ( this->_emcSignalPrep(particle,isLost) )  {  /////////////////////////////////////////////////////////////////////
	fparticle = _cache.particleInput.erase(fparticle); isLeft = false;   // removed from input -> added to EMC signal 
      } else {
	if ( !isLost && this->_hacSignalPrep(particle,isLost) ) { 
	  fparticle = _cache.particleInput.erase(fparticle); isLeft = false; // removed from input -> added to HAC signal
	}
      } // end of calorimeter signal collection for EM particles
    } else {
      // particles measured with HAC
      if ( this->_hacSignalPrep(particle,isLost) ) { fparticle = _cache.particleInput.erase(fparticle); isLeft = false; } // removed from input -> added to HAC signal 
    } // end of calorimeter signal collection for HAD particles
    if ( isLeft ) { ++fparticle; } // particle did not contribute to calorimeter signal, leave in input collection for downstream detetors
  } // input signal loop
  return !_emcTowers->empty() || !_hacTowers->empty();
}

fastjet::PseudoJet Detector::Signals::_getInput(const fastjet::PseudoJet& particle,ParticleInfo::type_t t)
{
  fastjet::PseudoJet fj(fastjet::PtYPhiM(particle.pt(),particle.rap(),particle.phi(),particle.m()));
  if ( particle.has_user_info<ParticleInfo::base_t>() ) { 
    fj.set_user_info(new ParticleInfo(t,particle.user_info<ParticleInfo::base_t>())); 
  }
  return fj;
}

bool Detector::Signals::_emcSignalPrep(const fastjet::PseudoJet& particle,bool& isLost)
{
  bool generatedSignal(false);
  if ( _experiment->emcAccept(particle,true) ) {
    // check  if  particle makes it into the EMC
    fastjet::PseudoJet fj;
    if ( _experiment->adjustPhi(particle,fj) ) {
      // smear with nominal energy resolution
      fj *= _experiment->emcResoSmearing(particle)/particle.e();
      // add signal information
      ParticleInfo::type_t t = particle.user_info<ParticleInfo::base_t>().vertex() == 0 
	? ParticleInfo::HardScatter : ParticleInfo::Pileup;
      fj.set_user_info(new ParticleInfo(t,particle.user_info<ParticleInfo::base_t>()));
      // collect for projection to towers
      _cache.emcTowerInput.push_back(fj);
      _emcTowers->add(fj);
      generatedSignal = true;
    } else {
      isLost = true; // particle does not reach calorimeter -> complete loss!
    }
  } // EM particle
  return generatedSignal;
}

bool Detector::Signals::_hacSignalPrep(const fastjet::PseudoJet& particle,bool& isLost)
{
  bool generatedSignal(false);
  if ( _experiment->hacAccept(particle,true) ) {
    // check  if  particle makes it into the HAC
    fastjet::PseudoJet fj;
    if ( _experiment->adjustPhi(particle,fj) ) {
      // smear pT using energy resolution - HAC has wider acceptance, incoming particle may be EM!!!!
      if ( ParticleInfo::isEM(particle) ) { 
	fj*=_experiment->emcResoSmearing(particle)/particle.e();
      } else{ 
        fj*=_experiment->hacResoSmearing(particle)/particle.e();
      }
      // add signal information
      ParticleInfo::Type t = particle.user_info<ParticleInfo::base_t>().vertex() == 0 ? ParticleInfo::HardScatter : ParticleInfo::Pileup;
      fj.set_user_info(new ParticleInfo(t,particle.user_info<ParticleInfo::base_t>()));
      // collect for projection to towers
      _cache.hacTowerInput.push_back(fj);
      _hacTowers->add(fj);
      generatedSignal = true;
    } else {
      isLost = true; // particle does not reach calorimeter -> complete loss!
    }
  }
  return generatedSignal;
}

bool Detector::Signals::_collectTowerOutput()
{
  size_t isize(_fullevent.size());
  double ptmin(0.), ptmax(0.);
  _experiment->selectedPtRange(Detector::EmCalorimeter,ptmin,ptmax);
  std::vector<fastjet::PseudoJet> etowers(_emcTowers->towers(fastjet::SelectorPtRange(ptmin,ptmax)));
  _experiment->selectedPtRange(Detector::HadCalorimeter,ptmin,ptmax);
  std::vector<fastjet::PseudoJet> htowers(_hacTowers->towers(fastjet::SelectorPtRange(ptmin,ptmax)));
  _calotowers.insert(_calotowers.end(),etowers.begin(),etowers.end());
  _calotowers.insert(_calotowers.end(),htowers.begin(),htowers.end());
  _fullevent.insert(_fullevent.end(),_calotowers.begin(),_calotowers.end());
  return _fullevent.size()-isize > 0;
}

bool Detector::Signals::_collectTrackOutput()
{
  size_t isize(_fullevent.size());
  _fullevent.insert(_fullevent.end(),_tracks.begin(),_tracks.end());
  return _fullevent.size()-isize > 0;
}

bool Detector::Signals::_collectMuonOutput()
{
  size_t isize(_fullevent.size());
  _fullevent.insert(_fullevent.end(),_muons.begin(),_muons.end());
  return _fullevent.size()-isize > 0;
}

void Detector::Signals::reset()
{
  _emcTowers->reset();
  _hacTowers->reset();
  _fullevent.clear();
  _calotowers.clear();
  _tracks.clear();
  _muons.clear();
  _noninteracting.clear();
  _cache.clear();
}

bool Detector::Signals::_filterInput(const std::vector<fastjet::PseudoJet>& input)
{
  // extract particles                                            ///////////////////////////////////
  for ( auto particle : input ) {                                 // Select inputs into categories //
    // sort after interaction                                     ///////////////////////////////////
    if ( ParticleInfo::isIonizing(particle) ) { 
      _cache.muonInput.push_back(particle); 
    } else if ( ParticleInfo::isNonInteracting(particle) ) {
      // no need to prepare inputs - no detector!
      _noninteracting.push_back(this->_getInput(particle,ParticleInfo::NonInteracting));
    } else {
      // inputs to detector response
      _cache.particleInput.push_back(particle);
    } // particle type/flavor check
  } // loop on input
  // true if input has been extracted
  return !_cache.particleInput.empty() || !_cache.muonInput.empty();
}
