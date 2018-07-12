#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <vector>
#include <cmath>

using namespace std::placeholders;

#define VIEW_PREFIX "viewer-001-getting-started"

// This function fills a vector of points to be plotted, depending on
// current time.
void fill_data(std::vector<ccmpl::Point>& curve, double& time) {

  curve.clear();
  for(double x = -3; x <= 3 ; x += .05) 
    curve.push_back({x,10*std::cos(.01*time)*std::cos(10*(x+.01*time))*std::exp(-x*x)});
}

int main(int argc, char* argv[]) {

  double current_time;

  // Let us use a predefined class ccmpl::Main in order to handle the
  // display in the main function.
  ccmpl::Main m(argc,argv,VIEW_PREFIX);

  // Let us define the layout, a 1x1 grid here. Args are width, height
  // the grid structure and, optionnaly, the background color of the figure
  auto display = ccmpl::layout(8.0, 4.0,
			       {"#"},
			       ccmpl::RGB(1., 1., 1.));
  display.set_ratios({1.}, {1.});

  // Let us define our charts (1 here)
  display().title   = "Gabor filter";                                                // set chart titles
  display().xtitle  = "space";
  display().ytitle  = "amplitude";
  display()         = "auto";                                                        // this sets the axis aspect argument.
  display()         = {-5, 5, -1, 1};                                                // set xmin,xmax,ymin,ymax
  display()         = ccmpl::autoscale::Y;                                           // We autoscale on Y only. X, XY are also available.
  display()        += ccmpl::line("'b-'",                                            // extra matplotlib arguments
				  std::bind(fill_data, _1, std::ref(current_time))); // the filling function

  // the ccmpl::Main object handles generation here
  m.generate(display, true); // true means "use GUI"

  // Execution

  // ccmpl::filename("img",i,"png") helps to define "img-%06d.png" names. Use ccmpl::nofile() if no image files are needed.

  for(current_time = 0; current_time < 100; ++current_time)
    std::cout << display("#", // Use # or - for each data element for trigerring its update.
			 ccmpl::nofile(),  // name of the generated pdf file
			 ccmpl::nofile()); // name of the generated png file. 

  std::cout << ccmpl::stop;
   
  return 0;	 
}
