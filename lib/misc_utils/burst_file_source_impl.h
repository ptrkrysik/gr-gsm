/* -*- c++ -*- */
/*
 * Copyright 2015 by Roman Khassraf <rkhassraf@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_FILE_SOURCE_IMPL_H
#define INCLUDED_GSM_BURST_FILE_SOURCE_IMPL_H

#include <gnuradio/gsm/misc_utils/burst_file_source.h>
#include <fstream>

namespace gr {
namespace gsm {

/*
 * pmt::deserialize() calls pubseekoff(n, std::ios_base::cur) after it has already
 * consumed n bytes. That is harmless for a std::stringbuf, but a std::filebuf really
 * skips the bytes and the file gets misparsed, the reader ends with "pmt::deserialize:
 * malformed input stream" (gr-gsm issue #477, GNU Radio issue #7672).
 *
 * GNU Radio fixed it in 3.10.12.0 (pull request #7682), every earlier release is
 * affected, e.g. the 3.10.9 of Ubuntu 24.04 and the 3.10.5 of Debian 12.
 * A buffer that refuses to seek avoids the problem on the affected releases and changes
 * nothing on the fixed ones, which do not seek at all. It can be removed when GNU Radio
 * older than 3.10.12.0 is no longer supported.
 */
class sequential_filebuf : public std::filebuf
{
protected:
    pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode) override
    {
        return pos_type(off_type(-1));
    }
    pos_type seekpos(pos_type, std::ios_base::openmode) override
    {
        return pos_type(off_type(-1));
    }
};

class burst_file_source_impl : public burst_file_source
{
private:
    std::shared_ptr<gr::thread::thread> d_thread;
    sequential_filebuf d_input_file;
    bool d_finished;
    void run();

public:
    burst_file_source_impl(const std::string& filename);
    ~burst_file_source_impl();
    bool start();
    bool stop();
    bool finished();
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_FILE_SOURCE_IMPL_H */
