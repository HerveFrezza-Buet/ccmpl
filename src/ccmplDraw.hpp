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

#include <functional>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <memory>

#include <ccmplTypes.hpp>
#include <ccmplChart.hpp>

namespace ccmpl {

  /////////
  //     //
  // Dot //
  //     //
  /////////

  class Dot : public chart::Data {
  public:
    Point point;
    std::function<void (Point&)> fill;
      
    template<typename FILL>
    Dot(const std::string& arglist,
	const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Dot() {}
      
    virtual chart::Element* clone() const {
      Dot* res = new Dot(args,fill);
      res->point = point;
      return res;
    }

    virtual void refill() {
      fill(point);
    }

    virtual void _print_data(std::ostream& os) {
      os << point.x << ' ' << point.y << std::endl;	
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_dot(os,suffix,args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_dot(os,suffix);
    }
      
  };

  template<typename FILL>
  Dot dot(const std::string& arglist,
	  const FILL& f) {
    return Dot(arglist,f);
  }


  //////////
  //      //
  // VBar //
  //      //
  //////////

  class Vbar : public chart::Data {
  public:
    double x;
    std::function<void (double&)> fill;

    template<typename FILL>
    Vbar(const std::string& arglist,
	 const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Vbar() {}
      
      
    virtual chart::Element* clone() const {
      Vbar* res = new Vbar(args,fill);
      res->x = x;
      return res;
    }

    virtual void refill() {
      fill(x);
    }

    virtual void _print_data(std::ostream& os) {
      os << x << std::endl;	
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_vbar(os,suffix);
    }

    virtual void plot(std::ostream& os) {
      python::plot_vbar(os,suffix,args);
    }
      
  };

  template<typename FILL>
  Vbar vbar(const std::string& arglist,
	    const FILL& f) {
    return Vbar(arglist,f);
  }

  //////////
  //      //
  // HBar //
  //      //
  //////////

  class Hbar : public chart::Data {
  public:
    double y;
    std::function<void (double&)> fill;
      
    template<typename FILL>
    Hbar(const std::string& arglist,
	 const FILL& f) : chart::Data(arglist), fill(f)  {}
    virtual ~Hbar() {}
      
    virtual chart::Element* clone() const {
      Hbar* res = new Hbar(args,fill);
      res->y = y;
      return res;
    }

    virtual void refill() {
      fill(y);
    }

    virtual void _print_data(std::ostream& os) {
      os << y << std::endl;	
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_hbar(os,suffix);
    }

    virtual void plot(std::ostream& os) {
      python::plot_hbar(os,suffix,args);
    }
      
  };

  template<typename FILL>
  Hbar hbar(const std::string& arglist,
	    const FILL& f) {
    return Hbar(arglist,f);
  }

  //////////
  //      //
  // Dots //
  //      //
  //////////

  class Dots : public chart::Data {
  public:
    std::vector<Point> points;
    std::function<void (std::vector<Point>&)> fill;
      
    template<typename FILL>
    Dots(const std::string& arglist,
	 const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Dots() {}
      
    virtual chart::Element* clone() const {
      Dots* res = new Dots(args,fill);
      res->points = points;
      return res;
    }

    virtual void refill() {
      fill(points);
    }
			    

    virtual void _print_data(std::ostream& os) {
      for(auto& pt : points) 
	os << ' ' << pt.x;
      os << std::endl;
      for(auto& pt : points) 
	os << ' ' << pt.y;
      os << std::endl;	
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_dots(os,suffix,args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_dots(os,suffix);
    }
      
  };

  template<typename FILL>
  Dots dots(const std::string& arglist, const FILL& f) {
    return Dots(arglist,f);
  }


  /////////////
  //         //
  // Between //
  //         //
  /////////////

 
  class Between : public chart::Data {
  public:
    std::vector<YRange> points;
    std::function<void (std::vector<YRange>&)> fill;
      
    template<typename FILL>
    Between(const std::string& arglist,
	    const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Between() {}
      
    virtual chart::Element* clone() const {
      Between* res = new Between(args,fill);
      res->points = points;
      return res;
    }

    virtual void refill() {
      fill(points);
    }
			    

    virtual void _print_data(std::ostream& os) {
      for(auto& pt : points) 
	os << ' ' << pt.x;
      os << std::endl;
      for(auto& pt : points) 
	os << ' ' << pt.y1;
      os << std::endl;	
      for(auto& pt : points) 
	os << ' ' << pt.y2;
      os << std::endl;	
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_between(os,suffix,args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_between(os,suffix);
    }
      
  };

  template<typename FILL>
  Between between(const std::string& arglist,const FILL& f) {
    return Between(arglist,f);
  }

  /////////
  //     //
  // Pie //
  //     //
  /////////

 
  class Pie : public chart::Data {
  public:

    struct Wedge {
      double      value;    //!< The value of the wedge
      RGB         color;    //!< The color of the wedge
      std::string label;    //!< The label of the wedge
      double      explode;  //!< The fraction of the radius with which to offset each wedge.
      Wedge(double value, const RGB& color, const std::string& label, double explode)
	: value(value), color(color), label(label), explode(explode) {}
    };
    
    std::vector<Wedge> wedges;
    std::function<void (std::vector<Wedge>&)> fill;
      
    template<typename FILL>
    Pie(const std::string& arglist,
	const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Pie() {}
      
    virtual chart::Element* clone() const {
      Pie* res = new Pie(args,fill);
      res->wedges = wedges;
      return res;
    }

    virtual void refill() {
      fill(wedges);
    }
			    

    virtual void _print_data(std::ostream& os) {
      for(auto& w : wedges) 
	os << ' ' << w.value;
      os << std::endl;
      
      for(auto& w : wedges) 
	os << w.color.r << ' ' << w.color.g << ' ' << w.color.b << std::endl;

      for(auto& w : wedges)
	os << w.label << std::endl;
      
      
      for(auto& w : wedges) 
	os << ' ' << w.explode;
      os << std::endl;
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_pie(os,suffix, args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_pie(os,suffix);
    }
      
  };

  template<typename FILL>
  Pie pie(const std::string& arglist,const FILL& f) {
    return Pie(arglist,f);
  }


  
  //////////
  //      //
  // Line //
  //      //
  //////////

 
  class Line : public chart::Data {
  public:
    std::vector<Point> points;
    std::function<void (std::vector<Point>&)> fill;
      
    template<typename FILL>
    Line(const std::string& arglist,
	 const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Line() {}
      
    virtual chart::Element* clone() const {
      Line* res = new Line(args,fill);
      res->points = points;
      return res;
    }

    virtual void refill() {
      fill(points);
    }
			    

    virtual void _print_data(std::ostream& os) {
      for(auto& pt : points) 
	os << ' ' << pt.x;
      os << std::endl;
      for(auto& pt : points) 
	os << ' ' << pt.y;
      os << std::endl;	
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_line(os,suffix);
    }

    virtual void plot(std::ostream& os) {
      python::plot_line(os,suffix,args);
    }
      
  };

  template<typename FILL>
  Line line(const std::string& arglist,const FILL& f) {
    return Line(arglist,f);
  }

  
  ///////////
  //       //
  // Lines //
  //       //
  ///////////

  class Lines : public chart::Data {
  public:
    std::vector<std::vector<Point> > lines;
    std::function<void (std::vector<std::vector<Point>>&)> fill;
      
    template<typename FILL>
    Lines(const std::string& arglist,
	  const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Lines() {}
      
    virtual chart::Element* clone() const {
      Lines* res = new Lines(args,fill);
      res->lines = lines;
      return res;
    }

    virtual void refill() {
      fill(lines);
    }
			    

    virtual void _print_data(std::ostream& os) {
      os << lines.size() << std::endl;
      for(auto& points : lines) {
	for(auto& pt : points) 
	  os << ' ' << pt.x;
	os << std::endl;
	for(auto& pt : points) 
	  os << ' ' << pt.y;
	os << std::endl;
      }
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_lines(os,suffix,args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_lines(os,suffix);
    }
      
  };

  template<typename FILL>
  Lines lines(const std::string& arglist,const FILL& f) {
    return Lines(arglist,f);
  }


  /////////////
  //         //
  // Vectors // 
  //         //
  /////////////

  class Vectors : public chart::Data {
  public:
    std::vector<std::pair<Point,Point>> vectors; // origin, vector.

    std::function<void (std::vector<std::pair<Point,Point>>&)> fill;
      
    template<typename FILL>
    Vectors(const std::string& arglist,
	    const FILL& f) 
      : chart::Data(arglist), 
      fill(f) {}
    virtual ~Vectors() {}
      
    virtual void refill() {
      fill(vectors);
    }

    virtual void _print_data(std::ostream& os) {
      for(auto& v : vectors) 
	os << ' ' << v.first.x;
      os << std::endl;
      for(auto& v : vectors) 
	os << ' ' << v.first.y;
      os << std::endl;	
      for(auto& v : vectors) 
	os << ' ' << v.second.x;
      os << std::endl;
      for(auto& v : vectors) 
	os << ' ' << v.second.y;
      os << std::endl;	
    }

    virtual chart::Element* clone() const {
      Vectors* res = new Vectors(args,fill);
      res->vectors = vectors;
      return res;
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_vectors(os,suffix,args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_vectors(os,suffix);
    }
      
  };

  template<typename FILL>
  Vectors vectors(const std::string& arglist,
		  const FILL& f) {
    return Vectors(arglist,f);
  }


  /////////////
  //         //
  // Surface //
  //         //
  /////////////


  class Surface : public chart::Data {
  public:
    std::vector<ValueAt> points;
    double min,max;

    std::function<void (std::vector<ValueAt>&)> fill;
      
    template<typename FILL>
    Surface(const std::string& arglist,
	    double min_value,
	    double max_value,
	    const FILL& f) 
      : chart::Data(arglist), 
      min(min_value),
      max(max_value),
      fill(f) {}
    virtual ~Surface() {}
      
    virtual void refill() {
      fill(points);
    }

    virtual void _print_data(std::ostream& os) {
      for(auto& pt : points) 
	os << ' ' << pt.x;
      os << std::endl;
      for(auto& pt : points) 
	os << ' ' << pt.y;
      os << std::endl;	
      for(auto& pt : points) 
	os << ' ' << pt.value;
      os << std::endl;	
    }

    virtual chart::Element* clone() const {
      Surface* res = new Surface(args,min,max,fill);
      res->points = points;
      return res;
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_surface(os,suffix,args,min,max);
    }

    virtual void plot(std::ostream& os) {
      python::plot_surface(os,suffix);
    }
      
  };

  template<typename FILL>
  Surface surface(const std::string& arglist,
		  double min_value,
		  double max_value,
		  const FILL& f) {
    return Surface(arglist,min_value,max_value,f);
  }

  /////////////
  //         //
  // Palette //
  //         //
  /////////////

  class Palette : public chart::Data {
  public:
    std::vector<ColorAt> points;
    std::function<void (std::vector<ColorAt>&)> fill;
      
    template<typename FILL>
    Palette(const std::string& arglist, const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Palette() {}
      
    virtual void _print_data(std::ostream& os) {
      for(auto& pt : points) 
	os << ' ' << pt.x;
      os << std::endl;
      for(auto& pt : points) 
	os << ' ' << pt.y;
      os << std::endl;
      for(auto& pt : points) 
	os << ' ' << pt.color.r
	   << ' ' << pt.color.g
	   << ' ' << pt.color.b
	   << ',';
      os << std::endl;
    }

    virtual chart::Element* clone() const {
      Palette* res = new Palette(args,fill);
      res->points = points;
      return res;
    }

    virtual void refill() {
      fill(points);
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_palette(os,suffix,args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_palette(os,suffix);
    }
      
  };

  template<typename FILL>
  Palette palette(const std::string& arglist, const FILL& f) {
    return Palette(arglist,f);
  }


  //////////////
  //          //
  // Confetti //
  //          //
  //////////////

  class Confetti : public chart::Data {
  public:
    std::vector<ColorAt> points;
    std::function<void (std::vector<ColorAt>&)> fill;
      
    template<typename FILL>
    Confetti(const std::string& arglist, const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Confetti() {}
      
    virtual void _print_data(std::ostream& os) {
      for(auto& pt : points) 
	os << ' ' << pt.x;
      os << std::endl;
      for(auto& pt : points) 
	os << ' ' << pt.y;
      os << std::endl;
      for(auto& pt : points) 
	os << ' ' << pt.color.r
	   << ' ' << pt.color.g
	   << ' ' << pt.color.b
	   << ',';
      os << std::endl;
    }

    virtual chart::Element* clone() const {
      Confetti* res = new Confetti(args,fill);
      res->points = points;
      return res;
    }

    virtual void refill() {
      fill(points);
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_confetti(os,suffix,args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_confetti(os,suffix);
    }
      
  };

  template<typename FILL>
  Confetti confetti(const std::string& arglist, const FILL& f) {
    return Confetti(arglist,f);
  }


  /////////////
  //         //
  // Histo1d // 
  //         //
  /////////////


  class Histo1d : public chart::Data {
  public:

    std::function<void (std::vector<double>&)> fill;
    std::vector<double> data;
    double min, max;
    unsigned int nb;
      
      
    template<typename FILL>
    Histo1d(const std::string& arglist, 
	    const FILL& f,
	    double min, double max, unsigned int nb_bins) 
      : chart::Data(arglist), fill(f),
      data(),
      min(min), max(max), nb(nb_bins){}
    virtual ~Histo1d() {}
      
    virtual void _print_data(std::ostream& os) {
      double coef = (max - min)/nb;
      double norm = 1/coef;

      // bar width
      os << coef << std::endl;
      
      // bin centers
      for(unsigned int b = 0; b < nb; ++b)
	os << ' ' << min + (b+.5)*coef;
      os << std::endl;

      // Histogram computation
      std::vector<unsigned int> h(nb, 0);
      for(auto d : data)
	if(min <= d && d < max)
	  ++(h[(int)((d-min)*norm)]);

      for(auto bar : h)
	os << ' ' << bar;
      os << std::endl;
      
    }

    virtual chart::Element* clone() const {
      Histo1d* res = new Histo1d(args,fill,
				 min,max,nb);
      
      res->data = data;
      return res;
    }

    virtual void refill() {
      fill(data);
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_histo1d(os,suffix, args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_histo1d(os,suffix);
    }
      
  };

  template<typename FILL>
  Histo1d histo1d(const std::string& arglist, const FILL& f,
		  double min, double max, unsigned int nb_bins) {
    return Histo1d(arglist,f,
		   min,max,nb_bins);
  }
  

  /////////////
  //         //
  // Histo2d // 
  //         //
  /////////////


  class Histo2d : public chart::Data {
  public:

    std::function<void (std::vector<Point>&)> fill;
    std::vector<Point> data;
    double x_min, x_max;
    unsigned int nbx;
    double y_min, y_max;
    unsigned int nby;
      
      
    template<typename FILL>
    Histo2d(const std::string& arglist, 
	    const FILL& f,
	    double xmin, double xmax, unsigned int nb_xbins,
	    double ymin, double ymax, unsigned int nb_ybins) 
      : chart::Data(arglist), fill(f),
      data(),
      x_min(xmin), x_max(xmax), nbx(nb_xbins),
      y_min(ymin), y_max(ymax), nby(nb_ybins){}
    virtual ~Histo2d() {}
      
    virtual void _print_data(std::ostream& os) {
      std::vector<unsigned int> hits(nbx*nby,0);
      for(auto& pt : data) {
	if(pt.x >= x_min && pt.y >= y_min
	   && pt.x < x_max && pt.y < y_max) {
	  unsigned int w = (unsigned int)((pt.x-x_min)*nbx/(x_max-x_min));
	  unsigned int h = (unsigned int)((pt.y-y_min)*nby/(y_max-y_min));
	  ++(hits[w+h*nbx]);
	}
      }

      for(unsigned int h = 0 ; h < nby ; ++h) {
	for(unsigned int w = 0 ; w < nbx ; ++w) 
	  os << ' ' << hits[w + h*nbx];
	os << " 0";
      }
      for(unsigned int w = 0 ; w <= nbx ; ++w)
	os << " 0";
      os << std::endl;
    }

    virtual chart::Element* clone() const {
      Histo2d* res = new Histo2d(args,fill,
				 x_min,x_max,nbx,
				 y_min,y_max,nby);
      res->data = data;
      return res;
    }

    virtual void refill() {
      fill(data);
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_histo2d(os,suffix,args,nbx,nby);
    }

    virtual void plot(std::ostream& os) {
      python::plot_histo2d(os,suffix,
			   x_min,x_max,nbx,
			   y_min,y_max,nby);
    }
      
  };

  template<typename FILL>
  Histo2d histo2d(const std::string& arglist, const FILL& f,
		  double xmin, double xmax, unsigned int nb_xbins,
		  double ymin, double ymax, unsigned int nb_ybins) {
    return Histo2d(arglist,f,
		   xmin,xmax,nb_xbins,
		   ymin,ymax,nb_ybins);
  }

  /////////////
  //         //
  // Histo3d // 
  //         //
  /////////////

  class Histo3d : public chart::Data {
  public:

    std::function<void (std::vector<Point>&)> fill;
    std::vector<Point> data;
    double x_min, x_max;
    unsigned int nbx;
    double y_min, y_max;
    unsigned int nby;
      
      
    template<typename FILL>
    Histo3d(const std::string& arglist, 
	    const FILL& f,
	    double xmin, double xmax, unsigned int nb_xbins,
	    double ymin, double ymax, unsigned int nb_ybins) 
      : chart::Data(arglist), fill(f),
      data(),
      x_min(xmin), x_max(xmax), nbx(nb_xbins),
      y_min(ymin), y_max(ymax), nby(nb_ybins){}
    virtual ~Histo3d() {}
      
    virtual void _print_data(std::ostream& os) {
      std::vector<unsigned int> hits(nbx*nby,0);
      for(auto& pt : data) {
	if(pt.x >= x_min && pt.y >= y_min
	   && pt.x < x_max && pt.y < y_max) {
	  unsigned int w = (unsigned int)((pt.x-x_min)*nbx/(x_max-x_min));
	  unsigned int h = (unsigned int)((pt.y-y_min)*nby/(y_max-y_min));
	  ++(hits[w+h*nbx]);
	}
      }

      for(auto hit : hits) 
	if(hit != 0)
	  os << ' ' << hit;
	else
	  os << ' ' << .01;
      os << std::endl;
    }

    virtual chart::Element* clone() const {
      Histo3d* res = new Histo3d(args,fill,
				 x_min,x_max,nbx,
				 y_min,y_max,nby);
      res->data = data;
      return res;
    }

    virtual void refill() {
      fill(data);
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_histo3d(os,suffix,args);
    }

    virtual void plot(std::ostream& os) {
      python::plot_histo3d(os,suffix,
			   x_min,x_max,nbx,
			   y_min,y_max,nby);
    }
      
  };

  template<typename FILL>
  Histo3d histo3d(const std::string& arglist, const FILL& f,
		  double xmin, double xmax, unsigned int nb_xbins,
		  double ymin, double ymax, unsigned int nb_ybins) {
    return Histo3d(arglist,f,
		   xmin,xmax,nb_xbins,
		   ymin,ymax,nb_ybins);
  }


  /////////////
  //         //
  // Patches //
  //         //
  /////////////

  
  class Patches : public chart::Data {
  public:

    std::vector<std::shared_ptr<Patch>> patches;
    std::function<void (std::vector<std::shared_ptr<Patch>>&)> fill;
      
    template<typename FILL>
    Patches(const FILL& f) : chart::Data(""), fill(f) {}
    virtual ~Patches() {}

    virtual void refill() {
      fill(patches);
    }

    virtual Element* clone() const {
      Patches* res = new Patches(fill);
      res->patches = patches;
      return res;
    }
      
    virtual void plot_getdata(std::ostream& os) {
      python::get_patches(os,suffix);
    }
      
    virtual void plot(std::ostream& os) {
      python::plot_patches(os,suffix);
    }

    virtual void _print_data(std::ostream& os) {
      os << "tmp = [";
      auto patch = patches.begin();
      if(patch != patches.end())
	(*(patch++))->toPython(os);
      while(patch != patches.end()) {
	os << ", ";
	(*(patch++))->toPython(os);
      }
      os << ']' << std::endl;
    }
  };

  template<typename FILL>
  Patches patches(const FILL& f) {
    return Patches(f);
  }



  /////////////
  //         //
  // Image   //
  //         //
  /////////////

  
  class Image : public chart::Data {
  public:

    // x and y are 1D 
    // z is a vectorized matrix which can contain Gray (depth=1) or RGB (depth=3) values
    std::vector<double> x, y, z;
    unsigned int width;
    unsigned int depth;
    std::function<void (std::vector<double>&, std::vector<double>&, std::vector<double>&, unsigned int&, unsigned int&)> fill;
      
    template<typename FILL>
    Image(const std::string& arglist, 
	  const FILL& f) : chart::Data(arglist), fill(f) {}
    virtual ~Image() {}

    virtual void refill() {
      fill(x, y, z, width, depth);
    }

    virtual Element* clone() const {
      Image* res = new Image(args, fill);
      res->x = x;
      res->y = y;
      res->z = z;
      res->width = width;
      res->depth = depth;
      return res;
    }
      
    virtual void plot_getdata(std::ostream& os) {
      python::get_image(os,suffix);
    }
      
    virtual void plot(std::ostream& os) {
      python::plot_image(os,suffix, args);
    }

    virtual void _print_data(std::ostream& os) {
      for(auto& xi : x) 
	os << ' ' << xi;
      os << std::endl;
      for(auto& yi : y) 
	os << ' ' << yi;
      os << std::endl;
      for(auto& zi : z) 
	os << ' ' << zi;
      os << std::endl;
      os << width << ' ' << depth << std::endl;
    }
  };

  template<typename FILL>
  Image image(const std::string& arglist, const FILL& f) {
    return Image(arglist, f);
  }


  //////////////
  //          //
  // Contours //
  //          //
  //////////////

  
  class Contours : public chart::Data {
  public:

    // x and y are 1D 
    // z is a vectorized matrix which can contain Gray (depth=1) or RGB (depth=3) values
    std::vector<double> z;
    double xmin; double xmax; unsigned int nb_x;
    double ymin; double ymax; unsigned int nb_y;
    double zmin; double zmax; unsigned int nb_z;
    unsigned int fontsize;
    
    std::function<void (std::vector<double>& z,
			double&, double&, unsigned int&,
			double&, double&, unsigned int&,
			double&, double&, unsigned int&)> fill;
      
    template<typename FILL>
    Contours(const std::string& arglist,
	     unsigned int fontsize,
	     const FILL& f) : chart::Data(arglist), fontsize(fontsize), fill(f) {}
    virtual ~Contours() {}

    virtual void refill() {
      fill(z,xmin,xmax,nb_x,ymin,ymax,nb_y,zmin,zmax,nb_z);
    }

    virtual Element* clone() const {
      Contours* res = new Contours(args, fontsize, fill);
      res->z        = z;
      res->xmin     = xmin;
      res->xmax     = xmax;
      res->nb_x     = nb_x;
      res->ymin     = ymin;
      res->ymax     = ymax;
      res->nb_y     = nb_y;
      res->zmin     = zmin;
      res->zmax     = zmax;
      res->nb_z     = nb_z;
      return res;
    }
      
    virtual void plot_getdata(std::ostream& os) {
      python::get_contours(os,suffix, args, fontsize);
    }
      
    virtual void plot(std::ostream& os) {
      python::plot_contours(os,suffix);
    }

    virtual void _print_data(std::ostream& os) {
      os << xmin << ' ' << xmax << ' ' << nb_x << std::endl
	 << ymin << ' ' << ymax << ' ' << nb_y << std::endl;
      for(auto v : ccmpl::range(zmin,zmax,nb_z)) os << ' ' << v;
      os << std::endl;
      for(auto zi : z) os << ' ' << zi;
      os << std::endl;
    }
  };

  template<typename FILL>
  Contours contours(const std::string& arglist, 
		    unsigned int fontsize, const FILL& f) {
    return Contours(arglist, fontsize, f);
  }

  /////////////
  //         //
  // Text    //
  //         //
  /////////////

  class Text : public chart::Data {
  public:
    Point pos;
    std::string text;
    std::function<void (Point&, std::string&)> fill;
      
    template<typename FILL>
    Text(const std::string& arglist,
	 const FILL& f) : chart::Data(arglist), pos(), text(""), fill(f) {}
    virtual ~Text() {}
      
    virtual chart::Element* clone() const {
      Text* res = new Text(args,fill);
      return res;
    }

    virtual void refill() {
      fill(pos, text);
    }
			    
    virtual void _print_data(std::ostream& os) {
      os << ' ' << pos.x << ' ' << pos.y << std::endl;
      os << ' ' << text << std::endl;
    }

    virtual void plot_getdata(std::ostream& os) {
      python::get_text(os,suffix);
    }

    virtual void plot(std::ostream& os) {
      python::plot_text(os,suffix,args);
    }
      
  };

  template<typename FILL>
  Text text(const std::string& arglist,const FILL& f) {
    return Text(arglist,f);
  }  




}
