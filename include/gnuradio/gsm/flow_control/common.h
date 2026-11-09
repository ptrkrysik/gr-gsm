/* -*- c++ -*- */
/*
 * Copyright 2017 Vadim Yanitskiy <axilirator@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_FLOW_CONTROL_COMMON_H
#define INCLUDED_GSM_FLOW_CONTROL_COMMON_H

namespace gr {
namespace gsm {

enum filter_policy {
    FILTER_POLICY_DEFAULT,
    FILTER_POLICY_PASS_ALL,
    FILTER_POLICY_DROP_ALL,
};

} /* namespace gsm */
} /* namespace gr */

#endif
