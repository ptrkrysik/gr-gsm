/* -*- c++ -*- */
/*
 * Copyright 2006,2009,2013 Free Software Foundation, Inc.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CONSTANTS_H
#define INCLUDED_GSM_CONSTANTS_H

#include <gnuradio/gsm/api.h>
#include <string>

namespace gr {
namespace gsm {

/*!
 * \brief return date/time of build, as set when 'cmake' is run
 */
GSM_API const std::string build_date();

/*!
 * \brief return version string defined by cmake (GrVersion.cmake)
 */
GSM_API const std::string version();

/*!
 * \brief return just the major version defined by cmake
 */
GSM_API const std::string major_version();

/*!
 * \brief return just the api version defined by cmake
 */
GSM_API const std::string api_version();

/*!
 * \brief return just the minor version defined by cmake
 */
GSM_API const std::string minor_version();

/*!
 * \brief return just the maint version defined by cmake
 */
GSM_API const std::string maint_version();

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CONSTANTS_H */
