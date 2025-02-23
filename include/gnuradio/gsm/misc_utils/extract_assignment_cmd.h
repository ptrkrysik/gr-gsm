/* -*- c++ -*- */
/*
 * Copyright 2018 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_EXTRACT_ASSIGNMENT_CMD_H
#define INCLUDED_GSM_EXTRACT_ASSIGNMENT_CMD_H

#include <gnuradio/block.h>
#include <gnuradio/gsm/api.h>
#include <vector>

namespace gr {
namespace gsm {

/*!
 * \brief <+description of block+>
 * \ingroup gsm
 *
 */
class GSM_API extract_assignment_cmd : virtual public gr::block
{
public:
    typedef std::shared_ptr<extract_assignment_cmd> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of gsm::extract_assignment_cmd.
     *
     * To avoid accidental use of raw pointers, gsm::extract_assignment_cmd's
     * constructor is in a private implementation
     * class. gsm::extract_assignment_cmd::make is the public interface for
     * creating new instances.
     */
    static sptr make();
    virtual std::vector<int> get_assignment_commands() = 0;
};

} // namespace gsm
} // namespace gr
#endif /* INCLUDED_GSM_EXTRACT_ASSIGNMENT_CMD_H */
