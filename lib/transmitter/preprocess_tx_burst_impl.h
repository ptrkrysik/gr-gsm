/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_PREPROCESS_TX_BURST_IMPL_H
#define INCLUDED_GSM_PREPROCESS_TX_BURST_IMPL_H

#include <gnuradio/gsm/transmitter/preprocess_tx_burst.h>
#include <gnuradio/gsm/misc_utils/time_spec.h>
#include <gnuradio/gsm/misc_utils/fn_time.h>

namespace gr {
namespace gsm {

class preprocess_tx_burst_impl : public preprocess_tx_burst
{
private:
    void process_burst(pmt::pmt_t burst);

public:
    preprocess_tx_burst_impl();
    ~preprocess_tx_burst_impl();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_PREPROCESS_TX_BURST_IMPL_H */

