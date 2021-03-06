#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <cmath>
#include <iterator>
#include <memory>

using namespace std::placeholders;

#define VIEW_PREFIX "viewer-009-image"


void fill_data(std::vector<double>& x, std::vector<double>& y, std::vector<double>& z, unsigned int& width, unsigned int& depth, double& time) {

  x.clear();
  y.clear();
  z.clear();
 
  double xmin = -4;
  double xmax = 4 + 2*cos(time/10.);
  double ymin= -4;
  double ymax = 4 + 2*cos(time/10.);
  width = 50;
  unsigned int height = 50;
  depth = 1;

  auto outx = std::back_inserter(x);
  auto outy = std::back_inserter(y);
  auto outz = std::back_inserter(z);
  for(unsigned int i = 0 ; i < height ; ++i) {
    double y = ymin + double(i)/(height-1.) * (ymax - ymin);  
    *(outy++) = y;
    for(unsigned int j = 0 ; j < width ; ++j) {
      double x = xmin + double(j)/(width-1.) * (xmax - xmin);
         
      if(i == 0) *(outx++) = x;
      *(outz++) = cos(x + y*y);
    }
  }
}

void fill_data_rgb(std::vector<double>& x, std::vector<double>& y, std::vector<double>& z, unsigned int& width, unsigned int& depth, double& time) {
  x.clear();
  y.clear();
  z.clear();
 
  double xmin = -4;
  double xmax = 4 + 2*cos(time/10.);
  double ymin= -4;
  double ymax = 4 + 2*cos(time/10.);
  width = 50;
  unsigned int height = 50;
  depth = 3;

  auto outx = std::back_inserter(x);
  auto outy = std::back_inserter(y);
  auto outz = std::back_inserter(z);
  for(unsigned int i = 0 ; i < height ; ++i) {
    double y = ymin + i/(height-1.) * (ymax - ymin);  
    *(outy++) = y;
    for(unsigned int j = 0 ; j < width ; ++j) {
      double x = xmin + j/(width-1.) * (xmax - xmin);
         
      if(i == 0) *(outx++) = x;
      *(outz++) = i/(height-1.);           // Red
      *(outz++) = j/(width-1.);            // Green
      *(outz++) = (1.0 + cos(x + y*y))/2.; // Blue
    }
  }
}



int main(int argc, char* argv[]) {

  double current_time;

  // Let us use a predefined class ccmpl::Main in order to handle the
  // display in the main function.
  ccmpl::Main m(argc,argv,VIEW_PREFIX);

  // Let us define the layout, a 1x2 grid here. Args are width, height
  // and the grid structure.
  auto display = ccmpl::layout(m.hostname, m.port,
			       8.0, 4.0, {"##"});
  display.set_ratios({1.,1.}, {1.});

  // Let us define our charts (2 here)
  // Gray image
  display().title   = "Gray Image  ";                                                    // set chart titles
  display()         = ccmpl::view2d({-3, 3}, {-3, 3}, ccmpl::aspect::equal, ccmpl::span::placeholder); 
  display()        += ccmpl::image("cmap='binary', interpolation='bilinear', clim=(0,1)", std::bind(fill_data, _1, _2, _3, _4, _5, std::ref(current_time))); // the filling function
  display++;

  // RGB Image
  display().title   = " RGB Image ";                     
  display()         = ccmpl::view2d({-3, 3}, {-3, 3}, ccmpl::aspect::equal, ccmpl::span::placeholder); 
  display()        += ccmpl::image("interpolation='bilinear'", std::bind(fill_data_rgb, _1, _2, _3, _4, _5, std::ref(current_time))); // the filling function

  // the ccmpl::Main object handles generation here.
  m.generate(display, true); // true means "use GUI".
  
  // Execution

  // ccmpl::filename("img",i,"png") helps to define "img-%06d.png" names. Use ccmpl::nofile() if no image files are needed.

  for(current_time = 0; current_time < 500; ++current_time)
    display("##", // Use # or - for each data element for trigerring its update.
	    ccmpl::nofile(),  // name of the generated pdf file
	    ccmpl::nofile()); // name of the generated png file. 

  !display;
   
  return 0;	 
}
