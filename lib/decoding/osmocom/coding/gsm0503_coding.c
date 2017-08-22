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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <osmocom/core/bits.h>
#include <osmocom/core/conv.h>
#include <osmocom/core/utils.h>
#include <osmocom/core/crcgen.h>
#include <osmocom/core/endian.h>

#include <osmocom/gsm/protocol/gsm_04_08.h>
#include "gsm0503.h"
#include <osmocom/codec/codec.h>

#include "gsm0503_interleaving.h"
#include "gsm0503_mapping.h"
#include "gsm0503_tables.h"
#include "gsm0503_coding.h"
#include "gsm0503_parity.h"

#ifdef __cplusplus
}
#endif


/*
 * EGPRS coding limits
 */

/* Max header size with parity bits */
#define EGPRS_HDR_UPP_MAX	54

/* Max encoded header size */
#define EGPRS_HDR_C_MAX		162

/* Max punctured header size */
#define EGPRS_HDR_HC_MAX	160

/* Max data block size with parity bits */
#define EGPRS_DATA_U_MAX	612

/* Max encoded data block size */
#define EGPRS_DATA_C_MAX	1836

/* Max single block punctured data size */
#define EGPRS_DATA_DC_MAX	1248

/* Dual block punctured data size */
#define EGPRS_DATA_C1		612
#define EGPRS_DATA_C2		EGPRS_DATA_C1

/* TS 101318 Chapter 5.1: 260 bits + 4bit sig */
#define GSM_FR_BYTES	33
/* TS 101318 Chapter 5.2: 112 bits, no sig */
#define GSM_HR_BYTES	14
/* TS 101318 Chapter 5.3: 244 bits + 4bit sig */
#define GSM_EFR_BYTES	31

struct gsm0503_mcs_code {
	uint8_t mcs;
	uint8_t usf_len;

	/* Header coding */
	uint8_t hdr_len;
	uint8_t hdr_code_len;
	uint8_t hdr_punc_len;
	const struct osmo_conv_code *hdr_conv;
	const uint8_t *hdr_punc;

	/* Data coding */
	uint16_t data_len;
	uint16_t data_code_len;
	uint16_t data_punc_len;
	const struct osmo_conv_code *data_conv;
	const uint8_t *data_punc[3];
};

static int osmo_conv_decode_ber(const struct osmo_conv_code *code,
	const sbit_t *input, ubit_t *output,
	int *n_errors, int *n_bits_total)
{
	int res, i, coded_len;
	ubit_t recoded[EGPRS_DATA_C_MAX];

	res = osmo_conv_decode(code, input, output);

	if (n_bits_total || n_errors) {
		coded_len = osmo_conv_encode(code, output, recoded);
		OSMO_ASSERT(sizeof(recoded) / sizeof(recoded[0]) >= coded_len);
	}

	/* Count bit errors */
	if (n_errors) {
		*n_errors = 0;
		for (i = 0; i < coded_len; i++) {
			if (!((recoded[i] && input[i] < 0) ||
					(!recoded[i] && input[i] > 0)) )
						*n_errors += 1;
		}
	}

	if (n_bits_total)
		*n_bits_total = coded_len;

	return res;
}

static int _xcch_decode_cB(uint8_t *l2_data, sbit_t *cB,
	int *n_errors, int *n_bits_total)
{
	ubit_t conv[224];
	int rv;

	osmo_conv_decode_ber(&gsm0503_xcch, cB,
		conv, n_errors, n_bits_total);

	rv = osmo_crc64gen_check_bits(&gsm0503_fire_crc40,
		conv, 184, conv + 184);
	if (rv)
		return -1;

	osmo_ubit2pbit_ext(l2_data, 0, conv, 0, 184, 1);

	return 0;
}

static int _xcch_encode_cB(ubit_t *cB, uint8_t *l2_data)
{
	ubit_t conv[224];

	osmo_pbit2ubit_ext(conv, 0, l2_data, 0, 184, 1);

	osmo_crc64gen_set_bits(&gsm0503_fire_crc40, conv, 184, conv + 184);

	osmo_conv_encode(&gsm0503_xcch, conv, cB);

	return 0;
}

/*
 * GSM xCCH block transcoding
 */
int gsm0503_xcch_decode(uint8_t *l2_data, sbit_t *bursts,
	int *n_errors, int *n_bits_total)
{
	sbit_t iB[456], cB[456];
	int i;

	for (i = 0; i < 4; i++)
		gsm0503_xcch_burst_unmap(&iB[i * 114], &bursts[i * 116], NULL, NULL);

	gsm0503_xcch_deinterleave(cB, iB);

	return _xcch_decode_cB(l2_data, cB, n_errors, n_bits_total);
}

int gsm0503_xcch_encode(ubit_t *bursts, uint8_t *l2_data)
{
	ubit_t iB[456], cB[456], hl = 1, hn = 1;
	int i;

	_xcch_encode_cB(cB, l2_data);

	gsm0503_xcch_interleave(cB, iB);

	for (i = 0; i < 4; i++)
		gsm0503_xcch_burst_map(&iB[i * 114], &bursts[i * 116], &hl, &hn);

	return 0;
}



/*
 * GSM PDTCH block transcoding
 */

int gsm0503_pdtch_decode(uint8_t *l2_data, sbit_t *bursts, uint8_t *usf_p,
	int *n_errors, int *n_bits_total)
{
	sbit_t iB[456], cB[676], hl_hn[8];
	ubit_t conv[456];
	int i, j, k, rv, best = 0, cs = 0, usf = 0; /* make GCC happy */

	for (i = 0; i < 4; i++)
		gsm0503_xcch_burst_unmap(&iB[i * 114], &bursts[i * 116],
			hl_hn + i * 2, hl_hn + i * 2 + 1);

	for (i = 0; i < 4; i++) {
		for (j = 0, k = 0; j < 8; j++)
			k += abs(((int)gsm0503_pdtch_hl_hn_sbit[i][j]) - ((int)hl_hn[j]));

		if (i == 0 || k < best) {
			best = k;
			cs = i+1;
		}
	}

	gsm0503_xcch_deinterleave(cB, iB);

	switch (cs) {
	case 1:
		osmo_conv_decode_ber(&gsm0503_xcch, cB,
			conv, n_errors, n_bits_total);

		rv = osmo_crc64gen_check_bits(&gsm0503_fire_crc40,
			conv, 184, conv + 184);
		if (rv)
			return -1;

		osmo_ubit2pbit_ext(l2_data, 0, conv, 0, 184, 1);

		return 23;
	case 2:
		for (i = 587, j = 455; i >= 0; i--) {
			if (!gsm0503_puncture_cs2[i])
				cB[i] = cB[j--];
			else
				cB[i] = 0;
		}

		osmo_conv_decode_ber(&gsm0503_cs2, cB,
			conv, n_errors, n_bits_total);

		for (i = 0; i < 8; i++) {
			for (j = 0, k = 0; j < 6; j++)
				k += abs(((int)gsm0503_usf2six[i][j]) - ((int)conv[j]));

			if (i == 0 || k < best) {
				best = k;
				usf = i;
			}
		}

		conv[3] = usf & 1;
		conv[4] = (usf >> 1) & 1;
		conv[5] = (usf >> 2) & 1;
		if (usf_p)
			*usf_p = usf;

		rv = osmo_crc16gen_check_bits(&gsm0503_cs234_crc16,
			conv + 3, 271, conv + 3 + 271);
		if (rv)
			return -1;

		osmo_ubit2pbit_ext(l2_data, 0, conv, 3, 271, 1);

		return 34;
	case 3:
		for (i = 675, j = 455; i >= 0; i--) {
			if (!gsm0503_puncture_cs3[i])
				cB[i] = cB[j--];
			else
				cB[i] = 0;
		}

		osmo_conv_decode_ber(&gsm0503_cs3, cB,
			conv, n_errors, n_bits_total);

		for (i = 0; i < 8; i++) {
			for (j = 0, k = 0; j < 6; j++)
				k += abs(((int)gsm0503_usf2six[i][j]) - ((int)conv[j]));

			if (i == 0 || k < best) {
				best = k;
				usf = i;
			}
		}

		conv[3] = usf & 1;
		conv[4] = (usf >> 1) & 1;
		conv[5] = (usf >> 2) & 1;
		if (usf_p)
			*usf_p = usf;

		rv = osmo_crc16gen_check_bits(&gsm0503_cs234_crc16,
			conv + 3, 315, conv + 3 + 315);
		if (rv)
			return -1;

		osmo_ubit2pbit_ext(l2_data, 0, conv, 3, 315, 1);

		return 40;
	case 4:
		for (i = 12; i < 456; i++)
			conv[i] = (cB[i] < 0) ? 1 : 0;

		for (i = 0; i < 8; i++) {
			for (j = 0, k = 0; j < 12; j++)
				k += abs(((int)gsm0503_usf2twelve_sbit[i][j]) - ((int)cB[j]));

			if (i == 0 || k < best) {
				best = k;
				usf = i;
			}
		}

		conv[9] = usf & 1;
		conv[10] = (usf >> 1) & 1;
		conv[11] = (usf >> 2) & 1;
		if (usf_p)
			*usf_p = usf;

		rv = osmo_crc16gen_check_bits(&gsm0503_cs234_crc16,
			conv + 9, 431, conv + 9 + 431);
		if (rv) {
			*n_bits_total = 456 - 12;
			*n_errors = *n_bits_total;
			return -1;
		}

		*n_bits_total = 456 - 12;
		*n_errors = 0;

		osmo_ubit2pbit_ext(l2_data, 0, conv, 9, 431, 1);

		return 54;
	default:
		*n_bits_total = 0;
		*n_errors = 0;
		break;
	}

	return -1;
}

int gsm0503_pdtch_encode(ubit_t *bursts, uint8_t *l2_data, uint8_t l2_len)
{
	ubit_t iB[456], cB[676];
	const ubit_t *hl_hn;
	ubit_t conv[334];
	int i, j, usf;

	switch (l2_len) {
	case 23:
		osmo_pbit2ubit_ext(conv, 0, l2_data, 0, 184, 1);

		osmo_crc64gen_set_bits(&gsm0503_fire_crc40, conv, 184, conv + 184);

		osmo_conv_encode(&gsm0503_xcch, conv, cB);

		hl_hn = gsm0503_pdtch_hl_hn_ubit[0];

		break;
	case 34:
		osmo_pbit2ubit_ext(conv, 3, l2_data, 0, 271, 1);
		usf = l2_data[0] & 0x7;

		osmo_crc16gen_set_bits(&gsm0503_cs234_crc16, conv + 3,
			271, conv + 3 + 271);

		memcpy(conv, gsm0503_usf2six[usf], 6);

		osmo_conv_encode(&gsm0503_cs2, conv, cB);

		for (i = 0, j = 0; i < 588; i++)
			if (!gsm0503_puncture_cs2[i])
				cB[j++] = cB[i];

		hl_hn = gsm0503_pdtch_hl_hn_ubit[1];

		break;
	case 40:
		osmo_pbit2ubit_ext(conv, 3, l2_data, 0, 315, 1);
		usf = l2_data[0] & 0x7;

		osmo_crc16gen_set_bits(&gsm0503_cs234_crc16, conv + 3,
			315, conv + 3 + 315);

		memcpy(conv, gsm0503_usf2six[usf], 6);

		osmo_conv_encode(&gsm0503_cs3, conv, cB);

		for (i = 0, j = 0; i < 676; i++)
			if (!gsm0503_puncture_cs3[i])
				cB[j++] = cB[i];

		hl_hn = gsm0503_pdtch_hl_hn_ubit[2];

		break;
	case 54:
		osmo_pbit2ubit_ext(cB, 9, l2_data, 0, 431, 1);
		usf = l2_data[0] & 0x7;

		osmo_crc16gen_set_bits(&gsm0503_cs234_crc16, cB + 9,
			431, cB + 9 + 431);

		memcpy(cB, gsm0503_usf2twelve_ubit[usf], 12);

		hl_hn = gsm0503_pdtch_hl_hn_ubit[3];

		break;
	default:
		return -1;
	}

	gsm0503_xcch_interleave(cB, iB);

	for (i = 0; i < 4; i++) {
		gsm0503_xcch_burst_map(&iB[i * 114], &bursts[i * 116],
			hl_hn + i * 2, hl_hn + i * 2 + 1);
	}

	return GSM0503_GPRS_BURSTS_NBITS;
}

/*
 * GSM TCH/F FR/EFR transcoding
 */

static void tch_fr_reassemble(uint8_t *tch_data,
	ubit_t *b_bits, int net_order)
{
	int i, j, k, l, o;

	tch_data[0] = 0xd << 4;
	memset(tch_data + 1, 0, 32);

	if (net_order) {
		for (i = 0, j = 4; i < 260; i++, j++)
			tch_data[j >> 3] |= (b_bits[i] << (7 - (j & 7)));

		return;
	}

	/* reassemble d-bits */
	i = 0; /* counts bits */
	j = 4; /* counts output bits */
	k = gsm0503_gsm_fr_map[0]-1; /* current number bit in element */
	l = 0; /* counts element bits */
	o = 0; /* offset input bits */
	while (i < 260) {
		tch_data[j >> 3] |= (b_bits[k + o] << (7 - (j & 7)));
		if (--k < 0) {
			o += gsm0503_gsm_fr_map[l];
			k = gsm0503_gsm_fr_map[++l]-1;
		}
		i++;
		j++;
	}
}

static void tch_fr_disassemble(ubit_t *b_bits,
	uint8_t *tch_data, int net_order)
{
	int i, j, k, l, o;

	if (net_order) {
		for (i = 0, j = 4; i < 260; i++, j++)
			b_bits[i] = (tch_data[j >> 3] >> (7 - (j & 7))) & 1;

		return;
	}

	i = 0; /* counts bits */
	j = 4; /* counts input bits */
	k = gsm0503_gsm_fr_map[0] - 1; /* current number bit in element */
	l = 0; /* counts element bits */
	o = 0; /* offset output bits */
	while (i < 260) {
		b_bits[k + o] = (tch_data[j >> 3] >> (7 - (j & 7))) & 1;
		if (--k < 0) {
			o += gsm0503_gsm_fr_map[l];
			k = gsm0503_gsm_fr_map[++l] - 1;
		}
		i++;
		j++;
	}
}

static void tch_hr_reassemble(uint8_t *tch_data, ubit_t *b_bits)
{
	int i, j;

	tch_data[0] = 0x00; /* F = 0, FT = 000 */
	memset(tch_data + 1, 0, 14);

	for (i = 0, j = 8; i < 112; i++, j++)
		tch_data[j >> 3] |= (b_bits[i] << (7 - (j & 7)));
}

static void tch_hr_disassemble(ubit_t *b_bits, uint8_t *tch_data)
{
	int i, j;

	for (i = 0, j = 8; i < 112; i++, j++)
		b_bits[i] = (tch_data[j >> 3] >> (7 - (j & 7))) & 1;
}

static void tch_efr_reassemble(uint8_t *tch_data, ubit_t *b_bits)
{
	int i, j;

	tch_data[0] = 0xc << 4;
	memset(tch_data + 1, 0, 30);

	for (i = 0, j = 4; i < 244; i++, j++)
		tch_data[j >> 3] |= (b_bits[i] << (7 - (j & 7)));
}

static void tch_efr_disassemble(ubit_t *b_bits, uint8_t *tch_data)
{
	int i, j;

	for (i = 0, j = 4; i < 244; i++, j++)
		b_bits[i] = (tch_data[j >> 3] >> (7 - (j & 7))) & 1;
}

static void tch_amr_reassemble(uint8_t *tch_data, ubit_t *d_bits, int len)
{
	int i, j;

	memset(tch_data, 0, (len + 7) >> 3);

	for (i = 0, j = 0; i < len; i++, j++)
		tch_data[j >> 3] |= (d_bits[i] << (7 - (j & 7)));
}

static void tch_amr_disassemble(ubit_t *d_bits, uint8_t *tch_data, int len)
{
	int i, j;

	for (i = 0, j = 0; i < len; i++, j++)
		d_bits[i] = (tch_data[j >> 3] >> (7 - (j & 7))) & 1;
}

static void tch_fr_d_to_b(ubit_t *b_bits, ubit_t *d_bits)
{
	int i;

	for (i = 0; i < 260; i++)
		b_bits[gsm610_bitorder[i]] = d_bits[i];
}

static void tch_fr_b_to_d(ubit_t *d_bits, ubit_t *b_bits)
{
	int i;

	for (i = 0; i < 260; i++)
		d_bits[i] = b_bits[gsm610_bitorder[i]];
}

static void tch_hr_d_to_b(ubit_t *b_bits, ubit_t *d_bits)
{
	int i;

	const uint16_t *map;

	if (!d_bits[93] && !d_bits[94])
		map = gsm620_unvoiced_bitorder;
	else
		map = gsm620_voiced_bitorder;

	for (i = 0; i < 112; i++)
		b_bits[map[i]] = d_bits[i];
}

static void tch_hr_b_to_d(ubit_t *d_bits, ubit_t *b_bits)
{
	int i;
	const uint16_t *map;

	if (!b_bits[34] && !b_bits[35])
		map = gsm620_unvoiced_bitorder;
	else
		map = gsm620_voiced_bitorder;

	for (i = 0; i < 112; i++)
		d_bits[i] = b_bits[map[i]];
}

static void tch_efr_d_to_w(ubit_t *b_bits, ubit_t *d_bits)
{
	int i;

	for (i = 0; i < 260; i++)
		b_bits[gsm660_bitorder[i]] = d_bits[i];
}

static void tch_efr_w_to_d(ubit_t *d_bits, ubit_t *b_bits)
{
	int i;

	for (i = 0; i < 260; i++)
		d_bits[i] = b_bits[gsm660_bitorder[i]];
}

static void tch_efr_protected(ubit_t *s_bits, ubit_t *b_bits)
{
	int i;

	for (i = 0; i < 65; i++)
		b_bits[i] = s_bits[gsm0503_gsm_efr_protected_bits[i] - 1];
}

static void tch_fr_unreorder(ubit_t *d, ubit_t *p, ubit_t *u)
{
	int i;

	for (i = 0; i < 91; i++) {
		d[i << 1] = u[i];
		d[(i << 1) + 1] = u[184 - i];
	}

	for (i = 0; i < 3; i++)
		p[i] = u[91 + i];
}

static void tch_fr_reorder(ubit_t *u, ubit_t *d, ubit_t *p)
{
	int i;

	for (i = 0; i < 91; i++) {
		u[i] = d[i << 1];
		u[184 - i] = d[(i << 1) + 1];
	}

	for (i = 0; i < 3; i++)
		u[91 + i] = p[i];
}

static void tch_hr_unreorder(ubit_t *d, ubit_t *p, ubit_t *u)
{
	memcpy(d, u, 95);
	memcpy(p, u + 95, 3);
}

static void tch_hr_reorder(ubit_t *u, ubit_t *d, ubit_t *p)
{
	memcpy(u, d, 95);
	memcpy(u + 95, p, 3);
}

static void tch_efr_reorder(ubit_t *w, ubit_t *s, ubit_t *p)
{
	memcpy(w, s, 71);
	w[71] = w[72] = s[69];
	memcpy(w + 73, s + 71, 50);
	w[123] = w[124] = s[119];
	memcpy(w + 125, s + 121, 53);
	w[178] = w[179] = s[172];
	memcpy(w + 180, s + 174, 50);
	w[230] = w[231] = s[222];
	memcpy(w + 232, s + 224, 20);
	memcpy(w + 252, p, 8);
}

static void tch_efr_unreorder(ubit_t *s, ubit_t *p, ubit_t *w)
{
	int sum;

	memcpy(s, w, 71);
	sum = s[69] + w[71] + w[72];
	s[69] = (sum > 2);
	memcpy(s + 71, w + 73, 50);
	sum = s[119] + w[123] + w[124];
	s[119] = (sum > 2);
	memcpy(s + 121, w + 125, 53);
	sum = s[172] + w[178] + w[179];
	s[172] = (sum > 2);
	memcpy(s + 174, w + 180, 50);
	sum = s[220] + w[230] + w[231];
	s[222] = (sum > 2);
	memcpy(s + 224, w + 232, 20);
	memcpy(p, w + 252, 8);
}

static void tch_amr_merge(ubit_t *u, ubit_t *d, ubit_t *p, int len, int prot)
{
	memcpy(u, d, prot);
	memcpy(u + prot, p, 6);
	memcpy(u + prot + 6, d + prot, len - prot);
}

static void tch_amr_unmerge(ubit_t *d, ubit_t *p,
	ubit_t *u, int len, int prot)
{
	memcpy(d, u, prot);
	memcpy(p, u+prot, 6);
	memcpy(d + prot, u + prot + 6, len - prot);
}

int gsm0503_tch_fr_decode(uint8_t *tch_data, sbit_t *bursts,
	int net_order, int efr, int *n_errors, int *n_bits_total)
{
	sbit_t iB[912], cB[456], h;
	ubit_t conv[185], s[244], w[260], b[65], d[260], p[8];
	int i, rv, len, steal = 0;

	for (i=0; i<8; i++) {
		gsm0503_tch_burst_unmap(&iB[i * 114],
			&bursts[i * 116], &h, i >> 2);
		steal -= h;
	}

	gsm0503_tch_fr_deinterleave(cB, iB);

	if (steal > 0) {
		rv = _xcch_decode_cB(tch_data, cB, n_errors, n_bits_total);
		if (rv) {
			/* Error decoding FACCH frame */
			return -1;
		}

		return 23;
	}

	osmo_conv_decode_ber(&gsm0503_tch_fr, cB, conv, n_errors, n_bits_total);

	tch_fr_unreorder(d, p, conv);

	for (i = 0; i < 78; i++)
		d[i + 182] = (cB[i + 378] < 0) ? 1 : 0;

	rv = osmo_crc8gen_check_bits(&gsm0503_tch_fr_crc3, d, 50, p);
	if (rv) {
		/* Error checking CRC8 for the FR part of an EFR/FR frame */
		return -1;
	}

	if (efr) {
		tch_efr_d_to_w(w, d);

		tch_efr_unreorder(s, p, w);

		tch_efr_protected(s, b);

		rv = osmo_crc8gen_check_bits(&gsm0503_tch_efr_crc8, b, 65, p);
		if (rv) {
			/* Error checking CRC8 for the EFR part of an EFR frame */
			return -1;
		}

		tch_efr_reassemble(tch_data, s);

		len = GSM_EFR_BYTES;
	} else {
		tch_fr_d_to_b(w, d);

		tch_fr_reassemble(tch_data, w, net_order);

		len = GSM_FR_BYTES;
	}

	return len;
}

int gsm0503_tch_fr_encode(ubit_t *bursts, uint8_t *tch_data,
	int len, int net_order)
{
	ubit_t iB[912], cB[456], h;
	ubit_t conv[185], w[260], b[65], s[244], d[260], p[8];
	int i;

	switch (len) {
	case GSM_EFR_BYTES: /* TCH EFR */

		tch_efr_disassemble(s, tch_data);

		tch_efr_protected(s, b);

		osmo_crc8gen_set_bits(&gsm0503_tch_efr_crc8, b, 65, p);

		tch_efr_reorder(w, s, p);

		tch_efr_w_to_d(d, w);

		goto coding_efr_fr;
	case GSM_FR_BYTES: /* TCH FR */
		tch_fr_disassemble(w, tch_data, net_order);

		tch_fr_b_to_d(d, w);

coding_efr_fr:
		osmo_crc8gen_set_bits(&gsm0503_tch_fr_crc3, d, 50, p);

		tch_fr_reorder(conv, d, p);

		memcpy(cB + 378, d + 182, 78);

		osmo_conv_encode(&gsm0503_tch_fr, conv, cB);

		h = 0;

		break;
	case GSM_MACBLOCK_LEN: /* FACCH */
		_xcch_encode_cB(cB, tch_data);

		h = 1;

		break;
	default:
		return -1;
	}

	gsm0503_tch_fr_interleave(cB, iB);

	for (i = 0; i < 8; i++) {
		gsm0503_tch_burst_map(&iB[i * 114],
			&bursts[i * 116], &h, i >> 2);
	}

	return 0;
}

int gsm0503_tch_hr_decode(uint8_t *tch_data, sbit_t *bursts, int odd,
	int *n_errors, int *n_bits_total)
{
	sbit_t iB[912], cB[456], h;
	ubit_t conv[98], b[112], d[112], p[3];
	int i, rv, steal = 0;

	/* Only unmap the stealing bits */
	if (!odd) {
		for (i = 0; i < 4; i++) {
			gsm0503_tch_burst_unmap(NULL, &bursts[i * 116], &h, 0);
			steal -= h;
		}

		for (i = 2; i < 5; i++) {
			gsm0503_tch_burst_unmap(NULL, &bursts[i * 116], &h, 1);
			steal -= h;
		}
	}

	/* If we found a stole FACCH, but only at correct alignment */
	if (steal > 0) {
		for (i = 0; i < 6; i++) {
			gsm0503_tch_burst_unmap(&iB[i * 114],
				&bursts[i * 116], NULL, i >> 2);
		}

		for (i = 2; i < 4; i++) {
			gsm0503_tch_burst_unmap(&iB[i * 114 + 456],
				&bursts[i * 116], NULL, 1);
		}

		gsm0503_tch_fr_deinterleave(cB, iB);

		rv = _xcch_decode_cB(tch_data, cB, n_errors, n_bits_total);
		if (rv) {
			/* Error decoding FACCH frame */
			return -1;
		}

		return GSM_MACBLOCK_LEN;
	}

	for (i = 0; i < 4; i++) {
		gsm0503_tch_burst_unmap(&iB[i * 114],
			&bursts[i * 116], NULL, i >> 1);
	}

	gsm0503_tch_hr_deinterleave(cB, iB);

	osmo_conv_decode_ber(&gsm0503_tch_hr, cB, conv, n_errors, n_bits_total);

	tch_hr_unreorder(d, p, conv);

	for (i = 0; i < 17; i++)
		d[i + 95] = (cB[i + 211] < 0) ? 1 : 0;

	rv = osmo_crc8gen_check_bits(&gsm0503_tch_fr_crc3, d + 73, 22, p);
	if (rv) {
		/* Error checking CRC8 for an HR frame */
		return -1;
	}

	tch_hr_d_to_b(b, d);

	tch_hr_reassemble(tch_data, b);

	return 15;
}

int gsm0503_tch_hr_encode(ubit_t *bursts, uint8_t *tch_data, int len)
{
	ubit_t iB[912], cB[456], h;
	ubit_t conv[98], b[112], d[112], p[3];
	int i;

	switch (len) {
	case 15: /* TCH HR */
		tch_hr_disassemble(b, tch_data);

		tch_hr_b_to_d(d, b);

		osmo_crc8gen_set_bits(&gsm0503_tch_fr_crc3, d + 73, 22, p);

		tch_hr_reorder(conv, d, p);

		osmo_conv_encode(&gsm0503_tch_hr, conv, cB);

		memcpy(cB + 211, d + 95, 17);

		h = 0;

		gsm0503_tch_hr_interleave(cB, iB);

		for (i = 0; i < 4; i++) {
			gsm0503_tch_burst_map(&iB[i * 114],
				&bursts[i * 116], &h, i >> 1);
		}

		break;
	case GSM_MACBLOCK_LEN: /* FACCH */
		_xcch_encode_cB(cB, tch_data);

		h = 1;

		gsm0503_tch_fr_interleave(cB, iB);

		for (i=0; i<6; i++) {
			gsm0503_tch_burst_map(&iB[i * 114],
				&bursts[i * 116], &h, i >> 2);
		}

		for (i=2; i<4; i++) {
			gsm0503_tch_burst_map(&iB[i * 114 + 456],
				&bursts[i * 116], &h, 1);
		}

		break;
	default:
		return -1;
	}

	return 0;
}

int gsm0503_tch_afs_decode(uint8_t *tch_data, sbit_t *bursts,
	int codec_mode_req, uint8_t *codec, int codecs, uint8_t *ft,
	uint8_t *cmr, int *n_errors, int *n_bits_total)
{
	sbit_t iB[912], cB[456], h;
	ubit_t d[244], p[6], conv[250];
	int i, j, k, best = 0, rv, len, steal = 0, id = 0;
	*n_errors = 0; *n_bits_total = 0;

	for (i=0; i<8; i++) {
		gsm0503_tch_burst_unmap(&iB[i * 114], &bursts[i * 116], &h, i >> 2);
		steal -= h;
	}

	gsm0503_tch_fr_deinterleave(cB, iB);

	if (steal > 0) {
		rv = _xcch_decode_cB(tch_data, cB, n_errors, n_bits_total);
		if (rv) {
			/* Error decoding FACCH frame */
			return -1;
		}

		return GSM_MACBLOCK_LEN;
	}

	for (i = 0; i < 4; i++) {
		for (j = 0, k = 0; j < 8; j++)
			k += abs(((int)gsm0503_afs_ic_sbit[i][j]) - ((int)cB[j]));

		if (i == 0 || k < best) {
			best = k;
			id = i;
		}
	}

	/* Check if indicated codec fits into range of codecs */
	if (id >= codecs) {
		/* Codec mode out of range, return id */
		return id;
	}

	switch ((codec_mode_req) ? codec[*ft] : codec[id]) {
	case 7: /* TCH/AFS12.2 */
		osmo_conv_decode_ber(&gsm0503_tch_afs_12_2, cB + 8,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 244, 81);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 81, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 12.2 frame */
			return -1;
		}

		tch_amr_reassemble(tch_data, d, 244);

		len = 31;

		break;
	case 6: /* TCH/AFS10.2 */
		osmo_conv_decode_ber(&gsm0503_tch_afs_10_2, cB + 8,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 204, 65);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 65, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 10.2 frame */
			return -1;
		}

		tch_amr_reassemble(tch_data, d, 204);

		len = 26;

		break;
	case 5: /* TCH/AFS7.95 */
		osmo_conv_decode_ber(&gsm0503_tch_afs_7_95, cB + 8,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 159, 75);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 75, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 7.95 frame */
			return -1;
		}

		tch_amr_reassemble(tch_data, d, 159);

		len = 20;

		break;
	case 4: /* TCH/AFS7.4 */
		osmo_conv_decode_ber(&gsm0503_tch_afs_7_4, cB + 8,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 148, 61);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 61, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 7.4 frame */
			return -1;
		}

		tch_amr_reassemble(tch_data, d, 148);

		len = 19;

		break;
	case 3: /* TCH/AFS6.7 */
		osmo_conv_decode_ber(&gsm0503_tch_afs_6_7, cB + 8,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 134, 55);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 55, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 6.7 frame */
			return -1;
		}

		tch_amr_reassemble(tch_data, d, 134);

		len = 17;

		break;
	case 2: /* TCH/AFS5.9 */
		osmo_conv_decode_ber(&gsm0503_tch_afs_5_9, cB + 8,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 118, 55);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 55, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 5.9 frame */
			return -1;
		}

		tch_amr_reassemble(tch_data, d, 118);

		len = 15;

		break;
	case 1: /* TCH/AFS5.15 */
		osmo_conv_decode_ber(&gsm0503_tch_afs_5_15, cB + 8,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 103, 49);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 49, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 5.15 frame */
			return -1;
		}

		tch_amr_reassemble(tch_data, d, 103);

		len = 13;

		break;
	case 0: /* TCH/AFS4.75 */
		osmo_conv_decode_ber(&gsm0503_tch_afs_4_75, cB + 8,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 95, 39);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 39, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 4.75 frame */
			return -1;
		}

		tch_amr_reassemble(tch_data, d, 95);

		len = 12;

		break;
	default:
		/* Unknown frame type */
		*n_bits_total = 448;
		*n_errors = *n_bits_total;
		return -1;
	}

	/* Change codec request / indication, if frame is valid */
	if (codec_mode_req)
		*cmr = id;
	else
		*ft = id;

	return len;
}

int gsm0503_tch_afs_encode(ubit_t *bursts, uint8_t *tch_data, int len,
	int codec_mode_req, uint8_t *codec, int codecs, uint8_t ft,
	uint8_t cmr)
{
	ubit_t iB[912], cB[456], h;
	ubit_t d[244], p[6], conv[250];
	int i;
	uint8_t id;

	if (len == GSM_MACBLOCK_LEN) { /* FACCH */
		_xcch_encode_cB(cB, tch_data);

		h = 1;

		goto facch;
	}

	h = 0;

	if (codec_mode_req) {
		if (cmr >= codecs) {
			/* FIXME: CMR ID is not in codec list! */
			return -1;
		}
		id = cmr;
	} else {
		if (ft >= codecs) {
			/* FIXME: FT ID is not in codec list! */
			return -1;
		}
		id = ft;
	}

	switch (codec[ft]) {
	case 7: /* TCH/AFS12.2 */
		if (len != 31)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 244);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 81, p);

		tch_amr_merge(conv, d, p, 244, 81);

		osmo_conv_encode(&gsm0503_tch_afs_12_2, conv, cB + 8);

		break;
	case 6: /* TCH/AFS10.2 */
		if (len != 26)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 204);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 65, p);

		tch_amr_merge(conv, d, p, 204, 65);

		osmo_conv_encode(&gsm0503_tch_afs_10_2, conv, cB + 8);

		break;
	case 5: /* TCH/AFS7.95 */
		if (len != 20)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 159);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 75, p);

		tch_amr_merge(conv, d, p, 159, 75);

		osmo_conv_encode(&gsm0503_tch_afs_7_95, conv, cB + 8);

		break;
	case 4: /* TCH/AFS7.4 */
		if (len != 19)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 148);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 61, p);

		tch_amr_merge(conv, d, p, 148, 61);

		osmo_conv_encode(&gsm0503_tch_afs_7_4, conv, cB + 8);

		break;
	case 3: /* TCH/AFS6.7 */
		if (len != 17)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 134);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 55, p);

		tch_amr_merge(conv, d, p, 134, 55);

		osmo_conv_encode(&gsm0503_tch_afs_6_7, conv, cB + 8);

		break;
	case 2: /* TCH/AFS5.9 */
		if (len != 15)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 118);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 55, p);

		tch_amr_merge(conv, d, p, 118, 55);

		osmo_conv_encode(&gsm0503_tch_afs_5_9, conv, cB + 8);

		break;
	case 1: /* TCH/AFS5.15 */
		if (len != 13)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 103);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 49, p);

		tch_amr_merge(conv, d, p, 103, 49);

		osmo_conv_encode(&gsm0503_tch_afs_5_15, conv, cB + 8);

		break;
	case 0: /* TCH/AFS4.75 */
		if (len != 12)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 95);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 39, p);

		tch_amr_merge(conv, d, p, 95, 39);

		osmo_conv_encode(&gsm0503_tch_afs_4_75, conv, cB + 8);

		break;
	default:
		/* FIXME: FT %ft is not supported */
		return -1;
	}

	memcpy(cB, gsm0503_afs_ic_ubit[id], 8);

facch:
	gsm0503_tch_fr_interleave(cB, iB);

	for (i = 0; i < 8; i++) {
		gsm0503_tch_burst_map(&iB[i * 114],
			&bursts[i * 116], &h, i >> 2);
	}

	return 0;

invalid_length:
	/* FIXME: payload length %len does not comply with codec type %ft */
	return -1;
}

int gsm0503_tch_ahs_decode(uint8_t *tch_data, sbit_t *bursts, int odd,
	int codec_mode_req, uint8_t *codec, int codecs, uint8_t *ft,
	uint8_t *cmr, int *n_errors, int *n_bits_total)
{
	sbit_t iB[912], cB[456], h;
	ubit_t d[244], p[6], conv[135];
	int i, j, k, best = 0, rv, len, steal = 0, id = 0;

	/* only unmap the stealing bits */
	if (!odd) {
		for (i = 0; i < 4; i++) {
			gsm0503_tch_burst_unmap(NULL, &bursts[i * 116], &h, 0);
			steal -= h;
		}
		for (i = 2; i < 5; i++) {
			gsm0503_tch_burst_unmap(NULL, &bursts[i * 116], &h, 1);
			steal -= h;
		}
	}

	/* if we found a stole FACCH, but only at correct alignment */
	if (steal > 0) {
		for (i = 0; i < 6; i++) {
			gsm0503_tch_burst_unmap(&iB[i * 114],
				&bursts[i * 116], NULL, i >> 2);
		}

		for (i = 2; i < 4; i++) {
			gsm0503_tch_burst_unmap(&iB[i * 114 + 456],
				&bursts[i * 116], NULL, 1);
		}

		gsm0503_tch_fr_deinterleave(cB, iB);

		rv = _xcch_decode_cB(tch_data, cB, n_errors, n_bits_total);
		if (rv) {
			/* Error decoding FACCH frame */
			return -1;
		}

		return GSM_MACBLOCK_LEN;
	}

	for (i = 0; i < 4; i++) {
		gsm0503_tch_burst_unmap(&iB[i * 114],
			&bursts[i * 116], NULL, i >> 1);
	}

	gsm0503_tch_hr_deinterleave(cB, iB);

	for (i = 0; i < 4; i++) {
		for (j = 0, k = 0; j < 4; j++)
			k += abs(((int)gsm0503_ahs_ic_sbit[i][j]) - ((int)cB[j]));

		if (i == 0 || k < best) {
			best = k;
			id = i;
		}
	}

	/* Check if indicated codec fits into range of codecs */
	if (id >= codecs) {
		/* Codec mode out of range, return id */
		return id;
	}

	switch ((codec_mode_req) ? codec[*ft] : codec[id]) {
	case 5: /* TCH/AHS7.95 */
		osmo_conv_decode_ber(&gsm0503_tch_ahs_7_95, cB + 4,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 123, 67);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 67, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 7.95 frame */
			return -1;
		}

		for (i = 0; i < 36; i++)
			d[i + 123] = (cB[i + 192] < 0) ? 1 : 0;

		tch_amr_reassemble(tch_data, d, 159);

		len = 20;

		break;
	case 4: /* TCH/AHS7.4 */
		osmo_conv_decode_ber(&gsm0503_tch_ahs_7_4, cB + 4,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 120, 61);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 61, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 7.4 frame */
			return -1;
		}

		for (i = 0; i < 28; i++)
			d[i + 120] = (cB[i + 200] < 0) ? 1 : 0;

		tch_amr_reassemble(tch_data, d, 148);

		len = 19;

		break;
	case 3: /* TCH/AHS6.7 */
		osmo_conv_decode_ber(&gsm0503_tch_ahs_6_7, cB + 4,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 110, 55);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 55, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 6.7 frame */
			return -1;
		}

		for (i = 0; i < 24; i++)
			d[i + 110] = (cB[i + 204] < 0) ? 1 : 0;

		tch_amr_reassemble(tch_data, d, 134);

		len = 17;

		break;
	case 2: /* TCH/AHS5.9 */
		osmo_conv_decode_ber(&gsm0503_tch_ahs_5_9, cB + 4,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 102, 55);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 55, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 5.9 frame */
			return -1;
		}

		for (i = 0; i < 16; i++)
			d[i + 102] = (cB[i + 212] < 0) ? 1 : 0;

		tch_amr_reassemble(tch_data, d, 118);

		len = 15;

		break;
	case 1: /* TCH/AHS5.15 */
		osmo_conv_decode_ber(&gsm0503_tch_ahs_5_15, cB + 4,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 91, 49);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 49, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 5.15 frame */
			return -1;
		}

		for (i = 0; i < 12; i++)
			d[i + 91] = (cB[i + 216] < 0) ? 1 : 0;

		tch_amr_reassemble(tch_data, d, 103);

		len = 13;

		break;
	case 0: /* TCH/AHS4.75 */
		osmo_conv_decode_ber(&gsm0503_tch_ahs_4_75, cB + 4,
			conv, n_errors, n_bits_total);

		tch_amr_unmerge(d, p, conv, 83, 39);

		rv = osmo_crc8gen_check_bits(&gsm0503_amr_crc6, d, 39, p);
		if (rv) {
			/* Error checking CRC8 for an AMR 4.75 frame */
			return -1;
		}

		for (i = 0; i < 12; i++)
			d[i + 83] = (cB[i + 216] < 0) ? 1 : 0;

		tch_amr_reassemble(tch_data, d, 95);

		len = 12;

		break;
	default:
		/* Unknown frame type */
		*n_bits_total = 159;
		*n_errors = *n_bits_total;
		return -1;
	}

	/* Change codec request / indication, if frame is valid */
	if (codec_mode_req)
		*cmr = id;
	else
		*ft = id;

	return len;
}

int gsm0503_tch_ahs_encode(ubit_t *bursts, uint8_t *tch_data, int len,
	int codec_mode_req, uint8_t *codec, int codecs, uint8_t ft,
	uint8_t cmr)
{
	ubit_t iB[912], cB[456], h;
	ubit_t d[244], p[6], conv[135];
	int i;
	uint8_t id;

	if (len == GSM_MACBLOCK_LEN) { /* FACCH */
		_xcch_encode_cB(cB, tch_data);

		h = 1;

		gsm0503_tch_fr_interleave(cB, iB);

		for (i = 0; i < 6; i++)
			gsm0503_tch_burst_map(&iB[i * 114], &bursts[i * 116],
				&h, i >> 2);
		for (i = 2; i < 4; i++)
			gsm0503_tch_burst_map(&iB[i * 114 + 456],
				&bursts[i * 116], &h, 1);

		return 0;
	}

	h = 0;

	if (codec_mode_req) {
		if (cmr >= codecs) {
			/* FIXME: CMR ID %d not in codec list */
			return -1;
		}
		id = cmr;
	} else {
		if (ft >= codecs) {
			/* FIXME: FT ID %d not in codec list */
			return -1;
		}
		id = ft;
	}

	switch (codec[ft]) {
	case 5: /* TCH/AHS7.95 */
		if (len != 20)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 159);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 67, p);

		tch_amr_merge(conv, d, p, 123, 67);

		osmo_conv_encode(&gsm0503_tch_ahs_7_95, conv, cB + 4);

		memcpy(cB + 192, d + 123, 36);

		break;
	case 4: /* TCH/AHS7.4 */
		if (len != 19)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 148);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 61, p);

		tch_amr_merge(conv, d, p, 120, 61);

		osmo_conv_encode(&gsm0503_tch_ahs_7_4, conv, cB + 4);

		memcpy(cB + 200, d + 120, 28);

		break;
	case 3: /* TCH/AHS6.7 */
		if (len != 17)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 134);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 55, p);

		tch_amr_merge(conv, d, p, 110, 55);

		osmo_conv_encode(&gsm0503_tch_ahs_6_7, conv, cB + 4);

		memcpy(cB + 204, d + 110, 24);

		break;
	case 2: /* TCH/AHS5.9 */
		if (len != 15)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 118);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 55, p);

		tch_amr_merge(conv, d, p, 102, 55);

		osmo_conv_encode(&gsm0503_tch_ahs_5_9, conv, cB + 4);

		memcpy(cB + 212, d + 102, 16);

		break;
	case 1: /* TCH/AHS5.15 */
		if (len != 13)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 103);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 49, p);

		tch_amr_merge(conv, d, p, 91, 49);

		osmo_conv_encode(&gsm0503_tch_ahs_5_15, conv, cB + 4);

		memcpy(cB + 216, d + 91, 12);

		break;
	case 0: /* TCH/AHS4.75 */
		if (len != 12)
			goto invalid_length;

		tch_amr_disassemble(d, tch_data, 95);

		osmo_crc8gen_set_bits(&gsm0503_amr_crc6, d, 39, p);

		tch_amr_merge(conv, d, p, 83, 39);

		osmo_conv_encode(&gsm0503_tch_ahs_4_75, conv, cB + 4);

		memcpy(cB + 216, d + 83, 12);

		break;
	default:
		/* FIXME: FT %ft is not supported */
		return -1;
	}

	memcpy(cB, gsm0503_afs_ic_ubit[id], 4);

	gsm0503_tch_hr_interleave(cB, iB);

	for (i = 0; i < 4; i++)
		gsm0503_tch_burst_map(&iB[i * 114], &bursts[i * 116], &h, i >> 1);

	return 0;

invalid_length:
	/* FIXME: payload length %len does not comply with codec type %ft */
	return -1;
}

/*
 * GSM RACH transcoding
 */

/*
 * GSM RACH apply BSIC to parity
 *
 * p(j) = p(j) xor b(j)     j = 0, ..., 5
 * b(0) = MSB of PLMN colour code
 * b(5) = LSB of BS colour code
 */
static int rach_apply_bsic(ubit_t *d, uint8_t bsic)
{
	int i;

	/* Apply it */
	for (i = 0; i < 6; i++)
		d[8 + i] ^= ((bsic >> (5 - i)) & 1);

	return 0;
}

int gsm0503_rach_decode(uint8_t *ra, sbit_t *burst, uint8_t bsic)
{
	ubit_t conv[14];
	int rv;

	osmo_conv_decode(&gsm0503_rach, burst, conv);

	rach_apply_bsic(conv, bsic);

	rv = osmo_crc8gen_check_bits(&gsm0503_rach_crc6, conv, 8, conv + 8);
	if (rv)
		return -1;

	osmo_ubit2pbit_ext(ra, 0, conv, 0, 8, 1);

	return 0;
}

int gsm0503_rach_encode(ubit_t *burst, uint8_t *ra, uint8_t bsic)
{
	ubit_t conv[14];

	osmo_pbit2ubit_ext(conv, 0, ra, 0, 8, 1);

	osmo_crc8gen_set_bits(&gsm0503_rach_crc6, conv, 8, conv + 8);

	rach_apply_bsic(conv, bsic);

	osmo_conv_encode(&gsm0503_rach, conv, burst);

	return 0;
}

/*
 * GSM SCH transcoding
 */
int gsm0503_sch_decode(uint8_t *sb_info, sbit_t *burst)
{
	ubit_t conv[35];
	int rv;

	osmo_conv_decode(&gsm0503_sch, burst, conv);

	rv = osmo_crc16gen_check_bits(&gsm0503_sch_crc10, conv, 25, conv + 25);
	if (rv)
		return -1;

	osmo_ubit2pbit_ext(sb_info, 0, conv, 0, 25, 1);

	return 0;
}

int gsm0503_sch_encode(ubit_t *burst, uint8_t *sb_info)
{
	ubit_t conv[35];

	osmo_pbit2ubit_ext(conv, 0, sb_info, 0, 25, 1);

	osmo_crc16gen_set_bits(&gsm0503_sch_crc10, conv, 25, conv + 25);

	osmo_conv_encode(&gsm0503_sch, conv, burst);

	return 0;
}
