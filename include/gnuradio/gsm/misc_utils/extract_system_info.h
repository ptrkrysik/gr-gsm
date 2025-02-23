/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_EXTRACT_SYSTEM_INFO_H
#define INCLUDED_GSM_EXTRACT_SYSTEM_INFO_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <vector>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API extract_system_info : virtual public gr::block
{
public:
    typedef std::shared_ptr<extract_system_info> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::extract_system_info.
     *
     * To avoid accidental use of raw pointers, gsm::extract_system_info's
     * constructor is in a private implementation
     * class. gsm::extract_system_info::make is the public interface for
     * creating new instances.
     */
    static sptr make();
    virtual std::vector<int> get_chans() = 0;
    virtual std::vector<int> get_pwrs() = 0;
    virtual std::vector<int> get_lac() = 0;
    virtual std::vector<int> get_cell_id() = 0;
    virtual std::vector<int> get_mcc() = 0;
    virtual std::vector<int> get_mnc() = 0;
    virtual std::vector<int> get_ccch_conf() = 0;
    virtual std::vector<int> get_cell_arfcns(int chan_id) = 0;
    virtual std::vector<int> get_neighbours(int chan_id) = 0;
    virtual void reset() = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_EXTRACT_SYSTEM_INFO_H */
