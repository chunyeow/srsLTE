/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2014 The srsLTE Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution.
 *
 * \section LICENSE
 *
 * This file is part of the srsLTE library.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

/******************************************************************************
 *  File:         phich.h
 *
 *  Description:  Physical Hybrid ARQ indicator channel.
 *
 *  Reference:    3GPP TS 36.211 version 10.0.0 Release 10 Sec. 6.9
 *****************************************************************************/

#ifndef PHICH_
#define PHICH_

#include "srslte/config.h"
#include "srslte/common/phy_common.h"
#include "srslte/mimo/precoding.h"
#include "srslte/mimo/layermap.h"
#include "srslte/modem/mod.h"
#include "srslte/modem/demod_soft.h"
#include "srslte/scrambling/scrambling.h"
#include "regs.h"



#define SRSLTE_PHICH_NORM_NSEQUENCES  8
#define SRSLTE_PHICH_EXT_NSEQUENCES   4
#define SRSLTE_PHICH_NBITS                   3

#define SRSLTE_PHICH_NORM_MSYMB     SRSLTE_PHICH_NBITS * 4
#define SRSLTE_PHICH_EXT_MSYMB      SRSLTE_PHICH_NBITS * 2
#define SRSLTE_PHICH_MAX_NSYMB      SRSLTE_PHICH_NORM_MSYMB
#define SRSLTE_PHICH_NORM_C         1
#define SRSLTE_PHICH_EXT_C          2
#define SRSLTE_PHICH_NORM_NSF       4
#define SRSLTE_PHICH_EXT_NSF        2

/* phich object */
typedef struct SRSLTE_API {
  srslte_cell_t cell;
  
  /* handler to REGs resource mapper */
  srslte_regs_t *regs;

  /* buffers */
  cf_t ce[SRSLTE_MAX_PORTS][SRSLTE_PHICH_MAX_NSYMB];
  cf_t symbols[SRSLTE_MAX_PORTS][SRSLTE_PHICH_MAX_NSYMB];
  cf_t x[SRSLTE_MAX_PORTS][SRSLTE_PHICH_MAX_NSYMB];
  cf_t d[SRSLTE_PHICH_MAX_NSYMB];
  cf_t d0[SRSLTE_PHICH_MAX_NSYMB];
  cf_t z[SRSLTE_PHICH_NBITS];

  /* bit message */
  uint8_t data[SRSLTE_PHICH_NBITS];
  float data_rx[SRSLTE_PHICH_NBITS];

  /* tx & rx objects */
  srslte_modem_table_t mod;
  srslte_demod_soft_t demod;
  srslte_sequence_t seq[SRSLTE_NSUBFRAMES_X_FRAME];
  srslte_precoding_t precoding; 

} srslte_phich_t;

SRSLTE_API int srslte_phich_init(srslte_phich_t *q, 
                                 srslte_regs_t *regs, 
                                 srslte_cell_t cell);

SRSLTE_API void srslte_phich_free(srslte_phich_t *q);

SRSLTE_API int srslte_phich_decode(srslte_phich_t *q, 
                                   cf_t *slot_symbols, 
                                   cf_t *ce[SRSLTE_MAX_PORTS],
                                   float noise_estimate, 
                                   uint32_t ngroup, 
                                   uint32_t nseq, 
                                   uint32_t nsubframe, 
                                   uint8_t *ack, 
                                   float *distance);

SRSLTE_API int srslte_phich_encode(srslte_phich_t *q, 
                                   uint8_t ack, 
                                   uint32_t ngroup, 
                                   uint32_t nseq, 
                                   uint32_t nsubframe,
                                   cf_t *slot_symbols[SRSLTE_MAX_PORTS]);

SRSLTE_API void srslte_phich_reset(srslte_phich_t *q, 
                                   cf_t *slot_symbols[SRSLTE_MAX_PORTS]);

SRSLTE_API uint32_t srslte_phich_ngroups(srslte_phich_t *q);

SRSLTE_API uint32_t srslte_phich_nsf(srslte_phich_t *q);

#endif // PHICH_
