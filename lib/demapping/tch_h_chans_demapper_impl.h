/* -*- c++ -*- */
/*
 * Copyright 2018 by Roman Khassraf <rkhassraf@gmail.com>
 *           2018 by Vasil Velichkov <vvvelichkov@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_TCH_H_CHANS_DEMAPPER_IMPL_H
#define INCLUDED_GSM_TCH_H_CHANS_DEMAPPER_IMPL_H

#include <gnuradio/gsm/demapping/tch_h_chans_demapper.h>

namespace gr {
namespace gsm {

class tch_h_chans_demapper_impl : public tch_h_chans_demapper
{
private:
    unsigned int d_timeslot;
    unsigned int d_tch_h_channel;

    // Downlink
    uint32_t d_frame_numbers_dl[3][8];    // for checking consecutive frame numbers of tch
    uint32_t d_frame_numbers_sacch_dl[4]; // for checking consecutive frame numbers of sacch
    pmt::pmt_t d_bursts_dl[3][8];         // for tch output headers+bursts
    pmt::pmt_t d_bursts_sacch_dl[4];      // for sacch output bursts

    // Uplink
    uint32_t d_frame_numbers_ul[3][8];    // for checking consecutive frame numbers of tch
    uint32_t d_frame_numbers_sacch_ul[4]; // for checking consecutive frame numbers of sacch
    pmt::pmt_t d_bursts_ul[3][8];         // for tch output headers+bursts
    pmt::pmt_t d_bursts_sacch_ul[4];      // for sacch output bursts

    void sacch_tch_demapper(uint32_t fn_mod13,
                            u_int32_t fn_mod26,
                            uint32_t frame_nr,
                            pmt::pmt_t* d_bursts_sacch,
                            uint32_t* d_frame_numbers_sacch,
                            pmt::pmt_t d_bursts[3][8],
                            uint32_t d_frame_numbers[3][8],
                            pmt::pmt_t msg_out);

    void filter_tch_chans(pmt::pmt_t msg);

public:
    tch_h_chans_demapper_impl(unsigned int timeslot_nr, unsigned int tch_h_channel);
    ~tch_h_chans_demapper_impl();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TCH_H_CHANS_DEMAPPER_IMPL_H */
