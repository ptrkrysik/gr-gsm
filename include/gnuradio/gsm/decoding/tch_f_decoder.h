/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_TCH_F_DECODER_H
#define INCLUDED_GSM_TCH_F_DECODER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

enum tch_mode {
    TCH_AFS12_2,
    TCH_AFS10_2,
    TCH_AFS7_95,
    TCH_AFS7_4,
    TCH_AFS6_7,
    TCH_AFS5_9,
    TCH_AFS5_15,
    TCH_AFS4_75,
    TCH_FS,
    TCH_EFR,
    TCH_HS,
};

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API tch_f_decoder : virtual public gr::block
{
public:
    typedef std::shared_ptr<tch_f_decoder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::tch_f_decoder.
     *
     * To avoid accidental use of raw pointers, gsm::tch_f_decoder's
     * constructor is in a private implementation
     * class. gsm::tch_f_decoder::make is the public interface for
     * creating new instances.
     */
    static sptr make(tch_mode mode, bool boundary_check = false);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TCH_F_DECODER_H */
