#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <cmath>
#include <iterator>
#include <memory>

using namespace std::placeholders;

#define VIEW_FILE "viewer-009-image.py"


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
  auto display = ccmpl::layout(8.0, 4.0, {"##"});
  display.set_ratios({1.,1.}, {1.});

  // Let us define our charts (2 here)
  // Gray image
  display().title   = "Gray Image  ";                                                    // set chart titles
  display()         = {-3, 3, -3, 3};                                                 // set xmin,xmax,ymin,ymax; actually, they will be overwritten by the ccmpl::image
  display()         = "equal";
  display()        += ccmpl::image("cmap='binary', interpolation='bilinear', clim=(0,1)", std::bind(fill_data, _1, _2, _3, _4, _5, std::ref(current_time))); // the filling function
  display++;

  // RGB Image
  display().title   = " RGB Image ";                                                    // set chart titles
  display()         = {-3, 3, -3, 3};                                                 // set xmin,xmax,ymin,ymax; actually, they will be overwritten by the ccmpl::image
  display()         = "equal";
  display()        += ccmpl::image("interpolation='bilinear'", std::bind(fill_data_rgb, _1, _2, _3, _4, _5, std::ref(current_time))); // the filling function


  

  if(generate_mode) {
    display.make_python(VIEW_FILE,true); // boolean tells wether we display some GUI or not.
    return 0;                            // Python script is generated, that's all for generation mode.
  }

  // Execution

  // ccmpl::filename("img",i,"png") helps to define "img-%06d.png" names. Use ccmpl::nofile() if no image files are needed.

  for(current_time = 0; current_time < 1000; ++current_time)
    std::cout << display("##", // Use # or - for each data element for trigerring its update.
			 ccmpl::nofile(),  // name of the generated pdf file
			 ccmpl::nofile()); // name of the generated png file. 

  std::cout << ccmpl::stop;
   
  return 0;	 
}
