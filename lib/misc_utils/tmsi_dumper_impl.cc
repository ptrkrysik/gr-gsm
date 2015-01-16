/* -*- c++ -*- */
/* @file
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
 * 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "tmsi_dumper_impl.h"
#include "grgsm/gsmtap.h"
#include <stdio.h>
#include <ctime>
FILE* tmsiFile;

namespace gr {
  namespace gsm {
   
    void tmsi_dumper_impl::dump_tmsi(pmt::pmt_t msg)
    {
	time_t now = time(0);
	tm *ltm = localtime(&now);
        pmt::pmt_t message_plus_header_blob = pmt::cdr(msg);
        uint8_t * message_plus_header = (uint8_t *)pmt::blob_data(message_plus_header_blob);
        size_t message_plus_header_len=pmt::blob_length(message_plus_header_blob);
        
        gsmtap_hdr * header = (gsmtap_hdr *)message_plus_header;
        int ii=sizeof(gsmtap_hdr);

	switch(message_plus_header[ii]){
	case 37:
	  //1 tmsi 6-9

	if((unsigned char)message_plus_header[ii+1] == 6 && (unsigned char)message_plus_header[ii+2] == 33)
	{
	if((unsigned char)message_plus_header[ii+6] != 43 && (unsigned char)message_plus_header[ii+6] != 43)
	{
	  //printf("INSIDE 37\n");
	  tmsiFile = fopen("tmsicount.txt", "a+");
	  fprintf(tmsiFile,"%02.2x%02.2x%02.2x%02.2x-%02i%02i%02i%02i%02i%02i-%i\n",
		  (unsigned char)message_plus_header[ii+6],
		  (unsigned char)message_plus_header[ii+7],
		  (unsigned char)message_plus_header[ii+8],
		  (unsigned char)message_plus_header[ii+9],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,
				0);
	  fclose(tmsiFile);
	}
	}
	  break;
	case 49:
	   if(message_plus_header[ii+2] == 33)//21 hex
	   {
	  //printf("INSIDE 49\n");
	  //1 imsi 5-12 33
	  tmsiFile = fopen("tmsicount.txt", "a");
	  fprintf(tmsiFile,"%i-%02i%02i%02i%02i%02i%02i-%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x\n",0,
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,

		  (unsigned char)message_plus_header[ii+5],	
		  (unsigned char)message_plus_header[ii+6],
		  (unsigned char)message_plus_header[ii+7],
		  (unsigned char)message_plus_header[ii+8],
		  (unsigned char)message_plus_header[ii+9],
		  (unsigned char)message_plus_header[ii+10],
		  (unsigned char)message_plus_header[ii+11],
		  (unsigned char)message_plus_header[ii+12]);
		  fclose(tmsiFile);
	}
	  break;
	case 65:
	  //printf("INSIDE 65\n"); (41 hex) (65 int) 6-8 13-16
	   if(message_plus_header[ii+2] == 33)//21 hex
	   {
	      tmsiFile = fopen("tmsicount.txt", "a+");
	      fprintf(tmsiFile,"%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n",
		  (unsigned char)message_plus_header[ii+6],
		  (unsigned char)message_plus_header[ii+7],
		  (unsigned char)message_plus_header[ii+8],
		  (unsigned char)message_plus_header[ii+9],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0,

		  (unsigned char)message_plus_header[ii+13],
		  (unsigned char)message_plus_header[ii+14],
		  (unsigned char)message_plus_header[ii+15],
		  (unsigned char)message_plus_header[ii+16],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0);
		  fclose(tmsiFile);
	   }
	  break;
	case 73:
	  //printf("INSIDE 73\n");
	  //3 tmsi 4-7 8-11 15-18
	  if(message_plus_header[ii+2] == 34)
	    {
	  tmsiFile = fopen("tmsicount.txt", "a+");
	  fprintf(tmsiFile,"%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n",
		  (unsigned char)message_plus_header[ii+4],
		  (unsigned char)message_plus_header[ii+5],
		  (unsigned char)message_plus_header[ii+6],
		  (unsigned char)message_plus_header[ii+7],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0,

		  (unsigned char)message_plus_header[ii+8],
		  (unsigned char)message_plus_header[ii+9],
		  (unsigned char)message_plus_header[ii+10],
		  (unsigned char)message_plus_header[ii+11],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0,

		  (unsigned char)message_plus_header[ii+15],
		  (unsigned char)message_plus_header[ii+16],
		  (unsigned char)message_plus_header[ii+17],
		  (unsigned char)message_plus_header[ii+18],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0);
		  fclose(tmsiFile);
	    }
	  break;
	case 77:
	  if(message_plus_header[ii+2] == 33 && message_plus_header[ii+4] == 8)
	  {
	    //1 imsi 1 tmsi 5-12 16-19
	  //printf("INSIDE 77 + 33 + 8\n");
	  tmsiFile = fopen("tmsicount.txt", "a+");
	  fprintf(tmsiFile,"%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x\n",
		  (unsigned char)message_plus_header[ii+16],
		  (unsigned char)message_plus_header[ii+17],
		  (unsigned char)message_plus_header[ii+18],
		  (unsigned char)message_plus_header[ii+19],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,

		  (unsigned char)message_plus_header[ii+5],	
		  (unsigned char)message_plus_header[ii+6],
		  (unsigned char)message_plus_header[ii+7],
		  (unsigned char)message_plus_header[ii+8],
		  (unsigned char)message_plus_header[ii+9],
		  (unsigned char)message_plus_header[ii+10],
		  (unsigned char)message_plus_header[ii+11],
		  (unsigned char)message_plus_header[ii+12]);
	   fclose(tmsiFile);
	  }
	  else if(message_plus_header[ii+2] == 33 && message_plus_header[ii+4] == 5)
	  {
	    //1 tmsi 1 imsi 6-9 12-19
	  //printf("INSIDE 77 + 33 + 5\n");
	  tmsiFile = fopen("tmsicount.txt", "a+");
	  fprintf(tmsiFile,"%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x\n",
		  (unsigned char)message_plus_header[ii+6],
		  (unsigned char)message_plus_header[ii+7],
		  (unsigned char)message_plus_header[ii+8],
		  (unsigned char)message_plus_header[ii+9],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,

		  (unsigned char)message_plus_header[ii+12],	
		  (unsigned char)message_plus_header[ii+13],
		  (unsigned char)message_plus_header[ii+14],
		  (unsigned char)message_plus_header[ii+15],
		  (unsigned char)message_plus_header[ii+16],
		  (unsigned char)message_plus_header[ii+17],
		  (unsigned char)message_plus_header[ii+18],
		  (unsigned char)message_plus_header[ii+19]);
	   fclose(tmsiFile);
	  }
	  else if(message_plus_header[ii+2] == 34)
	  {
	  //printf("INSIDE 77 + 34\n");
	  tmsiFile = fopen("tmsicount.txt", "a+");
	  fprintf(tmsiFile,"%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n",
		  (unsigned char)message_plus_header[ii+4],
		  (unsigned char)message_plus_header[ii+5],
		  (unsigned char)message_plus_header[ii+6],
		  (unsigned char)message_plus_header[ii+7],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0,

		  (unsigned char)message_plus_header[ii+8],
		  (unsigned char)message_plus_header[ii+9],
		  (unsigned char)message_plus_header[ii+10],
		  (unsigned char)message_plus_header[ii+11],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0,

		  (unsigned char)message_plus_header[ii+15],
		  (unsigned char)message_plus_header[ii+16],
		  (unsigned char)message_plus_header[ii+17],
		  (unsigned char)message_plus_header[ii+18],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0);
		  fclose(tmsiFile);
	  }
	  else if(message_plus_header[ii+2] == 36)
	  {
	  //printf("INSIDE 77 + 36\n");
	  tmsiFile = fopen("tmsicount.txt", "a+");
	  fprintf(tmsiFile,"%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n",
		  (unsigned char)message_plus_header[ii+4],
		  (unsigned char)message_plus_header[ii+5],
		  (unsigned char)message_plus_header[ii+6],
		  (unsigned char)message_plus_header[ii+7],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0,

		  (unsigned char)message_plus_header[ii+8],
		  (unsigned char)message_plus_header[ii+9],
		  (unsigned char)message_plus_header[ii+10],
		  (unsigned char)message_plus_header[ii+11],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0,

		  (unsigned char)message_plus_header[ii+12],
		  (unsigned char)message_plus_header[ii+13],
		  (unsigned char)message_plus_header[ii+14],
		  (unsigned char)message_plus_header[ii+15],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0,

		  (unsigned char)message_plus_header[ii+16],
		  (unsigned char)message_plus_header[ii+17],
		  (unsigned char)message_plus_header[ii+18],
		  (unsigned char)message_plus_header[ii+19],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0);
		  fclose(tmsiFile);
	  }
	  break;
	case 85:
	  if(message_plus_header[ii+2] == 34)//hex 22
	    {
	      //printf("INSIDE 85\n");// hex is 55
	      tmsiFile = fopen("tmsicount.txt", "a+");
	      fprintf(tmsiFile,"%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n%02.2x%02.2x%02.2x%02.2x-%02d%02d%02d%02d%02d%02d-%i\n",
		  (unsigned char)message_plus_header[ii+4],
		  (unsigned char)message_plus_header[ii+5],
		  (unsigned char)message_plus_header[ii+6],
		  (unsigned char)message_plus_header[ii+7],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0,

		  (unsigned char)message_plus_header[ii+8],
		  (unsigned char)message_plus_header[ii+9],
		  (unsigned char)message_plus_header[ii+10],
		  (unsigned char)message_plus_header[ii+11],
		   	   1900 + ltm->tm_year-2000,
		   	   1+ltm->tm_mon,
		   	   ltm->tm_mday,
		   	   ltm->tm_hour,
		   	   ltm->tm_min,
		   	   ltm->tm_sec,0);
		  fclose(tmsiFile);
	    }
	  break;

	}

        std::cout << std::endl;
    }
    
    tmsi_dumper::sptr
    tmsi_dumper::make()
    {
      return gnuradio::get_initial_sptr
        (new tmsi_dumper_impl());
    }

    /*
     * The private constructor
     */
    tmsi_dumper_impl::tmsi_dumper_impl()
      : gr::block("tmsi_dumper",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        message_port_register_in(pmt::mp("msgs"));
        set_msg_handler(pmt::mp("msgs"), boost::bind(&tmsi_dumper_impl::dump_tmsi, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    tmsi_dumper_impl::~tmsi_dumper_impl()
    {
    }
  } /* namespace gsm */
} /* namespace gr */

