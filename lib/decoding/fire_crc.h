/*
   The Hacker's Choice - http://www.thc.org
   Part of THC's GSM SCANNER PROJECT
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
