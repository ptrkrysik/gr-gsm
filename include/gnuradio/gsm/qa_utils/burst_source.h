/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_SOURCE_H
#define INCLUDED_GSM_BURST_SOURCE_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API burst_source : virtual public gr::block
{
public:
    typedef std::shared_ptr<burst_source> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of grgsm::burst_source.
     *
     * To avoid accidental use of raw pointers, grgsm::burst_source's
     * constructor is in a private implementation
     * class. grgsm::burst_source::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::vector<int>& framenumbers,
                     const std::vector<int>& timeslots,
                     const std::vector<std::string>& burst_data);

    virtual void set_framenumbers(const std::vector<int>& framenumbers) = 0;
    virtual void set_timeslots(const std::vector<int>& timeslots) = 0;
    virtual void set_burst_data(const std::vector<std::string>& burst_data) = 0;
    virtual void set_arfcn(uint16_t arfcn) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_SOURCE_H */
