/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_DECRYPTION_H
#define INCLUDED_GSM_DECRYPTION_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <vector>

namespace gr {
namespace gsm {

/*!
 * \brief Block doing decryption according to A5/x algorithms
 *
 */
class GSM_API decryption : virtual public gr::block
{
public:
    typedef std::shared_ptr<decryption> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::decryption.
     */
    static sptr make(const std::vector<uint8_t>& k_c, unsigned int a5_version);
    virtual void set_k_c(const std::vector<uint8_t>& k_c) = 0;
    virtual void set_a5_version(unsigned int a5_version) = 0;
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_DECRYPTION_H */
