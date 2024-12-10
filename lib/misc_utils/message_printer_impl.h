/* -*- c++ -*- */
/*
 * Copyright 2014,2024 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MESSAGE_PRINTER_IMPL_H
#define INCLUDED_GSM_MESSAGE_PRINTER_IMPL_H

#include <gnuradio/gsm/misc_utils/message_printer.h>

namespace gr {
namespace gsm {

class message_printer_impl : public message_printer
{
private:
    void message_print(pmt::pmt_t msg);
    pmt::pmt_t d_prepend_string;
    bool d_prepend_fnr;
    bool d_prepend_frame_count;
    bool d_print_gsmtap_header;

public:
    message_printer_impl(pmt::pmt_t prepend_string,
                         bool prepend_fnr = false,
                         bool prepend_frame_count = false,
                         bool print_gsmtap_header = false);
    ~message_printer_impl();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_PRINTER_IMPL_H */
