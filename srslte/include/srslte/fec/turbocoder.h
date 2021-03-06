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

/**********************************************************************************************
 *  File:         turbocoder.h
 *
 *  Description:  Turbo coder.
 *                Parallel Concatenated Convolutional Code (PCCC) with two 8-state constituent
 *                encoders and one turbo code internal interleaver. The coding rate of turbo
 *                encoder is 1/3.
 *
 *  Reference:    3GPP TS 36.212 version 10.0.0 Release 10 Sec. 5.1.3.2
 *********************************************************************************************/

#ifndef TURBOCODER_
#define TURBOCODER_

#include "srslte/config.h"
#include "srslte/fec/tc_interl.h"


#ifndef SRSLTE_TX_NULL
#define SRSLTE_TX_NULL 100
#endif

typedef struct SRSLTE_API {
  uint32_t max_long_cb;
  srslte_tc_interl_t interl;
} srslte_tcod_t;

SRSLTE_API int srslte_tcod_init(srslte_tcod_t *h, 
                                uint32_t max_long_cb);

SRSLTE_API void srslte_tcod_free(srslte_tcod_t *h);

SRSLTE_API int srslte_tcod_encode(srslte_tcod_t *h, 
                                  uint8_t *input, 
                                  uint8_t *output, 
                                  uint32_t long_cb);

#endif

