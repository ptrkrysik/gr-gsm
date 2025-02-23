/* -*- c++ -*- */
/*
 * Copyright 2018 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/gsm/endian.h>
#include <gnuradio/gsm/gsmtap.h>
#include <gnuradio/io_signature.h>

#include "extract_assignment_cmd_impl.h"

namespace gr {
namespace gsm {
void extract_assignment_cmd_impl::process_messages(pmt::pmt_t msg)
{
    pmt::pmt_t message_plus_header_blob = pmt::cdr(msg);
    uint8_t* message_plus_header = (uint8_t*)pmt::blob_data(message_plus_header_blob);
    gsmtap_hdr* header = (gsmtap_hdr*)message_plus_header;
    uint8_t* msg_elements = (uint8_t*)(message_plus_header + sizeof(gsmtap_hdr));

    if ((msg_elements[3] & 0xFF) == 0x06 && msg_elements[4] == 0x2e) {
        int frame_nr = be32toh(header->frame_number);
        d_assignment_commands.push_back(frame_nr);
    }
}


std::vector<int> extract_assignment_cmd_impl::get_assignment_commands() { return d_assignment_commands; }

extract_assignment_cmd::sptr extract_assignment_cmd::make()
{
    return gnuradio::get_initial_sptr(new extract_assignment_cmd_impl());
}

/*
 * The private constructor
 */
extract_assignment_cmd_impl::extract_assignment_cmd_impl()
    : gr::block("extract_assignment_cmd", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0))
{
    message_port_register_in(pmt::mp("msgs"));
    set_msg_handler(pmt::mp("msgs"),
                    boost::bind(&extract_assignment_cmd_impl::process_messages, this, boost::placeholders::_1));
}

/*
 * Our virtual destructor.
 */
extract_assignment_cmd_impl::~extract_assignment_cmd_impl() {}
} /* namespace gsm */
} /* namespace gr */
