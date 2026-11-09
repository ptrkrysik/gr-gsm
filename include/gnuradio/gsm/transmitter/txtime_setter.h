/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_GSM_TXTIME_SETTER_H
#define INCLUDED_GSM_TXTIME_SETTER_H

#include <gnuradio/gsm/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API txtime_setter : virtual public gr::block
{
public:
    typedef std::shared_ptr<txtime_setter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::txtime_setter.
     *
     * To avoid accidental use of raw pointers, gsm::txtime_setter's
     * constructor is in a private implementation
     * class. gsm::txtime_setter::make is the public interface for
     * creating new instances.
     */
    static sptr make(uint32_t init_fn, uint64_t init_time_secs, double init_time_fracs, uint64_t time_hint_secs, double time_hint_fracs, double timing_advance, double delay_correction);
    virtual void set_fn_time_reference(uint32_t fn, uint32_t ts, uint64_t time_secs, double time_fracs) = 0;
    virtual void set_time_hint(uint64_t time_hint_secs, double time_hint_fracs) = 0;
    virtual void set_delay_correction(double delay_correction) = 0;
    virtual void set_timing_advance(double timing_advance) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TXTIME_SETTER_H */

