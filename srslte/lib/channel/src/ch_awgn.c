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


#include <complex.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#include "gauss.h"
#include "srslte/channel/ch_awgn.h"

float srslte_ch_awgn_get_variance(float ebno_db, float rate) {
  float esno_db = ebno_db + 10 * log10f(rate);
  return sqrtf(1 / (powf(10, esno_db / 10)));
}

void srslte_ch_awgn_c(const cf_t* x, cf_t* y, float variance, uint32_t len) {
  cf_t tmp;
  uint32_t i;

  for (i=0;i<len;i++) {
    __real__ tmp = rand_gauss();
    __imag__ tmp = rand_gauss();
    tmp *= variance;
    y[i] = tmp + x[i];
  }
}
void srslte_ch_awgn_f(const float* x, float* y, float variance, uint32_t len) {
  uint32_t i;

  for (i=0;i<len;i++) {
    y[i] = x[i] + variance * rand_gauss();
  }
}

/* High-level API */
int srslte_ch_awgn_initialize(srslte_ch_awgn_hl* hl) {

  return 0;
}

int srslte_ch_awgn_work(srslte_ch_awgn_hl* hl) {
  srslte_ch_awgn_c(hl->input,hl->output,hl->ctrl_in.variance,hl->in_len);
  hl->out_len = hl->in_len;
  return 0;
}

int srslte_ch_awgn_stop(srslte_ch_awgn_hl* hl) {
  return 0;
}
