#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <cmath>
#include <iterator>
#include <memory>

using namespace std::placeholders;

#define VIEW_FILE "viewer-012-contours.py"


void fill_data(std::vector<double>& z,
	       double& xmin, double& xmax, unsigned int& nb_x,
	       double& ymin, double& ymax, unsigned int& nb_y) {

  xmin = -4;
  xmax =  4;
  nb_x = 40;
  
  ymin = -3;
  ymax =  3;
  nb_y = 30;

  z.clear();
  auto outz = std::back_inserter(z);

  // Use the range iterator for generating the grid values.
  
  for(auto y : ccmpl::range(ymin, ymax, nb_y))
    for(auto x : ccmpl::range(xmin, xmax, nb_x))
      *(outz++) = 0
}


int main(int argc, char* argv[]) {

  double current_time;

  for(double x : ccmpl::range(0,2,11))
    std::cout << x << std::endl;

  /*
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
  */
  return 0;	 
}
