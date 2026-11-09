/* -*- c++ -*- */
/*
 * Copyright 2014 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_COLLECT_SYSTEM_INFO_IMPL_H
#define INCLUDED_GSM_COLLECT_SYSTEM_INFO_IMPL_H

#include <gnuradio/gsm/misc_utils/collect_system_info.h>
#include <vector>

namespace gr {
namespace gsm {
class collect_system_info_impl : public collect_system_info
{
private:
    void process_messages(pmt::pmt_t msg);
    std::vector<int> d_framenumbers;
    std::vector<std::string> d_sit_types;
    std::vector<std::string> d_sit_data;
    std::string get_hex_string(uint8_t * msg_elements);
public:
    virtual std::vector<int> get_framenumbers();
    virtual std::vector<std::string> get_system_information_type();
    virtual std::vector<std::string> get_data();
    collect_system_info_impl();
    ~collect_system_info_impl();
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_COLLECT_SYSTEM_INFO_IMPL_H */
