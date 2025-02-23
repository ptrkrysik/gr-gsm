/* -*- c++ -*- */
/*
 * Copyright 2018 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_EXTRACT_ASSIGNMENT_CMD_IMPL_H
#define INCLUDED_GSM_EXTRACT_ASSIGNMENT_CMD_IMPL_H

#include <gnuradio/gsm/misc_utils/extract_assignment_cmd.h>
#include <vector>

namespace gr {
namespace gsm {
class extract_assignment_cmd_impl : public extract_assignment_cmd
{
private:
    void process_messages(pmt::pmt_t msg);
    std::vector<int> d_assignment_commands;

public:
    virtual std::vector<int> get_assignment_commands();
    extract_assignment_cmd_impl();
    ~extract_assignment_cmd_impl();
};
} // namespace gsm
} // namespace gr
#endif /* INCLUDED_GSM_EXTRACT_ASSIGNMENT_CMD_IMPL_H */
