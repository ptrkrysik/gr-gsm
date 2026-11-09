/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 * Copyright 2014 Vadim Yanitskiy <axilirator@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_TO_FN_TIME_H
#define INCLUDED_GSM_BURST_TO_FN_TIME_H

#include <gnuradio/gsm/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API burst_to_fn_time : virtual public gr::block
{
public:
    typedef std::shared_ptr<burst_to_fn_time> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::burst_to_fn_time.
     *
     * To avoid accidental use of raw pointers, gsm::burst_to_fn_time's
     * constructor is in a private implementation
     * class. gsm::burst_to_fn_time::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_TO_FN_TIME_H */
