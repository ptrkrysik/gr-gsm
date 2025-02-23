/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_EXTRACT_IMMEDIATE_ASSIGNMENT_H
#define INCLUDED_GSM_EXTRACT_IMMEDIATE_ASSIGNMENT_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <vector>

namespace gr {
namespace gsm {
/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API extract_immediate_assignment : virtual public gr::block
{
public:
    typedef std::shared_ptr<extract_immediate_assignment> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::extract_immediate_assignment.
     */
    static sptr
    make(bool print_immediate_assignments = false, bool ignore_gprs = false, bool unique_references = false);
    virtual std::vector<int> get_frame_numbers() = 0;
    virtual std::vector<int> get_arfcn_ids() = 0;
    virtual std::vector<std::string> get_channel_types() = 0;
    virtual std::vector<int> get_timeslots() = 0;
    virtual std::vector<int> get_subchannels() = 0;
    virtual std::vector<int> get_tseqs() = 0;
    virtual std::vector<int> get_hopping() = 0;
    virtual std::vector<int> get_maios() = 0;
    virtual std::vector<int> get_hsns() = 0;
    virtual std::vector<int> get_arfcns() = 0;
    virtual std::vector<int> get_timing_advances() = 0;
    virtual std::vector<std::string> get_mobile_allocations() = 0;
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_EXTRACT_IMMEDIATE_ASSIGNMENT_H */
