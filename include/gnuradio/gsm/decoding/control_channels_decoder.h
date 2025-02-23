/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CONTROL_CHANNELS_DECODER_H
#define INCLUDED_GSM_CONTROL_CHANNELS_DECODER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API control_channels_decoder : virtual public gr::block
{
public:
    typedef std::shared_ptr<control_channels_decoder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::control_channels_decoder.
     *
     * To avoid accidental use of raw pointers, gsm::control_channels_decoder's
     * constructor is in a private implementation
     * class. gsm::control_channels_decoder::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CONTROL_CHANNELS_DECODER_H */
