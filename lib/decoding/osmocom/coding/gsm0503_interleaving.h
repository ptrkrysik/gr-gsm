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

void gsm0503_xcch_deinterleave(sbit_t *cB, const sbit_t *iB);
void gsm0503_xcch_interleave(ubit_t *cB, ubit_t *iB);

void gsm0503_tch_fr_deinterleave(sbit_t *cB, sbit_t *iB);
void gsm0503_tch_fr_interleave(ubit_t *cB, ubit_t *iB);

void gsm0503_tch_hr_deinterleave(sbit_t *cB, sbit_t *iB);
void gsm0503_tch_hr_interleave(ubit_t *cB, ubit_t *iB);

void gsm0503_mcs1_ul_deinterleave(sbit_t *hc, sbit_t *dc, const sbit_t *iB);
void gsm0503_mcs1_ul_interleave(const ubit_t *hc,
	const ubit_t *dc, ubit_t *iB);

void gsm0503_mcs1_dl_deinterleave(sbit_t *u, sbit_t *hc,
	sbit_t *dc, const sbit_t *iB);
void gsm0503_mcs1_dl_interleave(const ubit_t *up, const ubit_t *hc,
	const ubit_t *dc, ubit_t *iB);

void gsm0503_mcs5_ul_deinterleave(sbit_t *hc, sbit_t *dc,
	const sbit_t *hi, const sbit_t *di);
void gsm0503_mcs5_ul_interleave(const ubit_t *hc, const ubit_t *dc,
	ubit_t *hi, ubit_t *di);

void gsm0503_mcs5_dl_deinterleave(sbit_t *hc, sbit_t *dc,
	const sbit_t *hi, const sbit_t *di);
void gsm0503_mcs5_dl_interleave(const ubit_t *hc, const ubit_t *dc,
	ubit_t *hi, ubit_t *di);

void gsm0503_mcs7_ul_deinterleave(sbit_t *hc, sbit_t *c1, sbit_t *c2,
	const sbit_t *hi, const sbit_t *di);
void gsm0503_mcs7_ul_interleave(const ubit_t *hc, const ubit_t *c1,
	const ubit_t *c2, ubit_t *hi, ubit_t *di);

void gsm0503_mcs7_dl_deinterleave(sbit_t *hc, sbit_t *c1, sbit_t *c2,
	const sbit_t *hi, const sbit_t *di);
void gsm0503_mcs7_dl_interleave(const ubit_t *hc, const ubit_t *c1,
	const ubit_t *c2, ubit_t *hi, ubit_t *di);

void gsm0503_mcs8_ul_deinterleave(sbit_t *hc, sbit_t *c1, sbit_t *c2,
	const sbit_t *hi, const sbit_t *di);
void gsm0503_mcs8_ul_interleave(const ubit_t *hc, const ubit_t *c1,
	const ubit_t *c2, ubit_t *hi, ubit_t *di);

void gsm0503_mcs8_dl_deinterleave(sbit_t *hc, sbit_t *c1, sbit_t *c2,
	const sbit_t *hi, const sbit_t *di);
void gsm0503_mcs8_dl_interleave(const ubit_t *hc, const ubit_t *c1,
	const ubit_t *c2, ubit_t *hi, ubit_t *di);
