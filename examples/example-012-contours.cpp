#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <cmath>
#include <iterator>
#include <memory>

using namespace std::placeholders;

#define VIEW_FILE "viewer-012-contours.py"


double height(double x, double y) {
  double dx   = x-2;
  double dy   = y-2;
  double res  = std::exp(-.1*(dx*dx+dy*dy));
  res        -= std::exp(-.1*x*x - y*y);
  dx          = x-4;
  dy          = y+3;
  res        -= std::exp(-.2*dx*dx - .1*dy*dy);
  
  return res;    
}

#define XRADIUS    4
#define YRADIUS    3
#define NB        50
#define NB_LEVELS 20
void fill_data(std::vector<double>& z,
	       double& xmin, double& xmax, unsigned int& nb_x,
	       double& ymin, double& ymax, unsigned int& nb_y,
	       double& zmin, double& zmax, unsigned int& nb_z) {

  xmin = -XRADIUS;
  xmax =  XRADIUS;
  nb_x = NB*XRADIUS;

  ymin = -YRADIUS;
  ymax =  YRADIUS;
  nb_y = NB*YRADIUS;

  zmin =         0;
  zmax =         0;
  nb_z = NB_LEVELS;

  z.clear();
  auto outz = std::back_inserter(z);

  // Use the range iterator for generating the grid values.
  
  for(auto y : ccmpl::range(ymin, ymax, nb_y))
    for(auto x : ccmpl::range(xmin, xmax, nb_x))
      *(outz++) = height(x,y);
}


int main(int argc, char* argv[]) {


  // Every program may start like this.

  if(argc != 2) {
    std::cout << "Usage : " << std::endl
	      << argv[0] << " generate" << std::endl
	      << argv[0] << " run | ./" << VIEW_FILE << std::endl;
    return 0;
  }
  bool generate_mode = std::string(argv[1])=="generate";


  // Let us define the layout, a 1x1 grid here. Args are width, height
  // and the grid structure.
  auto display = ccmpl::layout(8.0, 6.0, {"#"});
  display.set_ratios({4.}, {3.});

  // Gray image
  display().title   = "Contours  ";  
  display()         = {-XRADIUS, XRADIUS, -YRADIUS, YRADIUS};  
  display()         = "equal";
  display()        += ccmpl::contours("colors='k'", fill_data);
  display++;

  

  if(generate_mode) {
    display.make_python(VIEW_FILE,false);
    return 0;                            
  }

  // Execution
  
  std::cout << display("#", "ccmpl-012.pdf", ccmpl::nofile())
	    << ccmpl::stop;
  
  return 0;	 
}
