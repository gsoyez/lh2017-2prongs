
#include "DetectorTags.hh"
#include "DetectorDescription.hh"
#include "DetectorGeometry.hh"
#include "DetectorExperiment.hh"

#include <cmath>
#include <chrono>

#ifdef _MONITOR_WITH_ROOT
#include <iostream>
#endif

#define _EXPERIMENT_IMPL( D )						\
  Detector::D::D(bool prnt)						\
    : Experiment(Description::D::experimentName,true)			\
  {									\
    _tag            = Description::D::experimentTag;			\
    _recomode       = Description::D::recoMode;				\
    _solenoidField  = Description::D::solenoidField;			\
    _cavityRadius   = Description::D::cavityRadius;			\
    _cavityEnvelope = Description::D::cavityEnvelope;			\
    _trkPtResoA     = Description::D::trkPtResoA;			\
    _trkPtResoB     = Description::D::trkPtResoB;			\
    _trkPtMin       = Description::D::trkPtMin;				\
    _trkPtMax       = Description::D::trkPtMax;				\
    _trkEtaMin      = Description::D::trkEtaMin;			\
    _trkEtaMax      = Description::D::trkEtaMax;			\
    _emcResoA       = Description::D::emcResoA;				\
    _emcResoB       = Description::D::emcResoB;				\
    _emcResoC       = Description::D::emcResoC;				\
    _emcPtMin       = Description::D::emcPtMin;				\
    _emcPtMax       = Description::D::emcPtMax;				\
    _emcEtaMin      = Description::D::emcEtaMin;			\
    _emcEtaMax      = Description::D::emcEtaMax;			\
    _emcEtaBin      = Description::D::emcEtaBin;			\
    _emcPhiBin      = Description::D::emcPhiBin;			\
    _hacResoA       = Description::D::hacResoA;				\
    _hacResoB       = Description::D::hacResoB;				\
    _hacResoC       = Description::D::hacResoC;				\
    _hacPtMin       = Description::D::hacPtMin;				\
    _hacPtMax       = Description::D::hacPtMax;				\
    _hacEtaMin      = Description::D::hacEtaMin;			\
    _hacEtaMax      = Description::D::hacEtaMax;			\
    _hacEtaBin      = Description::D::hacEtaBin;			\
    _hacPhiBin      = Description::D::hacPhiBin;			\
    _muoPtMin       = Description::D::muoPtMin;				\
    _muoPtMax       = Description::D::muoPtMax;				\
    _muoEtaMin      = Description::D::muoEtaMin;			\
    _muoEtaMax      = Description::D::muoEtaMax;			\
    _expEtaMin      = Description::D::hacEtaMin;			\
    _expEtaMax      = Description::D::hacEtaMax;			\
    _computeDerivedQuantities();					\
    _fillAccept();							\
    if ( prnt ) {							\
      printf("\n#----------------------------------------------------------------------------------\n\n"); \
      setEMCTowerGrid(TowerGrid(_emcEtaBin,_emcEtaMin,_emcEtaMax,_emcPhiBin)); \
      setHACTowerGrid(TowerGrid(_hacEtaBin,_hacEtaMin,_hacEtaMax,_hacPhiBin)); \
      printf("%s\n",description().c_str());				\
      printf("#----------------------------------------------------------------------------------\n"); \
    }									\
  }									\
  Detector::D::~D() { }							\
  Detector::Experiment* Detector::D::clone() { return new D(false); }		

/////////////////////////////
// Build known experiments //
/////////////////////////////

//Detector::ATLASExperiment* Detector::buildATLAS() { return new ATLASExperiment(); }
//Detector::CMSExperiment*   Detector::buildCMS()   { return new CMSExperiment(); }

////////////////
// Experiment //
////////////////

Detector::Experiment::Experiment(const std::string& name,bool /*prnt*/)
  : _name(name)
  , _tag(UnknownExperiment)
  , _recomode(UnknownMode)
  , _solenoidField(0.)
  , _cavityRadius(0.)
  , _cavityEnvelope(0.)
  , _trkPtResoA(0.)
  , _trkPtResoA2(0.)
  , _trkPtResoB(0.)
  , _trkPtResoB2(0.)
  , _trkPtMin(0.)
  , _trkPtMax(0.)
  , _trkEtaMin(0.)
  , _trkEtaMax(0.)
  , _emcResoA(0.)
  , _emcResoA2(0.)
  , _emcResoB(0.)
  , _emcResoB2(0.)
  , _emcResoC(0.)
  , _emcResoC2(0.)
  , _emcPtMin(0.)
  , _emcPtMax(0.)
  , _emcEtaMin(0.)
  , _emcEtaMax(0.)
  , _emcEtaBin(1)    // play it safe!
  , _emcPhiBin(1)
  , _hacResoA(0.)
  , _hacResoA2(0.)
  , _hacResoB(0.)
  , _hacResoB2(0.)
  , _hacResoC(0.)
  , _hacResoC2(0.)
  , _hacPtMin(0.)
  , _hacPtMax(0.)
  , _hacEtaMin(0.)
  , _hacEtaMax(0.)
  , _hacEtaBin(1)
  , _hacPhiBin(1)
  , _muoPtMin(0.)
  , _muoPtMax(0.)
  , _muoEtaMin(0.)
  , _muoEtaMax(0.)
  , _expEtaMin(0.)
  , _expEtaMax(0.)
  , _magneticFieldOn(true)			\
{
  _random_engine = std::ranlux48(std::chrono::system_clock::now().time_since_epoch().count());
  _normal_dist   = std::normal_distribution<double>(0.,1.);

  _computeDerivedQuantities();
}

Detector::Experiment::~Experiment()
{ }

std::string Detector::Experiment::description()
{
  static char _buffer[2048];

  sprintf(_buffer,"# Detector::%-5.5s set up detector smearing and acceptance\n",_name.c_str());
  std::string mstr(_buffer);
  std::string magOn = magneticFieldOn() ? "ON" : "OFF";
  sprintf(_buffer,"# Detector::%-5.5s Solenoid field %.1f T in inner cavity with radius %4.0f mm (pTmin_det = %.3f GeV, 1/bend = %.1f m/(T GeV)) Field is %s\n",
	  _name.c_str(),_solenoidField,_cavityRadius,_trkPtCritical,_trkRadiusCalc*1000.,magOn.c_str());
  mstr += std::string(_buffer);
  sprintf(_buffer,"# Detector::%-5.5s Track acceptance %.1f < pT < %.1f GeV, eta in [%4.2f,%4.2f], relative pT resolution function %.3f%% p_T + %.3f%%\n",
	  _name.c_str(),_trkPtMin,_trkPtMax, _trkEtaMin, _trkEtaMax, _trkPtResoA*100., _trkPtResoB*100.);
  mstr += std::string(_buffer);
  sprintf(_buffer,"# Detector::%-5.5s EMC acceptance %.1f < pT < %.1f GeV, eta in [%4.2f,%4.2f], relative E resolution function %5.1f%%/sqrt(E) + %4.1f%%/E + %4.1f%%\n",
	  _name.c_str(), _emcPtMin, _emcPtMax, _emcEtaMin, _emcEtaMax, _emcResoA*100., _emcResoB*100., _emcResoC*100.);
  mstr += std::string(_buffer);	      
  sprintf(_buffer,"# Detector::%-5.5s HAC acceptance %.1f < pT < %.1f GeV, eta in [%4.2f,%4.2f], relative E resolution function %5.1f%%/sqrt(E) + %4.1f%%/E + %4.1f%%\n",
	  _name.c_str(), _hacPtMin, _hacPtMax, _hacEtaMin, _hacEtaMax, _hacResoA*100., _hacResoB*100., _hacResoC*100.);
  mstr += std::string(_buffer);
  return mstr;
}

void Detector::Experiment::setEtaCoverage(double etamax,double etamin) {
  printf("Detector::%-5.5s::setEtaCoverage(...) INFO updated overall eta coverage from [%7.3f,%7.3f] to [%7.3f,%7.3f]\n",
	 this->name().c_str(),_expEtaMin,_expEtaMax,etamin,etamax); 
  _expEtaMin = etamin; _expEtaMax =  etamax; 
}

void Detector::Experiment::setEMCTowerGrid(const TowerGrid& tgrid) {
  printf("Detector::%-5.5s::setEMCTowerGrid(...) EMC tower grid %i(%i) -> %3i(%3i) eta(phi) bins in [%5.2f(%6.3f),%5.2f(%6.3f)] -> [%5.2f(%6.3f),%5.2f(%6.3f)]\n",
	 this->name().c_str(),
	 _emcTowers.etaBins(),_emcTowers.phiBins(),tgrid.etaBins(),tgrid.phiBins(),
	 _emcTowers.etaMin(),_emcTowers.phiMin(),_emcTowers.etaMax(),_emcTowers.phiMax(),
	 tgrid.etaMin(),     tgrid.phiMin(),     tgrid.etaMax(),     tgrid.phiMax() ); 
  _emcTowers = tgrid; 
}

void Detector::Experiment::setHACTowerGrid(const TowerGrid& tgrid) {
  printf("Detector::%-5.5s::setHACTowerGrid(...) HAC tower grid %i(%i) -> %3i(%3i) eta(phi) bins in [%5.2f(%6.3f),%5.2f(%6.3f)] -> [%5.2f(%6.3f),%5.2f(%6.3f)]\n",
	 this->name().c_str(),
	 _hacTowers.etaBins(),_hacTowers.phiBins(),tgrid.etaBins(),    tgrid.phiBins(),
	 _hacTowers.etaMin(), _hacTowers.phiMin(), _hacTowers.etaMax(),_hacTowers.phiMax(),
	 tgrid.etaMin(),      tgrid.phiMin(),      tgrid.etaMax(),     tgrid.phiMax() ); 
  _hacTowers = tgrid; 
}

void Detector::Experiment::_setAcceptance(key_t key,double ptmin,double ptmax,double etamin,double etamax)
{ 
  auto fmap(_detector_acceptance.find(key));
  if ( fmap != _detector_acceptance.end() ) { 
    fmap->second.get<0>() = ptmin;
    fmap->second.get<1>() = ptmax;
    fmap->second.get<2>() = etamin;
    fmap->second.get<3>() = etamax; 
  } else {
    _detector_acceptance[key] = accept_t(ptmin,ptmax,etamin,etamax);
  }
}

void Detector::Experiment::_fillAccept()
{
  // clear if already filled
  if ( !_detector_acceptance.empty() ) { _detector_acceptance.clear(); }
  
  // (re)fill EMC
  accept_t emcaccept( _emcPtMin, _emcPtMax, _emcEtaMin, _emcEtaMax ); 
  accept_t trkaccept( _trkPtMin, _trkPtMax, _trkEtaMin, _trkEtaMax );
  for ( auto pdg : ParticleInfo::emShowerParticles() )  { 
    key_t ckey(pdg,EmCalorimeter); _detector_acceptance[ckey] = emcaccept; 
    if ( ParticleInfo::isCharged(pdg) ) { key_t tkey(pdg,Tracking ); _detector_acceptance[tkey] = trkaccept;
    }
  }
  // (re)fill HAC
  accept_t hacaccept( _hacPtMin, _hacPtMax, _hacEtaMin, _hacEtaMax );
  for ( auto pdg : ParticleInfo::hadShowerParticles() ) { 
    key_t ckey(pdg,HadCalorimeter); _detector_acceptance[ckey] = hacaccept; 
    if (  ParticleInfo::isCharged(pdg) ) { key_t tkey(pdg,Tracking); _detector_acceptance[tkey] = trkaccept;
    }
  }
  // (re)fill muon
  accept_t muaccept( _muoPtMin, _muoPtMax, _muoEtaMin, _muoEtaMax );
  for ( auto pdg : ParticleInfo::ionizingParticles() ) { key_t mkey(pdg,MuonSpectrometer); _detector_acceptance[mkey] = muaccept; }
  
}
  
double Detector::Experiment::emcResoSmearing(double e)
{
  if ( e <= 0. ) { return 0.; }
  double sigma(std::sqrt(_emcResoA2/e+_emcResoB2/(e*e)+_emcResoC2)*e);
  double es(sigma*_normal_dist(_random_engine));
  //  std::cout << "EMC: E = " << e << " GeV, sigma = " << sigma << ", fluct = " << es << "GeV, E_smear = " << e+es << " GeV" << std::endl; 
  return e+es;
}

double Detector::Experiment::hacResoSmearing(double e)
{
  if ( e <= 0. ) { return 0.; }
  double sigma(std::sqrt(_hacResoA2/e+_hacResoB2/(e*e)+_hacResoC2)*e);
  double es(sigma*_normal_dist(_random_engine));
  //  std::cout << "HAC: E = " << e << " GeV, sigma = " << sigma << ", fluct = " << es << " GeV, E_smear = " << e+es << " GeV" << std::endl; 
  return e+es;
}

double Detector::Experiment::trkResoSmearing(double pT)
{ 
  double sigma(std::sqrt(_trkPtResoA2*pT*pT+_trkPtResoB2)*pT);
  return pT+sigma*_normal_dist(_random_engine);
}
  
double Detector::Experiment::emcResoSmearing(const fastjet::PseudoJet& pjet)
{ return this->emcResoSmearing(pjet.e()); }

double Detector::Experiment::hacResoSmearing(const fastjet::PseudoJet& pjet)
{ return this->hacResoSmearing(pjet.e()); }

double Detector::Experiment::trkResoSmearing(const fastjet::PseudoJet& pjet)
{ return this->trkResoSmearing(pjet.perp()); }

fastjet::PseudoJet Detector::Experiment::smearedCaloSignal(const fastjet::PseudoJet& pjet)
{
  fastjet::PseudoJet newJet(pjet);
  if      ( ParticleInfo::isEM(pjet)  ) { newJet *= this->emcResoSmearing(pjet)/pjet.e(); }
  else if ( ParticleInfo::isHAD(pjet) ) { newJet *= this->hacResoSmearing(pjet)/pjet.e(); } 
  return newJet;
}

fastjet::PseudoJet Detector::Experiment::smearedTrack(const fastjet::PseudoJet& pjet)
{
  fastjet::PseudoJet newJet(pjet);
  newJet *= trkResoSmearing(pjet)/pjet.pt();
  return newJet;
}

bool Detector::Experiment::accept(DetectorTag t,const fastjet::PseudoJet& signal,bool etaOnly)
{
  key_t ckey(ParticleInfo::particlePdg(signal),t);
  auto fmap(_detector_acceptance.find(ckey));
  if ( fmap != _detector_acceptance.end() ) {
    if ( signal.pseudorapidity() > fmap->second.get<2>() && signal.pseudorapidity() < fmap->second.get<3>() ) {
      if ( etaOnly ) {
	return true;
      } else {
	return signal.pt() > fmap->second.get<0>() && signal.pt() < fmap->second.get<1>();
      } // etaOnly
    } // signal in eta range
  } // signal in map
  return false; 
}

bool Detector::Experiment::accept(const fastjet::PseudoJet& pjet,bool etaOnly)
{
  bool acf(false);
  switch ( pjet.user_info<ParticleInfo>().type() )
    {
    case ParticleInfo::CaloTower:
      acf = this->accept(Calorimeter,pjet,etaOnly);
      break;
    case ParticleInfo::Track:
      acf = this->accept(Tracking,pjet,etaOnly);
      break;
    case ParticleInfo::Muon:
      acf = this->accept(MuonSpectrometer,pjet,etaOnly);
      break;
    case ParticleInfo::NonInteracting:
      break;
    default:
      break;
    }
  return acf;  
}

bool Detector::Experiment::emcAccept(const fastjet::PseudoJet& pjet,bool etaOnly)
{ return this->accept(EmCalorimeter,pjet,etaOnly); }

bool Detector::Experiment::hacAccept(const fastjet::PseudoJet& pjet,bool etaOnly)
{ return this->accept(HadCalorimeter,pjet,etaOnly); }

bool Detector::Experiment::trkAccept(const fastjet::PseudoJet& pjet,bool etaOnly)
{ return this->accept(Tracking,pjet,etaOnly); }

bool Detector::Experiment::muoAccept(const fastjet::PseudoJet& pjet,bool etaOnly)
{ return this->accept(MuonSpectrometer,pjet,etaOnly); }

bool Detector::Experiment::selectedPtRange(detector_t t,double& ptmin,double& ptmax)
{
  bool flg(true);
  switch ( t )
    {
    case EmCalorimeter:
      ptmin = _emcPtMin; ptmax = _emcPtMax; break;
    case HadCalorimeter:
    case Calorimeter:
      ptmin = _hacPtMin; ptmax = _hacPtMax; break;
    case Tracking:
      ptmin = _trkPtMin; ptmax = _trkPtMax; break;
    case MuonSpectrometer:
      ptmin = _muoPtMin; ptmax = _muoPtMax; break;
    case UnknownDetector:
      flg = false; break;
    default:
      flg = false; break;
    }
  return flg;
}

bool Detector::Experiment::selectedEtaRange(detector_t t,double& etamin,double& etamax)
{
  bool flg(true);
  switch ( t )
    {
    case EmCalorimeter:
      etamin = _emcEtaMin; etamax = _emcEtaMax; break;
    case HadCalorimeter:
    case Calorimeter:
      etamin = _hacEtaMin; etamax = _hacEtaMax; break;
    case Tracking:
      etamin = _trkEtaMin; etamax = _trkEtaMax; break;
    case MuonSpectrometer:
      etamin = _muoEtaMin; etamax = _muoEtaMax; break;
    case UnknownDetector:
      flg = false; break;
    default:
      etamin = _expEtaMin; etamax = _expEtaMax; flg = false; break;
    }
  return flg;
}
   
void Detector::Experiment::_computeDerivedQuantities()
{
  _emcResoA2        = _emcResoA * _emcResoA;
  _emcResoB2        = _emcResoB * _emcResoB;
  _emcResoC2        = _emcResoC * _emcResoC;
  _hacResoA2        = _hacResoA * _hacResoA;
  _hacResoB2        = _hacResoB * _hacResoB;
  _hacResoC2        = _hacResoC * _hacResoC;
  _trkPtResoA2      = _trkPtResoA * _trkPtResoA;
  _trkPtResoB2      = _trkPtResoB * _trkPtResoB;
  if ( _solenoidField != 0. ) { 
    double f(0.15e-03*_solenoidField);
    _trkPtCritical = f * _cavityRadius; /* Tmm */ 
    _trkRadiusCalc = 1./f;              /* mm */
  }
}

bool Detector::Experiment::adjustPhi(const fastjet::PseudoJet& ptrack,fastjet::PseudoJet& pmodtrack)
{
  // magnetic field is off
  if ( magneticFieldOff() || !ParticleInfo::isCharged(ptrack) ) { pmodtrack = ptrack; return true; }

  // not reaching the calorimeter - remove the signal
  if ( ptrack.pt() < _trkPtCritical  ) { pmodtrack.reset_momentum(0.,0.,0.,0.); return false; }
  
  // get the radius of the trajetory
  double rtraj(_trkRadiusCalc*ptrack.pt());
  
  // create circular particle trajectory
  double dphi = ParticleInfo::particleCharge(ptrack) > 0 
    ? TowerGrid::phi_std(ptrack.phi_std()+TowerDescriptor::Constants::halfpi)
    : TowerGrid::phi_std(ptrack.phi_std()-TowerDescriptor::Constants::halfpi);
  Detector::Geometry::Line2d ut(Detector::Geometry::Point2d(0.,0.),Detector::Geometry::Point2d(rtraj*std::cos(dphi),rtraj*std::sin(dphi)));
  Detector::Geometry::Circle ccir(ut.end(),ut.length());

  //  double cosPhi(ptrack.px()/ptrack.pt());
  //  double sinPhi(ptrack.py()/ptrack.pt());
  //
  //find intersection of trajectory with calo frontface
  // Detector::Geometry::Line2d ur(Detector::Geometry::Point2d(0.,0.),Detector::Geometry::Point2d(cosPhi,sinPhi));
  // Detector::Geometry::Line2d ut(ur.getNormal());
  // double bx(ut.end().x()*rtraj); double by(ut.end().y()*rtraj);
  // if ( ParticleInfo::particleCharge(ptrack) < 0 ) { 
  //   ut.setPoints(Detector::Geometry::Point2d(0.,0.),Detector::Geometry::Point2d(-bx,-by));
  // } else {
  //   ut.setPoints(Detector::Geometry::Point2d(0.,0.),Detector::Geometry::Point2d(bx,by));
  // }
  // std::cout << "Detector::Experiment::adjustPhi - DEBUG - pT_track = " << ptrack.pt() << " GeV, phi/phi_turned/charge/radius = "
  // 	    << ptrack.phi_std() << "/" << dphi << "/" << ParticleInfo::particleCharge(ptrack) << "/" << rtraj << ", center_traj = (" << ut.end().x() << ","
  // 	    << ut.end().y() << "), l = " << ut.length() << ", phi = " << ut.angle()
  // 	    << std::endl;

  // find intersects with cavity envelope
  Detector::Geometry::Point2d cit0, cit1;
  double phi(0.);
  if ( ccir.intersect(_cavityEnvelope,cit0,cit1) == 1 ) {      // tangential
    phi = cit0.phi();
  } else {                                                     // secant
    double d0(std::abs(TowerGrid::phi_std(cit0.phi()-ptrack.phi())));
    double d1(std::abs(TowerGrid::phi_std(cit1.phi()-ptrack.phi())));
    if ( d0 < d1 ) { phi = cit0.phi(); } else { phi = cit1.phi(); }
    // Detector::Geometry::Point2d pri(_cavityRadius*ptrack.px()/ptrack.pt(),_cavityRadius*ptrack.py()/ptrack.pt());
    // Detector::Geometry::Point2d pr0(_cavityRadius*cit0.x()/cit0.r(),_cavityRadius*cit0.y()/cit0.r());
    // Detector::Geometry::Point2d pr1(_cavityRadius*cit1.x()/cit1.r(),_cavityRadius*cit1.y()/cit1.r());
    // double d0(pri.distance(pr0)); double d1(pri.distance(pr1));
    //    if ( d0 < d1 ) { phi = pr0.phi(); } else { phi = pr1.phi(); }
  }

  // new direction
  pmodtrack = fastjet::PtYPhiM(ptrack.pt(),ptrack.rap(),phi,0.);

  return true;
}

Detector::Experiment* Detector::Experiment::clone() { return new Experiment(); }
  
////////////////////////
// Specific Detectors //
////////////////////////

_EXPERIMENT_IMPL( ATLASExperiment )
_EXPERIMENT_IMPL( CMSExperiment )

// Detector::ATLAS Detector::ATLAS::ATLAS() 
//   : Experiment(Description::ATLAS::experimentName)
// {
//   _tag            = Description::ATLAS::experimentTag;
//   _recomode       = Description::ATLAS::recoMode;
//   _solenoidField  = Description::ATLAS::solenoidField;
//   _cavityRadius   = Description::ATLAS::cavityRadius;
//   _trkPtResoA     = Description::ATLAS::trkPtResoA;
//   _trkPtResoB     = Description::ATLAS::trkPtResoB;
//   _trkPtMin       = Description::ATLAS::trkPtMin;
//   _trkPtMax       = Description::ATLAS::trkPtMax;
//   _trkEtaMin      = Description::ATLAS::trkEtaMin;
//   _trkEtaMax      = Description::ATLAS::trkEtaMax;
//   _emcResoA       = Description::ATLAS::emcResoA;
//   _emcResoB       = Description::ATLAS::emcResoB;
//   _emcResoC       = Description::ATLAS::emcResoC;
//   _emcPtMin       = Description::ATLAS::emcPtMin;
//   _emcPtMax       = Description::ATLAS::emcPtMax;
//   _emcEtaMin      = Description::ATLAS::emcEtaMin;
//   _emcEtaMax      = Description::ATLAS::emcEtaMax;
//   _emcEtaBin      = Description::ATLAS::emcEtaBin;
//   _emcPhiBin      = Description::ATLAS::emcPhiBin;
//   _hacResoA       = Description::ATLAS::hacResoA;
//   _hacResoB       = Description::ATLAS::hacResoB;
//   _hacResoC       = Description::ATLAS::hacResoC;
//   _hacPtMin       = Description::ATLAS::hacPtMin;
//   _hacPtMax       = Description::ATLAS::hacPtMax;
//   _hacEtaMin      = Description::ATLAS::hacEtaMin;
//   _hacEtaMax      = Description::ATLAS::hacEtaMax;
//   _hacEtaBin      = Description::ATLAS::hacEtaBin;
//   _hacPhiBin      = Description::ATLAS::hacPhiBin;
//   _muoPtMin       = Description::ATLAS::muoPtMin;
//   _muoPtMax       = Description::ATLAS::muoPtMax;
//   _muoEtaMin      = Description::ATLAS::muoEtaMin;
//   _muoEtaMax      = Description::ATLAS::muoEtaMax;
//   _expEtaMin      = Description::ATLAS::hacEtaMin;
//   _expEtaMax      = Description::ATLAS::hacEtaMax;

//   setEMCTowerGrid(TowerGrid(_emcEtaBin,_emcEtaMin,_emcEtaMax,_emcPhiBin));
//   setHACTowerGrid(TowerGrid(_hacEtaBin,_hacEtaMin,_hacEtaMax,_hacPhiBin));

//   _computeDerivedQuantities();

//   _fillAccept();

//   printf("\n#--------------------------------\n");
//   description();
//   printf("\n#--------------------------------\n");
// }

// Detector::CMS Detector::CMS::CMS()
//   : Experiment(Descripion::CMS::experimentName)
// {
//   _tag            = CMS;
//   _recomode       = ParticleFlowExcl;
//   _solenoidField  = Description::CMS::solenoidField;
//   _cavityRadius   = Description::CMS::cavityRadius;
//   _trkPtResoA     = Description::CMS::trkPtResoA;
//   _trkPtResoB     = Description::CMS::trkPtResoB;
//   _trkPtMin       = Description::CMS::trkPtMin;
//   _trkPtMax       = Description::CMS::trkPtMax;
//   _trkEtaMin      = Description::CMS::trkEtaMin;
//   _trkEtaMax      = Description::CMS::trkEtaMax;
//   _emcResoA       = Description::CMS::emcResoA;
//   _emcResoB       = Description::CMS::emcResoB;
//   _emcResoC       = Description::CMS::emcResoC;
//   _emcPtMin       = Description::CMS::emcPtMin;
//   _emcPtMax       = Description::CMS::emcPtMax;
//   _emcEtaMin      = Description::CMS::emcEtaMin;
//   _emcEtaMax      = Description::CMS::emcEtaMax;
//   _emcEtaBin      = Description::CMS::emcEtaBin;
//   _emcPhiBin      = Description::CMS::emcPhiBin;
//   _hacResoA       = Description::CMS::hacResoA;
//   _hacResoB       = Description::CMS::hacResoB;
//   _hacResoC       = Description::CMS::hacResoC;
//   _hacPtMin       = Description::CMS::hacPtMin;
//   _hacPtMax       = Description::CMS::hacPtMax;
//   _hacEtaMin      = Description::CMS::hacEtaMin;
//   _hacEtaMax      = Description::CMS::hacEtaMax;
//   _hacEtaBin      = Description::CMS::hacEtaBin;
//   _hacPhiBin      = Description::CMS::hacPhiBin;
//   _muoPtMin       = Description::CMS::muoPtMin;
//   _muoPtMax       = Description::CMS::muoPtMax;
//   _muoEtaMin      = Description::CMS::muoEtaMin;
//   _muoEtaMax      = Description::CMS::muoEtaMax;
//   _expEtaMin      = Description::CMS::hacEtaMin;
//   _expEtaMax      = Description::CMS::hacEtaMax;

//   setEMCTowerGrid(TowerGrid(_emcEtaBin,_emcEtaMin,_emcEtaMax,_emcPhiBin));
//   setHACTowerGrid(TowerGrid(_hacEtaBin,_hacEtaMin,_hacEtaMax,_hacPhiBin));

//   _computeDerivedQuantities();

//   _fillAccept();

//   printf("\n#--------------------------------\n");
//   description();
//   printf("\n#--------------------------------\n");
// }




