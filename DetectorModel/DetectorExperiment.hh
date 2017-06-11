// -*- c++ -*-
#ifndef DETECTORMODEL_DETECTOREXPERIMENT_H
#define DETECTORMODEL_DETECTOREXPERIMENT_H

#include "fastjet/PseudoJet.hh"

#include "ParticleInfo.hh"
#include "TowerGrid.hh"

#include "DetectorDescription.hh"
#include "DetectorTags.hh"

#include <string>
#include <map>
#include <random>

#include <limits>

#include <boost/tuple/tuple.hpp>

///@brief Detector geometry and descriptions
namespace Detector {
  ///@brief Experimental setup 
  ///
  /// Very rough performance considerations using expectations from a given experiment
  class Experiment
  {
  public:
    
    ///@name Range and look-up
    ///@{
    typedef std::pair<double,double>                          range_t;    ///<@brief Useful for e.g. bin descriptions
    typedef std::map<range_t,double>                          lookup_t;   ///<@brief Lookup table using a value range key
    typedef ParticleInfo::pdg_t                               pdg_t;
    typedef ParticleInfo::pdglist_t                           pdglist_t;
    typedef unsigned int                                      detector_t;
    typedef boost::tuples::tuple<pdg_t,detector_t>            key_t;
    typedef boost::tuples::tuple<double,double,double,double> accept_t;
    ///@}
    
    ///@brief Default constructor
    ///
    /// Experiments can be set up with a name.
    Experiment(const std::string& name="Generic");
    ///@brief Destructor
    virtual ~Experiment();
    
    ///@name Set calorimeter parameters
    ///@{
    void setEtaCoverage(double etamin,double etamax);                            ///<@brief Set maximum eta coverage 
    void setEMCTowerGrid(const TowerGrid& tgrid);                                ///<@brief Set ElectroMagnetic Calorimeter (EMC) tower readout grid
    void setHACTowerGrid(const TowerGrid& tgrid);                                ///<@brief Set HAdronic Calorimeter (HAC) tower readout grid
    ///@brief Set nominal (relative) energy resolution in EMC
    ///
    /// Defined by three parameters, one for the stochastic/sampling term (@f$ a @f$), the noise term (@f$ b @f$), and the constant term (@f$ c @f$).
    /// The resolution is calculated using  
    ///@f[
    ///    \sigma/E = \sqrt{a^{2}/E+b^{2}/E^{2}+c^{2}}\,.
    ///@f]
    void setEMCEnergyResolution(double a=0.1,double b=0.,double c=0.005);
    ///@brief Set nominal (relative) energy resolution in HAC
    ///
    /// Defined by three parameters, one for the stochastic/sampling term (@f$ a @f$), the noise term (@f$ b @f$), and the constant term (@f$ c @f$).
    /// The resolution is calculated using  
    ///@f[
    ///    \sigma_{E}/E = \sqrt{a^{2}/E+b^{2}/E^{2}+c^{2}}\,.
    ///@f]
    void setHACEnergyResolution(double a=0.6,double b=0.,double c=0.05);
    ///@brief Set the acceptance for EMC
    ///
    /// The acceptance is specified for a given particle in terms of a @f$ p_{\rm T} @f$ threshold and an @f$ \eta @f$ range. 
    ///
    /// @warning Setting these parameters overwrites the default settings. This is not recommended unless you absolutely know
    /// what you are doing.
    void setEMCAcceptance(int pdg,double ptmin,double etamin,double etamax);
    ///@brief Set the acceptance for HAC
    ///
    /// The acceptance is specified for a given particle in terms of a @f$ p_{\rm T} @f$ threshold and an @f$ \eta @f$ range.
    ///
    /// @warning Setting these parameters overwrites the default settings. This is not recommended unless you absolutely know
    /// what you are doing.
    void setHACAcceptance(int pdg,double ptmin,double etamin,double etamax);
    ///@}

    ///@name Setting the tracking parameters
    ///@{
    ///@brief Set tracking resolution parameters
    ///
    /// The tracking resolution is given as fluctution of the reconstructed @f$ p_{\rm T} @f$ (no directional smearing outside of the magnetic field effect).
    /// The nominal resolution is calculated as
    ///@f[
    ///   \sigma_{p_{\rm T}}/p_{\rm T} = \sqrt{a^{2} p_{\rm T}^{2} + b^{2}}\,.
    ///@f] 
    void setTrackingResolution(double a=5e-04,double b=0.01);
    void setTrackingAcceptance(int pdg,double ptmin,double ptmax,double etamin,double etamx);
    ///@}

    void setMuonAcceptance(int pdg,double ptmin,double ptmax,double etamin,double etamx);

    const TowerGrid& getEMCTowerGrid() const;
    const TowerGrid& getHACTowerGrid() const;

    virtual double emcResoSmearing(double e);
    virtual double emcResoSmearing(const fastjet::PseudoJet& pjet);
    virtual double hacResoSmearing(double e);
    virtual double hacResoSmearing(const fastjet::PseudoJet& pjet);
    virtual double trkResoSmearing(double pT);
    virtual double trkResoSmearing(const fastjet::PseudoJet& pjet);

    virtual fastjet::PseudoJet smearedTrack(const fastjet::PseudoJet& pjet);
    virtual fastjet::PseudoJet smearedCaloSignal(const fastjet::PseudoJet& pjet);

    virtual bool emcAccept(const fastjet::PseudoJet& signal,bool etaOnly);
    virtual bool hacAccept(const fastjet::PseudoJet& signal,bool etaOnly);
    virtual bool trkAccept(const fastjet::PseudoJet& signal,bool etaOnly);
    virtual bool muoAccept(const fastjet::PseudoJet& signal,bool etaOnly);
    virtual bool accept(DetectorTag t,const fastjet::PseudoJet& signal,bool etaOnly);
    virtual bool accept(const fastjet::PseudoJet& signal,bool etaOnly);

    virtual bool adjustPhi(const fastjet::PseudoJet& ptrack,fastjet::PseudoJet& pmodtrack);
    fastjet::PseudoJet applyMagneticField(const fastjet::PseudoJet& ptrack);
 
    std::string description();

    const std::string& name()     const { return _name; }
    unsigned int       tag()      const { return _tag; }
    unsigned int       recoMode() const { return _recomode; }

    virtual Experiment* clone();

    virtual bool selectedPtRange(detector_t t,double& ptmin,double& ptmax);
    virtual bool selectedEtaRange(detector_t t,double& etamin,double& etamax); 

  protected:

    std::string  _name;
    unsigned int _tag;
    unsigned int _recomode;

    double    _solenoidField;
    double    _cavityRadius;
    Detector::Geometry::Circle _cavityEnvelope;
    double    _trkPtResoA;
    double    _trkPtResoA2;
    double    _trkPtResoB;
    double    _trkPtResoB2;
    double    _trkPtMin;
    double    _trkPtMax;
    double    _trkEtaMin;
    double    _trkEtaMax; 
    double    _emcResoA;
    double    _emcResoA2;
    double    _emcResoB;
    double    _emcResoB2;
    double    _emcResoC;
    double    _emcResoC2;
    double    _emcPtMin;
    double    _emcPtMax;
    double    _emcEtaMin;
    double    _emcEtaMax;
    int       _emcEtaBin;
    int       _emcPhiBin;
    double    _hacResoA;
    double    _hacResoA2;
    double    _hacResoB;
    double    _hacResoB2;
    double    _hacResoC;
    double    _hacResoC2;
    double    _hacPtMin;
    double    _hacPtMax;
    double    _hacEtaMin;
    double    _hacEtaMax;
    int       _hacEtaBin;
    int       _hacPhiBin;
    double    _muoPtMin;
    double    _muoPtMax;
    double    _muoEtaMin;
    double    _muoEtaMax;
    double    _expEtaMin;
    double    _expEtaMax;

    double    _trkPtCritical;
    double    _trkRadiusCalc;

    TowerGrid _emcTowers;
    TowerGrid _hacTowers;
    
    struct less
    {
      bool operator()(const key_t& k1,const key_t& k2) 
      { 
	int ip1((int)k1.get<0>());
	int ip2((int)k2.get<0>());
	bool less(ip1<ip2);
	if ( ip1 == ip2 ) {
	  int id1((int)k1.get<1>());
	  int id2((int)k2.get<1>());
	  less = id1 < id2;
	}
	return less;
      }
    };

    std::map<key_t,accept_t,less> _detector_acceptance;
    
    void _fillAccept();
    void _computeDerivedQuantities();
    void _setAcceptance(key_t key,double ptmin,double ptmax,double etamin,double etamax);

    std::ranlux48                    _random_engine;
    std::normal_distribution<double> _normal_dist;

  }; // Experiment
  
  class ATLASExperiment : public Experiment
  {
  public:
    ATLASExperiment();
    virtual ~ATLASExperiment();
    virtual Experiment* clone();
  };

  class CMSExperiment : public Experiment
  {
  public:
    CMSExperiment();
    virtual ~CMSExperiment();
    virtual Experiment* clone();
  };

  struct Build 
  {
    static ATLASExperiment* ATLAS() { return new ATLASExperiment(); }
    static CMSExperiment*   CMS()   { return new CMSExperiment();   }
  };

  // ATLASExperiment* buildATLAS() { return Build::ATLAS(); }
  // CMSExperiment*   buildCMS()   { return Build::CMS();   } 

} // Detector

#include "DetectorExperiment.icc"
#endif
