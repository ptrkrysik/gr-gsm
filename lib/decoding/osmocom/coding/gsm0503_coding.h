/*
 * (C) 2013 by Andreas Eversberg <jolly@eversberg.eu>
 * (C) 2015 by Alexander Chemeris <Alexander.Chemeris@fairwaves.co>
 * (C) 2016 by Tom Tsou <tom.tsou@ettus.com>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once

#include <stdint.h>
#include <osmocom/core/bits.h>

#define GSM0503_GPRS_BURSTS_NBITS	(116 * 4)
#define GSM0503_EGPRS_BURSTS_NBITS	(348 * 4)
#define NUM_BYTES(N) ((N + 8 - 1) / 8)



int gsm0503_xcch_encode(ubit_t *bursts, uint8_t *l2_data);
int gsm0503_xcch_decode(uint8_t *l2_data, sbit_t *bursts,
	int *n_errors, int *n_bits_total);

int gsm0503_pdtch_encode(ubit_t *bursts, uint8_t *l2_data, uint8_t l2_len);
int gsm0503_pdtch_decode(uint8_t *l2_data, sbit_t *bursts, uint8_t *usf_p,
	int *n_errors, int *n_bits_total);

int gsm0503_tch_fr_encode(ubit_t *bursts, uint8_t *tch_data, int len,
	int net_order);
int gsm0503_tch_fr_decode(uint8_t *tch_data, sbit_t *bursts, int net_order,
	int efr, int *n_errors, int *n_bits_total);

int gsm0503_tch_hr_encode(ubit_t *bursts, uint8_t *tch_data, int len);
int gsm0503_tch_hr_decode(uint8_t *tch_data, sbit_t *bursts, int odd,
	int *n_errors, int *n_bits_total);

int gsm0503_tch_afs_encode(ubit_t *bursts, uint8_t *tch_data, int len,
	int codec_mode_req, uint8_t *codec, int codecs, uint8_t ft,
	uint8_t cmr);
int gsm0503_tch_afs_decode(uint8_t *tch_data, sbit_t *bursts,
	int codec_mode_req, uint8_t *codec, int codecs, uint8_t *ft,
	uint8_t *cmr, int *n_errors, int *n_bits_total);

int gsm0503_tch_ahs_encode(ubit_t *bursts, uint8_t *tch_data, int len,
	int codec_mode_req, uint8_t *codec, int codecs, uint8_t ft, uint8_t cmr);
int gsm0503_tch_ahs_decode(uint8_t *tch_data, sbit_t *bursts, int odd,
	int codec_mode_req, uint8_t *codec, int codecs, uint8_t *ft,
	uint8_t *cmr, int *n_errors, int *n_bits_total);

int gsm0503_rach_encode(ubit_t *burst, uint8_t *ra, uint8_t bsic);
int gsm0503_rach_decode(uint8_t *ra, sbit_t *burst, uint8_t bsic);

int gsm0503_sch_encode(ubit_t *burst, uint8_t *sb_info);
int gsm0503_sch_decode(uint8_t *sb_info, sbit_t *burst);
