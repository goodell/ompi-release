/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
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

/** 
 * @file 
 *
 * Checkpoint/Restart Functionality for the ORTE layer
 */

#ifndef ORTE_CR_H
#define ORTE_CR_H

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

    /*
     * Initialization called in orte_init()
     */
    ORTE_DECLSPEC int orte_cr_init(void);

    /*
     * Finalization called in orte_finalize()
     */
    ORTE_DECLSPEC int orte_cr_finalize(void);

    /*
     * Interlayer Coodination Callback
     */
    ORTE_DECLSPEC int orte_cr_coord(int state);

    /*
     * Init/Finalize functions for ORTE Entry Point
     */
    ORTE_DECLSPEC int orte_cr_entry_point_init(void);
    ORTE_DECLSPEC int orte_cr_entry_point_finalize(void);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif /* ORTE_CR_H */
