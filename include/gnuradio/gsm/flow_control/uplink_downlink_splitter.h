/* -*- c++ -*- */
/*
 * Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_UPLINK_DOWNLINK_SPLITTER_H
#define INCLUDED_GSM_UPLINK_DOWNLINK_SPLITTER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API uplink_downlink_splitter : virtual public gr::block
{
public:
    typedef std::shared_ptr<uplink_downlink_splitter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::uplink_downlink_splitter.
     *
     * To avoid accidental use of raw pointers, gsm::uplink_downlink_splitter's
     * constructor is in a private implementation
     * class. gsm::uplink_downlink_splitter::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_UPLINK_DOWNLINK_SPLITTER_H */
