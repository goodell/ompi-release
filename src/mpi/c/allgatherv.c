/*
 * $HEADER$
 */

#include "ompi_config.h"
#include <stdio.h>

#include "mpi.h"
#include "mpi/c/bindings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Allgatherv = PMPI_Allgatherv
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

int MPI_Allgatherv(void *sendbuf, int sendcount, MPI_Datatype sendtype,
		           void *recvbuf, int *recvcounts,
				   int *displs, MPI_Datatype recvtype, MPI_Comm comm) {

	return MPI_SUCCESS;
}

