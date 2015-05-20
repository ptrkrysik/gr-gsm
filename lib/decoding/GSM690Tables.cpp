/* AMR (GSM 06.90) importance bit ordering */

/*
 * Copyright 2010  Sylvain Munaut <tnt@246tNt.com>
 * All Rights Reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GSM690Tables.h"

unsigned int GSM::g690_12_2_BitOrder[244] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
	 10,  11,  12,  13,  14,  23,  15,  16,  17,  18,
	 19,  20,  21,  22,  24,  25,  26,  27,  28,  38,
	141,  39, 142,  40, 143,  41, 144,  42, 145,  43,
	146,  44, 147,  45, 148,  46, 149,  47,  97, 150,
	200,  48,  98, 151, 201,  49,  99, 152, 202,  86,
	136, 189, 239,  87, 137, 190, 240,  88, 138, 191,
	241,  91, 194,  92, 195,  93, 196,  94, 197,  95,
	198,  29,  30,  31,  32,  33,  34,  35,  50, 100,
	153, 203,  89, 139, 192, 242,  51, 101, 154, 204,
	 55, 105, 158, 208,  90, 140, 193, 243,  59, 109,
	162, 212,  63, 113, 166, 216,  67, 117, 170, 220,
	 36,  37,  54,  53,  52,  58,  57,  56,  62,  61,
	 60,  66,  65,  64,  70,  69,  68, 104, 103, 102,
	108, 107, 106, 112, 111, 110, 116, 115, 114, 120,
	119, 118, 157, 156, 155, 161, 160, 159, 165, 164,
	163, 169, 168, 167, 173, 172, 171, 207, 206, 205,
	211, 210, 209, 215, 214, 213, 219, 218, 217, 223,
	222, 221,  73,  72,  71,  76,  75,  74,  79,  78,
	 77,  82,  81,  80,  85,  84,  83, 123, 122, 121,
	126, 125, 124, 129, 128, 127, 132, 131, 130, 135,
	134, 133, 176, 175, 174, 179, 178, 177, 182, 181,
	180, 185, 184, 183, 188, 187, 186, 226, 225, 224,
	229, 228, 227, 232, 231, 230, 235, 234, 233, 238,
	237, 236,  96, 199,
};
