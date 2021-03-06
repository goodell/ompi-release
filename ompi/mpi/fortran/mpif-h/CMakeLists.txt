# Copyright (c) 2008-2010 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2012 Cisco Systems, Inc.  All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

FILE(GLOB OMPI_FORTRAN_FILES "*.c")
FILE(GLOB OMPI_FORTRAN_HEADER_FILES "*.h")

# Remove all MPI_File related files if the option is not selected
IF (NOT OMPI_PROVIDE_MPI_FILE_INTERFACE)
  MESSAGE( STATUS "Skipping the MPI I/O interface")
  SET( TMP_SRC "" )
  FOREACH ( FILENAME ${OMPI_FORTRAN_FILES})
    GET_FILENAME_COMPONENT(relname ${FILENAME} NAME)
    IF (NOT ${relname} MATCHES "file.*[ch]$")
      IF (NOT ${relname} STREQUAL "register_datarep.c")
        LIST(APPEND TMP_SRC ${FILENAME})
      ENDIF (NOT ${relname} STREQUAL "register_datarep.c")
    ENDIF(NOT ${relname} MATCHES "file.*[ch]$") ENDFOREACH(FILENAME ${OMPI_FORTRAN_FILES})
  SET( OMPI_FORTRAN_FILES ${TMP_SRC})
ENDIF(NOT OMPI_PROVIDE_MPI_FILE_INTERFACE)

SET_SOURCE_FILES_PROPERTIES(${OMPI_FORTRAN_FILES} PROPERTIES
  COMPILE_FLAGS "${OMPI_C_DEF_PRE}OMPI_PROFILE_LAYER=0 ${OMPI_C_DEF_PRE}OMPI_COMPILING_FORTRAN_WRAPPERS=1")

IF(OMPI_ENABLE_MPI_PROFILING)
  # As weak symbols are not supported by MS compiler,
  # we have to compile the C source files again for profiler,
  # i.e. add the pre-processor "OMPI_PROFILING_DEFINES" explicitly.
  
  # copy the files to the build directory with a prefix.
  FOREACH(FILE_NAME ${OMPI_FORTRAN_FILES})
    GET_FILENAME_COMPONENT(relname ${FILE_NAME} NAME)
    CONFIGURE_FILE(${FILE_NAME}
                   ${PROJECT_BINARY_DIR}/profile/p${relname} [COPYONLY])
  ENDFOREACH(FILE_NAME ${OMPI_FORTRAN_FILES})

  FILE(GLOB OMPI_FORTRAN_PROFILE_FILES "${PROJECT_BINARY_DIR}/profile/*.c" )

  SET_SOURCE_FILES_PROPERTIES(${OMPI_FORTRAN_PROFILE_FILES} PROPERTIES 
    COMPILE_FLAGS "${OMPI_C_DEF_PRE}OMPI_PROFILE_LAYER=1 ${OMPI_C_DEF_PRE}OMPI_COMPILING_FORTRAN_WRAPPERS=1")

  SET(OMPI_FORTRAN_FILES ${OMPI_FORTRAN_FILES} ${OMPI_FORTRAN_PROFILE_FILES})
  SOURCE_GROUP(profile FILES  ${OMPI_FORTRAN_PROFILE_FILES})

ELSE(OMPI_ENABLE_MPI_PROFILING)
  MESSAGE( STATUS "Skipping the MPI profiling interface")
ENDIF(OMPI_ENABLE_MPI_PROFILING)


ADD_LIBRARY(libmpi_fortran ${OMPI_FORTRAN_FILES} ${OMPI_FORTRAN_HEADER_FILES})
TARGET_LINK_LIBRARIES(libmpi_mpifh libmpi)

IF(BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(${OMPI_C_DEF_PRE}_USRDLL ${OMPI_C_DEF_PRE}OPAL_IMPORTS
    ${OMPI_C_DEF_PRE}OMPI_IMPORTS ${OMPI_CXX_LAN_FLAG})
ELSE(BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(${OMPI_C_DEF_PRE}_LIB)
ENDIF(BUILD_SHARED_LIBS)

INSTALL(TARGETS libmpi_mpifh
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

IF (OMPI_DEBUG_BUILD AND WINDOWS_VS)
  INSTALL(FILES ${OpenMPI_BINARY_DIR}/Debug/libmpi_mpifh{CMAKE_DEBUG_POSTFIX}.pdb
    DESTINATION bin)
ENDIF (OMPI_DEBUG_BUILD AND WINDOWS_VS)
