/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_TIMESLOT_FILTER_H
#define INCLUDED_GSM_BURST_TIMESLOT_FILTER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <gnuradio/gsm/flow_control/common.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API burst_timeslot_filter : virtual public gr::block
{
public:
    typedef std::shared_ptr<burst_timeslot_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::burst_timeslot_filter.
     *
     * To avoid accidental use of raw pointers, gsm::burst_timeslot_filter's
     * constructor is in a private implementation
     * class. gsm::burst_timeslot_filter::make is the public interface for
     * creating new instances.
     */
    static sptr make(unsigned int timeslot);

    /* External API */
    virtual unsigned int get_tn(void) = 0;
    virtual unsigned int set_tn(unsigned int tn) = 0;

    /* Filtering policy */
    virtual filter_policy get_policy(void) = 0;
    virtual filter_policy set_policy(filter_policy policy) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_TIMESLOT_FILTER_H */
