/*
 * Copyright (c) 2009      Cisco Systems, Inc.  All rights reserved. 
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */
/** @file:
 */

#ifndef MCA_FDDP_BASE_H
#define MCA_FDDP_BASE_H

/*
 * includes
 */
#include "orte_config.h"

#include "opal/class/opal_list.h"
#include "opal/mca/mca.h"

#include "orte/mca/fddp/fddp.h"


/*
 * Global functions for MCA overall collective open and close
 */
BEGIN_C_DECLS

/*
 * function definitions
 */
ORTE_DECLSPEC    int orte_fddp_base_open(void);
ORTE_DECLSPEC    int orte_fddp_base_select(void);
ORTE_DECLSPEC    int orte_fddp_base_close(void);

/*
 * globals that might be needed
 */

ORTE_DECLSPEC extern int orte_fddp_base_output;
ORTE_DECLSPEC extern bool mca_fddp_base_selected;
ORTE_DECLSPEC extern opal_list_t mca_fddp_base_components_available;
ORTE_DECLSPEC extern orte_fddp_base_component_t mca_fddp_base_selected_component;

#if !ORTE_DISABLE_FULL_SUPPORT

/* no base functions to protect at this time */

#endif /* ORTE_DISABLE_FULL_SUPPORT */

END_C_DECLS
#endif
