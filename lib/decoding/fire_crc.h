/* -*- c++ -*- */
/*
 * Copyright 2005 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef INCLUDED_FIRE_CRC_H
#define INCLUDED_FIRE_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	unsigned int crc_size;
	unsigned int data_size;
	unsigned int syn_start;
	int syndrome_reg[40];
} FC_CTX;

int FC_init(FC_CTX *ctx, unsigned int crc_size, unsigned int data_size);
int FC_check_crc(FC_CTX *ctx, unsigned char *input_bits, unsigned char *control_data);

#ifdef __cplusplus
}
#endif

#endif
