#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gsm_constants.h"

#define DEBUGF(a...)	{ \
    fprintf(stderr, "%s:%d ", __FILE__, __LINE__); \
    fprintf(stderr, a); \
} while (0)



/*
 * Synchronization channel.
 *
 * Timeslot Repeat length  Frame Number (mod repeat length)
 * 0  51   1, 11, 21, 31, 41
 */

/*
 * Parity (FIRE) for the GSM SCH.
 *
 *  g(x) = x^10 + x^8 + x^6 + x^5 + x^4 + x^2 + 1
 */
#define DATA_BLOCK_SIZE  25
#define PARITY_SIZE  10
#define TAIL_BITS_SIZE  4
#define PARITY_OUTPUT_SIZE (DATA_BLOCK_SIZE + PARITY_SIZE + TAIL_BITS_SIZE)

static const unsigned char parity_polynomial[PARITY_SIZE + 1] = {
  1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1
};

static const unsigned char parity_remainder[PARITY_SIZE] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};


static void parity_encode(unsigned char *d, unsigned char *p)
{

  unsigned int i;
  unsigned char buf[DATA_BLOCK_SIZE + PARITY_SIZE], *q;

  memcpy(buf, d, DATA_BLOCK_SIZE);
  memset(buf + DATA_BLOCK_SIZE, 0, PARITY_SIZE);

  for (q = buf; q < buf + DATA_BLOCK_SIZE; q++)
    if (*q)
      for (i = 0; i < PARITY_SIZE + 1; i++)
        q[i] ^= parity_polynomial[i];
  for (i = 0; i < PARITY_SIZE; i++)
    p[i] = !buf[DATA_BLOCK_SIZE + i];
}


static int parity_check(unsigned char *d)
{

  unsigned int i;
  unsigned char buf[DATA_BLOCK_SIZE + PARITY_SIZE], *q;

  memcpy(buf, d, DATA_BLOCK_SIZE + PARITY_SIZE);

  for (q = buf; q < buf + DATA_BLOCK_SIZE; q++)
    if (*q)
      for (i = 0; i < PARITY_SIZE + 1; i++)
        q[i] ^= parity_polynomial[i];
  return memcmp(buf + DATA_BLOCK_SIZE, parity_remainder, PARITY_SIZE);
}


/*
 * Convolutional encoding and Viterbi decoding for the GSM SCH.
 * (Equivalent to the GSM SACCH.)
 *
 *  G_0 = 1 + x^3 + x^4
 *  G_1 = 1 + x + x^3 + x^4
 *
 * i.e.,
 *
 *  c_{2k} = u_k + u_{k - 3} + u_{k - 4}
 *  c_{2k + 1} = u_k + u_{k - 1} + u_{k - 3} + u_{k - 4}
 */
#define CONV_INPUT_SIZE  PARITY_OUTPUT_SIZE
#define CONV_SIZE  (2 * CONV_INPUT_SIZE)
#define K   5
#define MAX_ERROR  (2 * CONV_INPUT_SIZE + 1)


/*
 * Given the current state and input bit, what are the output bits?
 *
 *  encode[current_state][input_bit]
 */
static const unsigned int encode[1 << (K - 1)][2] = {
  {0, 3}, {3, 0}, {3, 0}, {0, 3},
  {0, 3}, {3, 0}, {3, 0}, {0, 3},
  {1, 2}, {2, 1}, {2, 1}, {1, 2},
  {1, 2}, {2, 1}, {2, 1}, {1, 2}
};


/*
 * Given the current state and input bit, what is the next state?
 *
 *  next_state[current_state][input_bit]
 */
static const unsigned int next_state[1 << (K - 1)][2] = {
  {0, 8}, {0, 8}, {1, 9}, {1, 9},
  {2, 10}, {2, 10}, {3, 11}, {3, 11},
  {4, 12}, {4, 12}, {5, 13}, {5, 13},
  {6, 14}, {6, 14}, {7, 15}, {7, 15}
};


/*
 * Given the previous state and the current state, what input bit caused
 * the transition?  If it is impossible to transition between the two
 * states, the value is 2.
 *
 *  prev_next_state[previous_state][current_state]
 */
static const unsigned int prev_next_state[1 << (K - 1)][1 << (K - 1)] = {
  { 0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  2},
  { 0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  2},
  { 2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2},
  { 2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2},
  { 2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2},
  { 2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2},
  { 2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2},
  { 2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2},
  { 2,  2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2},
  { 2,  2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2},
  { 2,  2,  2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2},
  { 2,  2,  2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2},
  { 2,  2,  2,  2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2},
  { 2,  2,  2,  2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1,  2},
  { 2,  2,  2,  2,  2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1},
  { 2,  2,  2,  2,  2,  2,  2,  0,  2,  2,  2,  2,  2,  2,  2,  1}
};


static inline unsigned int hamming_distance2(unsigned int w)
{

  return (w & 1) + !!(w & 2);
}


static void conv_encode(unsigned char *data, unsigned char *output)
{

  unsigned int i, state = 0, o;

  // encode data
  for (i = 0; i < CONV_INPUT_SIZE; i++) {
    o = encode[state][data[i]];
    state = next_state[state][data[i]];
    *output++ = !!(o & 2);
    *output++ = o & 1;
  }
}


static int conv_decode(unsigned char *data, unsigned char *output)
{

  int i, t;
  unsigned int rdata, state, nstate, b, o, distance, accumulated_error,
  min_state, min_error, cur_state;

  unsigned int ae[1 << (K - 1)];
  unsigned int nae[1 << (K - 1)]; // next accumulated error
  unsigned int state_history[1 << (K - 1)][CONV_INPUT_SIZE + 1];

  // initialize accumulated error, assume starting state is 0
  for (i = 0; i < (1 << (K - 1)); i++)
    ae[i] = nae[i] = MAX_ERROR;
  ae[0] = 0;

  // build trellis
  for (t = 0; t < CONV_INPUT_SIZE; t++) {

    // get received data symbol
    rdata = (data[2 * t] << 1) | data[2 * t + 1];

    // for each state
    for (state = 0; state < (1 << (K - 1)); state++) {

      // make sure this state is possible
      if (ae[state] >= MAX_ERROR)
        continue;

      // find all states we lead to
      for (b = 0; b < 2; b++) {

        // get next state given input bit b
        nstate = next_state[state][b];

        // find output for this transition
        o = encode[state][b];

        // calculate distance from received data
        distance = hamming_distance2(rdata ^ o);

        // choose surviving path
        accumulated_error = ae[state] + distance;
        if (accumulated_error < nae[nstate]) {

          // save error for surviving state
          nae[nstate] = accumulated_error;

          // update state history
          state_history[nstate][t + 1] = state;
        }
      }
    }

    // get accumulated error ready for next time slice
    for (i = 0; i < (1 << (K - 1)); i++) {
      ae[i] = nae[i];
      nae[i] = MAX_ERROR;
    }
  }

  // the final state is the state with the fewest errors
  min_state = (unsigned int) - 1;
  min_error = MAX_ERROR;
  for (i = 0; i < (1 << (K - 1)); i++) {
    if (ae[i] < min_error) {
      min_state = i;
      min_error = ae[i];
    }
  }

  // trace the path
  cur_state = min_state;
  for (t = CONV_INPUT_SIZE; t >= 1; t--) {
    min_state = cur_state;
    cur_state = state_history[cur_state][t]; // get previous
    output[t - 1] = prev_next_state[cur_state][min_state];
  }

  // return the number of errors detected (hard-decision)
  return min_error;
}


int decode_sch(const unsigned char *buf, int * t1_o, int * t2_o, int * t3_o, int * ncc_o, int * bcc_o)
{

  int errors, t1, t2, t3p, t3, ncc, bcc;
  unsigned char data[CONV_SIZE], decoded_data[PARITY_OUTPUT_SIZE];

  // extract encoded data from synchronization burst
  /* buf, 39 bit */
  /* buf + 39 + 64 = 103, 39 */
  memcpy(data, buf, SCH_DATA_LEN);
  memcpy(data + SCH_DATA_LEN, buf + SCH_DATA_LEN + N_SYNC_BITS, SCH_DATA_LEN);

  // Viterbi decode
  if (errors = conv_decode(data, decoded_data)) {
    // fprintf(stderr, "error: sch: conv_decode (%d)\n", errors);
    //DEBUGF("ERR: conv_decode %d\n", errors);
    //return errors;
  }

  // check parity
  if (parity_check(decoded_data)) {
    // fprintf(stderr, "error: sch: parity failed\n");
    //DEBUGF("ERR: parity_check failed\n");
    return 1;
  }

  // Synchronization channel information, 44.018 page 171. (V7.2.0)
  ncc =
    (decoded_data[ 7] << 2)  |
    (decoded_data[ 6] << 1)  |
    (decoded_data[ 5] << 0);
  bcc = 
    (decoded_data[ 4] << 2)  |
    (decoded_data[ 3] << 1)  |
    (decoded_data[ 2] << 0);
  t1 =
    (decoded_data[ 1] << 10) |
    (decoded_data[ 0] << 9)  |
    (decoded_data[15] << 8)  |
    (decoded_data[14] << 7)  |
    (decoded_data[13] << 6)  |
    (decoded_data[12] << 5)  |
    (decoded_data[11] << 4)  |
    (decoded_data[10] << 3)  |
    (decoded_data[ 9] << 2)  |
    (decoded_data[ 8] << 1)  |
    (decoded_data[23] << 0);
  t2 =
    (decoded_data[22] << 4)  |
    (decoded_data[21] << 3)  |
    (decoded_data[20] << 2)  |
    (decoded_data[19] << 1)  |
    (decoded_data[18] << 0);
  t3p =
    (decoded_data[17] << 2)  |
    (decoded_data[16] << 1)  |
    (decoded_data[24] << 0);

  t3 = 10 * t3p + 1;

  // modulo arithmetic t3 - t2 mod 26
//  tt = ((t3 + 26) - t2) % 26;

//  fn = (51 * 26 * t1) + (51 * tt) + t3;

  /*
   * BSIC: Base Station Identification Code
   *  BCC: Base station Color Code
   *  NCC: Network Color Code
   *
   * FN: Frame Number
   */

//  printf("bsic: %x (bcc: %u; ncc: %u)\tFN: %u\n", bsic, bsic & 7,
//          (bsic >> 3) & 7, fn);

//   if (fn_o)
//     *fn_o = fn;
//   if (bsic_o)
  if (t1_o && t2_o && t3_o && ncc_o && bcc_o) {
    *t1_o = t1;
    *t2_o = t2;
    *t3_o = t3;
    *bcc_o = bcc;
    *ncc_o = ncc;
  }

  return 0;
}
