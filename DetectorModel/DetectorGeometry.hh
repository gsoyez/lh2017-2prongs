// -*- c++ -*-
#ifndef DETECTORMODEL_DETECTORGEOMETRY_H
#define DETECTORMODEL_DETECTORGEOMETRY_H

#include <cmath>

namespace Detector {
  ///@brief Geometry
  namespace Geometry {
    ///@brief Numerical constants
    struct Constants
    {
      static constexpr double halfpi = std::asin(1.);
      static constexpr double pi     = 2.*halfpi;
      static constexpr double twopi  = 2.*pi;
    };
    ///@brief Helpers
    struct Helpers 
    {
      static double azimuth(double x,double y)
      { 
	if ( x == 0. ) { return y > 0. ? Constants::halfpi : -Constants::halfpi; } 
	return std::atan2(y,x);
      }
    };
    ///@brief A point in two  dimensions
    ///
    /// Provides simple vector algebra in 2-dim as well 
    class Point2d 
    {
    public:
      ///@brief Constructor from coordinates
      Point2d(double x=0.,double y=0.);
      ///@brief Copy constructor
      Point2d(const Point2d& p);
      
      ///@name Access to point
      ///@{
      double x() const;   ///<@brief @f$ x @f$ component (Cartesian coordinates)
      double y() const;   ///<@brief @f$ y @f$ component (Cartesian coordinates)
      double r() const;   ///<@brief distance from origin (0,0)
      double phi() const; ///<@brief azimuth 
      ///@}
      
      ///@name Setting the point
      ///@{
      void setCoordinates(double x,double y); ///<@brief Set point coordinates
      //Point2d& operator=(const Point2d& p);   ///<@brief Assigment operator
      ///@}

      ///@name Measures
      ///@{
      double distance(const Point2d& point) const; ///<@brief Distance from other point
      ///@}
      
      ///@name Translation 
      ///@{
      Point2d operator-(const Point2d& p)   const; ///<@brief Create new point at coordinates resulting from the component-wise subtraction 
      Point2d operator+(const Point2d& p)   const; ///<@brief Create new point at coordinates resulting from the component-wise addition
      Point2d& operator-=(const Point2d& p);       ///<@brief Move this point by subtraction  
      Point2d& operator+=(const Point2d& p);       ///<@brief Move this point by addition
      ///@}
      
      ///@name Comparators
      ///@{
      bool operator==(const Point2d& p) const;  ///<@brief Equality (same point)
      bool operator!=(const Point2d& p) const;  ///<@brief Inequality (not the same point)
      ///@}
      
    private:

      ///@name Coordinate store
      ///@{
      double _x;
      double _y;
      double _r;
      double _phi;
      ///@}
      
    };

    class Circle;

    ///@brief A line in  two dimensions
    ///
    /// The line is constructed from two points. It has length associated with it which is the 
    /// length of the line segment between these two points.
    class Line2d
    {
    public:
      
      ///@brief Construct from coordinates of two points
      Line2d(double x0=0.,double y0=0.,double x1=0.,double y1=0.);
      ///@brief Constuct from two points
      Line2d(const Point2d& p0,const Point2d& p1);

      ///@name Accessors
      ///@{
      double x0()                                   const; ///<@brief @f$ x @f$ coordinate first point (hook)
      double y0()                                   const; ///<@brief @f$ y @f$ coordinate first point (hook)
      double x1()                                   const; ///<@brief @f$ x @f$ coordinate second point
      double y1()                                   const; ///<@brief @f$ y @f$ coordinate second point (hook)
      const Point2d& begin()                        const; ///<@brief First point (beginning of line segment)
      const Point2d& end()                          const; ///<@brief Second point (end of line segment)
      double length()                               const; ///<@brief Length of line segment 
      double angle()                                const; ///<@brief Angle of line
      double angle(const Line2d& l)                 const; ///<@brief Angle between two lines
      double cos_angle()                            const; ///<@brief Cosine of angle of line
      double cos_angle(const Line2d& l)             const; ///<@brief Cosine of angle between two lines 
      bool   getSlopeAndOffset(double& m,double& s) const; ///<@brief Calculate offset and slope 
      ///@}

      ///@name Set line coordinates
      ///@{
      void setPoints(const Point2d& p0,const Point2d& p1);
      void setSlopeAndOffset(double m,double b);
      ///@}

      ///@name Intersections
      ///
      /// These functions calculate the intersection between the line and othe geometrical objects.
      ///@{
      ///@brief Intersection with other line
      ///
      ///@return 1 and the intersection point if lines intersect, else 0.
      unsigned int intersect(const Line2d& l,Point2d& p) const;
      ///@brief Intersection of line with circle
      ///
      ///@return 0 if no intersection, 1 if tangent, 2 if secant. 
      unsigned int intersect(const Circle& c,Point2d& p0,Point2d& p1) const;
      ///@}
 
      ///@name Measures
      ///@{
      double distance(const Point2d& p) const; ///<@ Shortest distance between point and line (0 if point is on line)
      double distance(const Line2d& l)  const;  ///<@ Shortest distance between two lines (0 if lines intersect)
      ///@}

      ///@name Generate other lines or move
      ///
      /// These are  vector-like features and functionality!
      ///@{
      Line2d operator+(const Line2d& l)    const; ///<@brief Generate new line by adding another line to this line
      Line2d operator-(const Line2d& l)    const; ///<@brief Generate new line by subtrating another line from this line
      Line2d& operator+=(const Line2d& l);        ///<@brief Move this line by adding another one
      Line2d& operator-=(const Line2d& l);        ///<@brief Move this line by subtracting another one
      bool operator==(const Line2d& l)     const; ///<@brief Compare two lines for equality
      bool operator!=(const Line2d& l)     const; ///<@brief Compare two lines for inequality
      Line2d getNormal()                   const; ///<@brief Get a line perpendicular to this line
      ///@}
      
    private:

      ///@name Line coordinates
      ///@{
      Point2d _p0;
      Point2d _p1;
      double _length;
      double _angle;
      double _cos_angle;
      ///@}
    };

    ///@brief Circle in two dimensions
    class Circle
    {
    public:
      ///@brief Construct a circle from center coordinates and a radius
      Circle(double x=0.,double y=0.,double r=0.);
      ////@brief Construct a circle from a center pointer and a radius 
      Circle(const Point2d& center,double r);

      ///@name Accessors
      ///@{
      const Point2d& center() const; ///<@brief Center of circle
      double         radius() const; ///<@brief Radius of circle
      ///@}
      
      ///@name Intersections
      ///@{
      unsigned int intersect(const Circle& c,Point2d& p1,Point2d& p2) const; ///<@brief Intersection with another circle (up to two points)
      unsigned int intersect(const Line2d& l,Point2d& p1,Point2d& p2) const; ///<@brief Intersection with a line (up to two points)
      ///@}

      ///@name Set circle geometry
      ///@{
      void setCenter(const Point2d& point); ///<@brief Set the center of the circle
      void setRadius(double r);             ///<@brief Set the radius
      ///@}

    private:

      ///@name Circle coordinates
      ///@{
      Point2d _center;
      double  _radius;
      ///@}
    };
  } // Geometry
} // Detector

// -- Point -------------------------------------------------------------------------------

inline double Detector::Geometry::Point2d::x()                          const { return _x; }
inline double Detector::Geometry::Point2d::y()                          const { return _y; }
inline double Detector::Geometry::Point2d::r()                          const { return _r; }
inline double Detector::Geometry::Point2d::phi()                        const { return _phi; }
inline double Detector::Geometry::Point2d::distance(const Point2d& p)   const { double dx(_x-p.x()); double dy(_y-p.y()); return std::sqrt(dx*dx+dy*dy); }
inline bool   Detector::Geometry::Point2d::operator==(const Point2d& p) const { return _x == p.x() && _y == p.y(); }
inline bool   Detector::Geometry::Point2d::operator!=(const Point2d& p) const { return _x != p.x() || _y != p.y(); }

inline void   Detector::Geometry::Point2d::setCoordinates(double x,double y) 
{ _x = x; _y = y; _r = std::sqrt(x*x+y*y); _phi = Detector::Geometry::Helpers::azimuth(_x,_y); }

inline Detector::Geometry::Point2d  Detector::Geometry::Point2d::operator+(const Point2d& p)  const { return Point2d(_x+p.x(),_y+p.y()); }
inline Detector::Geometry::Point2d  Detector::Geometry::Point2d::operator-(const Point2d& p)  const { return Point2d(_x-p.x(),_y-p.y()); }

inline Detector::Geometry::Point2d& Detector::Geometry::Point2d::operator+=(const Point2d& p) 
{ _x+=p.x(); _y+=p.y(); _r=std::sqrt(_x*_x+_y*_y); _phi = Detector::Geometry::Helpers::azimuth(_x,_y); return *this; }
inline Detector::Geometry::Point2d& Detector::Geometry::Point2d::operator-=(const Point2d& p) 
{ _x-=p.x(); _y-=p.y(); _r=std::sqrt(_x*_x+_y*_y); _phi = Detector::Geometry::Helpers::azimuth(_x,_y); return *this; }

// -- Line --------------------------------------------------------------------------------

inline double Detector::Geometry::Line2d::x0() const { return _p0.x(); }
inline double Detector::Geometry::Line2d::y0() const { return _p0.y(); }
inline double Detector::Geometry::Line2d::x1() const { return _p1.x(); }
inline double Detector::Geometry::Line2d::y1() const { return _p1.y(); }

inline const Detector::Geometry::Point2d& Detector::Geometry::Line2d::begin() const { return _p0; }
inline const Detector::Geometry::Point2d& Detector::Geometry::Line2d::end()   const { return _p1; }

inline double Detector::Geometry::Line2d::length()               const { return _length; }
inline double Detector::Geometry::Line2d::angle()                const { return _angle; }
inline double Detector::Geometry::Line2d::angle(const Line2d& l) const { return std::acos(this->cos_angle(l)); }
inline double Detector::Geometry::Line2d::cos_angle()            const { return _cos_angle; }

#endif
