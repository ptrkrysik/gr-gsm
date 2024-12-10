/* -*- c++ -*- */
/*
 * Copyright 2015 Pieter Robyns <pieter.robyns@uhasselt.be>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CX_CHANNEL_HOPPER_IMPL_H
#define INCLUDED_GSM_CX_CHANNEL_HOPPER_IMPL_H

#include <gnuradio/gsm/receiver/cx_channel_hopper.h>
#include <vector>

namespace gr {
namespace gsm {

class cx_channel_hopper_impl : public cx_channel_hopper
{
private:
    std::vector<int> d_ma; // Mobile Allocation list. Contains all channels that are used while channel hopping
    int d_maio;            // Mobile Allocation Index Offset
    int d_hsn;             // Hopping Sequence Number
    int d_narfcn;          // Length of d_ma

    int calculate_ma_sfh(int maio, int hsn, int n, int fn);
    void assemble_bursts(pmt::pmt_t msg);

public:
    cx_channel_hopper_impl(const std::vector<int>& ma, int maio, int hsn);
    ~cx_channel_hopper_impl();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CX_CHANNEL_HOPPER_IMPL_H */
