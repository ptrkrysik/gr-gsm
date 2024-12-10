/* -*- c++ -*- */
/*
 * Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_UNIVERSAL_CTRL_CHANS_DEMAPPER_IMPL_H
#define INCLUDED_GSM_UNIVERSAL_CTRL_CHANS_DEMAPPER_IMPL_H

#include <gnuradio/gsm/demapping/universal_ctrl_chans_demapper.h>
#include <vector>

namespace gr {
namespace gsm {

typedef enum { downlink = 0, uplink = 1 } um_radio_link_direction;
class universal_ctrl_chans_demapper_impl : public universal_ctrl_chans_demapper
{
private:
    // configuration of the block
    unsigned int d_timeslot_nr;
    std::vector<int> d_downlink_starts_fn_mod51;
    std::vector<int> d_downlink_channel_types;
    std::vector<int> d_downlink_subslots;

    std::vector<int> d_uplink_starts_fn_mod51;
    std::vector<int> d_uplink_channel_types;
    std::vector<int> d_uplink_subslots;

    // history of the downlink and uplink bursts
    uint32_t d_downlink_frame_numbers[4];
    pmt::pmt_t d_downlink_bursts[4];
    uint32_t d_uplink_frame_numbers[4];
    pmt::pmt_t d_uplink_bursts[4];

public:
    universal_ctrl_chans_demapper_impl(unsigned int timeslot_nr,
                                       const std::vector<int>& downlink_starts_fn_mod51,
                                       const std::vector<int>& downlink_channel_types,
                                       const std::vector<int>& downlink_subslots,
                                       const std::vector<int>& uplink_starts_fn_mod51,
                                       const std::vector<int>& uplink_channel_types,
                                       const std::vector<int>& uplink_subslots);
    ~universal_ctrl_chans_demapper_impl();

    void filter_ctrl_chans(pmt::pmt_t msg);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_UNIVERSAL_CTRL_CHANS_DEMAPPER_IMPL_H */
