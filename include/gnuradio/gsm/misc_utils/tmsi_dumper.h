/* -*- c++ -*- */
/*
 * Copyright 2015 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_TMSI_DUMPER_H
#define INCLUDED_GSM_TMSI_DUMPER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API tmsi_dumper : virtual public gr::block
{
public:
    typedef std::shared_ptr<tmsi_dumper> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::tmsi_dumper.
     */
    static sptr make();
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TMSI_DUMPER_H */
