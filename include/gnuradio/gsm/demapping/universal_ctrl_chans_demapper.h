/* -*- c++ -*- */
/*
 * Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_UNIVERSAL_CTRL_CHANS_DEMAPPER_H
#define INCLUDED_GSM_UNIVERSAL_CTRL_CHANS_DEMAPPER_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <vector>

namespace gr {
namespace gsm {

/*!
 * \brief A block for selecting various control channels.
 * \ingroup gsm
 *
 */
class GSM_API universal_ctrl_chans_demapper : virtual public gr::block
{
public:
    typedef std::shared_ptr<universal_ctrl_chans_demapper> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::universal_ctrl_chans_demapper.
     *
     */
    static sptr make(unsigned int timeslot_nr,
                     const std::vector<int>& downlink_starts_fn_mod51,
                     const std::vector<int>& downlink_channel_types,
                     const std::vector<int>& downlink_subslots,
                     const std::vector<int>& uplink_starts_fn_mod51 = std::vector<int>(),
                     const std::vector<int>& uplink_channel_types = std::vector<int>(),
                     const std::vector<int>& uplink_subslots = std::vector<int>());
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_UNIVERSAL_CTRL_CHANS_DEMAPPER_H */
