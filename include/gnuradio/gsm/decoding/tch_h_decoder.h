/* -*- c++ -*- */
/*
 * Copyright 2018 Vasil Velichkov <vvvelichkov@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_TCH_H_DECODER_H
#define INCLUDED_GSM_TCH_H_DECODER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API tch_h_decoder : virtual public gr::block
{
public:
    typedef std::shared_ptr<tch_h_decoder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::tch_h_decoder.
     *
     * To avoid accidental use of raw pointers, gsm::tch_h_decoder's
     * constructor is in a private implementation
     * class. gsm::tch_h_decoder::make is the public interface for
     * creating new instances.
     */
    static sptr make(unsigned int sub_channel, std::string multi_rate, bool boundary_check = false);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TCH_H_DECODER_H */
