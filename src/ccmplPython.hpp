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

#include <string>
#include <iostream>
#include <list>
#include <stdexcept>
#include <ccmplTypes.hpp>
#include <ccmplUtility.hpp>

namespace ccmpl {
  namespace python {
    
    std::string add_args(const std::string& args) {
      if(args != "")
	return std::string(", ")+args;
      else
	return args;
    }

    std::string parent_suffix(const std::string& suffix) {
      unsigned int j=0;
      unsigned int i=suffix.size();
      for(auto c : suffix) {
	if(c == '_') 
	  i = j;
	++j;
      }
	
      return std::string(suffix,0,i);
    }
      
    inline void header(std::ostream& os, bool gui) {
      os << "#!/usr/bin/env python3" << std::endl
	 << "# -*- coding: utf-8 -*-" << std::endl
	 << std::endl
	 << "import numpy as np" << std::endl
	 << "import matplotlib as mpl" << std::endl;
      if(!gui)
	os << "mpl.use('Agg') # No GUI..." << std::endl;
      os << "import matplotlib.pyplot as plt" << std::endl
	 << "import matplotlib.patches as patches" << std::endl
	 << "from mpl_toolkits.mplot3d import Axes3D" << std::endl
	 << "from matplotlib.image import NonUniformImage" << std::endl
	 << "from matplotlib import cm" << std::endl
	 << "import matplotlib.animation as manimation" << std::endl
	 << "import sys" << std::endl
	 << std::endl
	 << "pipe = sys.stdin" << std::endl
	 << "if len(sys.argv) == 2:" << std::endl
	 << "\tpipename = sys.argv[1]" << std::endl
	 << "\ttry:" << std::endl
	 << "\t\tpipe = open(pipename, 'r')" << std::endl
	 << "\texcept:" << std::endl
	 << "\t\tprint('cannot open pipe {}. Aborting.'.format(pipename))" << std::endl
	 << "\t\tsys.exit(1)" << std::endl
	 << std::endl;
    }
      
    inline void plot_list(const std::list<double>& list,
			  std::ostream& os) {
      os << "[";
      for(auto it = list.begin(); it != list.end(); ++it) 
	if(it != list.begin())
	  os << ", " << *it;
	else
	  os << *it;
      os << "]";
    }

    inline void open_plot(std::ostream& os, bool gui) {
      if(!gui)
	os << "#";
      os << "plt.ion()" << std::endl;
      os << "plt.rc('text', usetex=True)" << std::endl;
      os << "plt.rc('font', family='serif')" << std::endl;
    }

    inline void close_plot(std::ostream& os) {
    }

    inline void open_movie(std::ostream& os, const std::string& writer_name,
			   const std::string& movie_title, const std::string& movie_artist, const std::string& movie_comment,
			   unsigned int movie_fps, const std::string& movie_filename, unsigned int movie_dpi) {
      os << "movieWriter = manimation.writers['" << writer_name << "']" << std::endl
	 << "metadata = dict(title='" << movie_title << "', artist='" << movie_artist << "', comment='" << movie_comment << "')" << std::endl
	 << "writer = movieWriter(fps=" << movie_fps << ", metadata=metadata)" << std::endl
	 << "writer.setup(fig, \"" << movie_filename << "\", " << movie_dpi << ")" << std::endl;
    }

    inline void close_movie(std::ostream& os) {
      os << "writer.finish()" << std::endl;
    }


    inline void create_figure(std::ostream& os,
			      int w, int h, 
			      double width, double height,
			      const std::list<double>& width_ratios,
			      const std::list<double>& height_ratios,
			      ccmpl::RGB& facecolor) {
      os << "fig = plt.figure(figsize=(" << width << ',' << height << "), "
	 <<                  "facecolor = (" << facecolor.r << "," << facecolor.g << "," << facecolor.b << ")"
	 <<                  ")" << std::endl
	 << "gs  = mpl.gridspec.GridSpec(" << h << ',' << w;

      if(width_ratios.size() != 0) {
	os << ", width_ratios = ";
	plot_list(width_ratios, os);
      }

      if(height_ratios.size() != 0) {
	os << ", height_ratios = ";
	plot_list(height_ratios, os);
      }

      os << ")" << std::endl;
      os << std::endl;
    }
      

      
    inline void start_read(std::ostream& os) {
      os << "cont = pipe.readline().split()[0]=='cont'" << std::endl
	 << "while cont:" << std::endl
	 << "\tfiles    = pipe.readline().split(',')" << std::endl
	 << "\tpdf_name = files[0]" << std::endl
	 << "\tpng_name = files[1]" << std::endl
	 << "\tpng_dpi = int(files[2])" << std::endl;
    }
      
    inline void end_read(std::ostream& os, bool gui, bool movie) {
      os << "\tif pdf_name != '' : " << std::endl
	 << "\t\tplt.savefig(pdf_name, bbox_inches='tight')" << std::endl
	 << "#\t\tprint('file \"%s\" generated'%pdf_name)" << std::endl
	 << "\tif png_name != '' : " << std::endl
	 << "\t\tif png_dpi != 0:" << std::endl
	 << "\t\t\tplt.savefig(png_name, bbox_inches='tight', dpi=png_dpi)" << std::endl
	 << "\t\telse:" << std::endl
	 << "\t\t\tplt.savefig(png_name, bbox_inches='tight')" << std::endl
	 << "#\t\tprint('file \"%s\" generated'%png_name)" << std::endl;
      if(!gui) os << '#';
      os << "\tfig.canvas.flush_events()" << std::endl
	 << "\tfig.canvas.draw()" << std::endl;
      if(movie)
	os << "\twriter.grab_frame()" << std::endl;
      os << "\tcont = pipe.readline().split()[0]=='cont'" << std::endl;
    }
      
    inline void start_data(std::ostream& os) {
      os << "\tdata_line = pipe.readline().split()" << std::endl
	 << "\tif data_line[0] == 'data':" << std::endl;
    }
      
    inline void end_data(std::ostream& os) {
    }

    template<typename PRINT_AXIS2D, typename PRINT_AXIS3D>
    inline void open_graph(std::ostream& os,
			   const std::string& suffix,
			   const PRINT_AXIS2D& p2d,
			   const PRINT_AXIS3D& p3d,
			   const std::string& title,
			   const std::string& xtitle,
			   const std::string& ytitle,
			   const std::string& ztitle,
			   bool show_xtics,
			   bool show_ytics,
			   bool show_ztics,
			   const std::string& xticks_position,
			   const std::string& yticks_position,
			   const std::string& grid_info,
			   bool show_axis,
			   bool use_x_offset,
			   bool use_x_scientific,
			   const std::string& grid_pos,
			   bool is_3d) {
	
      os << "ax = plt.subplot(gs["<< grid_pos << "]";
      if(is_3d)
	os << ", projection='3d'";
      os << ")" << std::endl
	 << "plt.sca(ax)" << std::endl;

      if(is_3d)
	p3d(os, "ax");
      else
	p2d(os, "ax");
      
      if(grid_info != "")
	os << "ax.grid(" << grid_info << ')' << std::endl;
	
      os << "ax.get_xaxis().get_major_formatter().set_useOffset(" << (use_x_offset ? std::string("True") : std::string("False")) << ")" << std::endl;
      if(use_x_scientific) {
	os << "ax.get_xaxis().get_major_formatter().set_scientific(True)"<< std::endl;
	os << "ax.get_xaxis().get_major_formatter().set_powerlimits((0,1))" << std::endl;
      }
      else
	os << "ax.get_xaxis().get_major_formatter().set_scientific(False)" << std::endl;
      
      if(title != "")
	os << "ax.set_title(r'" << title << "')" << std::endl;
      if(xtitle != "")
	os << "ax.set_xlabel(r'" << xtitle << "')" << std::endl;
      if(ytitle != "")
	os << "ax.set_ylabel(r'" << ytitle << "')" << std::endl;
      if(is_3d && ztitle != "")
	os << "ax.set_zlabel(r'" << ztitle << "')" << std::endl;
      if(show_axis) {
	if(show_xtics)
	  os << "ax.get_xaxis().set_ticks_position('" << xticks_position << "')" << std::endl;
	else
	  os << "ax.get_xaxis().set_ticks([])" << std::endl;
	if(show_ytics) 
	  os << "ax.get_yaxis().set_ticks_position('" << yticks_position << "')" << std::endl;
	else
	  os << "ax.get_yaxis().set_ticks([])" << std::endl;
	if(is_3d && !show_ztics) os << "ax.zaxis.set_major_formatter(plt.NullFormatter())" << std::endl;
      }
      else
	os << "ax.axis('off')" << std::endl;
    }
      
    inline void close_graph(std::ostream& os, const std::string& suffix, bool show_legend, const std::string& legend_args) {
      if(show_legend)
	os << "ax.legend(" << legend_args << ")" << std::endl;
      os << "ax" << suffix << " = ax " << std::endl;
      os << "ax" << suffix << "_corners = None" << std::endl;
      os << std::endl;
    }

    inline void plot_between(std::ostream& os,
			     const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "between" << suffix << " = None" << std::endl;
    }
      
    inline void get_between(std::ostream& os, 
			    const std::string& suffix,
			    const std::string& args) {
      start_data(os);
      os << "\t\tx  = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\ty1 = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\ty2 = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\tif between" << suffix << " != None : between" << suffix << ".remove()" << std::endl
	 << "\t\tbetween" << suffix << " = ax" << suffix << ".fill_between(x,y1,y2" << add_args(args) << ")" << std::endl;
      end_data(os);
    }
      
    inline void plot_pie(std::ostream& os,
			 const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "pie" << suffix << " = None" << std::endl;
    }
      
    inline void get_pie(std::ostream& os, const std::string& suffix, const std::string& args) {
      start_data(os);
      os << "\t\tsizes = [float(v) for v in pipe.readline().split()]" << std::endl;

      os << "\t\tcolors = []" << std::endl
	 << "\t\tfor i in range(len(sizes)):" << std::endl
	 << "\t\t\tcolors.append([float(v) for v in pipe.readline().split()])" << std::endl;

      os << "\t\tlabels = []" << std::endl
	 << "\t\tfor i in range(len(sizes)):" << std::endl
	 << "\t\t\tlabels.append(pipe.readline()[:-1])" << std::endl;
      
      os << "\t\texplodes = [float(v) for v in pipe.readline().split()]" << std::endl;
      
      os << "\t\tif pie" << suffix << " != None : pie" << suffix << ".remove()" << std::endl
	 << "\t\tpie" << suffix << " = ax" << suffix << ".pie(sizes, explode=explodes, labels=labels, colors=colors" << add_args(args) << ")" << std::endl;
      
      end_data(os);
    }
    
    inline void plot_line(std::ostream& os,
			  const std::string& suffix,
			  const std::string& args) {
      os << "line" << suffix << ", = plt.plot([], []" << add_args(args) << ")" << std::endl;
    }
      
    inline void get_line(std::ostream& os, const std::string& suffix) {
      start_data(os);
      os << "\t\tx = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\ty = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\tline" << suffix << ".set_data(x,y)" << std::endl;
      end_data(os);
    }
      
    inline void plot_lines(std::ostream& os,
			   const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "lines" << suffix << " = None" << std::endl;
    }
      
    inline void get_lines(std::ostream& os,
			  const std::string& suffix,
			  const std::string& args) {
      start_data(os);
      os << "\t\tnb_lines = [int(v) for v in pipe.readline().split()][0]" << std::endl
	 << "\t\tif lines" << suffix << " != None :" << std::endl
	 << "\t\t\tfor line in lines" << suffix << " : line[0].remove()" << std::endl
	 << "\t\tlines" << suffix << " = []" << std::endl
	 << "\t\tfor l in range(nb_lines) :" << std::endl
	 << "\t\t\tx = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\t\ty = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\t\tlines" << suffix << ".append(ax" << suffix << ".plot(x, y" << add_args(args) << "))" << std::endl;
      end_data(os);
    }
      
    inline void plot_dot(std::ostream& os,
			 const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "dot" << suffix << " = None" << std::endl;
    }
      
    inline void get_dot(std::ostream& os, 
			const std::string& suffix,
			const std::string& args) {
      start_data(os);
      os << "\t\tpt = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\tif dot" << suffix << " != None : dot" << suffix << ".remove()" << std::endl
	 << "\t\tdot" << suffix << " = ax" << suffix << ".scatter([pt[0]],[pt[1]]" << add_args(args) << ")" << std::endl;
      end_data(os);
    }

    inline void plot_dots(std::ostream& os,
			  const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "dots" << suffix << " = None" << std::endl;
    }
      
    inline void get_dots(std::ostream& os, 
			 const std::string& suffix,
			 const std::string& args) {
      start_data(os);
      os << "\t\tx = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\ty = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\tif dots" << suffix << " != None : dots" << suffix << ".remove()" << std::endl
	 << "\t\tdots" << suffix << " = ax" << suffix << ".scatter(x,y" << add_args(args) << ")" << std::endl;
      end_data(os);
    }
      
    inline void plot_surface(std::ostream& os,
			     const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "surface" << suffix << " = None" << std::endl;
    }
      
    inline void get_surface(std::ostream& os, const std::string& suffix,
			    const std::string& args,
			    double vmin, double vmax) {
      std::string parent_suffix = suffix.substr(0, suffix.find_last_of('_'));
      start_data(os);
      os << "\t\tx = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\ty = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\tv = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\ttry:" << std::endl
	 << "\t\t\tif surface" << suffix << " != None : surface" << suffix << ".remove()" << std::endl
	 << "\t\t\tif ax" << parent_suffix << "_corners == None:" << std::endl
	 << "\t\t\t\tax" << parent_suffix << "_corners = [(x.min(), y.min()), (x.max(), y.max())]" << std::endl
	 << "\t\t\telse:" << std::endl
	 << "\t\t\t\tax" << parent_suffix << "_corners = [(min(x.min(), " << "ax" << parent_suffix << "_corners[0][0]" << "),"
	 <<                                               "min(y.min(), " << "ax" << parent_suffix << "_corners[0][1]" << ")),"
	 <<                                              "(max(x.max(), " << "ax" << parent_suffix << "_corners[1][0]" << "),"
	 <<                                               "max(y.max(), " << "ax" << parent_suffix << "_corners[1][1]" << "))]" << std::endl
	 << "\t\t\ttriangles = mpl.tri.Triangulation(x,y)" << std::endl
	 << "\t\t\tsurface" << suffix << " = ax" << suffix << ".tripcolor(triangles,v,shading='gouraud',vmin=" << vmin << ",vmax=" << vmax 
	 << add_args(args) << ')' << std::endl
	 << "\t\texcept (KeyError, ValueError, IndexError):" << std::endl
	 << "\t\t\tpass" << std::endl;

      end_data(os);
    }
      
    inline void plot_palette(std::ostream& os,
			     const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "palette" << suffix << " = None" << std::endl;
    }
      
    inline void get_palette(std::ostream& os, const std::string& suffix,
			    const std::string& args) {
      std::string parent_suffix = suffix.substr(0, suffix.find_last_of('_'));
      start_data(os);
      os << "\t\tx = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\ty = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\tcols = [[float(v) for v in l.split()] for l in pipe.readline().split(',')[:-1]]" << std::endl
	 << "\t\ttry:" << std::endl
	 << "\t\t\tif palette" << suffix << " != None : palette" << suffix << ".remove()" << std::endl
	 << "\t\t\tif ax" << parent_suffix << "_corners == None:" << std::endl
	 << "\t\t\t\tax" << parent_suffix << "_corners = [(x.min(), y.min()), (x.max(), y.max())]" << std::endl
	 << "\t\t\telse:" << std::endl
	 << "\t\t\t\tax" << parent_suffix << "_corners = [(min(x.min(), " << "ax" << parent_suffix << "_corners[0][0]" << "),"
	 <<                                               "min(y.min(), " << "ax" << parent_suffix << "_corners[0][1]" << ")),"
	 <<                                              "(max(x.max(), " << "ax" << parent_suffix << "_corners[1][0]" << "),"
	 <<                                               "max(y.max(), " << "ax" << parent_suffix << "_corners[1][1]" << "))]" << std::endl
	 << "\t\t\ttriangles = mpl.tri.Triangulation(x,y)" << std::endl
	 << "\t\t\tpalette" << suffix << " = ax" << suffix << ".tripcolor(triangles,range(len(x)),shading='gouraud',cmap=mpl.colors.ListedColormap(cols)"
	 << add_args(args) << ')'  << std::endl
	 << "\t\texcept (KeyError, ValueError, IndexError):" << std::endl
	 << "\t\t\tpass" << std::endl;
      end_data(os);
    }
      
    inline void plot_confetti(std::ostream& os,
			      const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "confetti" << suffix << " = None" << std::endl;
    }
      
    inline void get_confetti(std::ostream& os, const std::string& suffix,
			     const std::string& args) {
      start_data(os);
      os << "\t\tx = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\ty = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\tcols = [[float(v) for v in l.split()] for l in pipe.readline().split(',')[:-1]]" << std::endl
	 << "\t\tif confetti" << suffix << " != None : confetti" << suffix << ".remove()" << std::endl
	 << "\t\tconfetti" << suffix << " = ax" << suffix << ".scatter(x,y,c=cols" << add_args(args) << ')' << std::endl;
      end_data(os);
    }
      
    inline void plot_histo1d(std::ostream& os,
			     const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "histo1d" << suffix << " = None" << std::endl;
    }
    
    inline void get_histo1d(std::ostream& os,
			    const std::string& suffix,
			    const std::string& args) {
      start_data(os);
      os << "\t\tbar_width   = float(pipe.readline())" << std::endl
	 << "\t\tbar_centers = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\tbar_heights = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\tif histo1d" << suffix << " != None : histo1d" << suffix << ".remove()" << std::endl
	 << "\t\thisto1d" << suffix << " = ax" << suffix << ".bar(bar_centers, bar_heights, bar_width, align='center'" << add_args(args) << ")" << std::endl;
      end_data(os);
      
    }

    
    inline void plot_histo3d(std::ostream& os,
			     const std::string& suffix,
			     double xmin, double xmax, unsigned int nbx,
			     double ymin, double ymax, unsigned int nby) {
      bool first;
      double xstep,ystep;
      os << "ax" << suffix << " = ax" << std::endl;

      os << "x"  << suffix << " = np.array([";
      first = true;
      xstep = (xmax-xmin)/nbx;
      for(unsigned int y = 0; y < nby; ++y) 
	for(unsigned int x = 0; x< nbx; ++x,first=false) {
	  if(!first) os << ", ";
	  os << xmin + (x+.5)*xstep;
	}
      os << "])" << std::endl;

      os << "y"  << suffix << " = np.array([";
      first = true;
      ystep = (ymax-ymin)/nby;
      for(unsigned int y = 0; y < nby; ++y) { 
	double val = ymin + (y+.5)*ystep;
	for(unsigned int x = 0; x< nbx; ++x,first=false) {
	  if(!first) os << ", ";
	  os << val;
	}
      }
      os << "])" << std::endl;
	
      os << "z"  << suffix << " = np.zeros(" << nbx*nby << ")" << std::endl;
      os << "dx" << suffix << " = " << xstep << "*np.ones(" << nbx*nby << ")" << std::endl;
      os << "dy" << suffix << " = " << ystep << "*np.ones(" << nbx*nby << ")" << std::endl;

      os << "histo3d" << suffix << " = None" << std::endl;
    }
      
    inline void get_histo3d(std::ostream& os, const std::string& suffix,
			    const std::string& args) {
      start_data(os);
      os <<"\t\tif(histo3d" << suffix << " != None):" << std::endl;
      os <<"\t\t\thisto3d" << suffix <<".remove()" << std::endl;
      os << "\t\tdz = np.array([float(v) for v in pipe.readline().split()])" << std::endl;
	
      os << "\t\tax" << suffix 
	 << ".bar3d"
	 << "(x" << suffix 
	 << ", y" << suffix 
	 << ", z" << suffix
	 << ", dx" << suffix 
	 << ", dy" << suffix 
	 << ", dz"
	 << add_args(args) << ')' << std::endl;
      os << "\t\thisto3d" << suffix << " = ax" << suffix << ".collections[-1]" << std::endl;
      end_data(os);
    }
      
      
    inline void plot_histo2d(std::ostream& os,
			     const std::string& suffix,
			     double xmin, double xmax, unsigned int nbx,
			     double ymin, double ymax, unsigned int nby) {
      bool first;
      double xstep,ystep;
      os << "ax" << suffix << " = ax" << std::endl;

      os << "x"  << suffix << " = np.array([";
      first = true;
      xstep = (xmax-xmin)/nbx;
      for(unsigned int y = 0; y <= nby; ++y) 
	for(unsigned int x = 0; x <= nbx; ++x,first=false) {
	  if(!first) os << ", ";
	  os << xmin + x*xstep;
	}
      os << "]).reshape((" << nby+1 << ',' << nbx+1 <<"))" << std::endl;

      os << "y"  << suffix << " = np.array([";
      first = true;
      ystep = (ymax-ymin)/nby;
      for(unsigned int y = 0; y <= nby; ++y) { 
	double val = ymin + y*ystep;
	for(unsigned int x = 0; x<= nbx; ++x,first=false) {
	  if(!first) os << ", ";
	  os << val;
	}
      }
      os << "]).reshape((" << nby+1 << ',' << nbx+1 <<"))" << std::endl;
	

      os << "histo2d" << suffix << " = None" << std::endl;
    }
      
    inline void get_histo2d(std::ostream& os, const std::string& suffix,
			    const std::string& args,
			    unsigned int nbx, unsigned int nby) {
      start_data(os);
      os <<"\t\tif(histo2d" << suffix << " != None):" << std::endl;
      os <<"\t\t\thisto2d" << suffix <<".remove()" << std::endl;
      os << "\t\tz = np.array([float(v) for v in pipe.readline().split()])" << std::endl;
      // os << "\t\tzsum = z.sum()" << std::endl;
      // os << "\t\tif(zsum != 0):" << std::endl;
      // os << "\t\t\tz /= zsum" << std::endl;
	
      os << "\t\thisto2d" << suffix <<" = ax" << suffix 
	 << ".pcolormesh"
	 << "(x" << suffix 
	 << ", y" << suffix 
	 << ", z.reshape((" << nby+1 << ',' << nbx+1 <<"))"
	 << add_args(args) << ')' << std::endl;
      end_data(os);
    }
      
    inline void plot_vectors(std::ostream& os,
			     const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "vectors" << suffix << " = None" << std::endl;
    }
      
    inline void get_vectors(std::ostream& os, const std::string& suffix,
			    const std::string& args) {
      start_data(os);
      os << "\t\tx = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\ty = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\tu = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\tv = np.array([float(v) for v in pipe.readline().split()])" << std::endl
	 << "\t\tif vectors" << suffix << " != None : vectors" << suffix << ".remove()" << std::endl
	 << "\t\tvectors" << suffix << " = ax" << suffix << ".quiver(x,y,u,v" << add_args(args) << ')' << std::endl;
      end_data(os);
    }
      
    inline void plot_vbar(std::ostream& os,
			  const std::string& suffix,
			  const std::string& args) {
      os << "vbar" << suffix << ", = plt.plot([], []" << add_args(args) << ")" << std::endl;
    }
      
    inline void get_vbar(std::ostream& os, const std::string& suffix) {
      start_data(os);
      os << "\t\tx = float(pipe.readline())" << std::endl
	 << "\t\tymin, ymax = ax" << parent_suffix(suffix) << ".get_ylim()" << std::endl
	 << "\t\tvbar" << suffix << ".set_data([x,x],[ymin,ymax])" << std::endl;
      end_data(os);
    }
      
    inline void plot_hbar(std::ostream& os,
			  const std::string& suffix,
			  const std::string& args) {
      os << "hbar" << suffix << ", = plt.plot([], []" << add_args(args) << ")"<< std::endl;
    }
      
    inline void get_hbar(std::ostream& os, const std::string& suffix) {
      start_data(os);
      os << "\t\ty = float(pipe.readline())" << std::endl
	 << "\t\txmin, xmax = ax" << parent_suffix(suffix) << ".get_xlim()" << std::endl
	 << "\t\thbar" << suffix << ".set_data([xmin,xmax],[y,y])" << std::endl;
      end_data(os);
    }

    inline void plot_patches(std::ostream& os, 
			     const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl;
      os << "patches" << suffix << " = []" << std::endl;
    }

    inline void get_patches(std::ostream& os,
			    const std::string& suffix) {
      start_data(os);
      os << "\t\tfor p in patches" << suffix << ':' << std::endl
	 << "\t\t\tp.remove()" << std::endl
	 << "\t\tcommand = pipe.readline()" << std::endl
	 << "\t\tax = ax" << suffix << std::endl
	 << "\t\texec(command) # allocates patches in ax, stored in tmp." << std::endl
	 << "\t\tpatches" << suffix << " = tmp" << std::endl
	 << "\t\tfor p in patches" << suffix << ':' << std::endl
	 << "\t\t\tax.add_patch(p)" << std::endl;
      end_data(os);
    }

    inline void plot_image(std::ostream& os,
			   const std::string& suffix, 
			   const std::string& args) {
      os << "ax" << suffix << " = ax" << std::endl;
      os << "axim" << suffix << " = NonUniformImage(ax " << add_args(args)  << ")" << std::endl;
      os << "axim" << suffix << ".set_data([0,1],[0,1],np.array([0,0,0,0]).reshape((2,2,1))) # fake image" << std::endl;
      os << "axim" << suffix << ".set_extent((0,1,0,1))" << std::endl;
      os << "ax.images.append(axim" << suffix << ")" << std::endl;
    }

    inline void get_image(std::ostream& os,
			  const std::string& suffix) {
      start_data(os);
      os << "\t\tx = np.array([float(v) for v in pipe.readline().split()])" << std::endl;
      os << "\t\ty = np.array([float(v) for v in pipe.readline().split()])" << std::endl;
      os << "\t\trawz = [float(v) for v in pipe.readline().split()]" << std::endl;
      os << "\t\twidth, depth = [int(v) for v in pipe.readline().split()]" << std::endl;
      os << "\t\tim = np.array(rawz).reshape((len(rawz)//(width*depth), width, depth))" << std::endl;
      os << "\t\taxim" << suffix << ".set_data(x, y, im)" << std::endl;
      os << "\t\taxim" << suffix << ".set_extent((x.min(), x.max(), y.min(), y.max()))" << std::endl;
      os << "\t\tax"   << suffix << ".set_xlim((x.min(), x.max()))" << std::endl;
      os << "\t\tax"   << suffix << ".set_ylim((y.min(), y.max()))" << std::endl;
      end_data(os);
    }
    

    inline void plot_contours(std::ostream& os,
			      const std::string& suffix) {
      os << "ax" << suffix << " = ax" << std::endl
	 << "contours" << suffix << " = None" << std::endl;
    }

    inline void get_contours(std::ostream& os,
			     const std::string& suffix, 
			     const std::string& args,
			     unsigned int fontsize) {
      start_data(os);
      os << "\t\t(xmin,xmax,nb_x) = [float(v) for v in pipe.readline().split()]" << std::endl;
      os << "\t\t(ymin,ymax,nb_y) = [float(v) for v in pipe.readline().split()]" << std::endl;
      os << "\t\tV                = np.array([float(v) for v in pipe.readline().split()])" << std::endl;
      os << "\t\tstep = (xmax-xmin)/(nb_x-1)" << std::endl;
      os << "\t\tx    = np.arange(xmin, xmax+.5*step,step)" << std::endl;
      os << "\t\tstep = (ymax-ymin)/(nb_y-1)" << std::endl;
      os << "\t\ty    = np.arange(ymin, ymax+.5*step,step)" << std::endl;
      os << "\t\tX, Y = np.meshgrid(x, y)" << std::endl;
      os << "\t\tZ = np.array([float(v) for v in pipe.readline().split()]).reshape(int(nb_y),int(nb_x))" << std::endl;
      os << "\t\tax" << suffix << ".set_xlim((xmin,xmax))" << std::endl;
      os << "\t\tax" << suffix << ".set_ylim((ymin,ymax))" << std::endl;
      os << "\t\tif contours" << suffix << " != None : " << std::endl;
      os << "\t\t\tfor d in contours" << suffix << ".collections :" << std::endl;
      os << "\t\t\t\td.remove()" << std::endl;
      os << "\t\tcontours" << suffix << " = ax" << suffix << ".contour(X, Y, Z, V"<< add_args(args) << ')' << std::endl;
      if(fontsize != 0)
	os << "\t\tplt.clabel(contours" << suffix << ", fontsize=" << fontsize << ", inline=1)" << std::endl;
      end_data(os);
    }

    

    inline void plot_text(std::ostream& os,
			  const std::string& suffix,
			  const std::string& args) {
      os << "text" << suffix << " = plt.text(None, None, ''" 
	 << add_args(args) << ")" << std::endl;
    }
      
    inline void get_text(std::ostream& os, const std::string& suffix) {
      start_data(os);
      os << "\t\txy = [float(v) for v in pipe.readline().split()]" << std::endl
	 << "\t\ttext = pipe.readline().split()[0]" << std::endl
	 << "\t\ttext" << suffix << ".set_position(xy)" << std::endl
	 << "\t\ttext" << suffix << ".set_text(text)" << std::endl;
      end_data(os);
    }



  }
}
