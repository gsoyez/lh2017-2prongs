
#include "TowerGrid.hh"

#include <cstdio>

TowerGrid::index_t TowerGrid::_invalidIndex = -1;

TowerGrid::TowerGrid(int neta,double etamin,double etamax,int nphi,double phimin,double phimax,bool message)
  : _etaBins(neta)
  , _etaMin(etamin)
  , _etaMax(etamax)
  , _phiBins(nphi)
  , _phiMin(phi_std(phimin))
  , _phiMax(phi_std(phimax))
{
  if ( _etaMin >= _etaMax || _etaBins <= 0 || _etaBins == _invalidIndex ) {
    printf("TowerGrid::TowerGrid(...) ERROR invalid eta/y bin description (%i bins in [%.3f,%.3f], no grid\n",_etaBins,_etaMin,_etaMax);
    _etaWidth = -1;
  } else if ( message )  {
    printf("TowerGrid::TowerGrid(...) INFO eta/y bin description (%i bins in [%.3f,%.3f]\n",_etaBins,_etaMin,_etaMax);
  }
  if ( _phiMin >= _phiMax || _phiBins <= 0 || _phiBins == _invalidIndex ) { 
    printf("TowerGrid::TowerGrid(...) ERROR invalid phi bin description (%i bins in [%.3f,%.3f], no grid\n",_phiBins,_phiMin,_phiMax);
    _phiWidth = -1;
  } else if ( message ) {
    printf("TowerGrid::TowerGrid(...) INFO phi   bin description (%i bins in [%.3f,%.3f]\n",_phiBins,_phiMin,_phiMax);
  }
  _etaWidth = (_etaMax-_etaMin)/((double)_etaBins);
  _phiWidth = (_phiMax-_phiMin)/((double)_phiBins);
}

TowerGrid::TowerGrid() :
  TowerGrid(TowerDescriptor::Constants::Binning::Rap::bins,
	    TowerDescriptor::Constants::Binning::Rap::xmin,
	    TowerDescriptor::Constants::Binning::Rap::xmax,
	    TowerDescriptor::Constants::Binning::Phi::bins,
	    TowerDescriptor::Constants::Binning::Phi::xmin,
	    TowerDescriptor::Constants::Binning::Phi::xmax)
{ 
  _etaWidth = (_etaMax-_etaMin)/((double)_etaBins);
  _phiWidth = (_phiMax-_phiMin)/((double)_phiBins);
}

TowerGrid::~TowerGrid() { }

std::string TowerGrid::description(const std::string& tag,bool detailed) const 
{
  static char _buffer[1024];
  sprintf(_buffer,"TowerGrid[%s]::description() INFO %i bins in [%.3f,%.3f] in rapidity/eta (bin width %.3f)\nTowerGrid[%s]::description() INFO %i bins in [%.3f,%.3f] in phi (bin width %.3f)\n",
	  tag.c_str(),_etaBins,_etaMin,_etaMax,_etaWidth,tag.c_str(),_phiBins,_phiMin,_phiMax,_phiWidth);
  std::string mstr(_buffer);
  if ( detailed ) {
    sprintf(_buffer,"TowerGrid[%s]::description() INFO eta binning:\n",tag.c_str());
    mstr += std::string(_buffer);
    double eta(_etaMin);
    for ( int i(0); i<_etaBins; ++i ) { 
      sprintf(_buffer,"                            eta bin (%3i) [%f7.3f,%7.3f[\n",i+1,eta,eta+_etaWidth); eta += _etaWidth;
      mstr += std::string(_buffer);
    }
    sprintf(_buffer,"TowerGrid[%s]::description() INFO phi binning:\n",tag.c_str());
    mstr += std::string(_buffer);
    double phi(_phiMin);
    for ( int i(0); i<_phiBins; ++i ) { 
      sprintf(_buffer,"                            phi bin (%3i) [%f7.3f,%7.3f[\n",i+1,phi,phi+_phiWidth); phi += _phiWidth;
      mstr += std::string(_buffer); 
    }
  }
  return mstr;
}
