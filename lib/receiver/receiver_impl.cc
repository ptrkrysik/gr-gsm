/* -*- c++ -*- */
/*
 * @file
 * @author Piotr Krysik <ptrkrysik@gmail.com>
 * @section LICENSE
 *
 * Gr-gsm is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gr-gsm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gr-gsm; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <math.h>
#include <boost/circular_buffer.hpp>
#include <algorithm>
#include <numeric>
#include <vector>
#include <viterbi_detector.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <boost/scoped_ptr.hpp>

#include <sch.h>
#include "receiver_impl.h"
#include <grgsm/endian.h>

//files included for debuging
//#include "plotting/plotting.hpp"
//#include <pthread.h>

#define SYNC_SEARCH_RANGE 30

namespace gr
{
namespace gsm
{
receiver::sptr
receiver::make(int osr, const std::vector<int> &cell_allocation, const std::vector<int> &tseq_nums)
{
    return gnuradio::get_initial_sptr
           (new receiver_impl(osr, cell_allocation, tseq_nums));
}

/*
 * The private constructor
 */
receiver_impl::receiver_impl(int osr, const std::vector<int> &cell_allocation, const std::vector<int> &tseq_nums)
    : gr::sync_block("receiver",
                gr::io_signature::make(1, -1, sizeof(gr_complex)),
                gr::io_signature::make(0, 0, 0)),
    d_OSR(osr),
    d_chan_imp_length(CHAN_IMP_RESP_LENGTH),
    d_counter(0),
    d_fcch_start_pos(0),
    d_freq_offset_setting(0),
    d_state(fcch_search),
    d_burst_nr(osr),
    d_failed_sch(0),
    d_signal_dbm(-120),
    d_tseq_nums(tseq_nums),
    d_cell_allocation(cell_allocation),
    d_last_time(0.0)
{
    int i;
                                                                      //don't send samples to the receiver until there are at least samples for one
    set_output_multiple(floor((TS_BITS + 2 * GUARD_PERIOD) * d_OSR)); // burst and two gurad periods (one gurard period is an arbitrary overlap)
    gmsk_mapper(SYNC_BITS, N_SYNC_BITS, d_sch_training_seq, gr_complex(0.0, -1.0));
    for (i = 0; i < TRAIN_SEQ_NUM; i++)
    {
        gr_complex startpoint = (train_seq[i][0]==0) ? gr_complex(1.0, 0.0) : gr_complex(-1.0, 0.0); //if first bit of the seqeunce ==0  first symbol ==1
                                                                                                     //if first bit of the seqeunce ==1  first symbol ==-1
        gmsk_mapper(train_seq[i], N_TRAIN_BITS, d_norm_training_seq[i], startpoint);
    }
    message_port_register_out(pmt::mp("C0"));
    message_port_register_out(pmt::mp("CX"));
    message_port_register_out(pmt::mp("measurements"));
    configure_receiver();  //configure the receiver - tell it where to find which burst type
}

/*
 * Our virtual destructor.
 */
receiver_impl::~receiver_impl()
{
}

int
receiver_impl::work(int noutput_items,
	               gr_vector_const_void_star &input_items,
	               gr_vector_void_star &output_items)
{
//    std::vector<const gr_complex *> iii = (std::vector<const gr_complex *>) input_items; // jak zrobić to rzutowanie poprawnie
    gr_complex * input = (gr_complex *) input_items[0];
    std::vector<tag_t> freq_offset_tags;
    uint64_t start = nitems_read(0);
    uint64_t stop = start + noutput_items;

    float current_time = static_cast<float>(start)/(GSM_SYMBOL_RATE*d_OSR);
    if((current_time - d_last_time) > 0.1)
    {
        pmt::pmt_t msg = pmt::make_tuple(pmt::mp("current_time"),pmt::from_double(current_time));
        message_port_pub(pmt::mp("measurements"), msg);
        d_last_time = current_time;
    }

    pmt::pmt_t key = pmt::string_to_symbol("setting_freq_offset");
    get_tags_in_range(freq_offset_tags, 0, start, stop, key);
    bool freq_offset_tag_in_fcch = false;
    uint64_t tag_offset=-1; //-1 - just some clearly invalid value
    
    if(!freq_offset_tags.empty()){
        tag_t freq_offset_tag = freq_offset_tags[0];
        tag_offset = freq_offset_tag.offset - start;
        
        burst_type b_type = d_channel_conf.get_burst_type(d_burst_nr);
        if(d_state == synchronized && b_type == fcch_burst){
            uint64_t last_sample_nr = ceil((GUARD_PERIOD + 2.0 * TAIL_BITS + 156.25) * d_OSR) + 1;
            if(tag_offset < last_sample_nr){
                freq_offset_tag_in_fcch = true;
            }
            d_freq_offset_setting = pmt::to_double(freq_offset_tag.value);
        } else {
            d_freq_offset_setting = pmt::to_double(freq_offset_tag.value);
        }
    }
    
    switch (d_state)
    {
        //bootstrapping
    case fcch_search:
    {
        double freq_offset_tmp;
        if (find_fcch_burst(input, noutput_items,freq_offset_tmp))
        {
            pmt::pmt_t msg = pmt::make_tuple(pmt::mp("freq_offset"),pmt::from_double(freq_offset_tmp-d_freq_offset_setting),pmt::mp("fcch_search"));
            message_port_pub(pmt::mp("measurements"), msg);

            d_state = sch_search;
        }
        else
        {
            d_state = fcch_search;
        }
        break;
    }

    case sch_search:
    {
        std::vector<gr_complex> channel_imp_resp(CHAN_IMP_RESP_LENGTH*d_OSR);
        int t1, t2, t3;
        int burst_start = 0;
        unsigned char output_binary[BURST_SIZE];

        if (reach_sch_burst(noutput_items))                                //wait for a SCH burst
        {
            burst_start = get_sch_chan_imp_resp(input, &channel_imp_resp[0]); //get channel impulse response from it
            detect_burst(input, &channel_imp_resp[0], burst_start, output_binary); //detect bits using MLSE detection
            if (decode_sch(&output_binary[3], &t1, &t2, &t3, &d_ncc, &d_bcc) == 0)   //decode SCH burst
            {
                d_burst_nr.set(t1, t2, t3, 0);                                  //set counter of bursts value
                d_burst_nr++;

                consume_each(burst_start + BURST_SIZE * d_OSR + 4*d_OSR);   //consume samples up to next guard period
                d_state = synchronized;
            }
            else
            {
                d_state = fcch_search;                       //if there is error in the sch burst go back to fcch search phase
            }
        }
        else
        {
            d_state = sch_search;
        }
        break;
    }
    //in this state receiver is synchronized and it processes bursts according to burst type for given burst number
    case synchronized:
    {
        std::vector<gr_complex> channel_imp_resp(CHAN_IMP_RESP_LENGTH*d_OSR);
        int offset = 0;
        int to_consume = 0;
        unsigned char output_binary[BURST_SIZE];

        burst_type b_type;
        
        for(int input_nr=0; input_nr<d_cell_allocation.size(); input_nr++)
        {
            double signal_pwr = 0;
            input = (gr_complex *)input_items[input_nr];
            
            for(int ii=GUARD_PERIOD;ii<TS_BITS;ii++)
            {
                signal_pwr += abs(input[ii])*abs(input[ii]);
            }
            signal_pwr = signal_pwr/(TS_BITS);
            d_signal_dbm = round(10*log10(signal_pwr/50));
            if(input_nr==0){
                d_c0_signal_dbm = d_signal_dbm;
            }
            
            if(input_nr==0) //for c0 channel burst type is controlled by channel configuration
            {
                b_type = d_channel_conf.get_burst_type(d_burst_nr); //get burst type for given burst number
            }
            else 
            {
                b_type = normal_or_noise; //for the rest it can be only normal burst or noise (at least at this moment of development)
            }
            
            switch (b_type)
            {
            case fcch_burst:                                                                      //if it's FCCH  burst
            {
                const unsigned first_sample = ceil((GUARD_PERIOD + 2 * TAIL_BITS) * d_OSR) + 1;
                const unsigned last_sample = first_sample + USEFUL_BITS * d_OSR - TAIL_BITS * d_OSR;
                double freq_offset_tmp = compute_freq_offset(input, first_sample, last_sample);       //extract frequency offset from it

                send_burst(d_burst_nr, fc_fb, GSMTAP_BURST_FCCH, input_nr);

                pmt::pmt_t msg = pmt::make_tuple(pmt::mp("freq_offset"),pmt::from_double(freq_offset_tmp-d_freq_offset_setting),pmt::mp("synchronized"));
                message_port_pub(pmt::mp("measurements"), msg);
                break;
            }
            case sch_burst:                                                                      //if it's SCH burst
            {
                int t1, t2, t3, d_ncc, d_bcc;
                d_c0_burst_start = get_sch_chan_imp_resp(input, &channel_imp_resp[0]);                //get channel impulse response
                
                detect_burst(input, &channel_imp_resp[0], d_c0_burst_start, output_binary);           //MLSE detection of bits
                send_burst(d_burst_nr, output_binary, GSMTAP_BURST_SCH, input_nr);
                if (decode_sch(&output_binary[3], &t1, &t2, &t3, &d_ncc, &d_bcc) == 0)           //and decode SCH data
                {
                    // d_burst_nr.set(t1, t2, t3, 0);                                              //but only to check if burst_start value is correct
                    d_failed_sch = 0;
                    offset =  d_c0_burst_start - floor((GUARD_PERIOD) * d_OSR);                         //compute offset from burst_start - burst should start after a guard period
                    to_consume += offset;                                                          //adjust with offset number of samples to be consumed
                }
                else
                {
                    d_failed_sch++;
                    if (d_failed_sch >= MAX_SCH_ERRORS)
                    {
                        d_state = fcch_search; 
                        pmt::pmt_t msg = pmt::make_tuple(pmt::mp("freq_offset"),pmt::from_double(0.0),pmt::mp("sync_loss"));
                        message_port_pub(pmt::mp("measurements"), msg);
                        //DCOUT("Re-Synchronization!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                    }
                }
                break;
            }
            case normal_burst:
            {
                float normal_corr_max;                                                    //if it's normal burst
                d_c0_burst_start = get_norm_chan_imp_resp(input, &channel_imp_resp[0], &normal_corr_max, d_bcc); //get channel impulse response for given training sequence number - d_bcc
                detect_burst(input, &channel_imp_resp[0], d_c0_burst_start, output_binary);            //MLSE detection of bits
                send_burst(d_burst_nr, output_binary, GSMTAP_BURST_NORMAL, input_nr);
                break;
            }
            case dummy_or_normal:
            {
                unsigned int normal_burst_start, dummy_burst_start;
                float dummy_corr_max, normal_corr_max;

                dummy_burst_start = get_norm_chan_imp_resp(input, &channel_imp_resp[0], &dummy_corr_max, TS_DUMMY);
                normal_burst_start = get_norm_chan_imp_resp(input, &channel_imp_resp[0], &normal_corr_max, d_bcc);
                            
                if (normal_corr_max > dummy_corr_max)
                {
                    d_c0_burst_start = normal_burst_start;
                    detect_burst(input, &channel_imp_resp[0], normal_burst_start, output_binary);
                    send_burst(d_burst_nr, output_binary, GSMTAP_BURST_NORMAL, input_nr); 
                }
                else
                {
                    d_c0_burst_start = dummy_burst_start;
                    send_burst(d_burst_nr, dummy_burst, GSMTAP_BURST_DUMMY, input_nr);
                }
                break;
            }
            case rach_burst:
                break;
            case dummy:
                send_burst(d_burst_nr, dummy_burst, GSMTAP_BURST_DUMMY, input_nr);
                break;
            case normal_or_noise:
            {
                unsigned int burst_start;
                float normal_corr_max_tmp;
                float normal_corr_max=-1e6;
                int max_tn;
                std::vector<gr_complex> v(input, input + noutput_items);
                if(d_signal_dbm>=d_c0_signal_dbm-13)
                {
                    if(d_tseq_nums.size()==0)              //there is no information about training sequence
                    {                                      //however the receiver can detect it
                        get_norm_chan_imp_resp(input, &channel_imp_resp[0], &normal_corr_max, 0);
                        float ts_max=normal_corr_max;     //with use of a very simple algorithm based on finding
                        int ts_max_num=0;                 //maximum correlation
                        for(int ss=1; ss<=7; ss++)
                        {
                            get_norm_chan_imp_resp(input, &channel_imp_resp[0], &normal_corr_max, ss);
                            if(ts_max<normal_corr_max)
                            {
                                ts_max = normal_corr_max;
                                ts_max_num = ss;
                            }
                        }
                        d_tseq_nums.push_back(ts_max_num);
                    }
                    int tseq_num;
                    if(input_nr<=d_tseq_nums.size()){
                        tseq_num = d_tseq_nums[input_nr-1];
                    } else {
                        tseq_num = d_tseq_nums.back();
                    }
                    burst_start = get_norm_chan_imp_resp(input, &channel_imp_resp[0], &normal_corr_max, tseq_num);
//                  if(abs(d_c0_burst_start-burst_start)<=2){ //unused check/filter based on timing
                    if((normal_corr_max/sqrt(signal_pwr))>=0.9){
                        detect_burst(input, &channel_imp_resp[0], burst_start, output_binary);
                        send_burst(d_burst_nr, output_binary, GSMTAP_BURST_NORMAL, input_nr);
                    }
                }
                break;
            }
            case empty:   //if it's empty burst
                break;      //do nothing
            }
            
            if(input_nr==input_items.size()-1)
            {
                d_burst_nr++;   //go to next burst
                to_consume += TS_BITS * d_OSR + d_burst_nr.get_offset();  //consume samples of the burst up to next guard period
                consume_each(to_consume);
            }
            //and add offset which is introduced by
            //0.25 fractional part of a guard period
        }
    }
    break;
    }
    return 0;
}

bool receiver_impl::find_fcch_burst(const gr_complex *input, const int nitems, double & computed_freq_offset)
{
    boost::circular_buffer<float> phase_diff_buffer(FCCH_HITS_NEEDED * d_OSR); //circular buffer used to scan throug signal to find
    //best match for FCCH burst
    float phase_diff = 0;
    gr_complex conjprod;
    int start_pos = -1;
    int hit_count = 0;
    int miss_count = 0;
    float min_phase_diff;
    float max_phase_diff;
    double best_sum = 0;
    float lowest_max_min_diff = 99999;

    int to_consume = 0;
    int sample_number = 0;
    bool end = false;
    bool result = false;
    boost::circular_buffer<float>::iterator buffer_iter;
    
    /**@name Possible states of FCCH search algorithm*/
    //@{
    enum states
    {
        init,               ///< initialize variables
        search,             ///< search for positive samples
        found_something,    ///< search for FCCH and the best position of it
        fcch_found,         ///< when FCCH was found
        search_fail         ///< when there is no FCCH in the input vector
    } fcch_search_state;
    //@}

    fcch_search_state = init;

    while (!end)
    {
        switch (fcch_search_state)
        {

        case init: //initialize variables
            hit_count = 0;
            miss_count = 0;
            start_pos = -1;
            lowest_max_min_diff = 99999;
            phase_diff_buffer.clear();
            fcch_search_state = search;

            break;

        case search:                                                // search for positive samples
            sample_number++;

            if (sample_number > nitems - FCCH_HITS_NEEDED * d_OSR)   //if it isn't possible to find FCCH because
            {
                                                                       //there's too few samples left to look into,
                to_consume = sample_number;                            //don't do anything with those samples which are left
                                                                       //and consume only those which were checked
                fcch_search_state = search_fail;
            }
            else
            {
                phase_diff = compute_phase_diff(input[sample_number], input[sample_number-1]);

                if (phase_diff > 0)                                   //if a positive phase difference was found
                {
                    to_consume = sample_number;
                    fcch_search_state = found_something;                //switch to state in which searches for FCCH
                }
                else
                {
                    fcch_search_state = search;
                }
            }

            break;

        case found_something:  // search for FCCH and the best position of it
        {
            if (phase_diff > 0)
            {
                hit_count++;       //positive phase differencies increases hits_count
            }
            else
            {
                miss_count++;      //negative increases miss_count
            }

            if ((miss_count >= FCCH_MAX_MISSES * d_OSR) && (hit_count <= FCCH_HITS_NEEDED * d_OSR))
            {
                //if miss_count exceeds limit before hit_count
                fcch_search_state = init;       //go to init
                continue;
            }
            else if (((miss_count >= FCCH_MAX_MISSES * d_OSR) && (hit_count > FCCH_HITS_NEEDED * d_OSR)) || (hit_count > 2 * FCCH_HITS_NEEDED * d_OSR))
            {
                //if hit_count and miss_count exceeds limit then FCCH was found
                fcch_search_state = fcch_found;
                continue;
            }
            else if ((miss_count < FCCH_MAX_MISSES * d_OSR) && (hit_count > FCCH_HITS_NEEDED * d_OSR))
            {
                //find difference between minimal and maximal element in the buffer
                //for FCCH this value should be low
                //this part is searching for a region where this value is lowest
                min_phase_diff = * (min_element(phase_diff_buffer.begin(), phase_diff_buffer.end()));
                max_phase_diff = * (max_element(phase_diff_buffer.begin(), phase_diff_buffer.end()));

                if (lowest_max_min_diff > max_phase_diff - min_phase_diff)
                {
                    lowest_max_min_diff = max_phase_diff - min_phase_diff;
                    start_pos = sample_number - FCCH_HITS_NEEDED * d_OSR - FCCH_MAX_MISSES * d_OSR; //store start pos
                    best_sum = 0;

                    for (buffer_iter = phase_diff_buffer.begin();
                            buffer_iter != (phase_diff_buffer.end());
                            buffer_iter++)
                    {
                        best_sum += *buffer_iter - (M_PI / 2) / d_OSR;   //store best value of phase offset sum
                    }
                }
            }

            sample_number++;

            if (sample_number >= nitems)      //if there's no single sample left to check
            {
                fcch_search_state = search_fail;//FCCH search failed
                continue;
            }

            phase_diff = compute_phase_diff(input[sample_number], input[sample_number-1]);
            phase_diff_buffer.push_back(phase_diff);
            fcch_search_state = found_something;
        }
        break;

        case fcch_found:
        {
            to_consume = start_pos + FCCH_HITS_NEEDED * d_OSR + 1; //consume one FCCH burst

            d_fcch_start_pos = d_counter + start_pos;

            //compute frequency offset
            double phase_offset = best_sum / FCCH_HITS_NEEDED;
            double freq_offset = phase_offset * 1625000.0/6 / (2 * M_PI); //1625000.0/6 - GMSK symbol rate in GSM
            computed_freq_offset = freq_offset;

            end = true;
            result = true;
            break;
        }

        case search_fail:
            end = true;
            result = false;
            break;
        }
    }

    d_counter += to_consume;
    consume_each(to_consume);

    return result;
}

double receiver_impl::compute_freq_offset(const gr_complex * input, unsigned first_sample, unsigned last_sample)
{
    double phase_sum = 0;
    unsigned ii;

    for (ii = first_sample; ii < last_sample; ii++)
    {
        double phase_diff = compute_phase_diff(input[ii], input[ii-1]) - (M_PI / 2) / d_OSR;
        phase_sum += phase_diff;
    }

    double phase_offset = phase_sum / (last_sample - first_sample);
    double freq_offset = phase_offset * 1625000.0 / (12.0 * M_PI);
    return freq_offset;
}

inline float receiver_impl::compute_phase_diff(gr_complex val1, gr_complex val2)
{
    gr_complex conjprod = val1 * conj(val2);
    return fast_atan2f(imag(conjprod), real(conjprod));
}

bool receiver_impl::reach_sch_burst(const int nitems)
{
    //it just consumes samples to get near to a SCH burst
    int to_consume = 0;
    bool result = false;
    unsigned sample_nr_near_sch_start = d_fcch_start_pos + (FRAME_BITS - SAFETY_MARGIN) * d_OSR;

    //consume samples until d_counter will be equal to sample_nr_near_sch_start
    if (d_counter < sample_nr_near_sch_start)
    {
        if (d_counter + nitems >= sample_nr_near_sch_start)
        {
            to_consume = sample_nr_near_sch_start - d_counter;
        }
        else
        {
            to_consume = nitems;
        }
        result = false;
    }
    else
    {
        to_consume = 0;
        result = true;
    }

    d_counter += to_consume;
    consume_each(to_consume);
    return result;
}

int receiver_impl::get_sch_chan_imp_resp(const gr_complex *input, gr_complex * chan_imp_resp)
{
    std::vector<gr_complex> correlation_buffer;
    std::vector<float> power_buffer;
    std::vector<float> window_energy_buffer;

    int strongest_window_nr;
    int burst_start = 0;
    int chan_imp_resp_center = 0;
    float max_correlation = 0;
    float energy = 0;

    for (int ii = SYNC_POS * d_OSR; ii < (SYNC_POS + SYNC_SEARCH_RANGE) *d_OSR; ii++)
    {
        gr_complex correlation = correlate_sequence(&d_sch_training_seq[5], N_SYNC_BITS - 10, &input[ii]);
        correlation_buffer.push_back(correlation);
        power_buffer.push_back(std::pow(abs(correlation), 2));
    }
    //compute window energies
    std::vector<float>::iterator iter = power_buffer.begin();
    bool loop_end = false;
    while (iter != power_buffer.end())
    {
        std::vector<float>::iterator iter_ii = iter;
        energy = 0;

        for (int ii = 0; ii < (d_chan_imp_length) *d_OSR; ii++, iter_ii++)
        {
            if (iter_ii == power_buffer.end())
            {
                loop_end = true;
                break;
            }
            energy += (*iter_ii);
        }
        if (loop_end)
        {
            break;
        }
        iter++;
        window_energy_buffer.push_back(energy);
    }

    strongest_window_nr = max_element(window_energy_buffer.begin(), window_energy_buffer.end()) - window_energy_buffer.begin();
    //   d_channel_imp_resp.clear();

    max_correlation = 0;
    for (int ii = 0; ii < (d_chan_imp_length) *d_OSR; ii++)
    {
        gr_complex correlation = correlation_buffer[strongest_window_nr + ii];
        if (abs(correlation) > max_correlation)
        {
            chan_imp_resp_center = ii;
            max_correlation = abs(correlation);
        }
        //     d_channel_imp_resp.push_back(correlation);
        chan_imp_resp[ii] = correlation;
    }

    burst_start = strongest_window_nr + chan_imp_resp_center - 48 * d_OSR - 2 * d_OSR + 2 + SYNC_POS * d_OSR;
    return burst_start;
}


void receiver_impl::detect_burst(const gr_complex * input, gr_complex * chan_imp_resp, int burst_start, unsigned char * output_binary)
{
    float output[BURST_SIZE];
    std::vector<gr_complex> rhh_temp(CHAN_IMP_RESP_LENGTH*d_OSR);
    gr_complex rhh[CHAN_IMP_RESP_LENGTH];
    gr_complex filtered_burst[BURST_SIZE];
    int start_state = 3;
    unsigned int stop_states[2] = {4, 12};

    autocorrelation(chan_imp_resp, &rhh_temp[0], d_chan_imp_length*d_OSR);
    for (int ii = 0; ii < (d_chan_imp_length); ii++)
    {
        rhh[ii] = conj(rhh_temp[ii*d_OSR]);
    }

    mafi(&input[burst_start], BURST_SIZE, chan_imp_resp, d_chan_imp_length*d_OSR, filtered_burst);

    viterbi_detector(filtered_burst, BURST_SIZE, rhh, start_state, stop_states, 2, output);

    for (int i = 0; i < BURST_SIZE ; i++)
    {
        output_binary[i] = (output[i] > 0);
    }
}

void receiver_impl::gmsk_mapper(const unsigned char * input, int nitems, gr_complex * gmsk_output, gr_complex start_point)
{
    gr_complex j = gr_complex(0.0, 1.0);

    int current_symbol;
    int encoded_symbol;
    int previous_symbol = 2 * input[0] - 1;
    gmsk_output[0] = start_point;

    for (int i = 1; i < nitems; i++)
    {
        //change bits representation to NRZ
        current_symbol = 2 * input[i] - 1;
        //differentially encode
        encoded_symbol = current_symbol * previous_symbol;
        //and do gmsk mapping
        gmsk_output[i] = j * gr_complex(encoded_symbol, 0.0) * gmsk_output[i-1];
        previous_symbol = current_symbol;
    }
}

gr_complex receiver_impl::correlate_sequence(const gr_complex * sequence, int length, const gr_complex * input)
{
    gr_complex result(0.0, 0.0);
    int sample_number = 0;

    for (int ii = 0; ii < length; ii++)
    {
        sample_number = (ii * d_OSR) ;
        result += sequence[ii] * conj(input[sample_number]);
    }

    result = result / gr_complex(length, 0);
    return result;
}

//computes autocorrelation for positive arguments
inline void receiver_impl::autocorrelation(const gr_complex * input, gr_complex * out, int nitems)
{
    int i, k;
    for (k = nitems - 1; k >= 0; k--)
    {
        out[k] = gr_complex(0, 0);
        for (i = k; i < nitems; i++)
        {
            out[k] += input[i] * conj(input[i-k]);
        }
    }
}

inline void receiver_impl::mafi(const gr_complex * input, int nitems, gr_complex * filter, int filter_length, gr_complex * output)
{
    int ii = 0, n, a;

    for (n = 0; n < nitems; n++)
    {
        a = n * d_OSR;
        output[n] = 0;
        ii = 0;

        while (ii < filter_length)
        {
            if ((a + ii) >= nitems*d_OSR){
                break;
            }
            output[n] += input[a+ii] * filter[ii];
            ii++;
        }
    }
}

//especially computations of strongest_window_nr
int receiver_impl::get_norm_chan_imp_resp(const gr_complex *input, gr_complex * chan_imp_resp, float *corr_max, int bcc)
{
    std::vector<gr_complex> correlation_buffer;
    std::vector<float> power_buffer;
    std::vector<float> window_energy_buffer;

    int strongest_window_nr;
    int burst_start = 0;
    int chan_imp_resp_center = 0;
    float max_correlation = 0;
    float energy = 0;
   
    int search_center = (int)((TRAIN_POS + GUARD_PERIOD) * d_OSR);
    int search_start_pos = search_center + 1 - 5*d_OSR;
    //   int search_start_pos = search_center -  d_chan_imp_length * d_OSR;
    int search_stop_pos = search_center + d_chan_imp_length * d_OSR + 5 * d_OSR;

    for(int ii = search_start_pos; ii < search_stop_pos; ii++)
    {
        gr_complex correlation = correlate_sequence(&d_norm_training_seq[bcc][TRAIN_BEGINNING], N_TRAIN_BITS - 10, &input[ii]);
        correlation_buffer.push_back(correlation);
        power_buffer.push_back(std::pow(abs(correlation), 2));
    }
//    plot(power_buffer);
    //compute window energies
    std::vector<float>::iterator iter = power_buffer.begin();
    bool loop_end = false;
    while (iter != power_buffer.end())
    {
        std::vector<float>::iterator iter_ii = iter;
        energy = 0;

        for (int ii = 0; ii < (d_chan_imp_length)*d_OSR; ii++, iter_ii++)
        {
            if (iter_ii == power_buffer.end())
            {
                loop_end = true;
                break;
            }
            energy += (*iter_ii);
        }
        if (loop_end)
        {
            break;
        }
        iter++;

        window_energy_buffer.push_back(energy);
    }

    strongest_window_nr = max_element(window_energy_buffer.begin(), window_energy_buffer.end()-((d_chan_imp_length)*d_OSR)) - window_energy_buffer.begin();
    //strongest_window_nr = strongest_window_nr-d_OSR; 
    if(strongest_window_nr<0){
       strongest_window_nr = 0;
    }
    
    max_correlation = 0;
    for (int ii = 0; ii < (d_chan_imp_length)*d_OSR; ii++)
    {
        gr_complex correlation = correlation_buffer[strongest_window_nr + ii];
        if (abs(correlation) > max_correlation)
        {
            chan_imp_resp_center = ii;
            max_correlation = abs(correlation);
        }
        //     d_channel_imp_resp.push_back(correlation);
        chan_imp_resp[ii] = correlation;
    }
    
    *corr_max = max_correlation;

    //DCOUT("strongest_window_nr_new: " << strongest_window_nr);
    burst_start = search_start_pos + strongest_window_nr - TRAIN_POS * d_OSR; //compute first sample posiiton which corresponds to the first sample of the impulse response

    //DCOUT("burst_start: " << burst_start);
    return burst_start;
}


void receiver_impl::send_burst(burst_counter burst_nr, const unsigned char * burst_binary, uint8_t burst_type, unsigned int input_nr)
{
    boost::scoped_ptr<gsmtap_hdr> tap_header(new gsmtap_hdr());
   
    tap_header->version = GSMTAP_VERSION;
    tap_header->hdr_len = sizeof(gsmtap_hdr)/4;
    tap_header->type = GSMTAP_TYPE_UM_BURST;
    tap_header->timeslot = static_cast<uint8_t>(d_burst_nr.get_timeslot_nr());
    tap_header->frame_number = htobe32(d_burst_nr.get_frame_nr());
    tap_header->sub_type = burst_type;
    tap_header->arfcn = htobe16(d_cell_allocation[input_nr]) ; 
    tap_header->signal_dbm = static_cast<int8_t>(d_signal_dbm);
    tap_header->snr_db = 0;

    int8_t header_plus_burst[sizeof(gsmtap_hdr)+BURST_SIZE];
    memcpy(header_plus_burst, tap_header.get(), sizeof(gsmtap_hdr));
    memcpy(header_plus_burst+sizeof(gsmtap_hdr), burst_binary, BURST_SIZE);
    
    pmt::pmt_t blob_header_plus_burst = pmt::make_blob(header_plus_burst,sizeof(gsmtap_hdr)+BURST_SIZE);
    pmt::pmt_t msg = pmt::cons(pmt::PMT_NIL, blob_header_plus_burst);
    
    if(input_nr==0){
        message_port_pub(pmt::mp("C0"), msg);
    } else {
        message_port_pub(pmt::mp("CX"), msg);
    }
}

void receiver_impl::configure_receiver()
{
    d_channel_conf.set_multiframe_type(TIMESLOT0, multiframe_51);  
    d_channel_conf.set_burst_types(TIMESLOT0, TEST51, sizeof(TEST51) / sizeof(unsigned), dummy_or_normal);

    d_channel_conf.set_burst_types(TIMESLOT0, TEST_CCH_FRAMES, sizeof(TEST_CCH_FRAMES) / sizeof(unsigned), dummy_or_normal);
    d_channel_conf.set_burst_types(TIMESLOT0, FCCH_FRAMES, sizeof(FCCH_FRAMES) / sizeof(unsigned), fcch_burst);
    d_channel_conf.set_burst_types(TIMESLOT0, SCH_FRAMES, sizeof(SCH_FRAMES) / sizeof(unsigned), sch_burst);

    d_channel_conf.set_multiframe_type(TIMESLOT1, multiframe_51);
    d_channel_conf.set_burst_types(TIMESLOT1, TEST51, sizeof(TEST51) / sizeof(unsigned), dummy_or_normal);
    d_channel_conf.set_multiframe_type(TIMESLOT2, multiframe_51);
    d_channel_conf.set_burst_types(TIMESLOT2, TEST51, sizeof(TEST51) / sizeof(unsigned), dummy_or_normal);
    d_channel_conf.set_multiframe_type(TIMESLOT3, multiframe_51);
    d_channel_conf.set_burst_types(TIMESLOT3, TEST51, sizeof(TEST51) / sizeof(unsigned), dummy_or_normal);
    d_channel_conf.set_multiframe_type(TIMESLOT4, multiframe_51);
    d_channel_conf.set_burst_types(TIMESLOT4, TEST51, sizeof(TEST51) / sizeof(unsigned), dummy_or_normal);
    d_channel_conf.set_multiframe_type(TIMESLOT5, multiframe_51);
    d_channel_conf.set_burst_types(TIMESLOT5, TEST51, sizeof(TEST51) / sizeof(unsigned), dummy_or_normal);
    d_channel_conf.set_multiframe_type(TIMESLOT6, multiframe_51);
    d_channel_conf.set_burst_types(TIMESLOT6, TEST51, sizeof(TEST51) / sizeof(unsigned), dummy_or_normal);
    d_channel_conf.set_multiframe_type(TIMESLOT7, multiframe_51);
    d_channel_conf.set_burst_types(TIMESLOT7, TEST51, sizeof(TEST51) / sizeof(unsigned), dummy_or_normal);
}

void receiver_impl::set_cell_allocation(const std::vector<int> &cell_allocation)
{
    d_cell_allocation = cell_allocation;
}

void receiver_impl::set_tseq_nums(const std::vector<int> & tseq_nums)
{
    d_tseq_nums = tseq_nums;
}

void receiver_impl::reset()
{
    d_state = fcch_search;
}

} /* namespace gsm */
} /* namespace gr */

