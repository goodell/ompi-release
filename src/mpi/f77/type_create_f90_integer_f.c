/*
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_TYPE_CREATE_F90_INTEGER = mpi_type_create_f90_integer_f
#pragma weak pmpi_type_create_f90_integer = mpi_type_create_f90_integer_f
#pragma weak pmpi_type_create_f90_integer_ = mpi_type_create_f90_integer_f
#pragma weak pmpi_type_create_f90_integer__ = mpi_type_create_f90_integer_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_TYPE_CREATE_F90_INTEGER,
                           pmpi_type_create_f90_integer,
                           pmpi_type_create_f90_integer_,
                           pmpi_type_create_f90_integer__,
                           pmpi_type_create_f90_integer_f,
                           (MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr),
                           (r, newtype, ierr) )
#endif

#if OMPI_HAVE_WEAK_SYMBOLS
#pragma weak MPI_TYPE_CREATE_F90_INTEGER = mpi_type_create_f90_integer_f
#pragma weak mpi_type_create_f90_integer = mpi_type_create_f90_integer_f
#pragma weak mpi_type_create_f90_integer_ = mpi_type_create_f90_integer_f
#pragma weak mpi_type_create_f90_integer__ = mpi_type_create_f90_integer_f
#endif

#if ! OMPI_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_TYPE_CREATE_F90_INTEGER,
                           mpi_type_create_f90_integer,
                           mpi_type_create_f90_integer_,
                           mpi_type_create_f90_integer__,
                           mpi_type_create_f90_integer_f,
                           (MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr),
                           (r, newtype, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OMPI_HAVE_WEAK_SYMBOLS
#include "mpi/c/profile/defines.h"
#endif

void mpi_type_create_f90_integer_f(MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{

}
