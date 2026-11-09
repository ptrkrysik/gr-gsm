/* -*- c++ -*- */
/*
 * Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_UPLINK_DOWNLINK_SPLITTER_IMPL_H
#define INCLUDED_GSM_UPLINK_DOWNLINK_SPLITTER_IMPL_H

#include <gnuradio/gsm/flow_control/uplink_downlink_splitter.h>

namespace gr {
namespace gsm {

class uplink_downlink_splitter_impl : public uplink_downlink_splitter
{
public:
    uplink_downlink_splitter_impl();
    ~uplink_downlink_splitter_impl();

    void process_msg(pmt::pmt_t msg);
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_UPLINK_DOWNLINK_SPLITTER_IMPL_H */
