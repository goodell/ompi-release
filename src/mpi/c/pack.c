/*
 * $HEADERS$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "mpi.h"
#include "mpi/c/bindings.h"
#include "datatype/datatype.h"
#include "errhandler/errhandler.h"
#include "communicator/communicator.h"
#include "class/ompi_object.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Pack = PMPI_Pack
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

/* VPS: Just for now, to be removed later */
extern ompi_convertor_t *ompi_convertor;

static const char FUNC_NAME[] = "MPI_Pack";


int MPI_Pack(void *inbuf, int incount, MPI_Datatype datatype,
             void *outbuf, int outsize, int *position, MPI_Comm comm)
{
    int size, rc;
    ompi_convertor_t *local_convertor;
    struct iovec invec;

    if (MPI_PARAM_CHECK) {
      if (MPI_COMM_NULL == comm) {
	return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_COMM, FUNC_NAME);
      }

      if ((NULL == inbuf) || (NULL == outbuf) || (NULL == position)) {
	return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_ARG, FUNC_NAME);
      }
#if 0
      if (count < 0) {
	return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_COUNT, FUNC_NAME);
      }
#endif

      if (outsize < 0) {
	return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_ARG, FUNC_NAME);
      }
    
      if (MPI_DATATYPE_NULL == datatype) {
	return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_TYPE, FUNC_NAME);
      }
    }

    local_convertor = ompi_convertor_get_copy(ompi_convertor);
    ompi_convertor_init_for_send(local_convertor, 0, datatype, incount, 
				inbuf, 0);
    
    /* how long is the data ? Can we put it in the user buffer */
    ompi_convertor_get_packed_size(local_convertor, &size);
    if( (outsize - (*position)) < size) {
      return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_ARG, FUNC_NAME);
    }

    /* Prepare the iovec withh all informations */
    invec.iov_base = (char*)outbuf + (*position);

    /* If the position is not ZERO we already start
     * the packing for this datatype.
     */
    invec.iov_len = outsize - (*position);

    /* Do the actual packing */
    rc = ompi_convertor_pack(local_convertor, &invec, 1);
    *position += local_convertor->bConverted;
    
    /* Release the convertor. For Open MPI you should simply use
     * OBJ_RELEASE.
     */
    OBJ_RELEASE(local_convertor);

    OMPI_ERRHANDLER_RETURN(rc, comm, MPI_ERR_UNKNOWN, FUNC_NAME);
}
