#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <vector>
#include <cmath>

using namespace std::placeholders;

#define VIEW_FILE "viewer-004-gridspec.py"

void fill_gabor(std::vector<ccmpl::Point>& curve, double& time) {
  curve.clear();
  for(double x = -3; x <= 3 ; x += .05) 
    curve.push_back({x,std::cos(10*(x+.01*time))*std::exp(-x*x)});
}

void fill_circle(std::vector<ccmpl::Point>& curve) {
  curve.clear();
  for(double theta = 0; theta < 2*M_PI+.05; theta += .05) 
    curve.push_back({std::cos(theta), std::sin(theta)});
}

void fill_dot(ccmpl::Point& dot, double& time) {
  dot = {std::cos(.01*time), std::sin(.01*time)};
}

void fill_dots(std::vector<ccmpl::Point>& dots, double& time) {
  dots.clear();
  for(int i=0; i < 9; ++i) {
    double theta = i*2*M_PI/9.0 + 0.01*time;
    dots.push_back({std::cos(theta), std::sin(theta)});
  }
}

int main(int argc, char* argv[]) {

  double current_time;

  // Every program may start like this.

  if(argc != 2) {
    std::cout << "Usage : " << std::endl
	      << argv[0] << " generate" << std::endl
	      << argv[0] << " run | ./" << VIEW_FILE << std::endl;
    return 0;
  }
  bool generate_mode = std::string(argv[1])=="generate";


  // Let us define the layout, a 3x3 grid here. Args are width, height
  // and the grid structure. Only 3 charts will be inserted in the grid.
  // The layout allows for basic spanning :
  //    '.' --> no graph.
  //    '#' --> some graph here.
  //    '>' --> the graph is extended to the slot on the right.
  //    'V' --> the graph is extended to the slot on the bottom.
  //    'X' --> both '>' and 'V'.

  auto display = ccmpl::layout(8.0, 4.0, {  "X.."  ,
	                                    "..V"  ,
	                                    ">.."  });

  // Let us define our charts (3 here, with 1, 2 and 1 data elements respectively)
  display()         = {-1.1, 1.1, -1.1, 1.1};    
  display()         = ccmpl::show_tics(false,false); 
  display()         = "equal";    
  display()        += ccmpl::line("'b-',zorder=1",            fill_circle                                      ); // data element #1
  display()        += ccmpl::dot ("c='y',lw=1,s=50,zorder=2", std::bind(fill_dot,   _1, std::ref(current_time))); // data element #2
  display++;       
  display()         = {-1.1, 1.1, -1.1, 1.1};    
  display()         = ccmpl::show_tics(false,false); 
  display()         = "equal";                                                  
  display()        += ccmpl::dots("c='g',lw=1,s=20",          std::bind(fill_dots,  _1, std::ref(current_time))); // data element #3
  display++;       
  display().title   = "Gabor filter";    
  display()         = "equal";    
  display()         = {-3, 3, -1, 1};    
  display()         = ccmpl::show_tics(true,false); 
  display()        += ccmpl::line("'b-', linewidth=2.0",      std::bind(fill_gabor, _1, std::ref(current_time))); // data element #4

  if(generate_mode) {
    display.make_python(VIEW_FILE,true); 
    return 0;                          
  }

  // Execution
  
  current_time = 0;
  std::cout << display("####", ccmpl::nofile(), ccmpl::nofile());   // The 4 data elements are updated.
  for(++current_time; true ; ++current_time)
    std::cout << display(".###", ccmpl::nofile(), ccmpl::nofile()); // The data element #1 is not updated.

  return 0;	 
}
