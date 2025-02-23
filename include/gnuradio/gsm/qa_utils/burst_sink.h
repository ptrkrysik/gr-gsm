/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_SINK_H
#define INCLUDED_GSM_BURST_SINK_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <pmt/pmt.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API burst_sink : virtual public gr::block
{
public:
    typedef std::shared_ptr<burst_sink> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of grgsm::burst_sink.
     *
     * To avoid accidental use of raw pointers, grgsm::burst_sink's
     * constructor is in a private implementation
     * class. grgsm::burst_sink::make is the public interface for
     * creating new instances.
     */
    static sptr make();

    virtual std::vector<int> get_framenumbers() = 0;
    virtual std::vector<int> get_timeslots() = 0;
    virtual std::vector<std::string> get_burst_data() = 0;
    virtual pmt::pmt_t get_bursts() = 0;
    virtual std::vector<uint8_t> get_sub_types() = 0;
    virtual std::vector<uint8_t> get_sub_slots() = 0;
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_SINK_H */
