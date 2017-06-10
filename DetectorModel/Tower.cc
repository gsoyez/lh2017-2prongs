
#include "Tower.hh"

#include "ParticleInfo.hh"

#include <algorithm>

Tower::Tower(const TowerGrid& tgrid) 
  : _grid(new TowerGrid(tgrid))
  , _towers(_grid->etaBins()*_grid->phiBins(),fastjet::PseudoJet())
{
  for ( auto tow : _towers ) { tow.set_user_info(new ParticleInfo(ParticleInfo::CaloTower,ParticleInfo::base_t(0))); } 
}

void Tower::add(const fastjet::PseudoJet& pjet)
{
  TowerGrid::index_t idx(_grid->index(pjet.rap(),pjet.phi_std()));
  if ( idx >= 0 && idx < (int)_towers.size() ) {  _towers[idx] += fastjet::PtYPhiM(pjet.pt(),_grid->eta(idx),_grid->phi(idx),0.); }
}

void Tower::fill(const std::vector<fastjet::PseudoJet>& input)
{
  this->reset();
  for ( auto pjet : input ) { this->add(pjet); }
}

void Tower::reset() 
{ for ( size_t idx(0); idx < _towers.size(); ++idx ) { _towers[idx].reset_momentum(0.,0.,0.,0.); } }

std::vector<fastjet::PseudoJet> Tower::towers(fastjet::Selector sel) { return sel(_towers); }
