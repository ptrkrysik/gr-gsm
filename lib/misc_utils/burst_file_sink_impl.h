/* -*- c++ -*- */
/*
 * Copyright 2015 by Roman Khassraf <rkhassraf@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_FILE_SINK_IMPL_H
#define INCLUDED_GSM_BURST_FILE_SINK_IMPL_H

#include <gnuradio/gsm/misc_utils/burst_file_sink.h>
#include <fstream>

namespace gr {
namespace gsm {

class burst_file_sink_impl : public burst_file_sink
{
private:
    std::ofstream d_output_file;

public:
    burst_file_sink_impl(const std::string& filename);
    ~burst_file_sink_impl();
    void process_burst(pmt::pmt_t msg);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_FILE_SINK_IMPL_H */
