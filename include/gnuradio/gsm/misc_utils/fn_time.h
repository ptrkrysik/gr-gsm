/* -*- c++ -*- */
/*
 * Copyright 2017 Piotr Krysik <ptrkrysik@gmail.com>
 * Copyright 2017 Vadim Yanitskiy <axilirator@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_GSM_FN_TIME_H
#define INCLUDED_GSM_FN_TIME_H

#include <gnuradio/gsm/api.h>
#include <stdint.h>
#include <utility>

namespace gr {
namespace gsm {

/**
 * Computes difference between reference frame number
 * and a second frame number.
 * @param  fn_ref    reference frame number modulo GSM_HYPER_FRAME
 * @param  fn        second frame number modulo GSM_HYPER_FRAME
 * @param  time_ref  precise timestamp of the first sample in the fn_ref
 * @param  time_hint coarse time for fn that is used as a hint to avoid
 *                   ambiguities caused by modulo operation applied to
 *                   frame numbers
 * @return           difference between fn_ref and fn
 */
  typedef std::pair<unsigned long long, double> time_format;

GSM_API time_format fn_time_delta_cpp(uint32_t fn_ref, time_format time_ref, uint32_t fn_x,
    time_format time_hint, uint32_t ts_num, uint32_t ts_ref);

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_FN_TIME_H */

