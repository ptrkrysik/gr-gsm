/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURSTS_PRINTER_H
#define INCLUDED_GSM_BURSTS_PRINTER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <string>
#include <vector>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API bursts_printer : virtual public gr::block
{
public:
    typedef std::shared_ptr<bursts_printer> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::bursts_printer.
     *
     * To avoid accidental use of raw pointers, gsm::bursts_printer's
     * constructor is in a private implementation
     * class. gsm::bursts_printer::make is the public interface for
     * creating new instances.
     */
    static sptr make(pmt::pmt_t prepend_string,
                     bool prepend_fnr = false,
                     bool prepend_frame_count = false,
                     bool print_payload_only = false,
                     bool ignore_dummy_bursts = false);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURSTS_PRINTER_H */
