#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <vector>
#include <cmath>

using namespace std::placeholders;

#define VIEW_FILE "viewer-005-scatter.py"


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

ccmpl::Point normal(const ccmpl::Point& p) {
  ccmpl::Point pos = f(p);
  ccmpl::Point n(-pos.y, pos.x);
  double norm = std::sqrt(n.x*n.x + n.y*n.y);
  return {n.x/norm, n.y/norm};
}


void fill_dots(std::vector<ccmpl::Point>& dots) {
  dots.clear();
  for(double x = 0; x <= 1; x += DX)
    for(double y = 0; y <= 1; y += DY)
      dots.push_back(f({x,y}));
}

void fill_confetti(std::vector<ccmpl::ColorAt>& colored_dots) {
  colored_dots.clear();
  for(double x = 0; x <= 1; x += DX)
    for(double y = 0; y <= 1; y += DY) 
      colored_dots.push_back({f({x,y}), ccmpl::color::from_point(ccmpl::Point(x,y),
								 0,1,
								 0,1)});
}

void fill_vectors(std::vector<std::pair<ccmpl::Point,ccmpl::Point>>& vectors) {
  vectors.clear();
  for(double x = 0; x <= 1; x += DX)
    for(double y = 0; y <= 1; y += DY) {
      ccmpl::Point p = f({x,y});
      ccmpl::Point n = normal({x,y});
      vectors.push_back({p,n});
    }
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
  auto display = ccmpl::layout(9.0, 3.0, {"###"});

  display().title   = "Dots";
  display()         = {-1.1, 1.1, -1.1, 1.1};    
  display()         = ccmpl::show_tics(false,false); 
  display()         = "equal";                                                  
  display()        += ccmpl::dots("c='k',s=10", fill_dots);                         update_pattern += '#';
  display++;
  display().title   = "Confetti";
  display()         = {-1.1, 1.1, -1.1, 1.1};    
  display()         = ccmpl::show_tics(false,false); 
  display()         = "equal";                                                  
  display()        += ccmpl::confetti("s=10", fill_confetti);                       update_pattern += '#';
  display++;
  display().title   = "Vectors";
  display()         = {-1.1, 1.1, -1.1, 1.1};                          
  display()         = ccmpl::show_tics(false,false); 
  display()         = "equal";                           
  display()        += ccmpl::vectors("color='black',pivot='origin'", fill_vectors); update_pattern += '#'; 

  if(generate_mode) {
    display.make_python(VIEW_FILE,false);  // no gui, we only generate a single pdf.
    return 0;                          
  }

  // Execution
  
  std::cout << display(update_pattern, "ccmpl-005.pdf", ccmpl::nofile())
	    << ccmpl::stop;
  return 0;	 
}
