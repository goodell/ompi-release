/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 The Trustees of the University of Tennessee.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "ompi_config.h"

#include "opal/include/constants.h"
#include "opal/mca/mca.h"
#include "opal/mca/base/base.h"
#include "opal/mca/paffinity/paffinity.h"
#include "opal/mca/paffinity/base/base.h"
#include "opal/mca/paffinity/base/internal.h"


int opal_paffinity_base_close(void)
{
    /* Close all components that are still open (this should only
       happen during ompi_info). */

    if (opal_paffinity_base_components_opened_valid) {
        mca_base_components_close(opal_paffinity_base_output,
                                  &opal_paffinity_base_components_opened, NULL);
        OBJ_DESTRUCT(&opal_paffinity_base_components_opened);
        opal_paffinity_base_components_opened_valid = false;
    }

    /* All done */

    return OPAL_SUCCESS;
}
