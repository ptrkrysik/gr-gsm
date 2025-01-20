/* -*- c++ -*- */
/*
 * Copyright 2015 by Roman Khassraf <rkhassraf@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_FILE_SOURCE_H
#define INCLUDED_GSM_BURST_FILE_SOURCE_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief Block which reads GSM bursts from a file.
 * \ingroup gsm
 *
 */
class GSM_API burst_file_source : virtual public gr::block
{
public:
    typedef std::shared_ptr<burst_file_source> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of grgsm::burst_file_source.
     */
    static sptr make(const std::string& filename);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_FILE_SOURCE_H */
