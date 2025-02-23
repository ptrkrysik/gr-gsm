/* -*- c++ -*- */
/*
 * Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MSG_TO_TAG_IMPL_H
#define INCLUDED_GSM_MSG_TO_TAG_IMPL_H

#include <gnuradio/gsm/misc_utils/msg_to_tag.h>

namespace gr {
namespace gsm {

class msg_to_tag_impl : public msg_to_tag
{
private:
    std::deque<pmt::pmt_t> d_msg_queue;

public:
    msg_to_tag_impl();
    ~msg_to_tag_impl();
    void queue_msg(pmt::pmt_t msg);

    // Where all the action really happens
    int work(int noutput_items, gr_vector_const_void_star& input_items, gr_vector_void_star& output_items);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MSG_TO_TAG_IMPL_H */
