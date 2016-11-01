#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std::placeholders;

#define VIEW_FILE "viewer-011-text.py"

// This function fills a vector of points to be plotted, depending on
// current time.
void fill_data(ccmpl::Point& pos, std::string& text, int epoch) {

  pos = {cos((double)epoch/50.* M_PI), sin((double)epoch/50.* M_PI)};
  text = std::to_string(epoch);
}

int main(int argc, char* argv[]) {

  int current_time;

  // Every program may start like this.

  if(argc != 2) {
    std::cout << "Usage : " << std::endl
	      << argv[0] << " generate" << std::endl
	      << argv[0] << " run | ./" << VIEW_FILE << std::endl;
    return 0;
  }
  bool generate_mode = std::string(argv[1])=="generate";


  // Let us define the layout, a 1x1 grid here. Args are width, height
  // the grid structure and, optionnaly, the background color of the figure
  auto display = ccmpl::layout(8.0, 4.0, {"#"}, ccmpl::RGB(1., 1., 1.));
  display.set_ratios({1.}, {1.});

  // Let us define our charts (1 here)
  display().title   = "Text demo";                                                // set chart titles
  display().xtitle  = "";
  display().ytitle  = "";
  display()         = "equal";                                                       // this sets the axis aspect argument.
  display()         = {-2, 2, -2, 2};                                                // set xmin,xmax,ymin,ymax
  display()        += ccmpl::text("ha=\"center\", va=\"center\", family=\"cursive\", color=\"b\"",          // extra matplotlib arguments
				  std::bind(fill_data, _1, _2, std::ref(current_time)));// the filling function

  if(generate_mode) {
    display.make_python(VIEW_FILE,true); // boolean tells wether we display some GUI or not.
    return 0;                            // Python script is generated, that's all for generation mode.
  }

  // Execution

  // ccmpl::filename("img",i,"png") helps to define "img-%06d.png" names. Use ccmpl::nofile() if no image files are needed.

  for(current_time = 0; current_time < 1000; ++current_time)
    std::cout << display("#", // Use # or - for each data element for trigerring its update.
			 ccmpl::nofile(),  // name of the generated pdf file
			 ccmpl::nofile()); // name of the generated png file. 

  std::cout << ccmpl::stop;
   
  return 0;	 
}
