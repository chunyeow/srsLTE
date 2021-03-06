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
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include <stdbool.h>

#include "srslte/srslte.h"

int cell_id = -1, offset = 0;
srslte_cp_t cp = SRSLTE_CP_NORM;
uint32_t nof_prb=6; 

#define FLEN  SRSLTE_SF_LEN(fft_size)

void usage(char *prog) {
  printf("Usage: %s [cpoev]\n", prog);
  printf("\t-c cell_id [Default check for all]\n");
  printf("\t-p nof_prb [Default %d]\n", nof_prb);
  printf("\t-o offset [Default %d]\n", offset);
  printf("\t-e extended CP [Default normal]\n");
  printf("\t-v srslte_verbose\n");
}

void parse_args(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "cpoev")) != -1) {
    switch (opt) {
    case 'c':
      cell_id = atoi(argv[optind]);
      break;
    case 'p':
      nof_prb = atoi(argv[optind]);
      break;
    case 'o':
      offset = atoi(argv[optind]);
      break;
    case 'e':
      cp = SRSLTE_CP_EXT;
      break;
    case 'v':
      srslte_verbose++;
      break;
    default:
      usage(argv[0]);
      exit(-1);
    }
  }
}

int main(int argc, char **argv) {
  int N_id_2, ns, find_ns;
  cf_t *buffer, *fft_buffer;
  cf_t pss_signal[SRSLTE_PSS_LEN];
  float sss_signal0[SRSLTE_SSS_LEN]; // for subframe 0
  float sss_signal5[SRSLTE_SSS_LEN]; // for subframe 5
  int cid, max_cid; 
  uint32_t find_idx;
  srslte_sync_t sync;
  srslte_ofdm_t ifft;
  int fft_size; 
  
  parse_args(argc, argv);

  fft_size = srslte_symbol_sz(nof_prb);
  if (fft_size < 0) {
    fprintf(stderr, "Invalid nof_prb=%d\n", nof_prb);
    exit(-1);
  }

  buffer = malloc(sizeof(cf_t) * FLEN);
  if (!buffer) {
    perror("malloc");
    exit(-1);
  }

  fft_buffer = malloc(sizeof(cf_t) * FLEN);
  if (!fft_buffer) {
    perror("malloc");
    exit(-1);
  }
  
  if (srslte_ofdm_tx_init(&ifft, cp, nof_prb)) {
    fprintf(stderr, "Error creating iFFT object\n");
    exit(-1);
  }

  if (srslte_sync_init(&sync, FLEN, fft_size)) {
    fprintf(stderr, "Error initiating PSS/SSS\n");
    return -1;
  }

  /* Set a very high threshold to make sure the correlation is ok */
  srslte_sync_set_threshold(&sync, 5.0);
  srslte_sync_set_sss_algorithm(&sync, SSS_PARTIAL_3);

  if (cell_id == -1) {
    cid = 0;
    max_cid = 49;
  } else {
    cid = cell_id;
    max_cid = cell_id;
}
  while(cid <= max_cid) {
    N_id_2 = cid%3;

    /* Generate PSS/SSS signals */
    srslte_pss_generate(pss_signal, N_id_2);
    srslte_sss_generate(sss_signal0, sss_signal5, cid);

    srslte_sync_set_N_id_2(&sync, N_id_2);
    
    for (ns=0;ns<2;ns++) {
      memset(buffer, 0, sizeof(cf_t) * FLEN);
      srslte_pss_put_slot(pss_signal, buffer, nof_prb, cp);
      srslte_sss_put_slot(ns?sss_signal5:sss_signal0, buffer, nof_prb, cp);

      /* Transform to OFDM symbols */
      memset(fft_buffer, 0, sizeof(cf_t) * FLEN);
      srslte_ofdm_tx_slot(&ifft, buffer, &fft_buffer[offset]);
      
      if (srslte_sync_find(&sync, fft_buffer, 0, &find_idx) < 0) {
        fprintf(stderr, "Error running srslte_sync_find\n");
        exit(-1);
      }
      find_ns = 2*srslte_sync_get_sf_idx(&sync);
      printf("cell_id: %d find: %d, offset: %d, ns=%d find_ns=%d\n", cid, find_idx, offset,
          ns, find_ns);
      if (find_idx != offset + FLEN/2) {
        printf("offset != find_offset: %d != %d\n", find_idx, offset + FLEN/2);
        exit(-1);
      }
      if (ns*10 != find_ns) {
        printf("ns != find_ns\n", 10 * ns, find_ns);
        exit(-1);
      }
      if (srslte_sync_get_cp(&sync) != cp) {
        printf("Detected CP should be %s\n", SRSLTE_CP_ISNORM(cp)?"Normal":"Extended");
        exit(-1);
      }
    }
    cid++;
  }

  free(fft_buffer);
  free(buffer);

  srslte_sync_free(&sync);
  srslte_ofdm_tx_free(&ifft);

  printf("Ok\n");
  exit(0);
}

