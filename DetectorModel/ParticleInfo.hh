// -*- c++ -*-
#ifndef DETECTORMODEL_PARTICLEINFO_H
#define DETECTORMODEL_PARTICLEINFO_H

#include "fastjet/PseudoJet.hh"

#include "PU14.hh"

#include <vector>
#include <map>

///@brief Particle info object 
///
/// Particle description can be attached to @c fastjet::PseudoJet
///
class ParticleInfo :  public PU14
{
public:
  
  ///@brief Particle type indicator
  ///
  /// The particle type can be a logical combination  of several types.
  enum Type { Stable         = 0x2000, 
	      HardScatter    = 0x0001, Pileup    = 0x0002, 
	      Reconstructed  = 0x1000, CaloTower = 0x1010, Track = 0x1020, Muon = 0x1040, 
	      NonInteracting = 0x4000, 
	      Unknown        = 0x0000 };

  ///@name PDG lookup
  ///@{
  typedef int                pdg_t; 
  typedef std::vector<pdg_t> pdglist_t;
  typedef int                charge_t;
  typedef PU14               base_t;
  typedef unsigned int       type_t;
  ///@}

  ///@brief Default constructor
  ///
  /// Empty @c ParticleInfo object
  ParticleInfo();
  ///@brief Construct from base class
  ParticleInfo(const base_t& base);
  ///@brief Loaded constructor
  ///
  /// Meaningful constructor. 
  ParticleInfo(type_t t,const base_t& base);
  ///@brief Copy constructor
  ParticleInfo(const ParticleInfo& pinfo);
  ///@brief Destructor
  virtual ~ParticleInfo();

  ///@name Operators 
  ///@{
  //  ParticleInfo& operator=(const ParticleInfo& pinfo);  ///<@brief Assignment
  bool          operator>(const ParticleInfo& pinfo);  ///<@brief Comparison operator for ordered maps etc. (order by PDG)
  bool          operator<(const ParticleInfo& pinfo);  ///<@brief Comparison operator for ordered maps etc. (order by PDG)
  bool          operator==(const ParticleInfo& pinfo); ///<@brief Equality check (equal is same PDG)
  bool          operator!=(const ParticleInfo& pinfo); ///<@brief Inequality check (not equal is different PDG)
  ///@}

  ///@name Particle info
  ///@{
  virtual int      pdg()     const; ///<@brief PDG code
  virtual type_t   type()    const; ///<@brief Type
  virtual charge_t icharge() const; ///<@brief Integer charge 
  ///@}

  ///@name Setters
  ///@{
  // virtual void setPdg(int pdg);          ///<@brief Set PDG code 
  // virtual void setCharge(double charge); ///<@brief Set charge
  virtual void setType(type_t t);          ///<@brief Set type (cumulative @c or )
  virtual void removeType(type_t t);       ///<@brief Remove type
  virtual void clearType();                ///<@brief Reset type 
  ///@}

  ///@name Static accessors
  ///@{
  static bool isCharged(const fastjet::PseudoJet& pjet);         ///<@brief @return @c true if particle is charged
  static bool isEM(const fastjet::PseudoJet& pjet);              ///<@brief @return @c true if particle generates electromagnetic (EM) showers
  static bool isHAD(const fastjet::PseudoJet& pjet);             ///<@brief @return @c true if particle generates hadronic (HAD) showers
  static bool isNonInteracting(const fastjet::PseudoJet& pjet);  ///<@brief @return @c true if particle is non-interacting 
  static bool isIonizing(const fastjet::PseudoJet& pjet);        ///<@brief @return @c true if particle is only ionizing
  static const pdglist_t& emShowerParticles();                   ///<@brief @return List of PDG codes of particles generating EM showers
  static const pdglist_t& hadShowerParticles();                  ///<@brief @return List of PDG codes of particles generating HAD showers
  static const pdglist_t& nonInteractingParticles();             ///<@brief @return List of PDG codes of particles not interacting with matter
  static const pdglist_t& ionizingParticles();                   ///<@brief @return List of PDG codes of particles ionizing matter but not ionizing it
  static charge_t         particleCharge(pdg_t pdg);             ///<@brief @return Integer particle charge
  static charge_t         particleCharge(const fastjet::PseudoJet& pjet);
  static pdg_t            particlePdg(const fastjet::PseudoJet& pjet);
  static type_t           particleType(const fastjet::PseudoJet& pjet);
  static bool             isCharged(pdg_t pdg);                           
  ///@}

private:

  ///@name Data
  ///@{
  //  int          _pdg;    ///<@brief PDG code
  // double       _charge; ///<@brief Charge
  type_t _type;   ///<@brief Type
  ///@}

  ///@name Internally used types
  ///@{
  typedef std::map<pdg_t,charge_t> chargemap_t;
  ///@}

  ///@name Static stores for PDG codes
  ///@{
  static pdglist_t _emShowerParticles;         ///<@brief Particles generating EM showers 
  static pdglist_t _hadShowerParticles;        ///<@brief Particles generating HAD showers
  static pdglist_t _nonInteractingParticles;   ///<@brief Particles not interacting with matter
  static pdglist_t _ionizingParticles;         ///<@brief Particles only ionizing matter
  static std::map<pdg_t,charge_t> _chargeMap;  ///<@brief Charge lookup
  ///@}

  ///@name Helpers
  ///@{
  static const chargemap_t& _allocateCharge();
  static bool               _findInteraction(const fastjet::PseudoJet& pjet,const std::vector<int>& plist); ///<@brief Find particle in lists
  ///@}
};

inline bool ParticleInfo::operator<(const ParticleInfo& pinfo) { return  pdg() <  pinfo.pdg(); }
inline bool ParticleInfo::operator>(const ParticleInfo& pinfo) { return  pdg() >  pinfo.pdg(); }
inline bool ParticleInfo::operator==(const ParticleInfo& pinfo) { return pdg() == pinfo.pdg(); }
inline bool ParticleInfo::operator!=(const ParticleInfo& pinfo) { return pdg() != pinfo.pdg(); }

#endif
