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

    struct Limits2d {
      double xmin,xmax,ymin,ymax;
    };

    struct Limits3d {
      double xmin,xmax,ymin,ymax,zmin,zmax;
    };

    struct Tics {
      bool show_xtics;
      bool show_ytics;
      bool show_ztics;
      bool show_axis;
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

    struct Ratio {
      double num;
      double denom;

      Ratio() : num(0), denom(0) {}
      Ratio(double n, double d) : num(n), denom(d) {}
      Ratio(const Ratio& cp) : num(cp.num), denom(cp.denom) {}
      Ratio& operator=(const Ratio& cp) {
	if(&cp != this) {
	  num = cp.num;
	  denom = cp.denom;
	}
	return *this;
      }
      bool is_valid() {
	return num != 0 && denom != 0;
      }
    };
  }

  chart::Tics show_tics(bool x, bool y, bool z=false) {
    return {x,y,z,true};
  }

  chart::Tics hide_axis() {
    return {false, false, false, false};
  }

  chart::Ratio ratio(double num) {
    return {num, 1.0};
  }

  chart::Ratio ratio(double num, double denom) {
    return {num, denom};
  }

  chart::Legend legend() {
    return chart::Legend(true);
  }
  
  chart::Legend legend(const std::string& args) {
    return chart::Legend(args);
  }

  namespace chart {

    class Graph : public Elements {
    public:
      std::string title,xtitle,ytitle,ztitle;
      std::string aspect;
      Ratio ratio;
      double xmin,xmax;
      double ymin,ymax;
      double zmin,zmax;
      bool show_xtics;
      bool show_ytics;
      bool show_ztics;
      bool show_axis;
      bool autoscale_x;
      bool autoscale_y;
      bool use_x_offset;
      bool use_x_scientific;
      std::string grid_pos;
      bool is_3d;
      Legend legend;
      
      Graph(const std::string& pos) 
	: title(""), xtitle(""), ytitle(""), ztitle(""), aspect("auto"), ratio(),
	  xmin(0),xmax(1),
	  ymin(0),ymax(1),
	  zmin(0),zmax(1), 
	  show_xtics(true),
	  show_ytics(true),
	  show_ztics(true),
	  show_axis(true),
	  autoscale_x(false), autoscale_y(false),use_x_offset(true), use_x_scientific(false), grid_pos(pos),
	  is_3d(false),
	  legend() {}


      virtual Element* clone() const {
	Graph* res = new Graph(grid_pos);
	res->title            = title;
	res->xtitle           = xtitle;
	res->ytitle           = ytitle;
	res->ztitle           = ztitle;
	res->xmin             = xmin;
	res->xmax             = xmax;
	res->ymin             = ymin;
	res->ymax             = ymax;
	res->show_xtics       = show_xtics;
	res->show_ytics       = show_ytics;
	res->show_ztics       = show_ztics;
	res->show_axis        = show_axis;
	res->autoscale_x      = autoscale_x;
	res->autoscale_y      = autoscale_y;
	res->use_x_offset     = use_x_offset;
	res->use_x_scientific = use_x_scientific;
	res->is_3d            = is_3d;
	return res;
      }

      virtual void plot_getdata(std::ostream& os) {
	os << "\tax" << suffix << "_corners = None"         << std::endl;
	this->Elements::plot_getdata(os);
	if(autoscale_x && autoscale_y)
	  os << "\tax" << suffix << ".relim() "             << std::endl
	     << "\tif(ax" << suffix << "_corners != None):" << std::endl
	     << "\t\tax" << suffix << ".dataLim.update_from_data_xy(np.array(ax" << suffix << "_corners, float), ignore=False)" << std::endl
	     << "\tax" << suffix << ".autoscale_view(tight=True)" << std::endl;
	else if(autoscale_x) {
	  os << "\tax" << suffix << ".relim() "             << std::endl;
	  os << "\tax" << suffix << ".set_ylim(ymin" << suffix << ", ymax" << suffix << ')' << std::endl;
	  os << "\tax" << suffix << ".autoscale_view(tight=True, scalex=True, scaley=False)" << std::endl;
	}
	else if(autoscale_y) {
	  os << "\tax" << suffix << ".relim() "             << std::endl;
	  os << "\tax" << suffix << ".set_xlim(xmin" << suffix << ", xmax" << suffix << ')' << std::endl;
	  os << "\tax" << suffix << ".autoscale_view(tight=True, scalex=False, scaley=True)" << std::endl;
	}
      }

      virtual void plot(std::ostream& os) {

	// Let us process the aspect properly.
	if(ratio.is_valid()) {
	  double r = (xmax-xmin)/(ymax-ymin);
	  r *= ratio.denom/ratio.num;
	  aspect = std::to_string(r);
	}
	else
	  aspect = std::string("'") + aspect + std::string("'");
	
	
	python::open_graph(os,
			   suffix,
			   xmin,xmax,ymin,ymax,zmin,zmax,
			   title,xtitle,ytitle,ztitle,aspect,
			   show_xtics, show_ytics, show_ztics, show_axis,
			   autoscale_x, autoscale_y,
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
      
      void operator=(const Ratio& aspect_ratio) {
	ratio = aspect_ratio;
	aspect = "";
      }
      
      void operator=(const Legend& l) {
	legend = l;
      }
      
      void operator=(const char* aspect_label) {
	aspect = std::string(aspect_label);
	ratio = Ratio();
      }

      void operator=(const std::string& aspect_label) {
	aspect = aspect_label;
	ratio = Ratio();
      }
      
      void operator=(const std::initializer_list<double>& limits) {
	auto it = limits.begin();
	xmin = *(it++);
	xmax = *(it++);
	ymin = *(it++);
	ymax = *(it++);
	is_3d = it != limits.end();
	if(is_3d) {
	  zmin = *(it++);
	  zmax = *(it++);
	}
      }

      void operator=(const Limits2d& limits) {
	is_3d = false;
	xmin = limits.xmin;
	xmax = limits.xmax;
	ymin = limits.ymin;
	ymax = limits.ymax;
      }

      void operator=(const Limits3d& limits) {
	is_3d = true;
	xmin = limits.xmin;
	xmax = limits.xmax;
	ymin = limits.ymin;
	ymax = limits.ymax;
	zmin = limits.ymin;
	zmax = limits.ymax;
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
	
	std::cout << std::endl
		  << "File \"" << filename << "\" generated." << std::endl
		  << "You may need to set the execution flag for further use:" << std::endl
		  << "chmod +x " << filename << std::endl
		  << std::endl;
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
	
	std::cout << std::endl
		  << "File \"" << filename << "\" generated." << std::endl
		  << "You may need to set the execution flag for further use:" << std::endl
		  << "chmod +x " << filename << std::endl
		  << std::endl;
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
