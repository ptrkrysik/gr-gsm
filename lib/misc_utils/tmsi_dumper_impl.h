/* -*- c++ -*- */
/* @file
 * @author (C) 2015 by Piotr Krysik <ptrkrysik@gmail.com>
 * @section LICENSE
 *
 * Gr-gsm is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gr-gsm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gr-gsm; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 */


#ifndef INCLUDED_GSM_TMSI_DUMPER_IMPL_H
#define INCLUDED_GSM_TMSI_DUMPER_IMPL_H

#include <grgsm/misc_utils/tmsi_dumper.h>
#include <fstream>
#include <ctime>


namespace gr {
  namespace gsm {

    class tmsi_dumper_impl : public tmsi_dumper
    {
     private:
      std::ofstream d_output_file;
      pmt::pmt_t d_prepend_string;
      bool d_print_message;

      std::string write_timestamp(tm * now);
      std::string write_imsi(uint8_t * imsi);
      std::string write_tmsi(uint8_t * tmsi);
      void tmsi_print(std::string text);
      void tmsi_dump_paging_request_1(uint8_t * msg_elements);
      void tmsi_dump_paging_request_2(uint8_t * msg_elements);
      void tmsi_dump_paging_request_3(uint8_t * msg_elements);
      void tmsi_match_paging_request(pmt::pmt_t msg);

     public:
      tmsi_dumper_impl(pmt::pmt_t prepend_string, const std::string &filename, bool print_message=false);
      ~tmsi_dumper_impl();
      void process_message(pmt::pmt_t msg);
    };
  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TMSI_DUMPER_IMPL_H */
