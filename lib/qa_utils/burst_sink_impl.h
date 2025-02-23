/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_SINK_IMPL_H
#define INCLUDED_GSM_BURST_SINK_IMPL_H

#include <gnuradio/gsm/qa_utils/burst_sink.h>
#include <fstream>

namespace gr {
namespace gsm {

class burst_sink_impl : public burst_sink
{
private:
    std::vector<int> d_framenumbers;
    std::vector<int> d_timeslots;
    std::vector<std::string> d_burst_data;
    pmt::pmt_t d_bursts;
    std::vector<uint8_t> d_sub_types;
    std::vector<uint8_t> d_sub_slots;

public:
    burst_sink_impl();
    ~burst_sink_impl();
    void process_burst(pmt::pmt_t msg);
    virtual std::vector<int> get_framenumbers();
    virtual std::vector<int> get_timeslots();
    virtual std::vector<std::string> get_burst_data();
    virtual pmt::pmt_t get_bursts();
    virtual std::vector<uint8_t> get_sub_types();
    virtual std::vector<uint8_t> get_sub_slots();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_SINK_IMPL_H */
