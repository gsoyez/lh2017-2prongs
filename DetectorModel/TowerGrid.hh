// -*- c++ -*-
#ifndef DETECTORMODEL_TOWERGRID_H
#define DETECTORMODEL_TOWERGRID_H

#include <string>
#include <cmath>

///@brief Tower description
namespace TowerDescriptor {
  ///@brief Numerical and other constants
  namespace Constants {
    static const double pi    = std::asin(1.)*.2; ///<@brief @f$ \pi @f$
    static const double twopi = 2.*pi;            ///<@brief @f$ 2\pi @f$
    namespace Binning {
      namespace Rap {
	static const int    bins = 100;
	static const double xmin = -5.0;
	static const double xmax =  5.0;
      }
      namespace Phi {
	static const int    bins =  64;
	static const double xmin = -pi;
	static const double xmax =  pi;
      }
    }
  }
  namespace Binning {
    namespace Rap {
      static int    bins = Constants::Binning::Rap::bins;
      static double xmin = Constants::Binning::Rap::xmin;
      static double xmax = Constants::Binning::Rap::xmax;
    }
    namespace Phi {
      static int    bins = Constants::Binning::Phi::bins;
      static double xmin = Constants::Binning::Phi::xmin;
      static double xmax = Constants::Binning::Phi::xmax; 
    }
  }
}

///@brief Tower grid
///
/// Designed for describing the binning of the @f$ (\eta,\phi) @f$ (or @f$ (y,\phi) @f$ event plane.
/// Note that for these descriptions @f$ \eta @f$ and @f$ y @f$ are interchangeable. For historial
/// reasons the relevant mathods use the @c eta notation.
///
/// Only regular (equidistant) binning is supported so far.
class TowerGrid
{
public:

  ///@name Public types
  ///@{
  typedef int index_t; ///@brief Linear tower index type
  ///@}

  ///@brief Default constructor
  ///
  /// Empty grid
  TowerGrid();
  ///@brief Useful (loaded) constructor
  ///
  /// A default for azimuthal grid binning is provided.
  TowerGrid(int    neta,double etamin,double etamax,
	    int    nphi=TowerDescriptor::Constants::Binning::Phi::bins,
	    double phimin=TowerDescriptor::Constants::Binning::Phi::xmax,
	    double phimax=TowerDescriptor::Constants::Binning::Phi::xmin);
  ///@brief Base class Destructor
  virtual ~TowerGrid();

  ///@name Access to bin descriptions
  ///@{
  int     etaBins()  const; ///<@brief Number of @f$ \eta @f$ bins
  double  etaMin()   const; ///<@brief Lower limit of @f$ \eta @f$ range
  double  etaMax()   const; ///<@brief Upper limit of @f$ \eta @f$ range
  double  etaWidth() const; ///<@brief @f$ \eta @f$ bin width
  int     phiBins()  const; ///<@brief Number of @f$ \phi @f$ bins
  double  phiMin()   const; ///<@brief Lower limit of @f$ \phi @f$ range
  double  phiMax()   const; ///<@brief Upper limit of @f$ \phi @f$ range
  double  phiWidth() const; ///<@brief @f$ \phi @f$ bin width
  ///@}

  index_t index(double eta,double phi) const;
  index_t etaIndex(double eta)         const;
  index_t etaIndex(index_t index)      const; 
  index_t phiIndex(double phi)         const;
  index_t phiIndex(index_t index)      const;

  double eta(index_t index) const;
  double phi(index_t index) const;

  bool inRange(double eta,double phi) const;
  bool inEtaRange(double eta)         const;
  bool inPhiRange(double phi)         const;

  static double adjustPhi(double phi);
  static double phi_std(double phi);
  static double invalidIndex();

  std::string description(const std::string& tag="TowerGrid",bool detailed=false) const;

private:

  index_t _etaBins;
  double  _etaMin;
  double  _etaMax;
  double  _etaWidth;

  index_t _phiBins;
  double  _phiMin;
  double  _phiMax;
  double  _phiWidth;

  static index_t _invalidIndex;

};

inline TowerGrid::index_t TowerGrid::etaBins()  const { return _etaBins; }
inline double             TowerGrid::etaMin()   const { return _etaMin; }
inline double             TowerGrid::etaMax()   const { return _etaMax; }
inline double             TowerGrid::etaWidth() const { return _etaWidth; }

inline TowerGrid::index_t TowerGrid::phiBins()  const { return _phiBins; }
inline double             TowerGrid::phiMin()   const { return _phiMin; }
inline double             TowerGrid::phiMax()   const { return _phiMax; }
inline double             TowerGrid::phiWidth() const { return _phiWidth; }

inline bool TowerGrid::inEtaRange(double eta)         const { return eta > _etaMin && eta < _etaMax; }
inline bool TowerGrid::inPhiRange(double phi)         const { return phi > _phiMin && phi < _phiMax; }
inline bool TowerGrid::inRange(double eta,double phi) const { return inEtaRange(eta) && inPhiRange(phi); }

inline double TowerGrid::adjustPhi(double phi) 
{ 
  while ( phi < -TowerDescriptor::Constants::pi ) { phi += TowerDescriptor::Constants::twopi; }
  while ( phi >  TowerDescriptor::Constants::pi ) { phi -= TowerDescriptor::Constants::twopi; }
  return phi;
}

inline double TowerGrid::phi_std(double phi) { return adjustPhi(phi); }

inline TowerGrid::index_t TowerGrid::etaIndex(double eta) const
{ return inEtaRange(eta) ? (index_t)((eta-_etaMin)/_etaWidth) : _invalidIndex; }
inline TowerGrid::index_t TowerGrid::etaIndex(index_t index) const
{ 
  if ( index >= 0 ) { 
    index_t idx(index/_phiBins); 
    return idx < _etaBins ? idx : _invalidIndex;
  } else {
    return _invalidIndex; 
  }
}

inline TowerGrid::index_t TowerGrid::phiIndex(double phi) const
{ return inPhiRange(phi) ? (index_t)((phi-_phiMin)/_phiWidth) : _invalidIndex; }
inline TowerGrid::index_t TowerGrid::phiIndex(index_t index) const
{
  if ( index >= 0 ) {
    index_t idx(index%_phiBins);
    return idx < _phiBins ? idx : _invalidIndex;
  } else {
    return _invalidIndex; 
  }  
}

inline TowerGrid::index_t TowerGrid::index(double eta,double phi) const 
{
  phi = adjustPhi(phi);
  return inRange(eta,phi) ? etaIndex(eta)*_phiBins+phiIndex(phi) : -1; 
}

inline double TowerGrid::eta(index_t index) const 
{ index_t idx(etaIndex(index)); return idx != _invalidIndex ? _etaMin+(idx+0.5)*_etaWidth : _etaMin-_etaWidth; } 
#endif
