/*
 * gsm0503.h
 *
 * Copyright (C) 2016 sysmocom s.f.m.c. GmbH
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

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#include <osmocom/core/conv.h>

/*! \file gsm0503.h
 * Osmocom convolutional encoder/decoder for xCCH channels, see 3GPP TS 05.03
 */

/*! \brief structure describing convolutional code xCCH
 *
 *  Non-recursive code, flushed, not punctured code.
 */
extern const struct osmo_conv_code gsm0503_xcch;

/*! \brief structure describing convolutional code RACH
 */
extern const struct osmo_conv_code gsm0503_rach;

/*! \brief structure describing convolutional code SCH
 */
extern const struct osmo_conv_code gsm0503_sch;

/*! \brief structures describing convolutional codes CS2/3
 */
extern const struct osmo_conv_code gsm0503_cs2;
extern const struct osmo_conv_code gsm0503_cs3;

/*! \brief structure describing convolutional code TCH/FR
 */
extern const struct osmo_conv_code gsm0503_tch_fr;

/*! \brief structure describing convolutional code TCH/HR
 */
extern const struct osmo_conv_code gsm0503_tch_hr;

/*! \brief structure describing convolutional code TCH/AFS 12.2
 */
extern const struct osmo_conv_code gsm0503_tch_afs_12_2;

/*! \brief structure describing convolutional code TCH/AFS 10.2
 */
extern const struct osmo_conv_code gsm0503_tch_afs_10_2;

/*! \brief structure describing convolutional code TCH/AFS 7.95
 */
extern const struct osmo_conv_code gsm0503_tch_afs_7_95;

/*! \brief structure describing convolutional code TCH/AFS 7.4
 */
extern const struct osmo_conv_code gsm0503_tch_afs_7_4;

/*! \brief structure describing convolutional code TCH/AFS 6.7
 */
extern const struct osmo_conv_code gsm0503_tch_afs_6_7;

/*! \brief structure describing convolutional code TCH/AFS 5.9
 */
extern const struct osmo_conv_code gsm0503_tch_afs_5_9;

/*! \brief structure describing convolutional code TCH/AFS 5.15
 */
extern const struct osmo_conv_code gsm0503_tch_afs_5_15;

/*! \brief structure describing convolutional code TCH/AFS 4.75
 */
extern const struct osmo_conv_code gsm0503_tch_afs_4_75;

/*! \brief structure describing convolutional code TCH/AHS 7.95
 */
extern const struct osmo_conv_code gsm0503_tch_ahs_7_95;

/*! \brief structure describing convolutional code TCH/AHS 7.4
 */
extern const struct osmo_conv_code gsm0503_tch_ahs_7_4;

/*! \brief structure describing convolutional code TCH/AHS 6.7
 */
extern const struct osmo_conv_code gsm0503_tch_ahs_6_7;

/*! \brief structure describing convolutional code TCH/AHS 5.9
 */
extern const struct osmo_conv_code gsm0503_tch_ahs_5_9;

/*! \brief structure describing convolutional code TCH/AHS 5.15
 */
extern const struct osmo_conv_code gsm0503_tch_ahs_5_15;

/*! \brief structure describing convolutional code TCH/AHS 4.75
 */
extern const struct osmo_conv_code gsm0503_tch_ahs_4_75;

/*! \brief structure describing convolutional code EDGE MCS-1 DL HDR
 */
extern const struct osmo_conv_code gsm0503_mcs1_dl_hdr;

/*! \brief structure describing convolutional code EDGE MCS-1 UL HDR
 */
extern const struct osmo_conv_code gsm0503_mcs1_ul_hdr;

/*! \brief structure describing convolutional code EDGE MCS-1
 */
extern const struct osmo_conv_code gsm0503_mcs1;

/*! \brief structure describing convolutional code EDGE MCS-2
 */
extern const struct osmo_conv_code gsm0503_mcs2;

/*! \brief structure describing convolutional code EDGE MCS-3
 */
extern const struct osmo_conv_code gsm0503_mcs3;

/*! \brief structure describing convolutional code EDGE MCS-4
 */
extern const struct osmo_conv_code gsm0503_mcs4;

/*! \brief structure describing convolutional code EDGE MCS-5 DL HDR
 */
extern const struct osmo_conv_code gsm0503_mcs5_dl_hdr;

/*! \brief structure describing convolutional code EDGE MCS-5 UL HDR
 */
extern const struct osmo_conv_code gsm0503_mcs5_ul_hdr;

/*! \brief structure describing convolutional code EDGE MCS-5
 */
extern const struct osmo_conv_code gsm0503_mcs5;

/*! \brief structure describing convolutional code EDGE MCS-6
 */
extern const struct osmo_conv_code gsm0503_mcs6;

/*! \brief structure describing convolutional code EDGE MCS-7 DL HDR
 */
extern const struct osmo_conv_code gsm0503_mcs7_dl_hdr;

/*! \brief structure describing convolutional code EDGE MCS-7 UL HDR
 */
extern const struct osmo_conv_code gsm0503_mcs7_ul_hdr;

/*! \brief structure describing convolutional code EDGE MCS-7
 */
extern const struct osmo_conv_code gsm0503_mcs7;

/*! \brief structure describing convolutional code EDGE MCS-8
 */
extern const struct osmo_conv_code gsm0503_mcs8;

/*! \brief structure describing convolutional code EDGE MCS-9
 */
extern const struct osmo_conv_code gsm0503_mcs9;

#ifdef __cplusplus
}
#endif
