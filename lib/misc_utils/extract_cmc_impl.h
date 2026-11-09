/* -*- c++ -*- */
/*
 * Copyright 2014 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_EXTRACT_CMC_IMPL_H
#define INCLUDED_GSM_EXTRACT_CMC_IMPL_H

#include <gnuradio/gsm/misc_utils/extract_cmc.h>
#include <vector>

namespace gr {
namespace gsm {
class extract_cmc_impl : public extract_cmc
{
private:
    void process_messages(pmt::pmt_t msg);
    std::vector<int> d_framenumbers;
    std::vector<int> d_a5_versions;
    std::vector<int> d_start_ciphering;
public:
    virtual std::vector<int> get_framenumbers();
    virtual std::vector<int> get_a5_versions();
    virtual std::vector<int> get_start_ciphering();
    extract_cmc_impl();
    ~extract_cmc_impl();
};
} // namespace gsm
} // namespace gr
#endif /* INCLUDED_GSM_EXTRACT_CMC_IMPL_H */
