/* -*- c++ -*- */
/*
 * Copyright 2015 by Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_TCH_H_CHANS_DEMAPPER_H
#define INCLUDED_GSM_TCH_H_CHANS_DEMAPPER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief A block for selecting half rate traffic channels
 * \ingroup gsm
 *
 */
class GSM_API tch_h_chans_demapper : virtual public gr::block
{
public:
    typedef std::shared_ptr<tch_h_chans_demapper> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::tch_h_chans_demapper.
     */
    static sptr make(unsigned int timeslot_nr, unsigned int tch_h_channel);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TCH_H_CHANS_DEMAPPER_H */
