#pragma once

#include <string>

namespace ccmpl {
  class Main {
  private:
    bool generate_mode, movie;
    std::string pyfile, moviefile;
    std::string movie_writer;
    
    Main(const std::string& prefix)
      : pyfile(prefix+".py"),
	moviefile(prefix+".mp4"),
	movie_writer("ffmpeg") {}

  public:
    
    std::string hostname;
    int port;

    Main           (            )  = delete;
    Main           (const Main& )  = delete;
    Main           (const Main&&)  = delete;
    Main& operator=(const Main& )  = delete;
    Main& operator=(const Main&&)  = delete;

    Main(int argc, char** argv, const std::string& prefix)
      : Main(prefix) {
      if(argc != 2 && argc != 3) {
	std::cerr << std::endl
		  << "Usage : " << std::endl
		  << std::endl
		  << argv[0] << " movie" << std::endl
		  << argv[0] << " display" << std::endl
		  << "-----------------" << std::endl
		  << "python3 ./" << pyfile << " <port>      <-- run on machine <hostname>" << std::endl
		  << argv[0] << " <hostname> <port>" << std::endl
		  << std::endl;
	std::exit(0);
      }
      generate_mode = (argc == 2) && (std::string(argv[1])=="movie" || std::string(argv[1])=="display");
      movie         = generate_mode && std::string(argv[1])=="movie";
      if(argc == 3) {
	hostname = argv[1];
	port     = std::stoi(argv[2]);
      }
    }

    Main(bool generate_mode, bool movie, const std::string& prefix)
      : Main(prefix) {
      this->generate_mode = generate_mode;
      this->movie         = movie;
    }

    // Change the movie writer used when generating the python file
    // This function must be called before "generate" to be effective
    void set_movie_writer(std::string writer) {
      movie_writer = writer;
    }
    
    void generate(ccmpl::chart::Layout& display, bool use_gui) {
      if(generate_mode) {
	if(movie)
	  display.make_movie_python(pyfile,use_gui, movie_writer, "", "", "", 25, moviefile, 300);
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
