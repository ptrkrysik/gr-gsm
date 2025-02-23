/* -*- c++ -*- */
/*
 * Copyright 2015-2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CLOCK_OFFSET_CONTROL_H
#define INCLUDED_GSM_CLOCK_OFFSET_CONTROL_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API clock_offset_control : virtual public gr::block
{
public:
    typedef std::shared_ptr<clock_offset_control> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::clock_offset_control.
     *
     * To avoid accidental use of raw pointers, gsm::clock_offset_control's
     * constructor is in a private implementation
     * class. gsm::clock_offset_control::make is the public interface for
     * creating new instances.
     */
    static sptr make(float fc, float samp_rate, unsigned int osr = 4);
    virtual void set_fc(float fc) = 0;
    virtual void set_samp_rate(float samp_rate) = 0;
    virtual void set_osr(unsigned int osr) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CLOCK_OFFSET_CONTROL_H */
