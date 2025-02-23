/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_SOURCE_IMPL_H
#define INCLUDED_GSM_BURST_SOURCE_IMPL_H

#define BURST_SIZE 148

#include <gnuradio/gsm/qa_utils/burst_source.h>
#include <fstream>


namespace gr {
namespace gsm {

class burst_source_impl : public burst_source
{
private:
    std::shared_ptr<gr::thread::thread> d_thread;
    std::vector<int> d_framenumbers;
    std::vector<int> d_timeslots;
    std::vector<std::string> d_burst_data;
    bool d_finished;
    uint16_t d_arfcn;
    void run();

public:
    burst_source_impl(const std::vector<int>& framenumbers,
                      const std::vector<int>& timeslots,
                      const std::vector<std::string>& burst_data);
    ~burst_source_impl();
    virtual void set_framenumbers(const std::vector<int>& framenumbers);
    virtual void set_timeslots(const std::vector<int>& timeslots);
    virtual void set_burst_data(const std::vector<std::string>& burst_data);
    virtual void set_arfcn(uint16_t arfcn);
    bool start();
    bool stop();
    bool finished();
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_SOURCE_IMPL_H */
