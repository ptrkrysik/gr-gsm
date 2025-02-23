/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MESSAGE_SOURCE_IMPL_H
#define INCLUDED_GSM_MESSAGE_SOURCE_IMPL_H

#include <gnuradio/gsm/qa_utils/message_source.h>

namespace gr {
namespace gsm {

class message_source_impl : public message_source
{
private:
    std::shared_ptr<gr::thread::thread> d_thread;
    std::vector<std::vector<uint8_t>> d_msgs;
    bool d_finished;
    void run();

public:
    message_source_impl(const std::vector<std::string>& msg_data);
    ~message_source_impl();
    virtual void set_msg_data(const std::vector<std::string>& msg_data);
    bool start();
    bool stop();
    bool finished();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_SOURCE_IMPL_H */
