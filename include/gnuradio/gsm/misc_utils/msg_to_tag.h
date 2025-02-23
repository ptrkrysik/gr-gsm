/* -*- c++ -*- */
/*
 * Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_MSG_TO_TAG_H
#define INCLUDED_GSM_MSG_TO_TAG_H

#include <gnuradio/sync_block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup grgsm
 *
 */
class GSM_API msg_to_tag : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<msg_to_tag> sptr;
    /*!
     * \brief Return a shared_ptr to a new instance of grgsm::msg_to_tag.
     *
     * To avoid accidental use of raw pointers, grgsm::msg_to_tag's
     * constructor is in a private implementation
     * class. grgsm::msg_to_tag::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_MSG_TO_TAG_H */
