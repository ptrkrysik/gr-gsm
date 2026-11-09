/* -*- c++ -*- */
/*
 * Copyright 2014 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_GSM_COLLECT_SYSTEM_INFO_H
#define INCLUDED_GSM_COLLECT_SYSTEM_INFO_H

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
class GSM_API collect_system_info : virtual public gr::block
{
public:
    typedef std::shared_ptr<collect_system_info> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::collect_system_info.
     *
     * To avoid accidental use of raw pointers, gsm::collect_system_info's
     * constructor is in a private implementation
     * class. gsm::collect_system_info::make is the public interface for
     * creating new instances.
     */
    static sptr make();
    virtual std::vector<int> get_framenumbers() = 0;
    virtual std::vector<std::string> get_system_information_type() = 0;
    virtual std::vector<std::string> get_data() = 0;
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_COLLECT_SYSTEM_INFO_H */
