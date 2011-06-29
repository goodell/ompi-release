/*
 This is part of the OTF library. Copyright by ZIH, TU Dresden 2005-2011.
 Authors: Andreas Knuepfer, Robert Dietrich, Matthias Jurenz
*/

#ifndef SUMMARIZE_DATA_H
#define SUMMARIZE_DATA_H

#include "datastructs.h"


/* summarize the data for all trace processes on the current worker */
bool summarizeData( uint32_t my_rank, uint32_t num_ranks, AllData& alldata );


#endif /* SUMMARIZE_DATA_H */
