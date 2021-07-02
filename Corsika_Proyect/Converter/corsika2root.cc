#include <crs2r/MCorsikaReader.h>
using namespace crs2r;

#include <string>
#include <iostream>

void help ();

int main (int argc, char **argv) {
 
  if (argc<2) {
    help ();
    return 1;
  }
    
  std::string fname (argv [1]);
    
  MCorsikaReader Reader (fname);
  Reader.Read ();
    
  return 0;
    
}


void help () {
    
  std::cout << " ******************************************************** "
	    << std::endl
	    << " C++ CORSIKA-data reading tool by Ralf Ulrich, Feb 5, 2005 "
	    << std::endl
	    << " Forschungzentrum Karlsruhe, ralf.ulrich@ik.fzk.de "
	    << std::endl
	    << std::endl
	    << "    use: ./corsika2root <corsika-data-filename> "
	    << std::endl
	    << "    output: <corsika-data-filename>.root"
	    << std::endl
	    << " the reader will read any CORSIKA output and auto-detect"
	    << std::endl
	    << " thinning/non-thinning."
	    << std::endl
	    << " ******************************************************** "
	    << std::endl;
}
