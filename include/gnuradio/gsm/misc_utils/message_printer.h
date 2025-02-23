/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MESSAGE_PRINTER_H
#define INCLUDED_GSM_MESSAGE_PRINTER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API message_printer : virtual public gr::block
{
public:
    typedef std::shared_ptr<message_printer> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::message_printer.
     */
    static sptr make(pmt::pmt_t prepend_string,
                     bool prepend_fnr = false,
                     bool prepend_frame_count = false,
                     bool print_gsmtap_header = false);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_PRINTER_H */
