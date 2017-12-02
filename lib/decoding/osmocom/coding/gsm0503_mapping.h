/*
 * (C) 2013 by Andreas Eversberg <jolly@eversberg.eu>
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

#include <osmocom/core/bits.h>

void gsm0503_xcch_burst_unmap(sbit_t *iB, const sbit_t *eB,
	sbit_t *hl, sbit_t *hn);
void gsm0503_xcch_burst_map(ubit_t *iB, ubit_t *eB, const ubit_t *hl,
	const ubit_t *hn);

void gsm0503_tch_burst_unmap(sbit_t *iB, sbit_t *eB, sbit_t *h, int odd);
void gsm0503_tch_burst_map(ubit_t *iB, ubit_t *eB, const ubit_t *h, int odd);

void gsm0503_mcs5_ul_burst_map(const ubit_t *di, ubit_t *eB,
	const ubit_t *hi, int B);
void gsm0503_mcs5_ul_burst_unmap(sbit_t *di, const sbit_t *eB,
	sbit_t *hi, int B);

void gsm0503_mcs7_ul_burst_map(const ubit_t *di, ubit_t *eB,
	const ubit_t *hi, int B);
void gsm0503_mcs7_ul_burst_unmap(sbit_t *di, const sbit_t *eB,
	sbit_t *hi, int B);

void gsm0503_mcs5_dl_burst_map(const ubit_t *di, ubit_t *eB,
	const ubit_t *hi, const ubit_t *up, int B);
void gsm0503_mcs5_dl_burst_unmap(sbit_t *di, const sbit_t *eB,
	sbit_t *hi, sbit_t *up, int B);

void gsm0503_mcs7_dl_burst_map(const ubit_t *di, ubit_t *eB,
	const ubit_t *hi, const ubit_t *up, int B);
void gsm0503_mcs7_dl_burst_unmap(sbit_t *di, const sbit_t *eB,
	sbit_t *hi, sbit_t *up, int B);

void gsm0503_mcs5_burst_swap(sbit_t *eB);
