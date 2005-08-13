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

#include <malloc.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#define __USE_GNU
#include <dlfcn.h>

#include "opal/mca/memory/memory.h"
#include "opal/memory/memory_internal.h"

const opal_memory_base_component_1_0_0_t mca_memory_malloc_hooks_component = {
    /* First, the mca_component_t struct containing meta information
       about the component itself */
    {
        /* Indicate that we are a memory v1.0.0 component (which also
           implies a specific MCA version) */
        OPAL_MEMORY_BASE_VERSION_1_0_0,

        /* Component name and version */
        "malloc_hooks",
        OPAL_MAJOR_VERSION,
        OPAL_MINOR_VERSION,
        OPAL_RELEASE_VERSION,

        /* Component open and close functions */
        NULL,
        NULL
    },

    /* Next the MCA v1.0.0 component meta data */
    {
        /* Whether the component is checkpointable or not */
        true
    },
};

     
/* Prototypes for our hooks.  */
static void opal_mem_free_init_hook (void);
static void opal_mem_free_free_hook (void*, const void *);
static void* opal_mem_free_realloc_hook (void*, size_t, const void *);
     
/* Override initializing hook from the C library. */
void (*__malloc_initialize_hook) (void) = opal_mem_free_init_hook;


/* local variable - next in stack of free hooks */
static void (*old_free_hook)(void*, const void*);
static void* (*old_realloc_hook)(void*, size_t, const void*);


static void
opal_mem_free_init_hook (void)
{
    opal_mem_free_set_free_support(1);
    old_free_hook = __free_hook;
    old_realloc_hook = __realloc_hook;
    __free_hook = opal_mem_free_free_hook;
    __realloc_hook = opal_mem_free_realloc_hook;
}


static void
opal_mem_free_free_hook (void *ptr, const void *caller)
{
    /* dispatch about the pending free */
    opal_mem_free_release_hook(ptr, malloc_usable_size(ptr));

    __free_hook = old_free_hook;

    /* call the next chain down */
    free(ptr);

    /* save the hooks again and restore our hook again */
    old_free_hook = __free_hook;
    __free_hook = opal_mem_free_free_hook;
}


/* for better or worse, we must assume that the buffer being passed to
   realloc is not going to be expandable and therefore is going to be
   free()ed. */
static void*
opal_mem_free_realloc_hook (void *ptr, size_t size, const void *caller)
{
    void *ret;

    /* dispatch about the pending free */
    opal_mem_free_release_hook(ptr, malloc_usable_size(ptr));

    __realloc_hook = old_realloc_hook;

    /* call the next chain down */
    ret = realloc(ptr, size);

    /* save the hooks again and restore our hook again */
    old_realloc_hook = __realloc_hook;
    __realloc_hook = opal_mem_free_realloc_hook;

    return ret;
}


/* munmap is a weak symbol on any platform that I know of that
   supports malloc hooks, so we can just intercept it like this... */
int 
munmap(void* addr, size_t len)
{
    static int (*realmunmap)(void*, size_t);
    /* dispatch about the pending release */
    opal_mem_free_release_hook(addr, len);

    if (NULL == realmunmap) {
        realmunmap = dlsym(RTLD_NEXT, "munmap");
    }

    return realmunmap(addr, len);
}


