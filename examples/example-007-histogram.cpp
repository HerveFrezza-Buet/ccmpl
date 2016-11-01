#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <random>

#include <ccmpl.hpp>

using namespace std::placeholders;

#define VIEW_FILE "viewer-007-histogram.py"

#define X_MAX           4
#define X_MIN     -(X_MAX)
#define NB_X_BINS      20
#define Y_MAX           4
#define Y_MIN     -(Y_MAX)
#define NB_Y_BINS      20
#define Z_MAX          20
#define Z_MIN           0  

#define NB_HISTO_SAMPLES 500

void fill_histogram(std::vector<ccmpl::Point>& data, std::mt19937& generator) {
  data.clear();

  std::normal_distribution<double> gaussian(0.0,1.0);
  for(unsigned int i = 0; i < NB_HISTO_SAMPLES; ++i)
    data.push_back({gaussian(generator),gaussian(generator)});
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
  
  std::random_device rd;
  std::mt19937       gen(rd());
  

  std::string update_pattern = "";
  auto display = ccmpl::layout(12.0, 6.0, {"##"});

  std::string histo2d_args = std::string("cmap='jet', vmin=")
    + std::to_string(Z_MIN)
    + ", vmax="
    + std::to_string(Z_MAX);

  display().title   = "Histogram 2D";
  display()         = {X_MIN, X_MAX, Y_MIN, Y_MAX};    
  display()         = ccmpl::show_tics(true,true); 
  display()         = "equal";                                                  
  display()        += ccmpl::histo2d(histo2d_args, 
				     std::bind(fill_histogram, _1, std::ref(gen)),
				     X_MIN, X_MAX, NB_X_BINS,
				     Y_MIN, Y_MAX, NB_Y_BINS);  update_pattern += '#';
  display++;
  display().title   = "Histogram 3D";
  display()         = {X_MIN, X_MAX, Y_MIN, Y_MAX, Z_MIN, Z_MAX};    
  display()         = ccmpl::show_tics(false,false,true); 
  display()         = "equal";                                                  
  display()        += ccmpl::histo3d("color='#aaaaff', zsort='average', lw=.5", 
				     std::bind(fill_histogram, _1, std::ref(gen)),
				     X_MIN, X_MAX, NB_X_BINS,
				     Y_MIN, Y_MAX, NB_Y_BINS);  update_pattern += '#';

  if(generate_mode) {
    display.make_python(VIEW_FILE,false);  // no gui, we only generate a single pdf.
    return 0;                          
  }

  // Execution
  
  std::cout << display(update_pattern, "ccmpl-007.pdf", ccmpl::nofile())
	    << ccmpl::stop;
  return 0;	 
}
