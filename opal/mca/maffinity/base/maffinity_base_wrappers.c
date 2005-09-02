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
#include "opal/mca/maffinity/maffinity.h"
#include "opal/mca/maffinity/base/base.h"
#include "opal/mca/maffinity/base/internal.h"


int opal_maffinity_base_set(opal_maffinity_base_segment_t *segments,
                            size_t num_segments)
{
    if (!opal_maffinity_base_selected) {
        return OPAL_ERR_NOT_FOUND;
    }
    return opal_maffinity_base_module->maff_module_set(segments, num_segments);
}
