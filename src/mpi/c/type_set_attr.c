/*
 * $HEADER$
 */

#include "ompi_config.h"

#include "mpi.h"
#include "mpi/c/bindings.h"
#include "attribute/attribute.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Type_set_attr = PMPI_Type_set_attr
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

static char FUNC_NAME[] = "MPI_Type_set_attr";

int
MPI_Type_set_attr (MPI_Datatype type,
                   int type_keyval,
                   void *attribute_val)
{
    int ret;

    if (MPI_PARAM_CHECK) {
	if (MPI_DATATYPE_NULL == type) {
	    return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD,
					 MPI_ERR_TYPE, 
					 FUNC_NAME);
	}
    }

    ret = ompi_attr_set(TYPE_ATTR, type, type->d_keyhash, 
		       type_keyval, attribute_val, 0);

    OMPI_ERRHANDLER_RETURN(ret, MPI_COMM_WORLD,
			  MPI_ERR_OTHER, FUNC_NAME);  

}
