/*   This file is part of ccmpl
 *
 *   Copyright (C) 2015,  CentraleSupelec
 *
 *   Author : Herve Frezza-Buet
 *
 *   Contributor :
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public
 *   License (GPL) as published by the Free Software Foundation; either
 *   version 3 of the License, or any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *   General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *   Contact : Herve.Frezza-Buet@centralesupelec.fr
 *
 */

#pragma once

#include <cmath>
#include <iostream>

namespace ccmpl {

  namespace internal {
    inline double component_level(double theta) {
      if(theta < 0)      return component_level(theta+2*M_PI);
      if(theta > 2*M_PI) return component_level(theta-2*M_PI);
      
      if(theta < 2*M_PI/3) return 0;
      if(theta < M_PI)     return (theta-2*M_PI/3)/(M_PI/3);
      if(theta < 5*M_PI/3) return 1;
      return                      1-(theta-5*M_PI/3)/(M_PI/3);
    }
  }

  struct RGB {
    double r,g,b;
    RGB() : r(0), g(0), b(0) {}
    RGB(const RGB& cp) : r(cp.r), g(cp.g), b(cp.b) {}
    RGB(double rr, double gg, double bb) : r(rr), g(gg), b(bb) {}
  };

  namespace color {
    template<typename VALUE, typename POINT>
    RGB from_point(const POINT& p,
		   VALUE xmin, VALUE xmax,
		   VALUE ymin, VALUE ymax) {
      double x = 2*(p.x - xmin)/(double)(xmax - xmin) - 1;
      double y = 2*(p.y - ymin)/(double)(ymax - ymin) - 1;
      double theta = std::atan2(y,x); // in [-pi,pi]
      double n = std::sqrt(x*x+y*y);
      if(n>1) n=1;

      double r = ccmpl::internal::component_level(theta);
      double g = ccmpl::internal::component_level(theta + 2*M_PI/3);
      double b = ccmpl::internal::component_level(theta - 2*M_PI/3);

      return {n*r + (1-n),
	  g = n*g + (1-n),
	  b = n*b + (1-n)};
    }
  }

  struct Point {
    double x,y;
    Point() : x(0), y(0) {}
    Point(double xx, double yy) : x(xx), y(yy) {}

    Point operator-(const Point& other) const {
      return {x - other.x, y - other.y}; 
    }

    Point& operator-=(const Point& other) { 
      x -= other.x;
      y -= other.y;
      return *this;
    }

    Point operator+(const Point& other) const {
      return {x + other.x, y + other.y}; 
    }

    Point& operator+=(const Point& other) { 
      x += other.x;
      y += other.y;
      return *this;
    }  

    double operator*(const Point& other) const {
      return x*other.x + y*other.y; 
    }

    Point operator*(double value)  const {
      return {x * value, y * value}; 
    }

    Point& operator*=(double value) { 
      x *= value;
      y *= value;
      return *this;
    } 

    Point operator/(double value)  const {
      return {x / value, y / value}; 
    }

    Point& operator/=(double value) { 
      x /= value;
      y /= value;
      return *this;
    } 

    Point operator*() const {
      double norm = sqrt((*this) * (*this));
      return (*this)/norm;
    }
  };
    
  struct ValueAt {
    double x,y;
    double value; 
    ValueAt() :  x(0), y(0), value(0) {}
    ValueAt(double xx, double yy, double v) : x(xx), y(yy), value(v) {}
    ValueAt(const Point& p, double v) : x(p.x), y(p.y), value(v) {}
  };
    
  struct ColorAt {
    double x,y;
    RGB color;
    ColorAt(double xx, double yy,  const RGB& c) : x(xx), y(yy), color(c) {}
    ColorAt(const Point& p, const RGB& c) : x(p.x), y(p.y), color(c) {}
  };
    
  struct YRange {
    double x,y1,y2;
    YRange(double x, double y1, double y2) : x(x), y1(y1), y2(y2) {}
  };


  namespace patch {

    struct GC {
      bool fill = true;
      RGB edgecolor = {0.,0.,0.};
      RGB facecolor = {0.,0.,1.};
      double linewidth = 1.0;
      std::string linestyle = "solid";
      double zorder = 0;
      double alpha = 1.0;

      GC() = default;
      GC(const GC&) = default;
      GC& operator=(const GC&) = default;

      void print_edgecolor(std::ostream& os) {
	os << "edgecolor=(" 
	   << edgecolor.r << ',' 
	   << edgecolor.g << ',' 
	   << edgecolor.b << ')';
      }

      void print_facecolor(std::ostream& os) {
	os << "facecolor=(" 
	   << facecolor.r << ',' 
	   << facecolor.g << ',' 
	   << facecolor.b << ')';
      }
      void print_fill(std::ostream& os) {
	os << "fill=" << (fill ? "True" : "False");
      }
      void print_linewidth(std::ostream& os) {
	os << "linewidth=" << linewidth;
      }
      void print_linestyle(std::ostream& os) {
	os << "linestyle='" << linestyle << "'";
      }
      void print_zorder(std::ostream& os) {
          os << "zorder=" << zorder;
      }
      void print_alpha(std::ostream& os) {
          os << "alpha=" << alpha;
      }
    };
  }

  struct Patch {
    patch::GC gc;

    Patch(const patch::GC& gc) : gc(gc) {}
    Patch(const Patch&) = default;
    Patch& operator=(const Patch&) = default;

    virtual ~Patch() {}
    virtual void toPython(std::ostream& os)=0;
    virtual void print_attr(std::ostream& os) {
      gc.print_edgecolor(os);
      os << ',';
      gc.print_facecolor(os);
      os << ',';
      gc.print_fill(os);
      os << ',';
      gc.print_linewidth(os);
      os << ',';
      gc.print_linestyle(os);
      os << ',';
      gc.print_zorder(os);
      os << ',';
      gc.print_alpha(os);
    }
  };

  namespace patch {

    struct Circle : public Patch {
      Point center;
      double radius;
      Circle(const GC& gc, const Point& c, double r) : 
	Patch(gc),
	center(c), radius(r) {}
      Circle(const Circle&) = default;
      Circle& operator=(const Circle&) = default;
      virtual ~Circle() {}
      virtual void toPython(std::ostream& os) {
	os << "patches.Circle(("
	   << center.x << ',' << center.y << "), " << radius << ", ";
	print_attr(os);
	os << ')';
      }
    };

    struct Rectangle : public Patch {
      Point origin;
      double width,height;
      double angle;
      Rectangle(const GC& gc, const Point& o, double w, double h, double a) : 
	Patch(gc),
	origin(o), width(w), height(h), angle(a) {}
      Rectangle(const Rectangle&) = default;
      Rectangle& operator=(const Rectangle&) = default;
      virtual ~Rectangle() {}
      virtual void toPython(std::ostream& os) {
	os << "patches.Rectangle(("
	   << origin.x << ',' << origin.y << "), "
	   << width << ", " << height << ", " << angle << ", ";
	print_attr(os);
	os << ')';
      }
    };

    struct Arrow : public Patch {

      double x,y,dx,dy,width;
      Arrow(const GC& gc,
	    double x, double y, double dx, double dy,
	    double width) :
	Patch(gc),
	x(x), y(y),dx(dx),dy(dy), width(width){}

      Arrow(const Arrow&) = default;
      Arrow& operator=(const Arrow&) = default;
      virtual ~Arrow() {}
      virtual void toPython(std::ostream& os) {
	os << "patches.Arrow("
	   << x << "," << y << "," << dx << "," << dy
	   << ", width=" << width << ", ";
	print_attr(os);
	os << ')';
      }
    };

    struct FancyArrow : public Patch {
      double x,y,dx,dy,width,head_width,head_length,overhang;
      bool length_includes_head;
      FancyArrow(const GC& gc, 
		 double x, double y, double dx, double dy, 
		 double width, double head_width, double head_length,
		 double overhang,
		 bool length_includes_head) : 
	Patch(gc),
	x(x),y(y),dx(dx),dy(dy),
	width(width),head_width(head_width),head_length(head_length),
	overhang(overhang), length_includes_head(length_includes_head) {}
      FancyArrow(const FancyArrow&) = default;
      FancyArrow& operator=(const FancyArrow&) = default;
      virtual ~FancyArrow() {}
      virtual void toPython(std::ostream& os) {
	os << "patches.FancyArrow("
	   << x << ',' << y << ',' << dx << ',' << dy
	   << ", width = " << width
	   << ", head_width = " << head_width
	   << ", head_length = " << head_length
	   << ", overhang = " << overhang 
	   << ", length_includes_head = " << length_includes_head << ", ";
	print_attr(os);
	os << ')';
      }
    };

   struct Wedge : public Patch {
     Point center;
     double radius;
     double theta1, theta2;
     Wedge(const GC& gc,
	   Point c, double r,
	   double theta1, double theta2):
       Patch(gc),
       center(c), radius(r), 
       theta1(theta1), theta2(theta2) {}

     Wedge(const Wedge&) = default;
     Wedge& operator=(const Wedge&) = default;
     virtual ~Wedge() {}
     virtual void toPython(std::ostream& os) {
       os << "patches.Wedge(" 
	  << "(" << center.x << "," << center.y << ")," 
	  << radius << ","
	  << theta1 << ","
	  << theta2 << ", ";
       print_attr(os);
       os << ")";
     }
     
   };

   struct Arc : public Patch {
     Point center;
     double width, height;
     double angle, theta1, theta2;
     Arc(const GC& gc,
	 Point c, double w, double h,
	 double a, double t1, double t2):
       Patch(gc), center(c), width(w), height(h), 
       angle(a), theta1(t1), theta2(t2) {}
     Arc(const Arc&) = default;
     Arc& operator=(const Arc&) = default;
     virtual ~Arc() {}
     virtual void toPython(std::ostream& os) {
       os << "patches.Arc(" 
	  << "(" << center.x << "," << center.y << ")," 
	  << width << "," << height << ","
	  << angle << "," << theta1 << "," << theta2;
  
       os << ',';
       gc.print_edgecolor(os);
       os << ',';
       gc.print_facecolor(os);
       os << ',';
       //gc.print_fill(os);
       //os << ',';
       gc.print_linewidth(os);
       os << ',';
       gc.print_linestyle(os); 
       os << ',';
       gc.print_zorder(os);
       os << ',';
       gc.print_alpha(os);
       os << ')';

     }
   }; 



  }

  
}
