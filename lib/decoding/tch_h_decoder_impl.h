/* -*- c++ -*- */
/*
 * Copyright 2018 by Vasil Velichkov <vvvelichkov@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_TCH_H_DECODER_IMPL_H
#define INCLUDED_GSM_TCH_H_DECODER_IMPL_H

#include "tch_f_decoder_impl.h"
#include <gnuradio/gsm/decoding/tch_h_decoder.h>

namespace gr {
namespace gsm {

class tch_h_decoder_impl : public tch_h_decoder
{
private:
    unsigned int d_collected_bursts_num;
    pmt::pmt_t d_bursts[8];

    enum tch_mode d_tch_mode;
    unsigned int d_sub_channel;

    std::vector<uint8_t> d_multi_rate_codes;

    bool d_boundary_check;
    bool d_boundary_decode;
    bool d_header_sent;

    uint8_t d_ft;
    uint8_t d_cmr;

    void decode(pmt::pmt_t msg);

public:
    tch_h_decoder_impl(unsigned int sub_channel, std::string multi_rate, bool boundary_check = false);
    ~tch_h_decoder_impl();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TCH_H_DECODER_IMPL_H */
