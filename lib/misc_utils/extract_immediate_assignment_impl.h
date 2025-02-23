/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_EXTRACT_IMMEDIATE_ASSIGNMENT_IMPL_H
#define INCLUDED_GSM_EXTRACT_IMMEDIATE_ASSIGNMENT_IMPL_H

#include <gnuradio/gsm/misc_utils/extract_immediate_assignment.h>
#include <map>
#include <vector>

namespace gr {
namespace gsm {

class immediate_assignment
{
public:
    uint32_t arfcn_id;
    uint32_t frame_nr;
    std::string channel_type;
    uint8_t timeslot;
    uint8_t subchannel;
    uint8_t tseq;
    uint8_t hopping;
    uint8_t maio;
    uint8_t hsn;
    uint16_t arfcn;
    uint8_t timing_advance;
    std::string mobile_allocation;

    immediate_assignment()
        : arfcn_id(0),
          frame_nr(0),
          channel_type("unknown"),
          timeslot(0),
          subchannel(0),
          tseq(0),
          hopping(false),
          maio(0),
          hsn(0),
          arfcn(0),
          timing_advance(0),
          mobile_allocation(""){};
    ~immediate_assignment(){};
};

typedef std::map<uint32_t, immediate_assignment> immediate_assignment_map;

class extract_immediate_assignment_impl : public extract_immediate_assignment
{
private:
    void process_message(pmt::pmt_t msg);
    immediate_assignment_map d_assignment_map;
    bool d_print_immediate_assignments;
    bool d_ignore_gprs;
    bool d_unique_references;

public:
    virtual std::vector<int> get_arfcn_ids();
    virtual std::vector<int> get_frame_numbers();
    virtual std::vector<std::string> get_channel_types();
    virtual std::vector<int> get_timeslots();
    virtual std::vector<int> get_subchannels();
    virtual std::vector<int> get_tseqs();
    virtual std::vector<int> get_hopping();
    virtual std::vector<int> get_maios();
    virtual std::vector<int> get_hsns();
    virtual std::vector<int> get_arfcns();
    virtual std::vector<int> get_timing_advances();
    virtual std::vector<std::string> get_mobile_allocations();
    extract_immediate_assignment_impl(bool print_immediate_assignments = false,
                                      bool ignore_gprs = false,
                                      bool unique_references = false);
    ~extract_immediate_assignment_impl();
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_EXTRACT_IMMEDIATE_ASSIGNMENT_IMPL_H */
