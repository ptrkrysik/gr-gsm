/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MESSAGE_FILE_SOURCE_IMPL_H
#define INCLUDED_GSM_MESSAGE_FILE_SOURCE_IMPL_H

#include <gnuradio/gsm/misc_utils/message_file_source.h>
#include <fstream>

namespace gr {
namespace gsm {

class message_file_source_impl : public message_file_source
{
private:
    std::shared_ptr<gr::thread::thread> d_thread;
    std::ifstream d_input_file;
    bool d_finished;
    void run();

public:
    message_file_source_impl(const std::string& filename);
    ~message_file_source_impl();
    bool start();
    bool stop();
    bool finished();
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_FILE_SOURCE_IMPL_H */
