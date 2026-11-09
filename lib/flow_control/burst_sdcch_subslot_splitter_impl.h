/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_SDCCH_SUBSLOT_SPLITTER_IMPL_H
#define INCLUDED_GSM_BURST_SDCCH_SUBSLOT_SPLITTER_IMPL_H

#include <gnuradio/gsm/flow_control/burst_sdcch_subslot_splitter.h>

namespace gr {
namespace gsm {

class burst_sdcch_subslot_splitter_impl : public burst_sdcch_subslot_splitter
{
private:
    splitter_mode d_mode;

public:
    burst_sdcch_subslot_splitter_impl(splitter_mode mode);
    ~burst_sdcch_subslot_splitter_impl();
    void process_burst(pmt::pmt_t msg);

    /* External API */
    splitter_mode get_mode(void);
    splitter_mode set_mode(splitter_mode mode);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_SDCCH_SUBSLOT_SPLITTER_IMPL_H */
