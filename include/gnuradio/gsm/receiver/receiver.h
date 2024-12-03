/* -*- c++ -*- */
/*
 * Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_RECEIVER_H
#define INCLUDED_GSM_RECEIVER_H

#include <gnuradio/gsm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace gsm {

/*!
 * \brief GSM receiver block
 * \ingroup gsm
 *
 * Processes signal(/s) coming at rate gsm_symbol_rate*osr.
 * It has support for processing multiple synchronized
 * streams synchronized with a single broadcast channel
 * (for frequency hopping). It also has support for
 * reception of bursts from uplink channels.
 */
class GSM_API receiver : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<receiver> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::receiver.
     */
    static sptr make(int osr,
                     const std::vector<int>& cell_allocation,
                     const std::vector<int>& tseq_nums,
                     bool process_uplink = false);

    /*! TODO: fill the methods docs
     * \brief
     */
    virtual void set_cell_allocation(const std::vector<int>& cell_allocation) = 0;

    /*!
     * \brief
     */
    virtual void set_tseq_nums(const std::vector<int>& tseq_nums) = 0;

    /*!
     * \brief
     */
    virtual void reset() = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_RECEIVER_H */
