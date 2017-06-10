
#include "ParticleInfo.h"

#include <algorithm>

ParticleInfo::pdglist_t ParticleInfo::_emShowerParticles       = { 11, -11, 21, -21, 111, -111 };
ParticleInfo::pdglist_t ParticleInfo::_hadShowerParticles      = { 15, -15, 2212, -2212, 2112, -2112,  211, -211, 213, -213, 130, -130, 310, -310, 311, -311, 321, -321 };
ParticleInfo::pdglist_t ParticleInfo::_nonInteractingParticles = { 12, -12, 14, -14, 16, -16, 18,  -18 };
ParticleInfo::pdglist_t ParticleInfo::_ionizingParticles       = { 13, -13 };

const ParticleInfo::pdglist_t& ParticleInfo::emShowerParticles()       { return _emShowerParticles; }
const ParticleInfo::pdglist_t& ParticleInfo::hadShowerParticles()      { return _hadShowerParticles; }
const ParticleInfo::pdglist_t& ParticleInfo::nonInteractingParticles() { return _nonInteractingParticles; }
const ParticleInfo::pdglist_t& ParticleInfo::ionizingParticles()       { return _ionizingParticles; }

ParticleInfo::chargemap_t ParticleInfo::_chargeMap = std::map<ParticleInfo::pdg_t,ParticleInfo::charge_t>();

const ParticleInfo::chargemap_t& ParticleInfo::_allocateCharge()
{
  if ( _chargeMap.empty() ) {
    _chargeMap[11]    = -1;     // electron
    _chargeMap[-11]   =  1;     // positron
    _chargeMap[13]    = -1;     // muon-
    _chargeMap[-13]   =  1;     // muon+
    _chargeMap[15]    = -1;     // tau-
    _chargeMap[-15]   =  1;     // tau+
    _chargeMap[21]    =  0;     // photon
    _chargeMap[-21]   =  0;     // anti-photon
    _chargeMap[111]   =  0;     // pi0
    _chargeMap[-111]  =  0;     // anti-pi0
    _chargeMap[2212]  =  1;     // proton
    _chargeMap[-2212] = -1;     // anti-proton
    _chargeMap[2112]  =  0;     // neutron
    _chargeMap[-2112] =  0;     // anti-neutron
    _chargeMap[211]   =  1;     // pi+
    _chargeMap[-211]  = -1;     // pi-
    _chargeMap[213]   =  1;     // rho+
    _chargeMap[-213]  = -1;     // rho-
    _chargeMap[130]   =  0;     // K0L
    _chargeMap[-130]  =  0;     // anti-K0L
    _chargeMap[310]   =  0;     // K0S
    _chargeMap[-310]  =  0;     // anti-K0S
    _chargeMap[311]   =  0;     // K0   
    _chargeMap[-311]  =  0;     // anti-K0
    _chargeMap[321]   =  1;     // K+
    _chargeMap[321]   = -1;     // K-
  }
  return _chargeMap;
}

ParticleInfo::charge_t ParticleInfo::particleCharge(pdg_t pdg) 
{ const auto & cmap = _allocateCharge(); auto fmap = cmap.find(pdg); return (fmap!= cmap.end()) ? fmap->second : 0; }

bool ParticleInfo::isCharged(pdg_t pdg)
{ return particleCharge(pdg) != 0; }

ParticleInfo::pdg_t ParticleInfo::particlePdg(const fastjet::PseudoJet& pjet)
{ 
  if ( pjet.has_user_info<ParticleInfo>() ) { 
    return pjet.user_info<ParticleInfo>().pdg(); 
  } else if ( pjet.has_user_info<ParticleInfo::base_t>() ) {
    return pjet.user_info<ParticleInfo::base_t>().pdg_id(); 
  } else {
    return 0;
  }
}

ParticleInfo::type_t ParticleInfo::particleType(const fastjet::PseudoJet& pjet)
{
  if ( pjet.has_user_info<ParticleInfo>() ) { 
    return pjet.user_info<ParticleInfo>().type(); 
  } else {
    return Unknown;
  }
}

ParticleInfo::ParticleInfo() : base_t(0), _type((type_t)Unknown)
{ }

ParticleInfo::ParticleInfo(const base_t& base) : base_t(base), _type((type_t)Unknown) 
{ }
 
ParticleInfo::ParticleInfo(type_t t,const base_t& base) : base_t(base), _type((type_t)t)
{ }

ParticleInfo::ParticleInfo(const ParticleInfo& pinfo) : base_t((const base_t&)pinfo), _type(pinfo._type)
{ }

// ParticleInfo::ParticleInfo& ParticleInfo::operator=(const ParticleInfo& pinfo) 
// {
//   _pdg    = pinfo._pdg;
//   _charge = pinfo._charge;
//   _type   = pinfo_type; 
//   return *this;
// }

ParticleInfo::~ParticleInfo()
{ }

int                    ParticleInfo::pdg() const     { return base_t::pdg_id(); }
ParticleInfo::charge_t ParticleInfo::icharge() const { return base_t::three_charge()/3; }
ParticleInfo::type_t   ParticleInfo::type() const    { return _type; } 

void ParticleInfo::setType(type_t t) { _type = t; }

bool ParticleInfo::isCharged(const fastjet::PseudoJet& pjet)
{ return 
    ( pjet.has_user_info<ParticleInfo>() && pjet.user_info<ParticleInfo>().icharge() != 0 ) || 
    ( pjet.has_user_info<base_t>()       && pjet.user_info<base_t>().three_charge()  != 0 ); 
}

bool ParticleInfo::_findInteraction(const fastjet::PseudoJet& pjet,const std::vector<int>& plist)
{ return 
    ( pjet.has_user_info<ParticleInfo>() && std::find(plist.begin(),plist.end(),pjet.user_info<ParticleInfo>().pdg()) != plist.end() ) ||
    ( pjet.has_user_info<base_t>()       && std::find(plist.begin(),plist.end(),pjet.user_info<base_t>().pdg_id())    != plist.end() );
}

bool ParticleInfo::isEM(const fastjet::PseudoJet& pjet)             { return _findInteraction(pjet,_emShowerParticles); }
bool ParticleInfo::isHAD(const fastjet::PseudoJet& pjet)            { return _findInteraction(pjet,_hadShowerParticles); }
bool ParticleInfo::isNonInteracting(const fastjet::PseudoJet& pjet) { return _findInteraction(pjet,_nonInteractingParticles); }
bool ParticleInfo::isIonizing(const fastjet::PseudoJet& pjet)       { return _findInteraction(pjet,_ionizingParticles); }

