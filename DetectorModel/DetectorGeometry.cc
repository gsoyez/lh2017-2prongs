
#include "DetectorGeometry.hh"

///////////
// Point //
///////////

Detector::Geometry::Point2d::Point2d(double x,double y) 
  : _x(x)
  , _y(y)
  , _r(std::sqrt(x*x+y*y))
  , _phi(Detector::Geometry::Helpers::azimuth(_x,_y) )
{ }

Detector::Geometry::Point2d::Point2d(const Point2d& p) : _x(p.x()), _y(p.y()), _r(p.r()), _phi(p.phi()) { }

//////////
// Line //
//////////

Detector::Geometry::Line2d::Line2d(const Point2d& p0,const Point2d& p1) : _p0(p0), _p1(p1), _length(p0.distance(p1)), _angle(0.), _cos_angle(0.)
{ setPoints(p0,p1); }

Detector::Geometry::Line2d::Line2d(double x0,double y0,double x1,double y1) : Line2d(Point2d(x0,y0),Point2d(x1,y1))
{ }

void Detector::Geometry::Line2d::setPoints(const Point2d& p0,const Point2d& p1)
{ 
  _p0 = p0; _p1 = p1; _length = _p0.distance(_p1); 
  Point2d pp(_p1-_p0); _angle = pp.x() != 0. ? std::atan2(pp.y(),pp.x()) : 0.; 
  _cos_angle = std::cos(_angle); 
}

double Detector::Geometry::Line2d::cos_angle(const Line2d& l) const 
{ 
  double m1, m2, b1, b2; 
  this->getSlopeAndOffset(m1,b1); l.getSlopeAndOffset(m2,b2); 
  double dn(1.+m1*m2);
  return dn != 0. ? (m1-m2)/dn : 0.; 
}

void Detector::Geometry::Line2d::setSlopeAndOffset(double m,double c)
{ if ( m != 0. ) { setPoints(Point2d(0.,c),Point2d(-c/m,0.)); } else { setPoints(Point2d(0.,c),Point2d(0.,c)); } }


unsigned int Detector::Geometry::Line2d::intersect(const Line2d& l,Point2d& p) const
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

unsigned int Detector::Geometry::Line2d::intersect(const Circle& c,Point2d& p0,Point2d& p1) const
{
  // no line
  double dr(this->length());
  if ( dr == 0. ) { return 0; }

  // from https://math.stackexchange.com/questions/228841/how-do-i-calculate-the-intersections-of-a-straight-line-and-a-circle

  // collect variables
  double p(c.center().x()), q(c.center().y()), r(c.radius());
  double m(0.), b(0.);
  if ( !this->getSlopeAndOffset(m,b) ) { return 0; }
 
  // define terms
  double A(m*m+1);
  double B(2.*(m*b-m*q-p));
  double C(q*q-r*r+p*p-2.*b*q+b*b);
  double D(B*B-4.*A*C); 
  if ( D < 0. ) { return 0; } // no intercept
  if ( D < 0.0000001 ) {      // tangential
    double px(-B/(2.*A));
    double py(m*px+b);
    p0.setCoordinates(px,py);
    return 1;
  }
  D = std::sqrt(D);           // secant (two intercepts)
  double x0((-B+D)/(2.*A)); double y0(m*x0+b);
  double x1((-B-D)/(2.*A)); double y1(m*x1+b);
  p0.setCoordinates(x0,y0);
  p1.setCoordinates(x1,y1);

  return 2;
}

Detector::Geometry::Line2d Detector::Geometry::Line2d::getNormal() const
{
  double dx(this->end().x()-this->begin().x());
  double dy(this->end().y()-this->begin().y());
  return Line2d(Point2d(-dy,dx),Point2d(dy,-dx));
}

bool Detector::Geometry::Line2d::getSlopeAndOffset(double& m,double& c) const
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

Detector::Geometry::Circle::Circle(const Point2d& center,double r) : _center(center), _radius(r) { }
Detector::Geometry::Circle::Circle(double x0,double y0,double r) : Circle(Point2d(x0,y0),r) { }

const Detector::Geometry::Point2d& Detector::Geometry::Circle::center() const { return _center; }
double Detector::Geometry::Circle::radius() const { return _radius; }

unsigned int Detector::Geometry::Circle::intersect(const Line2d& l,Point2d& p0,Point2d& p1) const
{ return l.intersect(*this,p0,p1); }

// from https://math.stackexchange.com/questions/256100/how-can-i-find-the-points-at-which-two-circles-intersect

unsigned int Detector::Geometry::Circle::intersect(const Circle& cc,Point2d& p0,Point2d& p1) const
{
  //////////////////////////////////////////////////

  // check on basics
  double r(this->radius());
  if ( r == 0. ) { return 0; }

  // collect variables
  double p(this->center().x());
  double q(this->center().y());
  double u(cc.center().x());
  double v(cc.center().y());
  double R(cc.radius());

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
    double x(-qb/(2.*qa));
    double y(m*x+c);
    p0.setCoordinates(x,y);
    return 1;
  } 
  double x0((-qb+std::sqrt(qt))/(2.*qa));
  double y0(m*x0+c);
  double x1((-qb-std::sqrt(qt))/(2.*qa));
  double y1(m*x1+c);
  p0.setCoordinates(x0,y0);
  p1.setCoordinates(x1,y1);

  return 2;
}
