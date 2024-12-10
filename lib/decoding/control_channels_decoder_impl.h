/* -*- c++ -*- */
/*
 * Copyright 2014,2024 by Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CONTROL_CHANNELS_DECODER_IMPL_H
#define INCLUDED_GSM_CONTROL_CHANNELS_DECODER_IMPL_H

#include <gnuradio/gsm/decoding/control_channels_decoder.h>
extern "C" {
#include "osmocom/coding/gsm0503_coding.h"
}

namespace gr {
namespace gsm {

class control_channels_decoder_impl : public control_channels_decoder
{
private:
    unsigned int d_collected_bursts_num;
    pmt::pmt_t d_bursts[4];

    void decode(pmt::pmt_t msg);

public:
    control_channels_decoder_impl();
    ~control_channels_decoder_impl();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CONTROL_CHANNELS_DECODER_IMPL_H */
