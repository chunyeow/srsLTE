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
 *  File:         cfo.h
 *
 *  Description:  Carrier frequency offset correction using complex exponentials.
 *
 *  Reference:
 *****************************************************************************/

#ifndef CFO_
#define CFO_

#include <complex.h>

#include "srslte/config.h"
#include "srslte/utils/cexptab.h"

/** If the frequency is changed more than the tolerance, a new table is generated */
#define SRSLTE_CFO_TOLERANCE    0.00001

#define SRSLTE_CFO_CEXPTAB_SIZE 4096

typedef struct SRSLTE_API {
  float last_freq;
  float tol;
  int nsamples;
  srslte_cexptab_t tab;
  cf_t *cur_cexp;
}srslte_cfo_t;

SRSLTE_API int srslte_cfo_init(srslte_cfo_t *h, 
                               uint32_t nsamples);

SRSLTE_API void srslte_cfo_free(srslte_cfo_t *h);

SRSLTE_API int srslte_cfo_realloc(srslte_cfo_t *h, 
                                  uint32_t samples);

SRSLTE_API void srslte_cfo_set_tol(srslte_cfo_t *h, 
                                   float tol);

SRSLTE_API void srslte_cfo_correct(srslte_cfo_t *h, 
                                   cf_t *input,
                                   cf_t *output,
                                   float freq);

#endif // CFO_
