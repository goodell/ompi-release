// 
// $HEADER$
//	Function:	- wrapper for C program compilation
//

#include "ompi_config.h"
#include "tools/wrappers/ompi_wrap.h"


int
main(int argc, char *argv[])
{
  // The four wrapper compilers are extremely similar.  So similar,
  // in fact, that they can be parameterized on what is different.
  // Hence, we call the "wrapper compiler engine" to do all the work,
  // and pass in just a few arguments to customize for the language of
  // this wrapper compiler.

  ompi_sv_t str_vec;

  str_vec.clear();
  str_vec.push_back("OMPI_MPICC");
  str_vec.push_back("OMPI_CC");

  return ompi_wrap_engine(argc, argv,
			 str_vec, OMPI_CC, false, false, 
			 WRAPPER_EXTRA_CFLAGS);
}
