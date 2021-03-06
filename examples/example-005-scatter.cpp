#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <vector>
#include <cmath>

using namespace std::placeholders;

#define VIEW_PREFIX "viewer-005-scatter"


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

  // Let us use a predefined class ccmpl::Main in order to handle the
  // display in the main function.
  ccmpl::Main m(argc,argv,VIEW_PREFIX);
  
  std::string update_pattern = "";
  auto display = ccmpl::layout(m.hostname, m.port,
			       9.0, 3.0, {"###"});

  display().title   = "Dots";
  display()         = ccmpl::view2d({-1.1, 1.1}, {-1.1, 1.1}, ccmpl::aspect::equal, ccmpl::span::placeholder);
  display()         = ccmpl::show_tics(false,false);  
  display()        += ccmpl::dots("c='k',s=10", fill_dots);                         update_pattern += '#';
  display++;
  display().title   = "Confetti";
  display()         = ccmpl::view2d({-1.1, 1.1}, {-1.1, 1.1}, ccmpl::aspect::equal, ccmpl::span::placeholder);
  display()         = ccmpl::show_tics(false,false);     
  display()        += ccmpl::confetti("s=10", fill_confetti);                       update_pattern += '#';
  display++;
  display().title   = "Vectors"; 
  display()         = ccmpl::view2d({-1.1, 1.1}, {-1.1, 1.1}, ccmpl::aspect::equal, ccmpl::span::placeholder);
  display()         = ccmpl::show_tics(false,false); 
  display()        += ccmpl::vectors("color='black',pivot='tip'", fill_vectors); update_pattern += '#'; 

  // the ccmpl::Main object handles generation here.
  m.generate(display, false); // no gui, we only generate a single pdf

  // Execution
  
  display(update_pattern, "ccmpl-005.pdf", ccmpl::nofile());
  !display;
  return 0;	 
}
