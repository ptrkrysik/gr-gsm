/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_TIMESLOT_SPLITTER_H
#define INCLUDED_GSM_BURST_TIMESLOT_SPLITTER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API burst_timeslot_splitter : virtual public gr::block
{
public:
    typedef std::shared_ptr<burst_timeslot_splitter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::burst_timeslot_splitter.
     *
     * To avoid accidental use of raw pointers, gsm::burst_timeslot_splitter's
     * constructor is in a private implementation
     * class. gsm::burst_timeslot_splitter::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_TIMESLOT_SPLITTER_H */
