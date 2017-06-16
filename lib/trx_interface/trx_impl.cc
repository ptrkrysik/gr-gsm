/* -*- c++ -*- */
/* @file
 * @author Vadim Yanitskiy <axilirator@gmail.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <boost/lexical_cast.hpp>

#include "udp_socket.h"
#include "trx_impl.h"

namespace gr {
  namespace grgsm {

    trx::sptr
    trx::make(
      const std::string &remote_addr,
      const std::string &base_port)
    {
      int base_port_int = boost::lexical_cast<int> (base_port);

      return gnuradio::get_initial_sptr
        (new trx_impl(remote_addr, base_port_int));
    }

    /*
     * The private constructor
     */
    trx_impl::trx_impl(const std::string &remote_addr, int base_port)
    : gr::block("trx",
        gr::io_signature::make(0, 0, 0),
        gr::io_signature::make(0, 0, 0))
    {
        message_port_register_in(pmt::mp("bursts"));
        set_msg_handler(pmt::mp("bursts"),
          boost::bind(&trx_impl::handle_dl_burst, this, _1));

        // Prepare port numbers
        std::string clck_src_port = boost::lexical_cast<std::string> (base_port + 0);
        std::string clck_dst_port = boost::lexical_cast<std::string> (base_port + 100);
        std::string data_src_port = boost::lexical_cast<std::string> (base_port + 2);
        std::string data_dst_port = boost::lexical_cast<std::string> (base_port + 102);

        // Init DATA interface
        d_data_sock = new udp_socket(remote_addr, data_src_port, data_dst_port);
        d_clck_sock = new udp_socket(remote_addr, clck_src_port, clck_dst_port);
    }

    /*
     * Our virtual destructor.
     */
    trx_impl::~trx_impl()
    {
        // Release all UDP sockets and free memory
        delete d_data_sock;
        delete d_clck_sock;
    }

    /*
     * Create an UDP payload with clock indication
     */
    void
    trx_impl::clck_ind_send(uint32_t frame_nr)
    {
      char buf[20];
      size_t n;

      n = snprintf(buf, 20, "IND CLOCK %u", frame_nr);
      d_clck_sock->udp_send((uint8_t *) buf, n + 1);
    }

    /*
     * Create an UDP payload with burst bits
     * and some channel data.
     */
    void
    trx_impl::burst_pack(pmt::pmt_t msg, uint8_t *buf)
    {
      pmt::pmt_t header_plus_burst = pmt::cdr(msg);

      // Extract GSMTAP header from message
      gsmtap_hdr *header = (gsmtap_hdr *)
        pmt::blob_data(header_plus_burst);

      // Pack timeslot index
      buf[0] = header->timeslot;

      // Extract frame number
      uint32_t frame_nr = be32toh(header->frame_number);

      // HACK: send clock indications every 51-th frame
      if (frame_nr % 51 == 0)
        clck_ind_send(frame_nr);

      // Pack frame number
      buf[1] = (frame_nr >> 24) & 0xff;
      buf[2] = (frame_nr >> 16) & 0xff;
      buf[3] = (frame_nr >>  8) & 0xff;
      buf[4] = (frame_nr >>  0) & 0xff;

      // Pack RSSI (-dBm)
      buf[5] = -(uint8_t) header->signal_dbm;

      // Pack correlator timing offset (TOA)
      // FIXME: where to find this value?
      buf[6] = 0;
      buf[7] = 0;

      // Extract bits {0..1} from message
      // Despite GR-GSM uses int8_t, they are not real sbits {-127..127}
      uint8_t *burst = (uint8_t *)
        (pmt::blob_data(header_plus_burst)) + sizeof(gsmtap_hdr);

      // Convert to transceiver interface specific bits {255..0}
      for (int i = 0; i < 148; i++)
        buf[8 + i] = burst[i] ? 255 : 0;
    }

    void
    trx_impl::handle_dl_burst(pmt::pmt_t msg)
    {
      // 8 bytes of header + 148 bytes of burst
      uint8_t buf[156];

      // Compose a new UDP payload with burst
      burst_pack(msg, buf);

      // Send a burst
      d_data_sock->udp_send(buf, 156);
    }

  } /* namespace grgsm */
} /* namespace gr */
