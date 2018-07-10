#include <iostream>
#include <vector>
#include <ccmpl.hpp>


#define VIEW_PREFIX "viewer-013-pie"

void fill_data(std::vector<ccmpl::Pie::Wedge>& wedges) {
  wedges.clear();
  auto out = std::back_inserter(wedges);

  *(out++) = {20.,  ccmpl::RGB(1, 0, 0), "One",     0};
  *(out++) = {20.,  ccmpl::RGB(1, 1, 0), "Two",     0};
  *(out++) = {15.5, ccmpl::RGB(0, 1, 0), "Three",   0};
  *(out++) = {5.5,  ccmpl::RGB(0, 1, 1), "Four",  0.1}; // wedge offset here
  *(out++) = {39.,  ccmpl::RGB(0, 0, 1), "Five",    0};
}


int main(int argc, char* argv[]) {

  // Let us use a predefined class ccmpl::Main in order to handle the
  // display in the main function.
  ccmpl::Main m(argc,argv,VIEW_PREFIX);

  
  // Let us define the layout, a 1x1 grid here. Args are width, height
  // and the grid structure.
  auto display = ccmpl::layout(8.0, 6.0, {"#"});
  display.set_ratios({1.}, {1.});

  display().title   = "Pie chart  ";  
  display()         = {-1., 1., -1., 1.};  
  display()         = "equal";
  display()        += ccmpl::pie("autopct='%1.1f\\%%', shadow=True, startangle=20", fill_data);
  display++;

  // the ccmpl::Main object handles generation here.
  m.generate(display, false); // no gui, we only generate a single pdf

  // Execution
  std::cout << display("#", "ccmpl-013.pdf", ccmpl::nofile())
	    << ccmpl::stop;
  
  return 0;	 
}
