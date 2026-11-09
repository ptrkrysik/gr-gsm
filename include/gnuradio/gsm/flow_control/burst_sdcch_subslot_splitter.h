/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_SDCCH_SUBSLOT_SPLITTER_H
#define INCLUDED_GSM_BURST_SDCCH_SUBSLOT_SPLITTER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

enum splitter_mode { SPLITTER_SDCCH8, SPLITTER_SDCCH4 };

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API burst_sdcch_subslot_splitter : virtual public gr::block
{
public:
    typedef std::shared_ptr<burst_sdcch_subslot_splitter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::burst_sdcch_subslot_splitter.
     *
     * To avoid accidental use of raw pointers, gsm::burst_sdcch_subslot_splitter's
     * constructor is in a private implementation
     * class. gsm::burst_sdcch_subslot_splitter::make is the public interface for
     * creating new instances.
     */
    static sptr make(splitter_mode mode);

    /* External API */
    virtual splitter_mode get_mode(void) = 0;
    virtual splitter_mode set_mode(splitter_mode mode) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_SDCCH_SUBSLOT_SPLITTER_H */
