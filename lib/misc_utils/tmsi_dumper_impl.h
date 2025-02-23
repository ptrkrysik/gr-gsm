/* -*- c++ -*- */
/*
 * Copyright 2015 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_TMSI_DUMPER_IMPL_H
#define INCLUDED_GSM_TMSI_DUMPER_IMPL_H

#include <gnuradio/gsm/misc_utils/tmsi_dumper.h>
#include <ctime>
#include <fstream>

namespace gr {
namespace gsm {

class tmsi_dumper_impl : public tmsi_dumper
{
private:
    std::ofstream dump_file;
    void dump_tmsi(pmt::pmt_t msg);
    void write_timestamp(tm* now);
    void write_imsi(uint8_t* imsi);
    void write_tmsi(uint8_t* tmsi);

public:
    tmsi_dumper_impl();
    ~tmsi_dumper_impl();
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TMSI_DUMPER_IMPL_H */
