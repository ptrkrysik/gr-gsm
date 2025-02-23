/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MESSAGE_FILE_SINK_IMPL_H
#define INCLUDED_GSM_MESSAGE_FILE_SINK_IMPL_H

#include <gnuradio/gsm/misc_utils/message_file_sink.h>
#include <fstream>

namespace gr {
namespace gsm {

class message_file_sink_impl : public message_file_sink
{
private:
    std::ofstream d_output_file;

public:
    message_file_sink_impl(const std::string& filename);
    ~message_file_sink_impl();
    void process_message(pmt::pmt_t msg);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_FILE_SINK_IMPL_H */
