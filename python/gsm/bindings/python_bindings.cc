/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

// Headers for binding functions
/**************************************/
// The following comment block is used for
// gr_modtool to insert function prototypes
// Please do not delete
/**************************************/
// BINDING_FUNCTION_PROTOTYPES(
    void bind_tch_f_decoder(py::module& m);
    void bind_tch_h_decoder(py::module& m);
    void bind_control_channels_decoder(py::module& m);
    void bind_universal_ctrl_chans_demapper(py::module& m);
    void bind_tch_f_chans_demapper(py::module& m);
    void bind_tch_h_chans_demapper(py::module& m);
    void bind_bursts_printer(py::module& m);
    void bind_message_printer(py::module& m);
    void bind_cx_channel_hopper(py::module& m);
    void bind_receiver(py::module& m);
// ) END BINDING_FUNCTION_PROTOTYPES


// We need this hack because import_array() returns NULL
// for newer Python versions.
// This function is also necessary because it ensures access to the C API
// and removes a warning.
void* init_numpy()
{
    import_array();
    return NULL;
}

PYBIND11_MODULE(gsm_python, m)
{
    // Initialize the numpy C API
    // (otherwise we will see segmentation faults)
    init_numpy();

    // Allow access to base block methods
    py::module::import("gnuradio.gr");

    /**************************************/
    // The following comment block is used for
    // gr_modtool to insert binding function calls
    // Please do not delete
    /**************************************/
    // BINDING_FUNCTION_CALLS(
    bind_tch_f_decoder(m);
    bind_tch_h_decoder(m);
    bind_control_channels_decoder(m);
    bind_universal_ctrl_chans_demapper(m);
    bind_tch_f_chans_demapper(m);
    bind_tch_h_chans_demapper(m);
    bind_bursts_printer(m);
    bind_message_printer(m);
    bind_cx_channel_hopper(m);
    bind_receiver(m);
    // ) END BINDING_FUNCTION_CALLS
}
