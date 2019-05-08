#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <cmath>
#include <iterator>
#include <memory>

using namespace std::placeholders;

#define VIEW_PREFIX "viewer-008-patches"


void fill_data(std::vector<std::shared_ptr<ccmpl::Patch>>& patches, double& time) {
  patches.clear();
  double angle = 20*std::sin(.1*time);
  double rayon = .5+.2*std::sin(.2*time);
  auto out = std::back_inserter(patches);
  
  ccmpl::patch::GC gc;
  gc.edgecolor = {.5, 0., 0.};
  gc.facecolor = {1., 0., 0.};
  gc.linewidth = 2;
  gc.alpha = 0.5 * (1.0 + std::sin(.1*time));
  *(out++) = std::make_shared<ccmpl::patch::Rectangle>(gc,ccmpl::Point(-2,-2),4,1,angle);
  gc.alpha = 1.0;

  gc.edgecolor = {0., 0., .5};
  gc.facecolor = {0., 0., 1.};
  *(out++) = std::make_shared<ccmpl::patch::Circle>(gc,ccmpl::Point(-1,1),rayon);
  *(out++) = std::make_shared<ccmpl::patch::Circle>(gc,ccmpl::Point( 1,1),rayon);
  gc.edgecolor = {0., 0., 0.};
  gc.linewidth = 3;
  gc.fill      = false;
  gc.linestyle = "dashed";
  *(out++) = std::make_shared<ccmpl::patch::FancyArrow>(gc,
							0,2,0,-2,  // x,y,dx,dy
							.1,.5,.5,  // width,head_width,head_length
							0, // overhang
							true);     // length_includes_head
  
  gc.edgecolor = {0., 0., 0.};
  gc.linewidth = 2;
  gc.linestyle = "dashdot";

  *(out++) = std::make_shared<ccmpl::patch::Wedge>(gc,
						   ccmpl::Point(-4.5, 0),
						   1.0, 
						   15 + 10 * std::cos(time/3.0),
						   345 - 10 * std::cos(time/3.0));

  gc.edgecolor = {0.4, 0.4, 0.};
  gc.linewidth = 5;
  gc.linestyle = "solid";
  *(out++) = std::make_shared<ccmpl::patch::Arc>(gc,
						 ccmpl::Point(0., 2.),
						 2., 1.,
						 0.0,
						 45, 135);

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

  // Let us define our charts (1 here)
  display().title   = "Patches    ";                                                    // set chart titles
  display()         = ccmpl::view2d({-6, 3}, {-3, 3}, ccmpl::aspect::equal, ccmpl::span::placeholder); 
  display()        += ccmpl::patches(std::bind(fill_data, _1, std::ref(current_time))); // the filling function

  // the ccmpl::Main object handles generation here.
  m.generate(display, true); // true means "use GUI".  

  // Execution

  // ccmpl::filename("img",i,"png") helps to define "img-%06d.png" names. Use ccmpl::nofile() if no image files are needed.

  for(current_time = 0; current_time < 10000; ++current_time)
    display("#", // Use # or - for each data element for trigerring its update.
	    ccmpl::nofile(),  // name of the generated pdf file
	    ccmpl::nofile()); // name of the generated png file. 
  
  !display;
   
  return 0;	 
}
