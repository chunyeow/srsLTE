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


#ifndef FORMAT_
#define FORMAT_

typedef enum { 
  SRSLTE_FLOAT, 
  SRSLTE_COMPLEX_FLOAT, 
  SRSLTE_COMPLEX_SHORT, 
  SRSLTE_FLOAT_BIN, 
  SRSLTE_COMPLEX_FLOAT_BIN, 
  SRSLTE_COMPLEX_SHORT_BIN  
} srslte_datatype_t;

#endif // FORMAT_
