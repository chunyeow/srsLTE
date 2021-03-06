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
 *  File:         softbuffer.h
 *
 *  Description:  Buffer for RX and TX soft bits. This should be provided by MAC. 
 *                Provided here basically for the examples. 
 *
 *  Reference:
 *****************************************************************************/

#ifndef SOFTBUFFER_
#define SOFTBUFFER_

#include "srslte/config.h"
#include "srslte/common/phy_common.h"

typedef struct SRSLTE_API {
  uint32_t max_cb;
  uint32_t buff_size;
  float **buffer_f;  
} srslte_softbuffer_rx_t;

typedef struct SRSLTE_API {
  uint32_t max_cb;
  uint32_t buff_size;
  uint8_t **buffer_b;  
} srslte_softbuffer_tx_t;

SRSLTE_API int  srslte_softbuffer_rx_init(srslte_softbuffer_rx_t * q,
                                          srslte_cell_t cell);

SRSLTE_API void srslte_softbuffer_rx_reset(srslte_softbuffer_rx_t *p); 

SRSLTE_API void srslte_softbuffer_rx_free(srslte_softbuffer_rx_t *p);

SRSLTE_API int  srslte_softbuffer_tx_init(srslte_softbuffer_tx_t * q,
                                          srslte_cell_t cell);

SRSLTE_API void srslte_softbuffer_tx_reset(srslte_softbuffer_tx_t *p); 

SRSLTE_API void srslte_softbuffer_tx_free(srslte_softbuffer_tx_t *p);



#endif
