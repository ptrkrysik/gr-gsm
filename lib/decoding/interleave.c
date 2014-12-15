/*
   The Hacker's Choice - http://www.thc.org
   Part of THC's GSM SCANNER PROJECT
*/

#include <stdlib.h>
#include <stdio.h>
#include "interleave.h"

int
interleave_init(INTERLEAVE_CTX *ictx, int size, int block_size)
{
	ictx->trans_size = size;
	ictx->trans = (unsigned short *)malloc(size * sizeof *ictx->trans);

//	DEBUGF("size: %d\n", size);
//	DEBUGF("Block size: %d\n", block_size);
	int j, k, B;
	for (k = 0; k < size; k++)
	{
		B = k % 4;
		j = 2 * ((49 * k) % 57) + ((k % 8) / 4);
		ictx->trans[k] = B * block_size + j;
		/* Mapping: pos1 goes to pos2: pos1 -> pos2 */
		//printf("%d -> %d\n", ictx->trans[k], k);
	}
//	exit(0);
	return 0;
}

int
interleave_init_facch_f(INTERLEAVE_CTX *ictx, int size, int block_size, int block_offset)
{
	ictx->trans_size = size;
	ictx->trans = (unsigned short *)malloc(size * sizeof *ictx->trans);

//	DEBUGF("size: %d\n", size);
//	DEBUGF("Block size: %d\n", block_size);
	int j, k, B;
	for (k = 0; k < size; k++)
	{
		B = (k + block_offset) % 8;
		j = 2 * ((49 * k) % 57) + ((k % 8) / 4);
		ictx->trans[k] = B * block_size + j;
		/* Mapping: pos1 goes to pos2: pos1 -> pos2 */
//		DEBUGF("%d -> %d\n", ictx->trans[k], k);
	}
//	exit(0);
	return 0;
}

int
interleave_deinit(INTERLEAVE_CTX *ictx)
{
	if (ictx->trans != NULL)
	{
		free(ictx->trans);
		ictx->trans = NULL;
	}

	return 0;
}

void
interleave_decode(INTERLEAVE_CTX *ictx, unsigned char *dst, unsigned char *src)
{
    printf("Lol\n");
	int k;
	for (k = 0; k < ictx->trans_size; k++)
	{
		printf("k=%d, ictx->trans[k]=%d\n", k, ictx->trans[k]);
		dst[k] = src[ictx->trans[k]];
    }
}

