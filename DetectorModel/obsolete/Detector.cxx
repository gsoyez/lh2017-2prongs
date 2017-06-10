
#include "Detector.h"

#include <cmath>
#include <chrono>

Detector::ATLAS* Detector::buildATLAS() { return new ATLAS(); }
Detector::CMS*   Detector::buildCMS()   { return new CMS(); }

///////////
// Point //
///////////

Detector::Geometry::Point2d::Point2d(double x,double y) : _x(x), _y(y), _r(std::sqrt(x*x+y*y)) { }
Detector::Geometry::Point2d::Point2d(const Point2d& p) : _x(p.x()), _y(p.y()), _r(p.r()) { }

double Detector::Geometry::Point2d::x() const { return _x; }
double Detector::Geometry::Point2d::y() const { return _y; }
double Detector::Geometry::Point2d::r() const { return _r; }

void   Detector::Geometry::Point2d::setCoordinates(double x,double y) { _x = x; _y = y; _r = std::sqrt(x*x+y*y); }

double Detector::Geometry::Point2d::distance(const Point2d& p) { double dx(_x-p.x()); double dy(_y-p.y()); return std::sqrt(dx*dx+dy*dy); }

Detector::Geometry::Point2d Detector::Geometry::Point2d::operator+(const Point2d& p) { return Point2d(_x+p.x(),_y+p.y()); }
Detector::Geometry::Point2d Detector::Geometry::Point2d::operator-(const Point2d& p) { return Point2d(_x-p.x(),_y-p.y()); }

Detector::Geometry::Point2d& Detector::Geometry::Point2d::operator+=(const Point2d& p) { _x+=p.x(); _y+=p.y(); _r=std::sqrt(_x*_x+_y*_y); return *this; }
Detector::Geometry::Point2d& Detector::Geometry::Point2d::operator-=(const Point2d& p) { _x-=p.x(); _y-=p.y(); _r=std::sqrt(_x*_x+_y*_y); return *this; }

bool  Detector::Geometry::Point2d::operator==(const Point2d& p) { return _x == p.x() && _y == p.y(); }
bool  Detector::Geometry::Point2d::operator!=(const Point2d& p) { return _x != p.x() || _y != p.y(); }

//////////
// Line //
//////////

Detector::Geometry::Line2d::Line2d(const Point2d& p0,const Point2d& p1) : _p0(p0), _p1(p1), _length(p0.distance(p1)), _angle(0.), _cos_angle(0.)
{ setPoints(p0,p1); }

Detector::Geometry::Line2d::Line2d(double x0,double y0,double x1,double y1) : Line2d(Point2d(x0,y0),Point2d(x1,y1))
{ }

double Detector::Geometry::Line2d::x0() const { return _p0.x(); }
double Detector::Geometry::Line2d::y0() const { return _p0.y(); }

double Detector::Geometry::Line2d::x1() const { return _p1.x(); }
double Detector::Geometry::Line2d::y1() const { return _p1.y(); }

Detector::Geometry::Point2d& Detector::Geometry::Line2d::begin() const { return _p0; }
Detector::Geometry::Point2d& Detector::Geometry::Line2d::end()   const { return _p1; }

double Detector::Geometry::Line2d::length()               const { return _length; }
double Detector::Geometry::Line2d::angle()                const { return _angle; }
double Detector::Geometry::Line2d::angle(const Line2d& l) const { return std::acos(this->cos_angle(l)); }

void Detector::Geometry::setPoints(Point2d& p0,Point2d& p1)
{ 
  _p0 = p0; _p1 = p1; _length = _p0.distance(_p1); 
  Point2d pp(_p1-_p0); _angle = pp.x() != 0. ? std::atan2(pp.y(),pp.x()) : 0.; 
  _cos_angle = std::cos(_angle); 
}

void Detector::Geometry::setSlopeAndOffset(double m,double c)
{ if ( m != 0. ) { setPoints(Point2d(0.,c),Point2d(-c/m,0.)); } else { setPoints(Point2d(0.,c),Point2d(0.,c)); } }

double Detector::Geometry::Line2d::cos_angle()                const { return _cos_angle; }
double Detector::Geometry::Line2d::cos_angle(const Line2d& l) const { return this->length() != 0. && l.length() != 0. ? (this->x()*l.x()+this->y()*l.y())/(this->length()*l.length()) : 0.; } 


unsigned int Detector::Geometry::Line2d::intersect(const Line2d& l,Point2d& p)
{
  if ( l.length() == 0. || this->length() == 0. ) { return 0; }

  // coordinates
  double x0(this->x0());
  double y0(this->y0());
  double x1(this->x1());
  double y1(this->y1());

  double u0(l.x0());
  double v0(l.y0());
  double u1(l.x1());
  double v1(l.y1());

  // determinant
  double dx(x0-x1);
  double dy(y0-y1);
  double du(u0-u1);
  double dv(v0-v1);

  // no crossing
  double det(dx*dv-dy*du);
  if ( det == 0. ) { return 0; } // no intercept

  // crossing coordinates
  p.setCoordinates(((x0*y1-y0*x1)*du-dx*(u0*v1-v0*u1))/det,((x0*y1-y0*x1)*dv-dy*(u0*v1-v0*u1))/det);

  return 1;  
}

unsigned int Detector::Geometry::Line2d::intersect(const Circle& c,Point2d& p0,Point2d& p1)
{
  // no line
  double dr(this->length());
  if ( dr == 0. ) { return 0; }

  // from https://math.stackexchange.com/questions/228841/how-do-i-calculate-the-intersections-of-a-straight-line-and-a-circle

  // collect variables
  double p(c.center().x());
  double q(c.center().y());
  double r(c.radius());
  double c(this->end().y());
  double m((this->end().y()-this->begin().y())/(this->end().x()-this->begin().x()));

  // define terms
  double A(m*m+1);
  double B(2.*(m*c-m*q-p));
  double C(q*q-r*r+p*p-2.*c*q+c*c);
  double D(B*B-4.*A*C); 
  if ( D < 0. ) { return 0; } // no intercept
  if ( D < 0.0000001 ) {      // tangential
    double px(-B/(2.*A));
    double py(m*px+c);
    p0.setCoordinates(px,py);
    return 1;
  }
  D = std::sqrt(D);           // secant (two intercepts)
  double x0((-B+D)/(2.*A)); double y0(m*x0+c);
  double x1((-B-D)/(2.*A)); double y1(m*x1+c);
  p0.setCoordinates(x0,y0);
  p1.setCoordinates(x1,y1);

  return 2;

  // // shift circle to (0,0) and move line accordingly
  // Point2d cc(c.center());
  // Point2d lb(this->begin()); lb -= cc;
  // Point2d le(this->end());   le -= cc;

  // // collect variables
  // double x0(lb.x());
  // double x1(le.c());
  // double y0(lb.y());
  // double y1(le.y());
  // double cr(c.radius());

  // // derivates
  // double dx(x1-x0);
  // double dy(y1-y0);
  // double ds(dx*dx+dy*dy);
  // double dr(std::sqrt(ds));
  // double dt(x0*y1-y0*x1);

  // // check on basics
  // double dl(cr*cr*ds-dt*dt);
  // if ( dl < 0. ) { return 0; } // no intersection
  // double sd(std::sqrt(dl));

  // // tangent (1) or secant (2)
  // unsigned int nx = dl < 0.0000001 ? 1 : 2;
  
  // double px0 = dy < 0. ? (dl*dy-dx*sd)/ds : (dl*dy+dx*sd)/ds;
  // double py0 = (-dl*dx+std::abs(dy)*sd)/ds;
  // p0.setCoordinates(px0,py0); p0 += cc;
  // if ( nx > 1 ) {  
  //   double px1 = dy < 0. ? (dl*dy+dx*sd)/ds : (dl*dy-dx*sd)/ds;
  //   double py1 = (-dl*dx-std::abs(dy)*sd)/ds;
  //   p1.setCoordinates(px1,px2); p1 += cc;
  // }

  // return nx;
}

Detector::Geometry::Line2d Detector::Geometry::Line2d::getNormal() const
{
  double dx(this->end().x()-this->begin().x());
  double dy(this->end().y()-this->begin().y());
  return Tline2d(Point2d(-dy,dx),Point2d(dy,-dx));
}

bool Detector::Geometry::getSlopeAndOffset(double& m,double& c)
{
  double dx(this->end().x()-this->begin().x());
  double dy(this->end().y()-this->begin().y());
  if ( dx == 0. ) { m = 0.; } else { m = dy/dx; }
  c = this->begin().y()-m*this->begin().x();
  return true;
}

double Detector::Geometry::Line2d::distance(const Point2d& p) const 
{
  double l(this->length());
  if ( l == 0. ) { return 0.; }

  double x0(this->begin().x());
  double x1(this->end().x());
  double y0(this->begin().y());
  double y1(this->end().y());

  return std::abs((y1-y0)*p.x()-(x1-x0)*p.y()+x1*y0-y1*x0)/l;
}

////////////
// Circle //
////////////

Detector::Geometry::Circle Detector::Geometry::Circle(const Point2d& center,double r) : _center(center), _radius(r) { }
Detector::Geometry::Circle Detector::Geometry::Circle(double x0,double y0,double r) : Circle(Point2d(x0,y0),r) { }

const Detector::Geometry::Point2d& Detector::Geometry::Circle::center() const { return _center; }
double Detector::Geometry::Circle::radius() const { return _radius; }

unsigned int Detector::Geometry::Circle::intersect(const Line2d& l,Point2d& p0,Point2d& p1)
{ return l.intersect(*this,p0,p1); }

// from https://math.stackexchange.com/questions/256100/how-can-i-find-the-points-at-which-two-circles-intersect

unsigned int Detector::Geometry::Circle::intercept(const Circle& c,Point2d& p0,Point2d& p1)
{
  //////////////////////////////////////////////////

  // check on basics
  double r(this->radius());
  if ( r == 0. ) { return 0; }

  // collect variables
  double p(this->center().x());
  double q(this->center().y());
  double u(c.center().x());
  double v(c.center().y());
  double R(c.radius());

  // derivates: linear differences in circle offsets /////////////////////////////////
  double dx(p-u);                                    // Solving for y as function x //
  double dy(q-v);                                    /////////////////////////////////
  if ( dy == 0. || dx == 0. ) { return 0; }
  // derivates: differences of squares
  double sqrdr(r*r-R*R);
  double sqrdx(p*p-u*u);
  double sqrdy(q*q-v*v);
  double k(sqrdr-sqrdx-sqrdy);
  // derivates: y dependence on x
  double m(-dx/dy);
  double c(-k/(2.*dy));

  // solving: x                                       ////////////////////////////////
  double qa(1.+m*m);                                  // Investing y to solve for x //
  double qb(2.*(m*(c-q)-p));                          ////////////////////////////////
  double qt(c+q);
  double qc(qt*qt+p*p-r*r);
  double f(qb*qb-4*qa);
  if ( f < 0. )       { return 0; } // no overlap
  if ( f < 0.000001 ) {             // tangential
    double x(-b/(2.*a));
    double y(m*x+c);
    p0.setCoordinates(x,y);
    return 1;
  } 
  double x0((-b+std::sqrt(qt))/(2.*a));
  double y0(m*x0+c);
  double x1((-b-std::sqrt(qt))/(2.*a));
  double y1(m*x1+c);
  p0.setCoordinates(x0,y0);
  p1.setCoordinates(x1,y1);

  return 2;


  // double A(QR-QX-QY);
  // double M(-DX/DY);
  // double C(-A/(2*DY));
  // double K0(C-p);
  // double K1(C*C-2.0*C+q*q-r1*r1);
  //
}

////////////////
// Experiment //
////////////////

Detector::Experiment::Experiment(const std::string& name)
  : _name(name)
  , _tag(UnknownExperiment)
  , _recomode(UnknownMode)
  , _solenoidField(0.)
  , _cavityRadius(0.)
  , _trkPtResoA(0.)
  , _trkPtResoA2(0.)
  , _trkPtResoB(0.)
  , _trkPtResoB2(0.)
  , _trkPtMin(0.)
  , _trkPtMax(0.)
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
  , _expEtaMin(0.)
  , _expEtaMax(0.)
{
  _random_engine = std::ranlux48(std::chrono::system_time::now().time_since_epoch().count());
  _normal_dist   = std::normal_distribution<double>(0.,1.);


  _computeDerivedQuantities();
}

Detector::Experiment::~Experiment()
{ }

std::string Detector::Experiment:description()
{
  static char _buffer[1024];
  sprintf(_buffer,std::string("Detector::ATLAS(\042%s\042) set up\n"+
			      "Detector::ATLAS(\042%s\042) Solenoid field %.1f T in inner cavity with radius %4.0f mm\n"+
			      "Detector::ATLAS(\042%s\042) Track acceptance %.1f < pT < %.1f GeV, eta in [%4.2f,%4.2f], resolution function (%.3f p_T + %.3f) %%\n"+
			      "Detector::ATLAS(\042%s\042) EMC acceptance %.1f < pT < %.1f GeV, eta in [%4.2f,%4.2f], resolution function (%.1f/sqrt(E) + %.1f/E + %.1f/E^2) %%\n"+
			      "Detector::ATLAS(\042%s\042) HAC acceptance %.1f < pT < %.1f GeV, eta in [%4.2f,%4.2f], resolution function (%.1f/sqrt(E) + %.1f/E + %.1f/E^2) %%\n").c_str(),
	  _name.c_str(),
	  _name.c_str(),_solenoidField,_cavityRadius,
	  _name.c_str(),_trkPtMin,_trkPtMax, _trkEtaMin, _trkEtaMax, _trkResoA*100., trkResoB*100.,
	  _name.c_str(), _emcPtMin, _emcPtMax, _emcEtaMin, _emcEtaMax, _emcResoA*100., _emcResoB*100., _emcResoC*100.,
	  _name.c_str(), _hacPtMin, _hacPtMax, _hacEtaMin, _hacEtaMax, _hacResoA*100., _hacResoB*100., _hacResoC*100.);
  return std::string(_buffer);
}

void Detector::Experiment::setEtaCoverage(double etamax,double etamin) {
  printf("Experiment[\042%s\042]::setEtaCoverage(...) INFO updated overall eta coverage from [%7.3f,%7.3f] to [%7.3f,%7.3f]\n",
	 this->name().c_str(),_expEtaMin,_expEtaMax,etamin,etamax); 
  _expEtaMin = etamin; _expEtaMax =  etamax; 
}

void Detector::Experiment::setEMCTowerGrid(const TowerGrid& tgrid) {
  printf("Experiment[\042%s\042]::setEMCTowerGrid(...) INFO updated EMC tower grid from %i(%i) eta(phi) bins between [%7.3f(%6.3f),%7.3f(%6.3f)] to %i(%i) bins between [%7.3f(%6.3f),%7.3f(%6.3f)]\n",
	 this->name().c_str(),
	 _emcTowers.etaBins(),_emcTowers.phiBins(),_emcTowers.etaMin(),_emcTowers.phiMin(),_emcTowers.etaMax(),_emcTowers.phiMax(),
	 tgrid.etaBins(),     tgrid.phiBins(),     tgrid.etaMin(),     tgrid.phiMin(),     tgrid.etaMax(),     tgrid.phiMax() ); 
  _emcTowers = tgrid; 
}

void Detector::Experiment::setHACTowerGrid(const TowerGrid& tgrid) {
  printf("Experiment[\042%s\042]::setHACTowerGrid(...) INFO updated HAC tower grid from %i(%i) eta(phi) bins between [%7.3f(%6.3f),%7.3f(%6.3f)] to %i(%i) bins between [%7.3f(%6.3f),%7.3f(%6.3f)]\n",
	 this->name().c_str(),
	 _hacTowers.etaBins(),_hacTowers.phiBins(),_hacTowers.etaMin(),_hacTowers.phiMin(),_hacTowers.etaMax(),_hacTowers.phiMax(),
	 tgrid.etaBins(),     tgrid.phiBins(),     tgrid.etaMin(),     tgrid.phiMin(),     tgrid.etaMax(),     tgrid.phiMax() ); 
  _hacTowers = tgrid; 
}

void Detector::Experiment::_fillAccept()
{
  // clear if already filled
  if ( !_detector_acceptance.empty() ) { _detector_acceptance.clear(); }
  
  // (re)fill EMC
  accept_t emcaccept( _emcPtMin, _emcPtMax, _emcEtaMin, _emcEtaMax ); 
  accept_t trkaccept( _trkPtMin, _trkPtMax, _trkEtaMin, _trkEtaMax );
  for ( auto pdg : ParticleInfo::emShowerParticles() )  { 
    key_t ckey(pdg,Calorimeter); _detector_acceptance[ckey] = emaccept; 
    if ( ParticleInfo::charge(pdg) != 0. ) { key_t tkey(pdg,Tracking ); _detector_acceptance[tkey] = trkaccept;
    }
  }
  // (re)fill HAC
  accept_t hacaccept( _hacPtMin, _hacPtMax, _hacEtaMin, _hacEtaMax );
  for ( auto pdg : ParticleInfo::hadShowerParticles() ) { 
    key_t ckey(pdg,Calorimeter); _detector_acceptance[ckey] = hadaccept; 
    if (  ParticleInfo::charge(pdg) != 0. ) { key_t tkey(pdg,Tracking); _detector_acceptance[tkey] = trkaccept;
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
  return _normal_dist(e,sigma);
}

double Detector::Experiment::hacResoSmearing(double e)
{
  if ( e <= 0. ) { return 0.; }
  double sigma(std::sqrt(_hacResoA2/e+_hacResoB2/(e*e)+_hacResoC2)*e);
  return _normal_dist(e,sigma);
}

double Detector::Experiment::trkResoSmearing(double pT)
{ 
  double sigma(std::sqrt(_trkResoA2*pT*pT+_trkResoB2)*pT);
  return _normal_dist(pT,sigma);
}
  
double Detector::Experiment::emcResoSmearing(const fastjet::PseudoJet* pjet)
{ return this->emcResoSmearing(pjet.e()); }

double Detector::Experiment::hacResoSmearing(const fastjet::PseudoJet* pjet)
{ return this->emcResoSmearing(pjet.e()); }

double Detector::Experiment::trkResoSmearing(const fastjet::PseudoJet* pjet)
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
  key_t ckey(pjet.pdg(),t);
  auto fmap(_detector_acceptance.find(ckey));
  if ( fmap != _detector_acceptance.end() ) {
    if ( signal.pseudorapidity() > fmap->second.get<2> && signal.pseudorapidity() < fmap->second.get<3>() ) {
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
      acf = this->accept(Tracking,pjet,etaOny);
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
{ return this->accept(Calorimeter,pjet,etaOnly); }

bool Detector::Experiment::hacAccept(const fastjet::PseudoJet& pjet,bool etaOnly)
{ return this->accept(Calorimeter,pjet,etaOnly); }

bool Detector::Experiment::trkAccept(const fastjet::PseudoJet& pjet,bool etaOnly)
{ return this->accept(Tracking,pjet,etaOnly); }

bool Detector::Experiment::muoAccept(const fastjet::PseudoJet& pjet,bool etaOnly)
{ return this->accept(MuonSpectrometer,pjet,etaOnly); }
   
void Detector::Signals::_computeDerivedQuantities()
{
  _emcResoA2      = _emcResoA * _emcResoA;
  _emcResoB2      = _emcResoB * _emcResoB;
  _emcResoC2      = _emcResoC * _emcResoC;
  _hacResoA2      = _hacResoA * _hacResoA;
  _hacResoB2      = _hacResoB * _hacResoB;
  _hacResoC2      = _hacResoC * _hacResoC;
  _trkResoA2      = _trkResoA * _trkResoA;
  _trkResoB2      = _trkResoB * _trkResoB;
  if ( _solenoidField != 0. ) { 
    _trkPtCritical = 0.15 * _solenoidField * _cavityRadius/1000.; /* Tm */ 
    _trkRadiusCalc = 1000./(0.15*_solenoidField);                 /* mm */
    _expCavityEnv  = Circle(Point2d(0.,0.),_cavityRadius);
  }
}

bool Detector::Experiment::adjustPhi(const fastjet::PseudoJet& pjet,fastjet::PseudoJet& newJet)
{
  // not reaaching the calorimeter or not charged
  if ( pjet.pt() < _trkPtCritical || !ParticleInfo::isCharged(pjet) ) { newJet = pjet; return false; }
  
  // get the radius of the trajetory
  double rtraj(_trkRadiusCalc*pjet.pt());

  // create circular particle trajectory
  double cosPhi(pjet.px()/pjet.pt());
  double sinPhi(pjet.oy()/pjet.pt());

  //find intersection of trajectory with calo frontface
  Line2d ut(Line2d(Point2d(0.,0.),Point2d(rtraj*cosPhi,rtraj*sinPhi)).getNormal());
  if ( pjet.user_info<ParticleInfo>().charge() < 0 ) { ut.setPoints(Point2d(0.,0.),Point2d(-ut.end().x(),-ut.end().y())); } 

  Circle  ccir(ut.end(),ut.length());
  Point2d cit0; Pont2d cit1;
  double phi(0.);
  if ( ccir.intersect(_expCavityEnv,cit0,cit1) == 1 ) {
    phi = cit0.phi();
  } else {
    Point2d ep(_cavityRadius*cosPhi,_cavityRadius*sinPhi);
    if ( ep.distance(cit0) < ep.distance(cit1) ) { 
      phi = cit0.angle(); 
    } else {
      phi = cit1.angle();
    }
  }

  // new direction
  newJet = fastjet::PtYRapM(pjet.pt(),pjet.rap(),phi,0.);

  return true;
}
  

  
}

////////////////////////
// Specific Detectors //
////////////////////////

Detector::ATLAS Detector::ATLAS::ATLAS() 
  : Experiment("ATLAS")
{
  _tag            = ATLAS;
  _recomode       = CaloTower;
  _solenoidField  = Description::ATLAS::solenoidField;
  _cavityRadius   = Description::ATLAS::cavityRadius;
  _trkPtResoA     = Description::ATLAS::trkPtResoA;
  _trkPtResoB     = Description::ATLAS::trkPtResoB;
  _trkPtMin       = Description::ATLAS::trkPtMin;
  _trkPtMax       = Description::ATLAS::trkPtMax;
  _emcResoA       = Description::ATLAS::emcResoA;
  _emcResoB       = Description::ATLAS::emcResoB;
  _emcResoC       = Description::ATLAS::emcResoC;
  _emcPtMin       = Description::ATLAS::emcPtMin;
  _emcPtMax       = Description::ATLAS::emcPtMax;
  _emcEtaMin      = Description::ATLAS::emcEtaMin;
  _emcEtaMax      = Description::ATLAS::emcEtaMax;
  _emcEtaBin      = Description::ATLAS::emcEtaBin;
  _emcPhiBin      = Description::ATLAS::emcPhiBin;
  _hacResoA       = Description::ATLAS::hacResoA;
  _hacResoB       = Description::ATLAS::hacResoB;
  _hacResoC       = Description::ATLAS::hacResoC;
  _hacPtMin       = Description::ATLAS::hacPtMin;
  _hacPtMax       = Description::ATLAS::hacPtMax;
  _hacEtaMin      = Description::ATLAS::hacEtaMin;
  _hacEtaMax      = Description::ATLAS::hacEtaMax;
  _hacEtaBin      = Description::ATLAS::hacEtaBin;
  _hacPhiBin      = Description::ATLAS::hacPhiBin;
  _muoPtMin       = Description::ATLAS::muoPtMin;
  _muoPtMax       = Description::ATLAS::muoPtMax;
  _muoEtaMin      = Description::ATLAS::muoEtaMin;
  _muoEtaMax      = Description::ATLAS::muoEtaMax;
  _expEtaMin      = Description::ATLAS::hacEtaMin;
  _expEtaMax      = Description::ATLAS::hacEtaMax;

  setEMCTowerGrid(TowerGrid(_emcEtaBin,_emcEtaMin,_emcEtaMax,_emcPhiBin));
  setHACTowerGrid(TowerGrid(_hacEtaBin,_hacEtaMin,_hacEtaMax,_hacPhiBin));

  _computeDerivedQuantities();

  _fillAccept();

  printf("\n#--------------------------------\n");
  description();
  printf("\n#--------------------------------\n");
}

Detector::CMS Detector::CMS::CMS()
  : Experiment("CMS")
{
  _tag            = CMS;
  _recomode       = ParticleFlowExcl;
  _solenoidField  = Description::CMS::solenoidField;
  _cavityRadius   = Description::CMS::cavityRadius;
  _trkPtResoA     = Description::CMS::trkPtResoA;
  _trkPtResoB     = Description::CMS::trkPtResoB;
  _trkPtMin       = Description::CMS::trkPtMin;
  _trkPtMax       = Description::CMS::trkPtMax;
  _emcResoA       = Description::CMS::emcResoA;
  _emcResoB       = Description::CMS::emcResoB;
  _emcResoC       = Description::CMS::emcResoC;
  _emcPtMin       = Description::CMS::emcPtMin;
  _emcPtMax       = Description::CMS::emcPtMax;
  _emcEtaMin      = Description::CMS::emcEtaMin;
  _emcEtaMax      = Description::CMS::emcEtaMax;
  _emcEtaBin      = Description::CMS::emcEtaBin;
  _emcPhiBin      = Description::CMS::emcPhiBin;
  _hacResoA       = Description::CMS::hacResoA;
  _hacResoB       = Description::CMS::hacResoB;
  _hacResoC       = Description::CMS::hacResoC;
  _hacPtMin       = Description::CMS::hacPtMin;
  _hacPtMax       = Description::CMS::hacPtMax;
  _hacEtaMin      = Description::CMS::hacEtaMin;
  _hacEtaMax      = Description::CMS::hacEtaMax;
  _hacEtaBin      = Description::CMS::hacEtaBin;
  _hacPhiBin      = Description::CMS::hacPhiBin;
  _muoPtMin       = Description::CMS::muoPtMin;
  _muoPtMax       = Description::CMS::muoPtMax;
  _muoEtaMin      = Description::CMS::muoEtaMin;
  _muoEtaMax      = Description::CMS::muoEtaMax;
  _expEtaMin      = Description::CMS::hacEtaMin;
  _expEtaMax      = Description::CMS::hacEtaMax;

  setEMCTowerGrid(TowerGrid(_emcEtaBin,_emcEtaMin,_emcEtaMax,_emcPhiBin));
  setHACTowerGrid(TowerGrid(_hacEtaBin,_hacEtaMin,_hacEtaMax,_hacPhiBin));

  _computeDerivedQuantities();

  _fillAccept();

  printf("\n#--------------------------------\n");
  description();
  printf("\n#--------------------------------\n");
}




