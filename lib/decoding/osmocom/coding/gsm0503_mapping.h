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
