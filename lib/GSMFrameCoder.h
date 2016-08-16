/*

 * Copyright 2008 Free Software Foundation, Inc.
 * 
 * This file is part of GSMFrameCoder
 * 
 * GSMFrameCoder is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GSMFrameCoder is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GSMFrameCoder; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.

 *
 *
 * GSMFrameCoder.h
 *
 *  Created on: 19.12.2010
 *      Author: Johann Betz ( jbetz@informatik.uni-freiburg.de )
 */

#ifndef GSMFRAMECODER_H_
#define GSMFRAMECODER_H_

#include "BitVector1.h"

using namespace std;
class GSMFrameCoder {

	public:

	/**@name FEC signal processing state.  */
	/*//@{
	Parity mBlockCoder;			///< block coder for this channel
	BitVector mI[4];			///< i[][], as per GSM 05.03 2.2
	BitVector mC;				///< c[], as per GSM 05.03 2.2
	BitVector mU;				///< u[], as per GSM 05.03 2.2
	BitVector mD;				///< d[], as per GSM 05.03 2.2
	BitVector mP;				///< p[], as per GSM 05.03 2.2
	//@}*/


	/** Offset from the start of mU to the start of the L2 frame. */
	//virtual unsigned headerOffset() const { return 0; }

	/**
	  Encode u[] to c[].
	  Includes LSB-MSB reversal within each octet.
	*/
	void encode();

	/**
	  Interleave c[] to i[].
	  GSM 05.03 4.1.4.
	*/
    void interleave();

	/** Just encode u[] to c[], interleave and return the interleaved bursts as BitVector[] */
    BitVector1* justencode(const BitVector1&);
    BitVector1 *func(int argc, char *argv[]);
    string BitInv(char *inbits, int len);

};

#endif /* GSMFRAMECODER_H_ */
