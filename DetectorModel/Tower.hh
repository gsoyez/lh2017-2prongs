// -*- c++ -*-
#ifndef DETECTORMODEL_TOWER_H
#define DETECTORMODEL_TOWER_H

#include "fastjet/PseudoJet.hh"
#include "fastjet/Selector.hh"

#include "TowerGrid.hh"

#include <vector>
#include <memory>

class Tower
{
public:

  Tower(const TowerGrid& tgrid=TowerGrid());

  std::vector<fastjet::PseudoJet> towers(fastjet::Selector sel=fastjet::SelectorEMin(0.0001));

  void reset();

  void add(const fastjet::PseudoJet& pjet);
  void fill(const std::vector<fastjet::PseudoJet>& input);

  bool empty();

private:

  std::unique_ptr<TowerGrid>      _grid;
  std::vector<fastjet::PseudoJet> _towers;
};

inline bool Tower::empty() { return _towers.empty(); }
#endif
