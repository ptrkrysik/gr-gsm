/* -*- c++ -*- */
/*
 * Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CONTROLLED_FRACTIONAL_RESAMPLER_CC_H
#define INCLUDED_GSM_CONTROLLED_FRACTIONAL_RESAMPLER_CC_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup grgsm
 *
 */
class GSM_API controlled_fractional_resampler_cc : virtual public block
{
public:
    typedef std::shared_ptr<controlled_fractional_resampler_cc> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of grgsm::controlled_fractional_resampler_cc.
     *
     * To avoid accidental use of raw pointers, grgsm::controlled_fractional_resampler_cc's
     * constructor is in a private implementation
     * class. grgsm::controlled_fractional_resampler_cc::make is the public interface for
     * creating new instances.
     */
    static sptr make(float phase_shift, float resamp_ratio);

    virtual float mu() const = 0;
    virtual float resamp_ratio() const = 0;
    virtual void set_mu(float mu) = 0;
    virtual void set_resamp_ratio(float resamp_ratio) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CONTROLLED_FRACTIONAL_RESAMPLER_CC_H */
