/* -*- c++ -*- */
/*
 * Copyright 2015 Pieter Robyns <pieter.robyns@uhasselt.be>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CX_CHANNEL_HOPPER_H
#define INCLUDED_GSM_CX_CHANNEL_HOPPER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <vector>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API cx_channel_hopper : virtual public gr::block
{
public:
    typedef std::shared_ptr<cx_channel_hopper> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::cx_channel_hopper.
     *
     * To avoid accidental use of raw pointers, gsm::cx_channel_hopper's
     * constructor is in a private implementation
     * class. gsm::cx_channel_hopper::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::vector<int>& ma, int maio, int hsn);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CX_CHANNEL_HOPPER_H */
