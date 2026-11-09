/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 * Copyright 2014 Vadim Yanitskiy <axilirator@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_TO_FN_TIME_IMPL_H
#define INCLUDED_GSM_BURST_TO_FN_TIME_IMPL_H

#include <gnuradio/gsm/misc_utils/burst_to_fn_time.h>

namespace gr {
namespace gsm {

class burst_to_fn_time_impl : public burst_to_fn_time
{
private:
    void handle_burst(pmt::pmt_t msg_in);

public:
    burst_to_fn_time_impl(void);
    ~burst_to_fn_time_impl(void);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_TO_FN_TIME_IMPL_H */
