/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MESSAGE_SOURCE_H
#define INCLUDED_GSM_MESSAGE_SOURCE_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API message_source : virtual public gr::block
{
public:
    typedef std::shared_ptr<message_source> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of grgsm::message_source.
     *
     * To avoid accidental use of raw pointers, grgsm::message_source's
     * constructor is in a private implementation
     * class. grgsm::message_source::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::vector<std::string>& msg_data);

    virtual void set_msg_data(const std::vector<std::string>& msg_data) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_SOURCE_H */
