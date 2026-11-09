/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_GEN_TEST_AB_IMPL_H
#define INCLUDED_GSM_GEN_TEST_AB_IMPL_H

#include <gnuradio/gsm/transmitter/gen_test_ab.h>
#include <gnuradio/gsm/misc_utils/time_spec.h>
#include <gnuradio/gsm/misc_utils/fn_time.h>

namespace gr {
namespace gsm {

class gen_test_ab_impl : public gen_test_ab
{
private:
    void generate_ab(pmt::pmt_t burst);

public:
    gen_test_ab_impl();
    ~gen_test_ab_impl();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_GEN_TEST_AB_IMPL_H */

