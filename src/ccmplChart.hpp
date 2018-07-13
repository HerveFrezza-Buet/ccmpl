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

#include <vector>
#include <list>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <initializer_list>

#include <ccmplPython.hpp>

namespace ccmpl {
  namespace chart {
    class Element {
    public:
      
      std::string suffix;
      std::string args;
      unsigned int rank;
      
      Element(const std::string& arglist) : args(arglist) {}     

      virtual ~Element() {}
      virtual void setSuffix(const std::string& s,unsigned int r) {
	std::ostringstream ostr;
	rank = r;
	ostr << s << '_' << r;
	suffix = ostr.str();
      }

      virtual Element* clone() const {
	Element* res = new Element(args);
	res->suffix = suffix;
	res->rank   = rank;
	return res;
      }

      virtual void update_activity(std::string::const_iterator& it) {
      }
      
      virtual void print_data(std::ostream& os) {
      }
      
      virtual void plot_getdata(std::ostream& os) {
      }
      
      virtual void plot(std::ostream& os) {
      }
      
    };
    
    class Elements : public Element {
    public:
      std::vector<Element*> elements;
      
      Elements() : Element("") {}

      virtual ~Elements() {
	for(auto e : elements) delete e;
      }

      virtual void update_activity(std::string::const_iterator& it) {
	for(auto e : elements) e->update_activity(it);
      }
      
      void operator+=(const Element& e) {
	elements.push_back(e.clone());
      }

      void operator+=(Element* e) {
	elements.push_back(e);
      }

      virtual Element* clone() const {
	Elements* res = new Elements();
	for(auto e : elements) res->elements.push_back(e->clone());
	return res;
      }
      
      
      void setSuffix(const std::string& s) {
	unsigned int i=0;
	for(auto e_ptr : elements)
	  e_ptr->setSuffix(s,i++);
      }
      
      virtual void setSuffix(const std::string& s,unsigned int r) {
	this->Element::setSuffix(s,r);
	unsigned int i=0;
	for(auto e_ptr : elements)
	  e_ptr->setSuffix(this->suffix,i++);
      }
      
      virtual void print_data(std::ostream& os) {	
	for(auto e_ptr : elements) e_ptr->print_data(os);
      }
      
      virtual void plot_getdata(std::ostream& os) {
	for(auto e_ptr : elements) e_ptr->plot_getdata(os);
      }
      
      virtual void plot(std::ostream& os) {
	for(auto e_ptr : elements) e_ptr->plot(os);
      }
    };
    

    class Data : public Element {
    public:

      bool active;
      
      Data(const std::string& arglist) : Element(arglist), active(true) {}
      virtual ~Data() {}

      virtual void refill() = 0;

      virtual void update_activity(std::string::const_iterator& it) {
	active = (*(it++) == '#');
      }
      
      void print_data(std::ostream& os) {
	if(active) {
	  os << "data" << std::endl;
	  refill();
	  _print_data(os);
	}
	else
	  os << "nop" << std::endl;
      }

      virtual void _print_data(std::ostream& os) {
      }
    };

    struct Tics {
      bool show_xtics;
      bool show_ytics;
      bool show_ztics;
      bool show_axis;
    };

    struct NbTics {
      std::string axis;
      unsigned int nb;
    };

    struct Legend{
      bool show;
      std::string args;
      Legend() : show(false), args() {}
      Legend(bool show) : show(show), args() {}
      Legend(const std::string& args) : show(true), args(args) {}
      Legend(const Legend&) = default;
      Legend& operator=(const Legend&) = default;
    };

    
    struct GridInfo {
      std::string info;
      GridInfo(const std::string& info) : info(info) {}
    };
  }

  enum class limit : char {fit};
  struct Limit {
    bool autolim;
    double min,max;
    Limit() = delete;
    Limit(limit) : autolim(true), min(0), max(0) {}
    Limit(double min, double max) : autolim(false), min(min), max(max) {}
  };


  enum class aspect : char {fit, equal};
  struct Aspect {
    bool auto_aspect;
    double value;
    Aspect() = delete;
    Aspect(double value) : auto_aspect(false), value(value) {}
    Aspect(aspect a) : auto_aspect(false), value(1.0) {
      if(a == aspect::fit) {
	auto_aspect = true;
	value = 0;
      }
    }
  };

  enum class span : char {placeholder, limits};
  
  class view2d {
  private:
    bool auto_aspect = true;
    double aspect    = 0;
    bool auto_x      = false;
    double xmin      = 0;
    double xmax      = 0;
    bool auto_y      = false;
    double ymin      = 0;
    double ymax      = 0;
    std::string adj  = "datalim";

  public:
    view2d() = default;

    /**
     * @param xlim sets the x-axis limit. Provide something like {-1,2.5} for fixing the limits in the plot, or ccmpl::limit::fit for enabling matplotlib to adjuste the limit values to the data.
     * @param ylim same as xlim for y-axis.
     * @param a This is the aspect, i.e. the relative scale for x and y axis. If you provide 2, the zoom in y scale is twice the one in x-scale (a square will look like a vertical rectangle whose height is twice its width). You can provide ccmpl::aspect::equal instead of 1. You can provide ccmpl::aspect::fit for fitting the axis scales so that the limits are in the corner on the chart for both axes.
     * @param s This defines the area covered by the chart on the figure. If you provide ccmpl::span::placeholder, the whole placeholder allocated to the current subplot is spanned. If you provide ccmpl::span::limits, the chart is shrinked to the axis limts (its shape may not fit the one of the placeholder).
     */
    view2d(const Limit& xlim, const Limit& ylim, const Aspect& a, span s)
      : auto_aspect(a.auto_aspect),
	aspect(a.value),
	auto_x(xlim.autolim), xmin(xlim.min), xmax(xlim.max),
	auto_y(ylim.autolim), ymin(ylim.min), ymax(ylim.max),
	adj(s == span::placeholder ? "datalim" : "box") {}

    void python(std::ostream& os,
		const std::string& line_start) {
      if(auto_aspect)
	os << line_start << ".set_aspect('auto')" << std::endl;
      else
	os << line_start << ".set_aspect(" << aspect << ")" << std::endl;
      
      os << line_start << ".set_adjustable('" << adj << "')" << std::endl;

      if(!auto_x)
	os << line_start << ".set_xlim(" << xmin << ", " << xmax << ')' << std::endl;
	  
      if(!auto_y)
	os << line_start << ".set_ylim(" << ymin << ", " << ymax << ')' << std::endl;
    }
  };


  
  
  class view3d : private view2d {
  private:
    bool auto_z      = false;
    double zmin      = 0;
    double zmax      = 0;

  public:
    view3d() = default;

    /**
     * @param xlim sets the x-axis limit. Provide something like {-1,2.5} for fixing the limits in the plot, or ccmpl::limit::fit for enabling matplotlib to adjuste the limit values to the data.
     * @param ylim same as xlim for y-axis.
     * @param zlim same as xlim for z-axis.
     * @param a This is the aspect, i.e. the relative scale for x and y axis. If you provide 2, the zoom in y scale is twice the one in x-scale (a square will look like a vertical rectangle whose height is twice its width). You can provide ccmpl::aspect::equal instead of 1. You can provide ccmpl::aspect::fit for fitting the axis scales so that the limits are in the corner on the chart for both axes.
     * @param f This defines the area covered by the chart on the figure. If you provide ccmpl::span::placeholder, the whole placeholder allocated to the current subplot is spanned. If you provide ccmpl::span::limits, the chart is shrinked to the axis limts (its shape may not fit the one of the placeholder).
     */
    view3d(const Limit& xlim, const Limit& ylim, const Limit& zlim, const Aspect& a, span s)
      : view2d(xlim, ylim, a, s),
	auto_z(zlim.autolim), zmin(zlim.min), zmax(zlim.max) {}

    void python(std::ostream& os,
		const std::string& line_start) {
      this->view2d::python(os, line_start);
	  
      if(!auto_z)
	os << line_start << ".set_zlim(" << zmin << ", " << zmax << ')' << std::endl;
    }
  };


  inline chart::NbTics nb_xtics(unsigned int nb) {
    return {std::string("x"), nb};
  }
  
  inline chart::NbTics nb_ytics(unsigned int nb) {
    return {std::string("y"), nb};
  }
  
  inline chart::NbTics nb_ztics(unsigned int nb) {
    return {std::string("z"), nb};
  }

  inline chart::Tics show_tics(bool x, bool y, bool z=false) {
    return {x,y,z,true};
  }

  inline chart::Tics hide_axis() {
    return {false, false, false, false};
  }

  inline chart::Legend legend() {
    return chart::Legend(true);
  }
  
  inline chart::Legend legend(const std::string& args) {
    return chart::Legend(args);
  }

  inline chart::GridInfo grid(const std::string& info) {
    return chart::GridInfo(info);
  }

  namespace chart {

    class Graph : public Elements {
    public:
      std::string title,xtitle,ytitle,ztitle;
      view2d v2d;
      view3d v3d;
      bool show_xtics;
      bool show_ytics;
      bool show_ztics;
      std::string xticks_position, yticks_position;
      bool show_axis;
      bool use_x_offset;
      bool use_x_scientific;
      std::string grid_pos;
      bool is_3d;
      Legend legend;
      std::list<chart::NbTics> nb_tics;
      std::string grid_info;
      
      Graph(const std::string& pos) 
	: title(""), xtitle(""), ytitle(""), ztitle(""),
	  v2d(),
	  v3d(),
	  show_xtics(true),
	  show_ytics(true),
	  show_ztics(true),
	  xticks_position("default"),
	  yticks_position("default"),
	  show_axis(true),
	  use_x_offset(true), use_x_scientific(false), grid_pos(pos),
	  is_3d(false),
	  legend(),
	  nb_tics(),
	  grid_info() {}


      virtual Element* clone() const {
	Graph* res = new Graph(grid_pos);
	res->title            = title;
	res->xtitle           = xtitle;
	res->ytitle           = ytitle;
	res->ztitle           = ztitle;
	res->v2d              = v2d;
	res->v3d              = v3d;
	res->show_xtics       = show_xtics;
	res->show_ytics       = show_ytics;
	res->show_ztics       = show_ztics;
	res->xticks_position  = xticks_position;
	res->yticks_position  = yticks_position;
	res->show_axis        = show_axis;
	res->use_x_offset     = use_x_offset;
	res->use_x_scientific = use_x_scientific;
	res->is_3d            = is_3d;
	res->legend           = legend;
	res-> nb_tics         = nb_tics;
	res->grid_info        = grid_info;
	return res;
      }

      virtual void plot_getdata(std::ostream& os) {
	// os << "\tax" << suffix << "_corners = None"         << std::endl;
	// this->Elements::plot_getdata(os);
	// if(autoscale_x && autoscale_y)
	//   os << "\tax" << suffix << ".relim() "             << std::endl
	//      << "\tif(ax" << suffix << "_corners != None):" << std::endl
	//      << "\t\tax" << suffix << ".dataLim.update_from_data_xy(np.array(ax" << suffix << "_corners, float), ignore=False)" << std::endl
	//      << "\tax" << suffix << ".autoscale_view(tight=True)" << std::endl;
	// else if(autoscale_x) {
	//   os << "\tax" << suffix << ".relim() "             << std::endl;
	//   os << "\tax" << suffix << ".set_ylim(ymin" << suffix << ", ymax" << suffix << ')' << std::endl;
	//   os << "\tax" << suffix << ".autoscale_view(tight=True, scalex=True, scaley=False)" << std::endl;
	// }
	// else if(autoscale_y) {
	//   os << "\tax" << suffix << ".relim() "             << std::endl;
	//   os << "\tax" << suffix << ".set_xlim(xmin" << suffix << ", xmax" << suffix << ')' << std::endl;
	//   os << "\tax" << suffix << ".autoscale_view(tight=True, scalex=False, scaley=True)" << std::endl;
	// }

	// if(grid_info != "")
	//   os << "\tax" << suffix << ".grid(" << grid_info << ')' << std::endl;

	this->Elements::plot_getdata(os);
	os << "\tax" << suffix << ".relim()" << std::endl;
	os << "\tax" << suffix << ".autoscale_view()" << std::endl;

      }

      virtual void plot(std::ostream& os) {
	python::open_graph(os,
			   suffix,
			   [this](std::ostream& os, const std::string& prefix){this->v2d.python(os, prefix);},
			   [this](std::ostream& os, const std::string& prefix){this->v3d.python(os, prefix);},
			   title,xtitle,ytitle,ztitle,
			   show_xtics, show_ytics, show_ztics,
			   xticks_position, yticks_position,
			   grid_info,
			   show_axis,
			   use_x_offset, use_x_scientific, grid_pos, is_3d);
	this->Elements::plot(os);
	python::close_graph(os, suffix, legend.show, legend.args);
      }

      void operator=(bool show_values_on_axis) {
	show_xtics = show_values_on_axis;
	show_ytics = show_values_on_axis;
	show_ztics = show_values_on_axis;
      }

      void operator=(const Tics& tics) {
	show_xtics = tics.show_xtics;
	show_ytics = tics.show_ytics;
	show_ztics = tics.show_ztics;
	show_axis  = tics.show_axis;
      }
      
      
      void operator=(const Legend& l) {
	legend = l;
      }
      
      void operator=(const GridInfo& gi) {
	grid_info = gi.info;
      }
      
      void operator=(const NbTics& nbt) {
	nb_tics.push_back(nbt);
      }

      void operator=(const view2d& view_2d) {
	is_3d = false;
	v2d   = view_2d;
      }

      void operator=(const view3d& view3d) {
	is_3d = true;
	v3d = view3d;
      }
    };
    
    class Layout : public Elements {
    private:
      unsigned int width,height;
      double xsize, ysize;
      ccmpl::RGB facecolor;
      std::vector<Graph*> graphs;
      std::vector<Graph*>::iterator current;
      std::string pdf_name, png_name;
      int png_dpi;
      std::list<double> wratios, hratios;

      
    public:
      
      Layout(const Layout& other)
	: width(other.width), height(other.height),
	  xsize(other.xsize), ysize(other.ysize),
	  facecolor(other.facecolor),
	  graphs(), current(),
	  pdf_name(other.pdf_name), 
	  png_name(other.png_name),
	  png_dpi(other.png_dpi),
	  wratios(other.wratios), hratios(other.hratios) {
	auto out = std::back_inserter(graphs);
	for(auto g_ptr: graphs)
	  *(out++) = reinterpret_cast<Graph*>(g_ptr->clone());
	current = graphs.begin();
      }
      
      Layout& operator=(const Layout&) = delete;


      /**
       * layout can contain '.' (no graph), '#' (some graph here), '>' (colspan = 2), 'V' (linespan = 2), 'X' (2x2 span).
       *
       */

      Layout(double sx, double sy,const std::initializer_list<const char*>& placeholders, ccmpl::RGB fc=ccmpl::RGB(.75, .75, .75))
	: xsize(sx), ysize(sy), facecolor(fc) {
	
	height = placeholders.size();
	unsigned int lineid = 0;
	for(auto& l : placeholders) {
	  unsigned int columnid = 0;
	  std::string line(l);
	  width = line.size();
	  for(auto c : line) {
	    std::ostringstream grid_pos;
	    Graph* g;
	    switch(c) {
	    case '#':
	      grid_pos << lineid << ',' << columnid;
	      g = new Graph(grid_pos.str());
	      *this += g;
	      graphs.push_back(g);
	      break;
	    case '>':
	      grid_pos << lineid << ',' << columnid << ':' << columnid+2;
	      g = new Graph(grid_pos.str());
	      *this += g;
	      graphs.push_back(g);
	      break;
	    case 'V':
	      grid_pos << lineid << ':' << lineid+2 << ',' << columnid;
	      g = new Graph(grid_pos.str());
	      *this += g;
	      graphs.push_back(g);
	      break;
	    case 'X':
	      grid_pos << lineid << ':' << lineid+2 << ',' << columnid << ':' << columnid+2;
	      g = new Graph(grid_pos.str());
	      *this += g;
	      graphs.push_back(g);
	      break;
	    default:
	      *this += new Element(""); // empty slot
	    }
	    ++columnid;
	  }
	  ++lineid;
	}
	current = graphs.begin();
      }

      virtual ~Layout() {
      }

      virtual Element* clone() const {
	std::cerr << "Layout cannot be cloned" << std::endl;
	return (Element*)0;
      }

      virtual void print_data(std::ostream& os) {
	os << "cont" << std::endl
	   << pdf_name << ',' << png_name << ',' << png_dpi << ',' << std::endl;
	this->Elements::print_data(os);
	os << std::flush;
      }

      void operator++(int unused) {
	++current;
      }

      Graph& operator()() {
	return *(*current);
      }

      Layout& operator()(const std::string& s,
			 const std::string& pdf,
			 const std::string& png) {
	auto it = s.begin();
	update_activity(it);
	pdf_name = pdf;
	png_name = png;
	png_dpi = 0;
	return *this;
      }

      Layout& operator()(const std::string& s,
			 const std::string& pdf,
			 const std::pair<std::string, int>& png_data) {
	auto it = s.begin();
	update_activity(it);
	pdf_name = pdf;
	png_name = png_data.first;
	png_dpi = png_data.second;
	return *this;
      }


      void set_ratios(const std::initializer_list<double>& width_ratios, 
		      const std::initializer_list<double>& height_ratios) {
	wratios.clear();
	std::copy(width_ratios.begin(), width_ratios.end(), std::back_inserter(wratios));
	hratios.clear();
	std::copy(height_ratios.begin(), height_ratios.end(), std::back_inserter(hratios));
      }

      void make_python(std::string filename, bool use_gui) {

	setSuffix("");

	std::ofstream file;
	file.exceptions(std::ios::failbit | std::ios::badbit);
	file.open(filename.c_str());

	python::header(file,use_gui);
	python::open_plot(file, use_gui);
	python::create_figure(file,
			      width,height,
			      xsize,ysize,
			      wratios, hratios,
			      facecolor);
	this->plot(file);
	python::close_plot(file);

	python::start_read(file);
	plot_getdata(file);
	python::end_read(file,use_gui,false);

	file.close();
	
      }

      void make_movie_python(std::string filename, bool use_gui, std::string writer_name,
			   std::string movie_title, std::string movie_artist, std::string movie_comment,
			   unsigned int movie_fps, std::string movie_filename, unsigned int movie_dpi) {

	setSuffix("");

	std::ofstream file;
	file.exceptions(std::ios::failbit | std::ios::badbit);
	file.open(filename.c_str());

	python::header(file,use_gui);
	python::create_figure(file,
			      width,height,
			      xsize,ysize,
			      wratios, hratios,
			      facecolor);
	this->plot(file);

	python::open_movie(file, writer_name, movie_title, movie_artist, movie_comment, movie_fps, movie_filename, movie_dpi);
	python::start_read(file);
	plot_getdata(file);
	python::end_read(file,use_gui, true);
	python::close_movie(file);

	file.close();
      }
    };

    inline std::ostream& operator<<(std::ostream& os, Layout& l) {
      l.print_data(os);
      return os;
    }
  }
    
  // To keep the default facecolor in the Layout class, we define two functions for the creation of Layout
  inline chart::Layout layout(double xsize, double ysize,
			      const std::initializer_list<const char*>& placeholders) {
    return chart::Layout(xsize, ysize, placeholders);
  }
  inline chart::Layout layout(double xsize, double ysize,
			      const std::initializer_list<const char*>& placeholders, 
			      ccmpl::RGB facecolor) {
    return chart::Layout(xsize, ysize, placeholders, facecolor);
  }

  inline std::ostream& stop(std::ostream& os) {
    os << "end" << std::endl;
    return os;
  }
}
