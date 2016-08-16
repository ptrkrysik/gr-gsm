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
 * GSMFrameCoder.cpp
 *
 *  Created on: 19.12.2010
 *      Author: Johann Betz ( jbetz@informatik.uni-freiburg.de )
 */

#include <fstream>

#include "GSMFrameCoder.h"

//#include "GSMTransfer.h"
#include <string>
#include <iostream>
//#include <stdio.h>
#include <stdlib.h>

using namespace std;

Parity1 mBlockCoder(0x10004820009ULL, 40, 224); ///< block coder for this channel
BitVector1 mI[4]; ///< i[][], as per GSM 05.03 2.2
BitVector1 mC(456); ///< c[], as per GSM 05.03 2.2
BitVector1 mU(228); ///< u[], as per GSM 05.03 2.2
BitVector1 mD(mU.head(184)); ///< d[], as per GSM 05.03 2.2
BitVector1 mP(mU.segment(184, 40)); ///< p[], as per GSM 05.03 2.2*/
ViterbiR2O41 mVCoder;

void GSMFrameCoder::encode() {
	// Perform the FEC encoding of GSM 05.03 4.1.2 and 4.1.3

	// GSM 05.03 4.1.2
	// Generate the parity bits.
	mBlockCoder.writeParityWord(mD, mP);
	//OBJLOG(DEEPDEBUG) << "XCCHL1Encoder u[]=" << mU;
	// GSM 05.03 4.1.3
	// Apply the convolutional encoder.
	mU.encode(mVCoder, mC);
	//OBJLOG(DEEPDEBUG) << "XCCHL1Encoder c[]=" << mC;
}

void GSMFrameCoder::interleave() {
	// GSM 05.03, 4.1.4.  Verbatim.
	for (int k = 0; k < 456; k++) {
		int B = k % 4;
		int j = 2 * ((49 * k) % 57) + ((k % 8) / 4);
		mI[B][j] = mC[k];
	}
}

BitVector1* GSMFrameCoder::justencode(const BitVector1 &frame) {
	frame.copyToSegment(mU, 0);
	mD.LSB8MSB();
	encode(); // Encode u[] to c[], GSM 05.03 4.1.2 and 4.1.3.
	interleave(); // Interleave c[] to i[][], GSM 05.03 4.1.4.
	return mI;
}

void strip_spaces(char* string) {
	char *p1 = string;
	char *p2 = string;
	p1 = string;
	while (*p1 != 0) {
		if (isspace(*p1)) {
			++p1;
		} else
			*p2++ = *p1++;
	}
	*p2 = 0;
}

string htoi(const char *ptr) {
	char ch = *ptr;
	int length = strlen(ptr);
	string value;

	while (ch == ' ' || ch == '\t')
		ch = *(++ptr);
	int i;
	for (i = 0; i < length; i++) {
		//const char* bin;
		string bin;
		if (!isspace(ch)) {
			switch (tolower(ch)) {
			case '0':
				bin = "0000";
				break;
			case '1':
				bin = "0001";
				break;
			case '2':
				bin = "0010";
				break;
			case '3':
				bin = "0011";
				break;
			case '4':
				bin = "0100";
				break;
			case '5':
				bin = "0101";
				break;
			case '6':
				bin = "0110";
				break;
			case '7':
				bin = "0111";
				break;
			case '8':
				bin = "1000";
				break;
			case '9':
				bin = "1001";
				break;
			case 'a':
				bin = "1010";
				break;
			case 'b':
				bin = "1011";
				break;
			case 'c':
				bin = "1100";
				break;
			case 'd':
				bin = "1101";
				break;
			case 'e':
				bin = "1110";
				break;
			case 'f':
				bin = "1111";
				break;
			default:
				return value.c_str();
			}
			value.append(bin);
		}
		ch = *(++ptr);
	}
    return value;
}

string GSMFrameCoder::BitInv(char *inbits, int len)
{
    string BitInved;
    for (int aa=0; aa<len/8; aa++)
    {
        for (int bb=7; bb>=0; bb--)
        {
            if (inbits[bb+aa*8] == 0x00)
                BitInved.push_back('0');
            else
                BitInved.push_back('1');
        }
    }
//    BitInved.append("\0");
    return BitInved;
}

BitVector1* GSMFrameCoder::func(int argc, char *argv[]) {
	bool binary = false;
	int bpos = 1;
    BitVector1 framebits;
    string bits;
    char* input;
	input = (char*) malloc(500 * sizeof(int));

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-b") == 0) {
				binary = true;
				bpos = 1;
				if (i < argc - 1) {
					bpos = i + 1;
				}
			} else {
                strcpy(input, argv[i]);

			}
		}

		// Set up the interleaving buffers.
		for (int k = 0; k < 4; k++) {
            mI[k] = BitVector1(114);
			// Fill with zeros just to make Valgrind happy.
			mI[k].fill(0);
		}

		// zero out u[] to take care of tail fields
		mU.zero();

		if (!binary) {
			//todo convert into binary
            string inbits = htoi(input);
//            cout << "Decoding bits " << inbits << endl;
            framebits = BitVector1(inbits.c_str());
//			cout << "frame bits " << framebits << endl;
		} else {
//			cout << "Decoding binary" << endl;
            bits = BitInv(argv[bpos],184);
//            cout << "bits going to encode ... \n";
//            cout << bits << endl;
//            for (int bb=0; bb<184 ; bb++)
//            {
//                if (bits[bb] == 0x00)
//                    std::cout << '0';
//                else if (bits[bb] == 0x01)
//                    std::cout << '1';
//                else
//                    std::cout << "else" ;
//            }
//            std::cout.flush();
//            std::cout << std::endl;

            framebits = BitVector1(bits);
		}
//        cout << "frame bits \n" << framebits << endl;
        BitVector1* encframe;

		encframe = justencode(framebits);
//		cout << "Encoded Frame, Burst1: " << "\n" << encframe[0] << endl;
//		cout << "Encoded Frame, Burst2: " << "\n" << encframe[1] << endl;
//		cout << "Encoded Frame, Burst3: " << "\n" << encframe[2] << endl;
//		cout << "Encoded Frame, Burst4: " << "\n" << encframe[3] << endl;

        return encframe;
	} else {
		cout << "Usage:\n" << "gsmframecoder <burst to encode>\n"
				<< "-b <burst to encode> - give the burst in binary representation instead of hex"
				<< endl;
	}
}

