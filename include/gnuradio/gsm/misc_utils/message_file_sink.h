/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MESSAGE_FILE_SINK_H
#define INCLUDED_GSM_MESSAGE_FILE_SINK_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API message_file_sink : virtual public gr::block
{
public:
    typedef std::shared_ptr<message_file_sink> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of grgsm::message_file_sink.
     */
    static sptr make(const std::string& filename);
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_FILE_SINK_H */
