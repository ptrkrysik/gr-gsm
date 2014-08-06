/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_GSM_MESSAGE_PRINTER_IMPL_H
#define INCLUDED_GSM_MESSAGE_PRINTER_IMPL_H

#include <gsm/message_printer.h>

namespace gr {
  namespace gsm {

    class message_printer_impl : public message_printer
    {
     private:
      void message_print(pmt::pmt_t msg);
     public:
      message_printer_impl();
      ~message_printer_impl();
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MESSAGE_PRINTER_IMPL_H */

