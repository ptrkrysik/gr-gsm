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

#include "fire_crc.h"
#include <stdio.h>
#include <string.h>

#define REM(x, y)	(x) % (y)

static int FC_syndrome_shift(FC_CTX *ctx, unsigned int bit);

static int
outit(int *data, int len)
{
	int i;

	for (i = 0; i < len; i++)
		printf("%d ", data[i]);
	printf("\n");
}

int
FC_init(FC_CTX *ctx, unsigned int crc_size, unsigned int data_size)
{
	ctx->crc_size = crc_size;
	ctx->data_size = data_size;
	ctx->syn_start = 0;

	return 0;
}

int
FC_check_crc(FC_CTX *ctx, unsigned char *input_bits, unsigned char *control_data)
{ 
	int j,error_count = 0, error_index = 0, success_flag = 0, syn_index = 0;
	unsigned int i;

	ctx->syn_start = 0;
	// reset the syndrome register
	memset(ctx->syndrome_reg, 0, sizeof ctx->syndrome_reg);

	// shift in the data bits
    for (i=0; i < ctx->data_size; i++) {
        error_count = FC_syndrome_shift(ctx, input_bits[i]);
        control_data[i] = input_bits[i];
    }

    // shift in the crc bits
    for (i=0; i < ctx->crc_size; i++) {
        error_count = FC_syndrome_shift(ctx, 1-input_bits[i+ctx->data_size]);
    }

    // Find position of error burst
    if (error_count == 0) {
        error_index = 0;
    }
    else {
        error_index = 1;
        error_count = FC_syndrome_shift(ctx, 0);
        error_index += 1;
        while (error_index < (ctx->data_size + ctx->crc_size) ) {
            error_count = FC_syndrome_shift(ctx, 0);
            error_index += 1;
            if ( error_count == 0 ) break;
        }
    }

    // Test for correctable errors
    //printf("error_index %d\n",error_index);
    if (error_index == 224) success_flag = 0;
    else {

        // correct index depending on the position of the error
        if (error_index == 0) syn_index = error_index;
        else syn_index = error_index - 1;
        
        // error burst lies within data bits
        if (error_index < 184) {
            //printf("error < bit 184,%d\n",error_index);
            j = error_index;
            while ( j < (error_index+12) ) {
                if (j < 184) {
                    control_data[j] = control_data[j] ^ 
                       ctx->syndrome_reg[REM(ctx->syn_start+39-j+syn_index,40)];
                }               
                else break;
                j = j + 1;
            }
        }
        else if ( error_index > 212 ) {
            //printf("error > bit 212,%d\n",error_index);
            j = 0;
            while ( j < (error_index - 212) ) {
                control_data[j] = control_data[j] ^ 
                      ctx->syndrome_reg[REM(ctx->syn_start+39-j-224+syn_index,40)];
                j = j + 1;
            }
        }
        // for 183 < error_index < 213 error in parity alone so ignore
        success_flag = 1;
    }
    return success_flag;
}    

static int 
FC_syndrome_shift(FC_CTX *ctx, unsigned int bit)
{
	int error_count = 0;
	unsigned int i;

	if (ctx->syn_start == 0)
		ctx->syn_start = 39;
	else ctx->syn_start -= 1;

	int temp_syndrome_reg[sizeof ctx->syndrome_reg];

	memcpy(temp_syndrome_reg, ctx->syndrome_reg, sizeof temp_syndrome_reg);

    temp_syndrome_reg[REM(ctx->syn_start+3,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+3,40)] ^
                       ctx->syndrome_reg[ctx->syn_start];
    temp_syndrome_reg[REM(ctx->syn_start+17,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+17,40)] ^
                       ctx->syndrome_reg[ctx->syn_start];
    temp_syndrome_reg[REM(ctx->syn_start+23,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+23,40)] ^
                       ctx->syndrome_reg[ctx->syn_start];
    temp_syndrome_reg[REM(ctx->syn_start+26,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+26,40)] ^
                       ctx->syndrome_reg[ctx->syn_start];

    temp_syndrome_reg[REM(ctx->syn_start+4,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+4,40)] ^ bit;
    temp_syndrome_reg[REM(ctx->syn_start+6,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+6,40)] ^ bit;
    temp_syndrome_reg[REM(ctx->syn_start+10,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+10,40)] ^ bit;
    temp_syndrome_reg[REM(ctx->syn_start+16,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+16,40)] ^ bit;
    temp_syndrome_reg[REM(ctx->syn_start+27,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+27,40)] ^ bit;
    temp_syndrome_reg[REM(ctx->syn_start+29,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+29,40)] ^ bit;
    temp_syndrome_reg[REM(ctx->syn_start+33,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+33,40)] ^ bit;
    temp_syndrome_reg[REM(ctx->syn_start+39,40)] = 
                       ctx->syndrome_reg[REM(ctx->syn_start+39,40)] ^ bit;

    temp_syndrome_reg[ctx->syn_start] = ctx->syndrome_reg[ctx->syn_start] ^ bit;

	memcpy(ctx->syndrome_reg, temp_syndrome_reg, sizeof ctx->syndrome_reg);

    for (i = 0; i < 28; i++) {
       error_count = error_count + ctx->syndrome_reg[REM(ctx->syn_start+i,40)];
    }
    return error_count;
}
 

