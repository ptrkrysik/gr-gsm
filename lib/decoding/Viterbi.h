/*
* Copyright 2013, 2014 Range Networks, Inc.
*
* This software is distributed under multiple licenses;
* see the COPYING file in the main directory for licensing
* information for this specific distribution.
*
* This use of this software may be subject to additional restrictions.
* See the LEGAL file in the main directory for details.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/


#ifndef _VITERBI_H_
#define _VITERBI_H_ 1

// (pat) Virtual base class for Viterbi and Turbo coder/decoders.
class ViterbiBase {
	public:
	virtual void encode(const BitVector &in, BitVector& target) const = 0;
	virtual void decode(const SoftVector &in, BitVector& target) = 0;
	// (pat) Return error count from most recent decoder run.
	// If you get -1 from this, the method is not defined in the Viterbi class.
	virtual int getBEC() { return -1; }
	//virtual ~ViterbiBase();   Currently None of these have destructors.

	// These functions are logically part of the Viterbi functionality, even though they do not use any class variables.
	unsigned applyPoly(uint64_t val, uint64_t poly);
	unsigned applyPoly(uint64_t val, uint64_t poly, unsigned order);
};
#endif
