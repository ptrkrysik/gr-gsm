/* -*- c++ -*- */
/*
 * Copyright 2014-2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CONTROLLED_ROTATOR_CC_H
#define INCLUDED_GSM_CONTROLLED_ROTATOR_CC_H

#include <gnuradio/sync_block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API controlled_rotator_cc : virtual public sync_block
{
public:
    typedef std::shared_ptr<controlled_rotator_cc> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::controlled_rotator_cc.
     *
     * To avoid accidental use of raw pointers, gsm::controlled_rotator_cc's
     * constructor is in a private implementation
     * class. gsm::controlled_rotator_cc::make is the public interface for
     * creating new instances.
     */
    static sptr make(double phase_inc);

    virtual void set_phase_inc(double phase_inc) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CONTROLLED_ROTATOR_CC_H */
