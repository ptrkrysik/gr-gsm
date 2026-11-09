/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_DUMMY_BURST_FILTER_H
#define INCLUDED_GSM_DUMMY_BURST_FILTER_H

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
class GSM_API dummy_burst_filter : virtual public gr::block
{
public:
    typedef std::shared_ptr<dummy_burst_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::dummy_burst_filter.
     *
     * To avoid accidental use of raw pointers, gsm::dummy_burst_filter's
     * constructor is in a private implementation
     * class. gsm::dummy_burst_filter::make is the public interface for
     * creating new instances.
     */
    static sptr make();

    /* External API */
    /* Filtering policy */
    virtual filter_policy get_policy(void) = 0;
    virtual filter_policy set_policy(filter_policy policy) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_DUMMY_BURST_FILTER_H */
