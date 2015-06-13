/* -*- c++ -*- */
/*
 * @file
 * @author Roman Khassraf <rkhassraf@gmail.com>
 * @section LICENSE
 *
 * Gr-gsm is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gr-gsm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gr-gsm; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_GSM_TCH_F_DECODER_IMPL_H
#define INCLUDED_GSM_TCH_F_DECODER_IMPL_H

#include "VocoderFrame.h"
#include "GSM610Tables.h"
#include "GSM660Tables.h"
#include "GSM690Tables.h"
#include <grgsm/decoding/tch_f_decoder.h>


#define DATA_BLOCK_SIZE		184
#define PARITY_SIZE		    40
#define FLUSH_BITS_SIZE		4
#define PARITY_OUTPUT_SIZE (DATA_BLOCK_SIZE + PARITY_SIZE + FLUSH_BITS_SIZE)

#define CONV_INPUT_SIZE		PARITY_OUTPUT_SIZE
#define CONV_SIZE		    (2 * CONV_INPUT_SIZE)

#define BLOCKS			    8
#define iBLOCK_SIZE		    (CONV_SIZE / BLOCKS)

namespace gr {
    namespace gsm {

        class tch_f_decoder_impl : public tch_f_decoder
        {
            private:
                unsigned int d_collected_bursts_num;
                unsigned short interleave_trans[CONV_SIZE];
                pmt::pmt_t d_bursts[8];
                FILE * d_speech_file;
                enum tch_mode d_tch_mode;
                void decode(pmt::pmt_t msg);
            public:
                tch_f_decoder_impl(tch_mode mode, const std::string &file);
                ~tch_f_decoder_impl();
        };

    } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TCH_F_DECODER_IMPL_H */

