// -*- c++ -*-
#ifndef DETECTORMODEL_DETECTORSIGNALS_H
#define DETECTORMODEL_DETECTORSIGNALS_H

#include "fastjet/PseudoJet.hh"
#include "ParticleInfo.hh"

#include "DetectorExperiment.hh"

#include "Tower.hh"

#include <memory>
#include <vector>
#include <list>

#ifdef _MONITOR_WITH_ROOT

#include <TH1D.h>
#include <TH2D.h>

#define _DECL_MONITOR( T )						\
  TH1D* h_n##T ;							\
  TH1D* h_pt##T ;							\
  TH1D* h_eta##T ;							\
  TH1D* h_phi##T ;							\
  TH2D* d_phi_vs_phi##T ;						\
  TH2D* d_dphi_vs_phi##T ;						\
  TH2D* d_dphi_vs_pt##T;						\
  TH2D* d_esmear_vs_e##T ;						\
  TH2D* d_de_vs_e##T ;							\
  TH2D* d_dpxy_vs_phi##T;						\
  TH2D* d_dpxy_vs_pt##T;						\
  void fillHists##T (const std::vector<fastjet::PseudoJet>& data);	\
  void writeHists##T ()

#define _IMPL_MONITOR( T )						\
  void Detector::Signals::fillHists##T (const std::vector<fastjet::PseudoJet>& data) \
  {									\
    h_n##T ->Fill( static_cast<double>(data.size()) );			\
    for ( auto item : data ) {						\
      h_pt##T ->Fill( item.pt() );					\
      h_eta##T ->Fill( item.pseudorapidity() );				\
      h_phi##T ->Fill( item.phi_std() );				\
    }									\
  }									\
  void Detector::Signals::writeHists##T ()				\
  {									\
    if ( h_n##T != 0 )           { h_n##T ->Write();}			\
    if ( h_pt##T != 0 )          { h_pt##T ->Write(); }			\
    if ( h_eta##T != 0 )         { h_eta##T ->Write(); }			\
    if ( h_phi##T != 0 )         { h_phi##T ->Write(); }			\
    if ( d_phi_vs_phi##T != 0  && d_phi_vs_phi##T ->GetEntries() > 0 )  { d_phi_vs_phi##T ->Write(); } \
    if ( d_dphi_vs_phi##T != 0 && d_dphi_vs_phi##T ->GetEntries() > 0 ) { d_dphi_vs_phi##T ->Write(); } \
    if ( d_dphi_vs_pt##T != 0 && d_dphi_vs_pt##T ->GetEntries() > 0 ) { d_dphi_vs_pt##T ->Write(); } \
    if ( d_esmear_vs_e##T != 0 && d_esmear_vs_e##T ->GetEntries() > 0 ) { d_esmear_vs_e##T ->Write(); } \
    if ( d_de_vs_e##T != 0 && d_de_vs_e##T ->GetEntries() > 0 ) { d_de_vs_e##T ->Write(); } \
    if ( d_dpxy_vs_phi##T != 0 && d_dpxy_vs_phi##T ->GetEntries() > 0 ) { d_dpxy_vs_phi##T ->Write(); } \
    if ( d_dpxy_vs_pt##T != 0 && d_dpxy_vs_pt##T ->GetEntries() > 0 ) { d_dpxy_vs_pt##T ->Write(); } \
  }

#define _WRITE_ALL					\
  writeHists_input();					\
  writeHists_tower();					\
  writeHists_tower_emc();				\
  writeHists_tower_hac();				\
  writeHists_track();					\
  writeHists_muons();					\
  writeHists_noint()

#define _BOOK_HIST( T, N )			\
  h_n##T           = new TH1D( ( N + std::string("/h_n")           + std::string( #T ) ).c_str(), "Number spectrum",        200, 0., 1000. ); \
  h_pt##T          = new TH1D( ( N + std::string("/h_pt")          + std::string( #T ) ).c_str(), "p_{T} spectrum",         200, 0., 1000. ); \
  h_eta##T         = new TH1D( ( N + std::string("/h_eta")         + std::string( #T ) ).c_str(), "#eta spectrum",          100, -5., 5.   ); \
  h_phi##T         = new TH1D( ( N + std::string("/h_phi")         + std::string( #T ) ).c_str(), "#phi spectrum",          128,  -3.2, 3.2  ); \
  d_phi_vs_phi##T  = new TH2D( ( N + std::string("/d_phi_vs_phi")  + std::string( #T ) ).c_str(), "#phi_{smeared} vs #phi", 128, -3.2, 3.2, 128, -3.2, 3.2); \
  d_dphi_vs_phi##T = new TH2D( ( N + std::string("/d_dphi_vs_phi") + std::string( #T ) ).c_str(), "#Delta#phi vs #phi",     128, -3.2, 3.2, 500, -1.0, 1.0 ); \
  d_dphi_vs_pt##T  = new TH2D( ( N + std::string("/d_dphi_vs_pt")  + std::string( #T ) ).c_str(), "#Delta#phi vs log(p_{T})", 200, 0.01, 1000., 500, -1., 1. ); \
  d_esmear_vs_e##T = new TH2D( ( N + std::string("/d_esmear_vs_e") + std::string( #T ) ).c_str(), "E_{smeared} vs E",       200, 0., 1000., 200, 0., 1000. ); \
  d_de_vs_e##T     = new TH2D( ( N + std::string("/d_de_vs_e")     + std::string( #T ) ).c_str(), "#DeltaE_{smeared} vs E", 200, 0., 1000., 200,-2.5,2.5   ); \
  d_dpxy_vs_phi##T = new TH2D( ( N + std::string("/d_dpxy_vs_phi") + std::string( #T ) ).c_str(), "#Delta(p_{x,y}) vs #phi", 128, -3.2, 3.2, 300,-15.,15.   ); \
  d_dpxy_vs_pt##T  = new TH2D( ( N + std::string("/d_dpxy_vs_pt")  + std::string( #T ) ).c_str(), "#Delta(p_{x,y}) vs p_{T}", 200, 0., 1000., 300,-15.,15.  )

#define _FILL_SCATTER( T, P, J )				\
  d_phi_vs_phi##T  ->Fill( P.phi_std(), J.phi_std());		\
  d_dphi_vs_phi##T ->Fill( P.phi_std(),TowerGrid::phi_std( J.phi_std() - P.phi_std())); \
  d_dphi_vs_pt##T ->Fill( std::log10( P.pt() ),TowerGrid::phi_std( J.phi_std() - P.phi_std())); \
  d_esmear_vs_e##T ->Fill( P.e(), J.e());				\
  d_de_vs_e##T     ->Fill( P.e(),( J.e() - J.e() )/ P.e() );		\
  d_dpxy_vs_phi##T ->Fill( P.phi_std(), J.px() - P.px() );		\
  d_dpxy_vs_phi##T ->Fill( P.phi_std(), J.py() - P.py() );		\
  d_dpxy_vs_pt##T  ->Fill( P.pt(), J.px() - P.px() );			\
  d_dpxy_vs_pt##T  ->Fill( P.pt(), J.py() - P.py() ) 

#endif

namespace Detector {
  ///@brief Signal generator for detector-like signals
  ///
  /// This object collects incoming particles into appropriate representations of detector signals.
  /// These signals include calorimeter towers, charged tracks, and muon tracks. All non-interacting
  /// particles are ignored/not processed. The simulated detector features are implemented in @c
  /// Detector::Experiment object, which ideally provides acceptance cuts and smearing.  
  class Signals
  {
  public:
    
    ///@brief Constructor
    ///
    /// Will self-disable if invalid pointer is given.
    Signals(Detector::Experiment* eptr=0);
    ///@brief Destructor
    virtual ~Signals();
    
    ///@name Collect input
    ///@{
    virtual bool fill(const std::vector<fastjet::PseudoJet>& input);
    virtual bool operator()(const std::vector<fastjet::PseudoJet>& input);
    ///@}
    
    ///@name Access reconstructed event
    ///@{
    std::vector<fastjet::PseudoJet> get()               const;  ///<@brief Full event
    std::vector<fastjet::PseudoJet> getTowers()         const;  ///<@brief CaloTowers
    std::vector<fastjet::PseudoJet> getTracks()         const;  ///<@brief Charged tracks
    std::vector<fastjet::PseudoJet> getPFlow()          const;  ///<@brief Particle flow objects
    std::vector<fastjet::PseudoJet> getMuons()          const;  ///<@brief Muons
    std::vector<fastjet::PseudoJet> getNonInteracting() const;  ///<@brief Non-interacting particles 
    ///@}
    
    ///@brief Reset internal caches
    void reset();

    ///@brief Finalize
    ///
    /// Any final statistics collection and clean-up
#ifndef _MONITOR_WITH_ROOT
    bool finalize();
#else
    bool finalize(const std::string& fn);
#endif

  private:
    
    ///@name Process control and output stores
    ///@{
    std::unique_ptr<Tower> _emcTowers;
    std::unique_ptr<Tower> _hacTowers;
    bool _enabled;
    ///@}

    ///@name Internal output cache for reconstructed/smeared signals
    ///@{
    std::vector<fastjet::PseudoJet>  _fullevent;       ///<@brief Full event 
    std::vector<fastjet::PseudoJet>  _calotowers;      ///<@brief Calorimeter towers
    std::vector<fastjet::PseudoJet>  _emc_towers;      ///<@brief EMC towers
    std::vector<fastjet::PseudoJet>  _hac_towers;      ///<@brief HAC towers
    std::vector<fastjet::PseudoJet>  _tracks;          ///<@brief Tracks
    std::vector<fastjet::PseudoJet>  _muons;           ///<@brief Muons
    std::vector<fastjet::PseudoJet>  _noninteracting;  ///<@brief Non-interacting particles
    ///@}

    ///@name Internal input collection
    ///@{
    ///@brief Cache with collections of collections 
    struct Cache
    {
    public:
      std::list<fastjet::PseudoJet> particleInput; ///<@brief All detectable particles
      std::list<fastjet::PseudoJet> trackInput;    ///<@brief Charged particle input for tracking
      std::list<fastjet::PseudoJet> muonInput;     ///<@brief Muon input
      std::list<fastjet::PseudoJet> emcTowerInput; ///<@brief EMC calorimeter towers
      std::list<fastjet::PseudoJet> hacTowerInput; ///<@brief HAC calorimeter towers
      ///@brief Common clear
      void clear() { particleInput.clear(); trackInput.clear(); muonInput.clear(); emcTowerInput.clear(); hacTowerInput.clear(); }
      ///@brief Constructor 
      Cache() { }
    };
    ///@brief Input cache
    Cache _cache;
    ///@}

    ///@name External object references and instances
    ///@{    
    std::unique_ptr<Detector::Experiment> _experiment;  ///<@brief Experiment model 
    ///@}

    ///@name Helpers
    ///@{
    bool               _filterInput(const std::vector<fastjet::PseudoJet>& input);
    fastjet::PseudoJet _getInput(const fastjet::PseudoJet& particle,ParticleInfo::type_t t);
    bool               _fillTowers();
    bool               _emcSignalPrep(const fastjet::PseudoJet& particle,bool& isLost);
    bool               _hacSignalPrep(const fastjet::PseudoJet& particle,bool& isLost);
    bool               _fillTracks();
    bool               _fillMuons();
    bool               _collectTowerOutput();
    bool               _collectTrackOutput();
    bool               _collectMuonOutput();
    bool               _setup();
    ///@}
#ifdef _MONITOR_WITH_ROOT
    ///@name Monitoring
    ///@{
    void book();                  ///<@brief Book histograms
    _DECL_MONITOR( _input );      ///<@brief Input monitoring histograms, fill, write
    _DECL_MONITOR( _tower );      ///<@brief Tower monitoring histograms, fill, write
    _DECL_MONITOR( _tower_emc );  ///<@brief Tower monitoring histograms, fill, write
    _DECL_MONITOR( _tower_hac );  ///<@brief Tower monitoring histograms, fill, write
    _DECL_MONITOR( _track );      ///<@brief Track monitoring histograms, fill, write
    _DECL_MONITOR( _muons );      ///<@brief Muon monitoring histograms, fill, write
    _DECL_MONITOR( _noint );      ///<@brief Non-interacting monitoring histograms, fill, write
    ///@}
#endif
  };
}

inline std::vector<fastjet::PseudoJet> Detector::Signals::get()               const { return _fullevent; }
inline std::vector<fastjet::PseudoJet> Detector::Signals::getTowers()         const { return _calotowers; }
inline std::vector<fastjet::PseudoJet> Detector::Signals::getTracks()         const { return _tracks; }
inline std::vector<fastjet::PseudoJet> Detector::Signals::getMuons()          const { return _muons; }
inline std::vector<fastjet::PseudoJet> Detector::Signals::getNonInteracting() const { return _noninteracting; }
inline std::vector<fastjet::PseudoJet> Detector::Signals::getPFlow()          const
{ 
  std::vector<fastjet::PseudoJet> pf; 
  pf.reserve(_calotowers.size()+_tracks.size());
  pf.insert(pf.end(),_calotowers.begin(),_calotowers.end());
  pf.insert(pf.end(),_tracks.begin(),_tracks.end());
  return pf;
}
#endif
