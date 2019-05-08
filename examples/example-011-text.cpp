#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std::placeholders;

#define VIEW_PREFIX "viewer-011-text"

// This function fills a vector of points to be plotted, depending on
// current time.
void fill_data(ccmpl::Point& pos, std::string& text, int epoch) {

  pos = {cos((double)epoch/50.* M_PI), sin((double)epoch/50.* M_PI)};
  text = std::to_string(epoch);
}

int main(int argc, char* argv[]) {

  int current_time;

  // Let us use a predefined class ccmpl::Main in order to handle the
  // display in the main function.
  ccmpl::Main m(argc,argv,VIEW_PREFIX);

  // Let us define the layout, a 1x1 grid here. Args are width, height
  // the grid structure and, optionnaly, the background color of the figure
  auto display = ccmpl::layout(m.hostname, m.port,
			       8.0, 4.0, {"#"}, ccmpl::RGB(1., 1., 1.));
  display.set_ratios({1.}, {1.});

  // Let us define our charts (1 here)
  display().title   = "Text demo";                                                // set chart titles
  display().xtitle  = "";
  display().ytitle  = "";
  display()         = ccmpl::view2d({-2, 2}, {-2, 2}, ccmpl::aspect::equal, ccmpl::span::placeholder);
  display()        += ccmpl::text("ha=\"center\", va=\"center\", family=\"cursive\", color=\"b\"",          // extra matplotlib arguments
				  std::bind(fill_data, _1, _2, std::ref(current_time)));// the filling function

  // the ccmpl::Main object handles generation here.
  m.generate(display, true); // true means "use GUI".

  // Execution

  // ccmpl::filename("img",i,"png") helps to define "img-%06d.png" names. Use ccmpl::nofile() if no image files are needed.

  for(current_time = 0; current_time < 1000; ++current_time)
    display("#", // Use # or - for each data element for trigerring its update.
	    ccmpl::nofile(),  // name of the generated pdf file
	    ccmpl::nofile()); // name of the generated png file. 
  
  !display;
   
  return 0;	 
}
