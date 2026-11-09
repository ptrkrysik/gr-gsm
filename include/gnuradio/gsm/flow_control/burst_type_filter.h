/* -*- c++ -*- */
/*
 * Copyright 2017 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_TYPE_FILTER_H
#define INCLUDED_GSM_BURST_TYPE_FILTER_H

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
class GSM_API burst_type_filter : virtual public gr::block
{
public:
    typedef std::shared_ptr<burst_type_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::burst_type_filter.
     *
     * To avoid accidental use of raw pointers, gsm::burst_type_filter's
     * constructor is in a private implementation
     * class. gsm::burst_type_filter::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::vector<uint8_t>& selected_burst_types);

    /* External API */
    /* Filtering policy */
    virtual filter_policy get_policy(void) = 0;
    virtual filter_policy set_policy(filter_policy policy) = 0;
    virtual void set_selected_burst_types(const std::vector<uint8_t>& selected_burst_types) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_TYPE_FILTER_H */
