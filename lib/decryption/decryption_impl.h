/* -*- c++ -*- */
/*
 * Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_DECRYPTION_IMPL_H
#define INCLUDED_GSM_DECRYPTION_IMPL_H

#include <gnuradio/gsm/decryption/decryption.h>
#include <vector>

namespace gr {
namespace gsm {

class decryption_impl : public decryption
{
private:
    std::vector<uint8_t> d_k_c;
    bool d_k_c_valid;
    uint8_t d_a5_version;
    void decrypt(pmt::pmt_t msg);
    void validate_k_c();

public:
    decryption_impl(const std::vector<uint8_t>& k_c, unsigned int a5_version);
    ~decryption_impl();
    virtual void set_k_c(const std::vector<uint8_t>& k_c);
    virtual void set_a5_version(unsigned int a5_version);
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_DECRYPTION_IMPL_H */
