/*
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"
#include "group/group.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_GROUP_SIZE = mpi_group_size_f
#pragma weak pmpi_group_size = mpi_group_size_f
#pragma weak pmpi_group_size_ = mpi_group_size_f
#pragma weak pmpi_group_size__ = mpi_group_size_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_GROUP_SIZE,
                           pmpi_group_size,
                           pmpi_group_size_,
                           pmpi_group_size__,
                           pmpi_group_size_f,
                           (MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr),
                           (group, size, ierr) )
#endif

#if OMPI_HAVE_WEAK_SYMBOLS
#pragma weak MPI_GROUP_SIZE = mpi_group_size_f
#pragma weak mpi_group_size = mpi_group_size_f
#pragma weak mpi_group_size_ = mpi_group_size_f
#pragma weak mpi_group_size__ = mpi_group_size_f
#endif

#if ! OMPI_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_GROUP_SIZE,
                           mpi_group_size,
                           mpi_group_size_,
                           mpi_group_size__,
                           mpi_group_size_f,
                           (MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr),
                           (group, size, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OMPI_HAVE_WEAK_SYMBOLS
#include "mpi/c/profile/defines.h"
#endif

void mpi_group_size_f(MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr)
{
  ompi_group_t *c_group;

  /* Make the fortran to c representation conversion */
  c_group = MPI_Group_f2c(*group);
  
  *ierr = MPI_Group_size(c_group, size); 
}
