/* -*- c++ -*- */
/*
 * Copyright 2014 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_GSM_EXTRACT_CMC_H
#define INCLUDED_GSM_EXTRACT_CMC_H

#include <gnuradio/gsm/api.h>
#include <gnuradio/block.h>
#include <vector>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API extract_cmc : virtual public gr::block
{
public:
    typedef std::shared_ptr<extract_cmc> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::extract_cmc.
     *
     * To avoid accidental use of raw pointers, gsm::extract_cmc's
     * constructor is in a private implementation
     * class. gsm::extract_cmc::make is the public interface for
     * creating new instances.
     */
    static sptr make();
    virtual std::vector<int> get_framenumbers() = 0;
    virtual std::vector<int> get_a5_versions() = 0;
    virtual std::vector<int> get_start_ciphering() = 0;
};

} // namespace gsm
} // namespace gr
#endif /* INCLUDED_GSM_EXTRACT_CMC_H */
