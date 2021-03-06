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


#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#include "srslte/srslte.h"

void usage(char *arg) {
  printf("Usage: %s nbits snr_db\n",arg);
}

int main(int argc, char **argv) {
  srslte_binsource_hl bs;
  srslte_mod_hl mod;
  srslte_ch_awgn_hl ch;
  srslte_demod_soft_hl demod_s;
  srslte_demod_hard_hl demod_h;

  bzero(&bs,sizeof(bs));
  bzero(&mod,sizeof(mod));
  bzero(&ch,sizeof(ch));
  bzero(&demod_s,sizeof(demod_s));
  bzero(&demod_h,sizeof(demod_h));

  if (argc<3) {
    usage(argv[0]);
    exit(-1);
  }

  int nbits = atoi(argv[1]);
  float snr_db = atof(argv[2]);
  float var = sqrt(pow(10,-snr_db/10));

  bs.init.seed = 0;
  bs.init.cache_seq_nbits = 0;
  bs.ctrl_in.nbits = nbits;
  bs.output = malloc(nbits);

  mod.in_len = nbits;
  mod.init.std = SRSLTE_MOD_BPSK;
  mod.input = bs.output;
  mod.output = malloc(nbits*sizeof(_Complex float));

  ch.in_len = nbits;
  ch.input = mod.output;
  ch.ctrl_in.variance = var;
  ch.output = malloc(nbits*sizeof(_Complex float));

  demod_h.in_len = nbits;
  demod_h.init.std = SRSLTE_MOD_BPSK;
  demod_h.input = ch.output;
  demod_h.output = malloc(nbits);

  demod_s.in_len = nbits;
  demod_s.init.std = SRSLTE_MOD_BPSK;
  demod_s.input = ch.output;
  demod_s.output = malloc(sizeof(float)*nbits);
  demod_s.ctrl_in.alg_type = SRSLTE_DEMOD_SOFT_ALG_APPROX;
  demod_s.ctrl_in.sigma = var;

  if (  srslte_binsource_initialize(&bs)     ||
      mod_initialize(&mod)       ||
      srslte_ch_awgn_initialize(&ch)       ||
      srslte_demod_hard_initialize(&demod_h) ||
      srslte_demod_soft_initialize(&demod_s)
  ) {
    printf("Error initializing modules\n");
    exit(-1);
  }

  srslte_binsource_work(&bs);
  mod_work(&mod);
  srslte_ch_awgn_work(&ch);
  srslte_demod_hard_work(&demod_h);
  srslte_demod_soft_work(&demod_s);

  /* hard decision for soft demodulation */
  uint8_t* tmp = malloc(nbits);
  for (int i=0;i<nbits;i++) {
    tmp[i] = demod_s.output[i]>0?1:0;
  }

  printf("Hard errors: %u/%d\n",srslte_bit_diff(bs.output,demod_h.output,nbits),nbits);
  printf("Soft errors: %u/%d\n",srslte_bit_diff(bs.output,tmp,nbits),nbits);

  free(bs.output);
  free(mod.output);
  free(ch.output);
  free(demod_h.output);
  free(demod_s.output);
  free(tmp);

  printf("Exit\n");
  exit(0);
}
