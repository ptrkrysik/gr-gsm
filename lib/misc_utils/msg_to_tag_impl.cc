/* -*- c++ -*- */
/*
 * Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "msg_to_tag_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace gsm {

msg_to_tag::sptr msg_to_tag::make() { return gnuradio::get_initial_sptr(new msg_to_tag_impl()); }

void msg_to_tag_impl::queue_msg(pmt::pmt_t msg)
{
    if (pmt::is_dict(msg)) {
        try {
            pmt::pmt_t keys = pmt::dict_keys(msg);
        } catch (const pmt::wrong_type& e) {
            msg = pmt::dict_add(pmt::make_dict(), pmt::car(msg), pmt::cdr(msg));
        }
    }
    d_msg_queue.push_back(msg);
}

/*
 * The private constructor
 */
msg_to_tag_impl::msg_to_tag_impl()
    : gr::sync_block("msg_to_tag",
                     gr::io_signature::make(1, 1, sizeof(gr_complex)),
                     gr::io_signature::make(1, 1, sizeof(gr_complex)))
{
    message_port_register_in(pmt::mp("msg"));
    set_msg_handler(pmt::mp("msg"), boost::bind(&msg_to_tag_impl::queue_msg, this, boost::placeholders::_1));
}

/*
 * Our virtual destructor.
 */
msg_to_tag_impl::~msg_to_tag_impl() {}

int msg_to_tag_impl::work(int noutput_items, gr_vector_const_void_star& input_items, gr_vector_void_star& output_items)
{
    while (!d_msg_queue.empty()) {
        pmt::pmt_t msg(d_msg_queue.front());
        d_msg_queue.pop_front();
        if (pmt::is_dict(msg)) {
            pmt::pmt_t klist(pmt::dict_keys(msg));
            for (size_t i = 0; i < pmt::length(klist); i++) {
                pmt::pmt_t k(pmt::nth(i, klist));
                pmt::pmt_t v(pmt::dict_ref(msg, k, pmt::PMT_NIL));
                add_item_tag(0, nitems_written(0), k, v, alias_pmt());
            }
        } else if (pmt::is_number(msg)) {
            add_item_tag(0, nitems_written(0), pmt::intern(""), msg, alias_pmt());
        } else if (pmt::is_symbol(msg)) {
            add_item_tag(0, nitems_written(0), msg, pmt::intern(""), alias_pmt());
        }
    }

    memcpy(output_items[0], input_items[0], sizeof(gr_complex) * noutput_items);
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace gsm */
} /* namespace gr */
