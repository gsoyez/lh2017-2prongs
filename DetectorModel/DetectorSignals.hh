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
    bool               _emcSignalPrep(const fastjet::PseudoJet& particle);
    bool               _hacSignalPrep(const fastjet::PseudoJet& particle);
    bool               _fillTracks();
    bool               _fillMuons();
    bool               _collectTowerOutput();
    bool               _collectTrackOutput();
    bool               _collectMuonOutput();
    bool               _setup();
    ///@}
  };
}
#endif
