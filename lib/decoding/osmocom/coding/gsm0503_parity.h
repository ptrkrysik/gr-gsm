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

#include <osmocom/core/crcgen.h>

const struct osmo_crc64gen_code gsm0503_fire_crc40;
const struct osmo_crc16gen_code gsm0503_cs234_crc16;
const struct osmo_crc8gen_code gsm0503_mcs_crc8_hdr;
const struct osmo_crc16gen_code gsm0503_mcs_crc12;
const struct osmo_crc8gen_code gsm0503_rach_crc6;
const struct osmo_crc16gen_code gsm0503_sch_crc10;
const struct osmo_crc8gen_code gsm0503_tch_fr_crc3;
const struct osmo_crc8gen_code gsm0503_tch_efr_crc8;
const struct osmo_crc8gen_code gsm0503_amr_crc6;
