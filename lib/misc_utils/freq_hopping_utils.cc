/* -*- c++ -*- */
/* @file
 * @author Piotr Krysik <ptrkrysik@gmail.com>
 * @section LICENSE
 * 
 * Gr-gsm is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * Gr-gsm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with gr-gsm; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 * 
 */

//#include "grgsm/misc_utils/freq_hopping_utils.h"
#include <cmath>

namespace gr {
  namespace gsm {
    unsigned char RNTABLE[114] = {
        48, 98, 63, 1, 36, 95, 78, 102, 94, 73, \
        0, 64, 25, 81, 76, 59, 124, 23, 104, 100, \
        101, 47, 118, 85, 18, 56, 96, 86, 54, 2, \
        80, 34, 127, 13, 6, 89, 57, 103, 12, 74, \
        55, 111, 75, 38, 109, 71, 112, 29, 11, 88, \
        87, 19, 3, 68, 110, 26, 33, 31, 8, 45, \
        82, 58, 40, 107, 32, 5, 106, 92, 62, 67, \
        77, 108, 122, 37, 60, 66, 121, 42, 51, 126, \
        117, 114, 4, 90, 43, 52, 53, 113, 120, 72, \
        16, 49, 7, 79, 119, 61, 22, 84, 9, 97, \
        91, 15, 21, 24, 46, 39, 93, 105, 65, 70, \
        125, 99, 17, 123 \
    };
    
    int calculate_ma_sfh(int maio, int hsn, int n, int fn)
    {
      int mai = 0;
      int s = 0;
      int nbin = floor(log2(n) + 1);
      int t1 = fn / 1326;
      int t2 = fn % 26;
      int t3 = fn % 51;

      if (hsn == 0){      
          mai = (fn + maio) % n;
      } else {
          int t1r = t1 % 64;
          int m = t2 + RNTABLE[(hsn ^ t1r) + t3];
          int mprim = m % (1 << nbin);
          int tprim = t3 % (1 << nbin);

          if (mprim < n){
              s = mprim;
          } else {
              s = (mprim + tprim) % n;
          }
          mai = (s + maio) % n;
      }
    }
  } // namespace gsm
} // namespace gr
