/*
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdio.h>

#include "include/constants.h"
#include "mca/mca.h"
#include "mca/base/base.h"
#include "mca/pcm/pcm.h"


int mca_pcm_base_close(void)
{
  /* Close all remaining available modules (may be one if this is a
     OMPI RTE program, or [possibly] multiple if this is ompi_info) */

  mca_base_modules_close(mca_pcm_base_output, 
                         &mca_pcm_base_modules_available, NULL);

  /* All done */

  return OMPI_SUCCESS;
}
