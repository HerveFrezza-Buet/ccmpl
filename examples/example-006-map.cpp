#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <vector>
#include <cmath>

using namespace std::placeholders;

#define VIEW_FILE "viewer-006-map.py"


#define R_MIN .2
#define R_MAX  1
#define THETA_MIN .5
#define THETA_MAX (2*M_PI-(THETA_MIN))

#define DX  .1
#define DY .05


// Let us define a function from [0,1]^2 to [-1,1]^2
ccmpl::Point f(const ccmpl::Point& p) {
  double r     = R_MIN + p.x*(R_MAX-R_MIN);
  double theta = THETA_MIN + p.y*(THETA_MAX-THETA_MIN);
  return {r*std::cos(theta),r*std::sin(theta)};
}


void fill_surface(std::vector<ccmpl::ValueAt>& surface) {
  surface.clear();
  for(double x = 0; x <= 1; x += DX)
    for(double y = 0; y <= 1; y += DY) 
      surface.push_back({f({x,y}),y});
}

void fill_palette(std::vector<ccmpl::ColorAt>& colored_dots) {
  colored_dots.clear();
  for(double x = 0; x <= 1; x += DX)
    for(double y = 0; y <= 1; y += DY) 
      colored_dots.push_back({f({x,y}), ccmpl::color::from_point(ccmpl::Point(x,y),
								 0,1,
								 0,1)});
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

  std::string update_pattern = "";
  auto display = ccmpl::layout(8.0, 4.0, {"##"});

  display().title   = "Surface";
  display()         = {-1.1, 1.1, -1.1, 1.1};    
  display()         = ccmpl::show_tics(false,false); 
  display()         = "equal";                                                  
  display()        += ccmpl::surface("cmap='binary'", 0, 1, fill_surface);  update_pattern += '#';
  display++;
  display().title   = "Palette";
  display()         = {-1.1, 1.1, -1.1, 1.1};                          
  display()         = ccmpl::show_tics(false,false); 
  display()         = "equal";                           
  display()        += ccmpl::palette("", fill_palette);                     update_pattern += '#'; 

  if(generate_mode) {
    display.make_python(VIEW_FILE,false);  // no gui, we only generate a single pdf.
    return 0;                          
  }

  // Execution
  
  std::cout << display(update_pattern, "ccmpl-006.pdf", ccmpl::nofile())
	    << ccmpl::stop;
  return 0;	 
}
