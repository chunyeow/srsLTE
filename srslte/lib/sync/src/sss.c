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


#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "srslte/sync/sss.h"
#include "srslte/dft/dft.h"
#include "srslte/utils/convolution.h"
#include "srslte/utils/vector.h"

void generate_sss_all_tables(srslte_sss_tables_t *tables, uint32_t N_id_2);
void convert_tables(srslte_sss_fc_tables_t *fc_tables, srslte_sss_tables_t *in);
void generate_N_id_1_table(uint32_t table[30][30]);

int srslte_sss_synch_init(srslte_sss_synch_t *q, uint32_t fft_size) {
  
  if (q                 != NULL  &&
      fft_size          <= 2048)
  {
    uint32_t N_id_2;
    srslte_sss_tables_t sss_tables;

    bzero(q, sizeof(srslte_sss_synch_t));
    
    if (srslte_dft_plan(&q->dftp_input, fft_size, SRSLTE_DFT_FORWARD, SRSLTE_DFT_COMPLEX)) {
      srslte_sss_synch_free(q);
      return SRSLTE_ERROR;
    }
    srslte_dft_plan_set_mirror(&q->dftp_input, true);
    srslte_dft_plan_set_dc(&q->dftp_input, true);

    q->fft_size = fft_size; 
    
    generate_N_id_1_table(q->N_id_1_table);
    
    for (N_id_2=0;N_id_2<3;N_id_2++) {
      generate_sss_all_tables(&sss_tables, N_id_2);
      convert_tables(&q->fc_tables[N_id_2], &sss_tables);
    }
    q->N_id_2 = 0;
    return SRSLTE_SUCCESS;
  } 
  return SRSLTE_ERROR_INVALID_INPUTS;
}

int srslte_sss_synch_realloc(srslte_sss_synch_t *q, uint32_t fft_size) {
  if (q                 != NULL  &&
      fft_size          <= 2048)
  {
    srslte_dft_plan_free(&q->dftp_input);
    if (srslte_dft_plan(&q->dftp_input, fft_size, SRSLTE_DFT_FORWARD, SRSLTE_DFT_COMPLEX)) {
      srslte_sss_synch_free(q);
      return SRSLTE_ERROR;
    }
    srslte_dft_plan_set_mirror(&q->dftp_input, true);
    srslte_dft_plan_set_norm(&q->dftp_input, true);
    srslte_dft_plan_set_dc(&q->dftp_input, true);
    
    q->fft_size = fft_size;
    return SRSLTE_SUCCESS;
  }
  return SRSLTE_ERROR_INVALID_INPUTS;
}

void srslte_sss_synch_free(srslte_sss_synch_t *q) {
  srslte_dft_plan_free(&q->dftp_input);
  bzero(q, sizeof(srslte_sss_synch_t));
}

/** Sets the N_id_2 to search for */
int srslte_sss_synch_set_N_id_2(srslte_sss_synch_t *q, uint32_t N_id_2) {
  if (!srslte_N_id_2_isvalid(N_id_2)) {
    fprintf(stderr, "Invalid N_id_2 %d\n", N_id_2);
    return SRSLTE_ERROR;
  } else {
    q->N_id_2 = N_id_2;
    return SRSLTE_SUCCESS;
  }
}

/** 36.211 10.3 section 6.11.2.2
 */
void srslte_sss_put_slot(float *sss, cf_t *slot, uint32_t nof_prb, srslte_cp_t cp) {
  uint32_t i, k;

  k = (SRSLTE_CP_NSYMB(cp) - 2) * nof_prb * SRSLTE_NRE + nof_prb * SRSLTE_NRE / 2 - 31;
  
  if (k > 5) {
    memset(&slot[k - 5], 0, 5 * sizeof(cf_t));
    for (i = 0; i < SRSLTE_SSS_LEN; i++) {
      __real__ slot[k + i] = sss[i];
      __imag__ slot[k + i] = 0;
    }
    memset(&slot[k + SRSLTE_SSS_LEN], 0, 5 * sizeof(cf_t));
  }
}

/** Sets the SSS correlation peak detection threshold */
void srslte_sss_synch_set_threshold(srslte_sss_synch_t *q, float threshold) {
  q->corr_peak_threshold = threshold;
}

/** Returns the subframe index based on the m0 and m1 values */
uint32_t srslte_sss_synch_subframe(uint32_t m0, uint32_t m1) {
  if (m1 > m0) {
    return 0;
  } else {
    return 5;
  }
}

/** Returns the N_id_1 value based on the m0 and m1 values */
int srslte_sss_synch_N_id_1(srslte_sss_synch_t *q, uint32_t m0, uint32_t m1) {
  if (m0==m1 || m0 > 29 || m1 > 29) {
    return SRSLTE_ERROR;
  }
  int N_id_1; 
  if (m1 > m0) {
    N_id_1 = q->N_id_1_table[m0][m1 - 1];
  } else {
    N_id_1 = q->N_id_1_table[m1][m0 - 1];
  } 
  return N_id_1;
}

/** High-level API */

int srslte_sss_synch_initialize(srslte_sss_synch_hl* h) {

  if (srslte_sss_synch_init(&h->obj, 128)) {
    return SRSLTE_ERROR;
  }
  srslte_sss_synch_set_N_id_2(&h->obj, h->init.N_id_2);

  return SRSLTE_SUCCESS;
}

int srslte_sss_synch_work(srslte_sss_synch_hl* hl) {

  if (hl->ctrl_in.correlation_threshold) {
    srslte_sss_synch_set_threshold(&hl->obj, hl->ctrl_in.correlation_threshold);
  }
 
  return SRSLTE_SUCCESS;
}

int srslte_sss_synch_stop(srslte_sss_synch_hl* hl) {
  srslte_sss_synch_free(&hl->obj);
  return SRSLTE_SUCCESS;
}

