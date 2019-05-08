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

  // Let us define the layout, a 1x2 grid here. Args are width, height
  // the grid structure and, optionnaly, the background color of the figure
  auto display = ccmpl::layout(m.hostname, m.port,
			       8.0, 8.0,
			       {"#", "#"}, // Two stacked plots here (see next examples).
			       ccmpl::RGB(1., 1., 1.));
  display.set_ratios({2.}, {1., 1.});

  // Let us define our charts 
  display().title   = "Gabor filter";                                                // set chart titles
  display().xtitle  = "space";
  display().ytitle  = "amplitude";
  display()         = ccmpl::view2d({-5, 5}, {-10, 10}, ccmpl::aspect::fit, ccmpl::span::placeholder); 
  display()        += ccmpl::line("'b-'",                                            // extra matplotlib arguments
				  std::bind(fill_data, _1, std::ref(current_time))); // the filling function
  
  display++; // skip to next chart description... only y-limit management changes.
  
  display().title   = "Gabor filter";                                                // set chart titles
  display().xtitle  = "space";
  display().ytitle  = "amplitude";
  display()         = ccmpl::view2d({-5, 5}, ccmpl::limit::fit, ccmpl::aspect::fit, ccmpl::span::placeholder); 
  display()        += ccmpl::line("'b-'",                                            // extra matplotlib arguments
				  std::bind(fill_data, _1, std::ref(current_time))); // the filling function

  // the ccmpl::Main object handles generation here
  m.generate(display, true); // true means "use GUI"

  // Execution

  // ccmpl::filename("img",i,"png") helps to define "img-%06d.png" names. Use ccmpl::nofile() if no image files are needed.

  for(current_time = 0; current_time < 100; ++current_time)
    display("##",             // Use # or - for each data element for trigerring its update.
	    ccmpl::nofile(),  // name of the generated pdf file
	    ccmpl::nofile()); // name of the generated png file. 
  
  !display; // This stops the remote display.
   
  return 0;	 
}
