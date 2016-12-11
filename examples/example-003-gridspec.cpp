#include <iostream>
#include <ccmpl.hpp>
#include <functional>
#include <iterator>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std::placeholders;

#define VIEW_FILE "viewer-003-gridspec.py"

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

// As we use further ccmpl::lines<3>(...), we have to provide a
// function that fills 10 lines.
void fill_lines(std::array<std::vector<ccmpl::Point>, 10>& lines) {
  for(auto& line : lines) {
    line.clear();
    auto out = std::back_inserter(line);
    ccmpl::Point dir  = {rand()/(1.0+RAND_MAX)*.2 - .1, rand()/(1.0+RAND_MAX)*.2 - .1};
    ccmpl::Point prev = {0,0};
    *(out++) = prev;
    for(unsigned int i = 0; i < 20; ++i) {
      ccmpl::Point move = {rand()/(1.0+RAND_MAX)*.1 - .05, rand()/(1.0+RAND_MAX)*.1 - .05};
      prev += dir + move;
      *(out++) = prev;
    }
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


  // Let us define the layout, a 3x2 grid here. Args are width, height
  // and the grid structure. Only 4 charts will be inserted in the grid.
  auto display = ccmpl::layout(8.0, 4.0, {  "#.#"  ,
	                                    ".##"  }, 
    ccmpl::RGB(1., 1., 1.));                          // Set background to white
  display.set_ratios({1.,2.,1.}, {1.,1.});

  // Let us define our charts (3 here, with 1, 2 and 1 data elements respectively)
  display()         = {-1.1, 1.1, -1.1, 1.1};    
  display()         = ccmpl::show_tics(false,false); // hide x and y tics
  display()         = "equal";    
  display()        += ccmpl::line("'b-',zorder=1",            fill_circle                                      ); // data element #1
  display()        += ccmpl::dot ("c='y',lw=1,s=50,zorder=2", std::bind(fill_dot,   _1, std::ref(current_time))); // data element #2
  display++;        // Skip to next chart.
  display()         = {-1.1, 1.1, -1.1, 1.1};    
  display()         = ccmpl::hide_axis(); // hide axis (ticks as well)
  display()         = "equal";                                                  
  display()        += ccmpl::dots("c='g',lw=1,s=20",          std::bind(fill_dots,  _1, std::ref(current_time))); // data element #3
  display++;        // Skip to next chart.
  display().title   = "Gabor filter";     
  display()         = ccmpl::ratio(5,2); // width/height = 5/2 
  display()         = {-3, 3, -1, 1};    
  display()         = ccmpl::show_tics(true,false); // hide y tics  
  display()        += ccmpl::line("'b-', linewidth=2.0",      std::bind(fill_gabor, _1, std::ref(current_time))); // data element #4
  display++;        // Skip to next chart.
  display().title   = "Crazy lines";     
  display()         = "equal";       
  display()         = {-1., 1., -1., 1.};
  display()         = ccmpl::show_tics(false,false); // hide x and y tics
  display()        += ccmpl::lines<10>("'r-', linewidth=1.0",  fill_lines                                       ); // data element #5                                       

  if(generate_mode) {
    // WARNING : display.make_python(...,false) generates a matplotlib bug when ccmpl::lines is used. 
    display.make_python(VIEW_FILE,true); 
    return 0;                          
  }

  // Execution
  
  current_time = 0;
  std::cout << display("#####", ccmpl::nofile(), ccmpl::nofile());   // The 5 data elements are updated.
  for(++current_time; true ; ++current_time)
    std::cout << display(".###.", ccmpl::nofile(), ccmpl::nofile()); // The data element #1 and #5 are not updated since they do not depend on time.

  return 0;	 
}
