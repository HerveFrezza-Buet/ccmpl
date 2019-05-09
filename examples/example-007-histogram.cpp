#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <random>

#include <ccmpl.hpp>

using namespace std::placeholders;

#define VIEW_PREFIX "viewer-007-histogram"

#define X_MAX           4
#define X_MIN     -(X_MAX)
#define NB_X_BINS      20
#define Y_MAX           4
#define Y_MIN     -(Y_MAX)
#define NB_Y_BINS      20
#define Z_MAX          20
#define Z_MIN           0  

#define NB_HISTO_SAMPLES 500

void fill_histogram_1d(std::vector<double>& data, std::mt19937& generator) {
  data.clear();

  std::normal_distribution<double> gaussian(0.0,1.0);
  for(unsigned int i = 0; i < NB_HISTO_SAMPLES; ++i)
    data.push_back(gaussian(generator));
}

void fill_histogram_2d(std::vector<ccmpl::Point>& data, std::mt19937& generator) {
  data.clear();

  std::normal_distribution<double> gaussian(0.0,1.0);
  for(unsigned int i = 0; i < NB_HISTO_SAMPLES; ++i)
    data.push_back({gaussian(generator),gaussian(generator)});
}


int main(int argc, char* argv[]) {
  // Let us use a predefined class ccmpl::Main in order to handle the
  // display in the main function.
  ccmpl::Main m(argc,argv,VIEW_PREFIX);
  
  std::random_device rd;
  std::mt19937       gen(rd());
  

  auto display = ccmpl::layout(m.hostname, m.port,
			       12.0, 12.0, {"##", ">."});

  std::string histo2d_args = std::string("cmap='jet', vmin=")
    + std::to_string(Z_MIN)
    + ", vmax="
    + std::to_string(Z_MAX);

  display().title   = "Histogram 2D";
  display()         = ccmpl::view2d({X_MIN, X_MAX}, {Y_MIN, Y_MAX}, ccmpl::aspect::equal, ccmpl::span::placeholder);
  display()         = ccmpl::show_tics(true,true);                           
  display()        += ccmpl::histo2d(histo2d_args, 
				     std::bind(fill_histogram_2d, _1, std::ref(gen)),
				     X_MIN, X_MAX, NB_X_BINS,
				     Y_MIN, Y_MAX, NB_Y_BINS); 
  display++;
  display().title   = "Histogram 3D";
  display()         = ccmpl::view3d({X_MIN, X_MAX}, {Y_MIN, Y_MAX}, ccmpl::limit::fit, ccmpl::aspect::equal, ccmpl::span::placeholder);
  display()         = ccmpl::show_tics(false,false,true); 
  display()        += ccmpl::histo3d("color='#aaaaff', zsort='average', lw=.5", 
				     std::bind(fill_histogram_2d, _1, std::ref(gen)),
				     X_MIN, X_MAX, NB_X_BINS,
				     Y_MIN, Y_MAX, NB_Y_BINS); 
  display++;
  display().title   = "Histogram 1D";
  display()         = ccmpl::view2d({X_MIN, X_MAX}, ccmpl::limit::fit, ccmpl::aspect::fit, ccmpl::span::placeholder);
  display()         = ccmpl::show_tics(true,true); 
  display()         = ccmpl::grid("linestyle='--', color='b', alpha=.1"); 
  display()        += ccmpl::histo1d("color=(1,.5,.6), linewidth=1, edgecolor='k'",
				     std::bind(fill_histogram_1d, _1, std::ref(gen)),
				     X_MIN, X_MAX, NB_X_BINS); 

  // the ccmpl::Main object handles generation here.
  m.generate(display, false); // no gui, we only generate a single pdf

  // Execution
  
  display("###", "ccmpl-007.pdf", ccmpl::nofile());
  !display;
  
  return 0;	 
}
