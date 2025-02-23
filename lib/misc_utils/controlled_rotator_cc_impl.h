/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CONTROLLED_ROTATOR_CC_IMPL_H
#define INCLUDED_GSM_CONTROLLED_ROTATOR_CC_IMPL_H

#include <gnuradio/blocks/rotator.h>
#include <gnuradio/gsm/misc_utils/controlled_rotator_cc.h>

namespace gr {
namespace gsm {

class controlled_rotator_cc_impl : public controlled_rotator_cc
{
private:
    gr_complex d_phase_inc;
    //      double d_samp_rate;
    blocks::rotator d_r;

public:
    controlled_rotator_cc_impl(double phase_inc);
    ~controlled_rotator_cc_impl();

    virtual void set_phase_inc(double phase_inc);
    //      virtual void set_samp_rate(double samp_rate);

    // Where all the action really happens
    int work(int noutput_items, gr_vector_const_void_star& input_items, gr_vector_void_star& output_items);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CONTROLLED_ROTATOR_CC_IMPL_H */
