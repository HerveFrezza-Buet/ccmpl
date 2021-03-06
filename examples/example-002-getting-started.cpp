#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <vector>
#include <cmath>

using namespace std::placeholders;

#define VIEW_PREFIX "viewer-002-getting-started"

// This function fills a vector of points to be plotted, depending on
// current time.
void fill_data(std::vector<ccmpl::Point>& curve, double& time) {

  curve.clear();
  for(double x = -3; x <= 3 ; x += .05) 
    curve.push_back({x,std::cos(10*(x+.01*time))*std::exp(-x*x)});
}

void fill_vbar(double& bar_position, double& time) {
   bar_position = std::cos(.01*time);
}

void fill_hbar(double& bar_position, double& time) {
   bar_position = std::sin(.01*time);
}

int main(int argc, char* argv[]) {

  double current_time;

  // Let us use a predefined class ccmpl::Main in order to handle the
  // display in the main function.
  ccmpl::Main m(argc,argv,VIEW_PREFIX);


  // Let us define the layout, a 1x1 grid here. Args are width, height
  // and the grid structure.
  auto display = ccmpl::layout(m.hostname, m.port,
			       8.0, 4.0, {"#"});
  display.set_ratios({1.}, {1.});

  // Let us define our charts (1 here, with 3 data elements)
  display().title   = "Gabor filter";                                       
  display().xtitle  = "space";
  display().ytitle  = "amplitude";
  display()         = ccmpl::view2d({-5, 5}, {-1, 1}, ccmpl::aspect::equal, ccmpl::span::placeholder); 
  display()         = ccmpl::legend("loc = 'lower right'"); // sets the legend
  display()        += ccmpl::line("'b-', linewidth=2.0, label='Gabor'", std::bind(fill_data, _1, std::ref(current_time))); // data element #1
  display()        += ccmpl::hbar("'r'",                 std::bind(fill_hbar, _1, std::ref(current_time)));                // data element #2
  display()        += ccmpl::vbar("'g'",                 std::bind(fill_vbar, _1, std::ref(current_time)));                // data element #3

  // the ccmpl::Main object handles generation here.
  m.generate(display, true); // true means "use GUI".

  // Execution

  for(current_time = 0; current_time < 100 ; ++current_time)
    display("###", // Use # or - for each data element for trigerring its update.
	    ccmpl::nofile(), ccmpl::nofile());
  !display;
   
  

  return 0;	 
}
