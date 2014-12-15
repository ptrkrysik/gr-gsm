/*
 * A pedagogical implementation of the GSM A5/1 and A5/2 "voice privacy"
 * encryption algorithms.
 *
 * Copyright (C) 1998-1999: Marc Briceno, Ian Goldberg, and David Wagner
 *
 * The source code below is optimized for instructional value and clarity.
 * Performance will be terrible, but that's not the point.
 *
 * This software may be export-controlled by US law.
 *
 * This software is free for commercial and non-commercial use as long as
 * the following conditions are adhered to.
 * Copyright remains the authors' and as such any Copyright notices in
 * the code are not to be removed.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The license and distribution terms for any publicly available version
 * or derivative of this code cannot be changed.  i.e. this code cannot
 * simply be copied and put under another distribution license
 * [including the GNU Public License].
 *
 * Background: The Global System for Mobile communications is the most
 * widely deployed digital cellular telephony system in the world. GSM
 * makes use of four core cryptographic algorithms, none of which has
 * been published by the GSM MOU.  This failure to subject the
 * algorithms to public review is all the more puzzling given that over
 * 215 million GSM subscribers are expected to rely on the claimed
 * security of the system.
 *
 * The four core GSM cryptographic algorithms are:
 * A3              authentication algorithm
 * A5/1 "stronger" over-the-air voice-privacy algorithm
 * A5/2  "weaker"  over-the-air voice-privacy algorithm
 * A8              voice-privacy key generation algorithm
 *
 * In April of 1998, our group showed that COMP128, the algorithm used by the
 * overwhelming majority of GSM providers for both A3 and A8 functionality
 * is fatally flawed and allows for cloning of GSM mobile phones.
 *
 * Furthermore, we demonstrated that all A8 implementations we could locate,
 * including the few that did not use COMP128 for key generation, had been
 * deliberately weakened by reducing the keyspace from 64 bits to 54 bits.
 * The remaining 10 bits are simply set to zero!
 *
 * See http://www.scard.org/gsm for additional information.
 *
 * [May 1999]
 * One question so far unanswered is if A5/1, the "stronger" of the two
 * widely deployed voice-privacy algorithm is at least as strong as the
 * key. Meaning: "Does A5/1 have a work factor of at least 54 bits"?
 * Absent a publicly available A5/1 reference implementation, this question
 * could not be answered. We hope that our reference implementation below,
 * which has been verified against official A5/1 test vectors, will provide
 * the cryptographic community with the base on which to construct the
 * answer to this important question.
 *
 * Initial indications about the strength of A5/1 are not encouraging.
 * A variant of A5, while not A5/1 itself, has been estimated to have a
 * work factor of well below 54 bits. See http://jya.com/crack-a5.htm for
 * background information and references.
 *
 * With COMP128 broken and A5/1 published below, we will now turn our
 * attention to A5/2.
 *
 * [August 1999]
 * 19th Annual International Cryptology Conference - Crypto'99
 * Santa Barbara, California
 *
 * A5/2 has been added to the previously published A5/1 source. Our
 * implementation has been verified against official test vectors.
 *
 * This means that our group has now reverse engineered the entire set
 * of cryptographic algorithms used in the overwhelming majority of GSM
 * installations, including all the over-the-air "voice privacy" algorithms.
 *
 * The "voice privacy" algorithm A5/2 proved especially weak. Which perhaps
 * should come as no surprise, since even GSM MOU members have admitted that
 * A5/2 was designed with heavy input by intelligence agencies to ensure
 * breakability. Just how insecure is A5/2?  It can be broken in real time
 * with a work factor of a mere 16 bits. GSM might just as well use no "voice
 * privacy" algorithm at all.
 *
 * We announced the break of A5/2 at the Crypto'99 Rump Session.
 * Details will be published in a scientific paper following soon.
 *
 *
 * -- Marc Briceno      <marc@scard.org>
 *    Voice:            +1 (925) 798-4042
 *
 */

#ifndef A5_1_2_H
#define A5_1_2_H

#include <stdio.h>


/* Masks for the shift registers */
#define R1MASK  0x07FFFF /* 19 bits, numbered 0..18 */
#define R2MASK  0x3FFFFF /* 22 bits, numbered 0..21 */
#define R3MASK  0x7FFFFF /* 23 bits, numbered 0..22 */
#ifdef A5_2
#define R4MASK  0x01FFFF /* 17 bits, numbered 0..16 */
#endif /* A5_2 */


#ifndef A5_2
/* Middle bit of each of the three shift registers, for clock control */
#define R1MID   0x000100 /* bit 8 */
#define R2MID   0x000400 /* bit 10 */
#define R3MID   0x000400 /* bit 10 */
#else /* A5_2 */
/* A bit of R4 that controls each of the shift registers */
#define R4TAP1  0x000400 /* bit 10 */
#define R4TAP2  0x000008 /* bit 3 */
#define R4TAP3  0x000080 /* bit 7 */
#endif /* A5_2 */


/* Feedback taps, for clocking the shift registers.
 * These correspond to the primitive polynomials
 * x^19 + x^5 + x^2 + x + 1, x^22 + x + 1,
 * x^23 + x^15 + x^2 + x + 1, and x^17 + x^5 + 1. */


#define R1TAPS  0x072000 /* bits 18,17,16,13 */
#define R2TAPS  0x300000 /* bits 21,20 */
#define R3TAPS  0x700080 /* bits 22,21,20,7 */
#ifdef A5_2
#define R4TAPS  0x010800 /* bits 16,11 */
#endif /* A5_2 */


typedef unsigned char byte;
typedef unsigned long word;
typedef word bit;


/* Calculate the parity of a 32-bit word, i.e. the sum of its bits modulo 2
*/
bit parity(word x)
{
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return x&1;
}


/* Clock one shift register.  For A5/2, when the last bit of the frame
 * is loaded in, one particular bit of each register is forced to '1';
 * that bit is passed in as the last argument. */
#ifndef A5_2
word clockone(word reg, word mask, word taps)
{
#else /* A5_2 */
word clockone(word reg, word mask, word taps, word loaded_bit) {
#endif /* A5_2 */
  word t = reg & taps;
  reg = (reg << 1) & mask;
  reg |= parity(t);
#ifdef A5_2
  reg |= loaded_bit;
#endif /* A5_2 */
  return reg;
}


/* The three shift registers.  They're in global variables to make the code
 * easier to understand.
 * A better implementation would not use global variables. */
word R1, R2, R3;
#ifdef A5_2
word R4;
#endif /* A5_2 */


/* Return 1 iff at least two of the parameter words are non-zero. */
bit majority(word w1, word w2, word w3) {
  int sum = (w1 != 0) + (w2 != 0) + (w3 != 0);
  if (sum >= 2)
    return 1;
  else
    return 0;
}


/* Clock two or three of R1,R2,R3, with clock control
 * according to their middle bits.
 * Specifically, we clock Ri whenever Ri's middle bit
 * agrees with the majority value of the three middle bits.  For A5/2,
 * use particular bits of R4 instead of the middle bits.  Also, for A5/2,
 * always clock R4.
 * If allP == 1, clock all three of R1,R2,R3, ignoring their middle bits.
 * This is only used for key setup.  If loaded == 1, then this is the last
 * bit of the frame number, and if we're doing A5/2, we have to set a
 * particular bit in each of the four registers. */
void clock(int allP, int loaded) {
#ifndef A5_2
  bit maj = majority(R1 & R1MID, R2 & R2MID, R3 & R3MID);
  if (allP || (((R1&R1MID) != 0) == maj))
    R1 = clockone(R1, R1MASK, R1TAPS);
  if (allP || (((R2&R2MID) != 0) == maj))
    R2 = clockone(R2, R2MASK, R2TAPS);
  if (allP || (((R3&R3MID) != 0) == maj))
    R3 = clockone(R3, R3MASK, R3TAPS);
#else /* A5_2 */
  bit maj = majority(R4 & R4TAP1, R4 & R4TAP2, R4 & R4TAP3);
  if (allP || (((R4&R4TAP1) != 0) == maj))
    R1 = clockone(R1, R1MASK, R1TAPS, loaded << 15);
  if (allP || (((R4&R4TAP2) != 0) == maj))
    R2 = clockone(R2, R2MASK, R2TAPS, loaded << 16);
  if (allP || (((R4&R4TAP3) != 0) == maj))
    R3 = clockone(R3, R3MASK, R3TAPS, loaded << 18);
  R4 = clockone(R4, R4MASK, R4TAPS, loaded << 10);
#endif /* A5_2 */
}


/* Generate an output bit from the current state.
 * You grab a bit from each register via the output generation taps;
 * then you XOR the resulting three bits.  For A5/2, in addition to
 * the top bit of each of R1,R2,R3, also XOR in a majority function
 * of three particular bits of the register (one of them complemented)
 * to make it non-linear.  Also, for A5/2, delay the output by one
 * clock cycle for some reason. */
bit getbit() {
  bit topbits = (((R1 >> 18) ^ (R2 >> 21) ^ (R3 >> 22)) & 0x01);
#ifndef A5_2
  return topbits;
#else /* A5_2 */
  static bit delaybit = 0;
  bit nowbit = delaybit;
  delaybit = (
               topbits
               ^ majority(R1 & 0x8000, (~R1) & 0x4000, R1 & 0x1000)
               ^ majority((~R2) & 0x10000, R2 & 0x2000, R2 & 0x200)
               ^ majority(R3 & 0x40000, R3 & 0x10000, (~R3) & 0x2000)
             );
  return nowbit;
#endif /* A5_2 */
}


/* Do the A5 key setup.  This routine accepts a 64-bit key and
 * a 22-bit frame number. */
void keysetup(byte key_reversed[8], word frame) {
  int i;
  bit keybit, framebit;

  byte key[8];
  for(i=0; i<8; i++){
    key[i] = key_reversed[7-i];
  }
  /* Zero out the shift registers. */
  R1 = R2 = R3 = 0;
#ifdef A5_2
  R4 = 0;
#endif /* A5_2 */


  /* Load the key into the shift registers,
   * LSB of first byte of key array first,
   * clocking each register once for every
   * key bit loaded.  (The usual clock
   * control rule is temporarily disabled.) */
  for (i = 0; i < 64; i++) {
    clock(1, 0); /* always clock */
    keybit = (key[i/8] >> (i & 7)) & 1; /* The i-th bit of the key */
    R1 ^= keybit;
    R2 ^= keybit;
    R3 ^= keybit;
#ifdef A5_2
    R4 ^= keybit;
#endif /* A5_2 */
  }


  /* Load the frame number into the shift registers, LSB first,
   * clocking each register once for every key bit loaded.
   * (The usual clock control rule is still disabled.)
   * For A5/2, signal when the last bit is being clocked in. */
  for (i = 0; i < 22; i++) {
    clock(1, i == 21); /* always clock */
    framebit = (frame >> i) & 1; /* The i-th bit of the frame # */
    R1 ^= framebit;
    R2 ^= framebit;
    R3 ^= framebit;
#ifdef A5_2
    R4 ^= framebit;
#endif /* A5_2 */
  }


  /* Run the shift registers for 100 clocks
   * to mix the keying material and frame number
   * together with output generation disabled,
   * so that there is sufficient avalanche.
   * We re-enable the majority-based clock control
   * rule from now on. */
  for (i = 0; i < 100; i++) {
    clock(0, 0);
  }
  /* For A5/2, we have to load the delayed output bit.  This does _not_
   * change the state of the registers.  For A5/1, this is a no-op. */
  getbit();


  /* Now the key is properly set up. */
}


/* Generate output.  We generate 228 bits of
 * keystream output.  The first 114 bits is for
 * the A->B frame; the next 114 bits is for the
 * B->A frame.  You allocate a 15-byte buffer
 * for each direction, and this function fills
 * it in. */
void run(byte AtoBkeystream[], byte BtoAkeystream[]) {
  int i;


  /* Zero out the output buffers. */
  for (i = 0; i <= 113 / 8; i++)
    AtoBkeystream[i] = BtoAkeystream[i] = 0;


  /* Generate 114 bits of keystream for the
   * A->B direction.  Store it, MSB first. */
  for (i = 0; i < 114; i++) {
    clock(0, 0);
    AtoBkeystream[i/8] |= getbit() << (7 - (i & 7));
  }


  /* Generate 114 bits of keystream for the
   * B->A direction.  Store it, MSB first. */
  for (i = 0; i < 114; i++) {
    clock(0, 0);
    BtoAkeystream[i/8] |= getbit() << (7 - (i & 7));
  }
}

void runA51(unsigned char AtoBkeystream[]) {
  int i;

  /* Zero out the output buffers. */
  for (i = 0; i < 114; i++)
    AtoBkeystream[i] = 0;


  /* Generate 114 bits of keystream for the
   * A->B direction.  Store it, MSB first. */
  for (i = 0; i < 114; i++) {
    clock(0, 0);
    AtoBkeystream[i] = getbit();
  }
}



#endif /* A5_1_2_H */
