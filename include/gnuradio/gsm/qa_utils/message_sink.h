/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MESSAGE_SINK_H
#define INCLUDED_GSM_MESSAGE_SINK_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API message_sink : virtual public gr::block
{
public:
    typedef std::shared_ptr<message_sink> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of grgsm::message_sink.
     *
     * To avoid accidental use of raw pointers, grgsm::message_sink's
     * constructor is in a private implementation
     * class. grgsm::message_sink::make is the public interface for
     * creating new instances.
     */
    static sptr make();

    virtual std::vector<std::string> get_messages() = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_SINK_H */
