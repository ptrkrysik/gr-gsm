/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MESSAGE_SINK_IMPL_H
#define INCLUDED_GSM_MESSAGE_SINK_IMPL_H

#include <gnuradio/gsm/qa_utils/message_sink.h>

namespace gr {
namespace gsm {

class message_sink_impl : public message_sink
{
private:
    std::vector<std::string> d_messages;

public:
    message_sink_impl();
    ~message_sink_impl();
    void process_message(pmt::pmt_t msg);
    virtual std::vector<std::string> get_messages();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_SINK_IMPL_H */
