/* -*- c++ -*- */
/*
 * Copyright 2014,2024 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURSTS_PRINTER_IMPL_H
#define INCLUDED_GSM_BURSTS_PRINTER_IMPL_H

#define DUMMY_BURST_LEN 148

#include <gnuradio/gsm/misc_utils/bursts_printer.h>
#include <set>

namespace gr {
namespace gsm {

class bursts_printer_impl : public bursts_printer
{
private:
    void bursts_print(pmt::pmt_t burst);
    pmt::pmt_t d_prepend_string;
    bool d_prepend_fnr;
    bool d_prepend_frame_count;
    bool d_print_payload_only;
    bool d_ignore_dummy_bursts;
    bool is_dummy_burst(int8_t* burst, size_t burst_len);
    static const int8_t d_dummy_burst[];

public:
    bursts_printer_impl(pmt::pmt_t prepend_string,
                        bool prepend_fnr = false,
                        bool prepend_frame_count = false,
                        bool print_payload_only = false,
                        bool ignore_dummy_bursts = false);
    ~bursts_printer_impl();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURSTS_PRINTER_IMPL_H */
