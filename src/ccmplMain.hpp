#pragma once

#include <string>

namespace ccmpl {
  class Main {
  private:
    bool generate_mode, movie;
    std::string pyfile, moviefile;

    Main(const std::string& prefix)
      : pyfile(prefix+".py"), moviefile(prefix+".mp4") {}

  public:

    Main           (            )  = delete;
    Main           (const Main& )  = delete;
    Main           (const Main&&)  = delete;
    Main& operator=(const Main& )  = delete;
    Main& operator=(const Main&&)  = delete;

    Main(int argc, char** argv, const std::string& prefix)
      : Main(prefix) {
      if(argc != 2) {
	std::cerr << std::endl
		  << "Usage : " << std::endl
		  << std::endl
		  << argv[0] << " movie" << std::endl
		  << argv[0] << " display" << std::endl
		  << "-----------------" << std::endl
		  << argv[0] << " run | python3 ./" << pyfile << std::endl
		  << std::endl;
	std::exit(0);
      }
      generate_mode = std::string(argv[1])=="movie" || std::string(argv[1])=="display";
      movie         = std::string(argv[1])=="movie";
    }

    Main(bool generate_mode, bool movie, const std::string& prefix)
      : Main(prefix) {
      this->generate_mode = generate_mode;
      this->movie         = movie;
    }

    void generate(ccmpl::chart::Layout& display, bool use_gui) {
      if(generate_mode) {
	if(movie)
	  display.make_movie_python(pyfile,use_gui, "avconv", "", "", "", 25, moviefile, 300);
	else
	  display.make_python(pyfile,use_gui); 
	std::exit(0);                            
      }    
    }

    bool movie_mode() const {
      return movie;
    }
  };
}
