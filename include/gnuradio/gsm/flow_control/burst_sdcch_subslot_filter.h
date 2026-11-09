/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_SDCCH_SUBSLOT_FILTER_H
#define INCLUDED_GSM_BURST_SDCCH_SUBSLOT_FILTER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <gnuradio/gsm/flow_control/common.h>

namespace gr {
namespace gsm {

enum subslot_filter_mode { SS_FILTER_SDCCH8, SS_FILTER_SDCCH4 };

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API burst_sdcch_subslot_filter : virtual public gr::block
{
public:
    typedef std::shared_ptr<burst_sdcch_subslot_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::burst_sdcch_subslot_filter.
     *
     * To avoid accidental use of raw pointers, gsm::burst_sdcch_subslot_filter's
     * constructor is in a private implementation
     * class. gsm::burst_sdcch_subslot_filter::make is the public interface for
     * creating new instances.
     */
    static sptr make(subslot_filter_mode mode, unsigned int subslot);

    /* External API */
    virtual unsigned int get_ss(void) = 0;
    virtual unsigned int set_ss(unsigned int ss) = 0;

    virtual subslot_filter_mode get_mode(void) = 0;
    virtual subslot_filter_mode set_mode(subslot_filter_mode mode) = 0;

    /* Filtering policy */
    virtual filter_policy get_policy(void) = 0;
    virtual filter_policy set_policy(filter_policy policy) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_SDCCH_SUBSLOT_FILTER_H */
