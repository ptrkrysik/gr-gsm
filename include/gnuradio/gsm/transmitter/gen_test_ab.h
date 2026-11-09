/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_GSM_GEN_TEST_AB_H
#define INCLUDED_GSM_GEN_TEST_AB_H

#include <gnuradio/gsm/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace gsm {

/*!
 * \brief A block that does preprocessing tasks before sending bursts to modulator.
 * \ingroup gsm
 *
 * Currently it removes GSMTAP header from a burst and puts it in first part of PDU
 * pair and removes tailing zeros from Access Bursts coming from TRX interface.
 */
class GSM_API gen_test_ab : virtual public gr::block
{
public:
    typedef std::shared_ptr<gen_test_ab> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::gen_test_ab.
     *
     * To avoid accidental use of raw pointers, gsm::gen_test_ab's
     * constructor is in a private implementation
     * class. gsm::gen_test_ab::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_GEN_TEST_AB_H */

