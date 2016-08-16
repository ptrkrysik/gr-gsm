/*
* Copyright 2008, 2009 Free Software Foundation, Inc.
*
* This software is distributed under the terms of the GNU Affero Public License.
* See the COPYING file in the main directory for details.
*
* This use of this software may be subject to additional restrictions.
* See the LEGAL file in the main directory for details.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef FECVECTORS_H
#define FECVECTORS_H

#include "Vector1.h"
#include <stdint.h>


class BitVector1;
class SoftVector1;



/** Shift-register (LFSR) generator. */
class Generator1 {

	private:

	uint64_t mCoeff;	///< polynomial coefficients. LSB is zero exponent.
	uint64_t mState;	///< shift register state. LSB is most recent.
	uint64_t mMask;		///< mask for reading state
	unsigned mLen;		///< number of bits used in shift register
	unsigned mLen_1;	///< mLen - 1

	public:

    Generator1(uint64_t wCoeff, unsigned wLen)
		:mCoeff(wCoeff),mState(0),
		mMask((1ULL<<wLen)-1),
		mLen(wLen),mLen_1(wLen-1)
	{ assert(wLen<64); }

	void clear() { mState=0; }

	/**@name Accessors */
	//@{
	uint64_t state() const { return mState & mMask; }
	unsigned size() const { return mLen; }
	//@}

	/**
		Calculate one bit of a syndrome.
		This is in the .h for inlining.
	*/
	void syndromeShift(unsigned inBit)
	{
		const unsigned fb = (mState>>(mLen_1)) & 0x01;
		mState = (mState<<1) ^ (inBit & 0x01);
		if (fb) mState ^= mCoeff;
	}

	/**
		Update the generator state by one cycle.
		This is in the .h for inlining.
	*/
	void encoderShift(unsigned inBit)
	{
		const unsigned fb = ((mState>>(mLen_1)) ^ inBit) & 0x01;
		mState <<= 1;
		if (fb) mState ^= mCoeff;
	}


};




/** Parity (CRC-type) generator and checker based on a Generator. */
class Parity1 : public Generator1 {

	protected:

	unsigned mCodewordSize;

	public:

    Parity1(uint64_t wCoefficients, unsigned wParitySize, unsigned wCodewordSize)
        :Generator1(wCoefficients, wParitySize),
		mCodewordSize(wCodewordSize)
	{ }

	/** Compute the parity word and write it into the target segment.  */
    void writeParityWord(const BitVector1& data, BitVector1& parityWordTarget, bool invert=true);

	/** Compute the syndrome of a received sequence. */
    uint64_t syndrome(const BitVector1& receivedCodeword);
};




/**
	Class to represent convolutional coders/decoders of rate 1/2, memory length 4.
	This is the "workhorse" coder for most GSM channels.
*/
class ViterbiR2O41 {

	private:
		/**name Lots of precomputed elements so the compiler can optimize like hell. */
		//@{
		/**@name Core values. */
		//@{
		static const unsigned mIRate = 2;	///< reciprocal of rate
		static const unsigned mOrder = 4;	///< memory length of generators
		//@}
		/**@name Derived values. */
		//@{
		static const unsigned mIStates = 0x01 << mOrder;	///< number of states, number of survivors
		static const uint32_t mSMask = mIStates-1;			///< survivor mask
		static const uint32_t mCMask = (mSMask<<1) | 0x01;	///< candidate mask
		static const uint32_t mOMask = (0x01<<mIRate)-1;	///< ouput mask, all iRate low bits set
		static const unsigned mNumCands = mIStates*2;		///< number of candidates to generate during branching
		static const unsigned mDeferral = 6*mOrder;			///< deferral to be used
		//@}
		//@}

		/** Precomputed tables. */
		//@{
		uint32_t mCoeffs[mIRate];					///< polynomial for each generator
		uint32_t mStateTable[mIRate][2*mIStates];	///< precomputed generator output tables
		uint32_t mGeneratorTable[2*mIStates];		///< precomputed coder output table
		//@}
	
	public:

		/**
		  A candidate sequence in a Viterbi decoder.
		  The 32-bit state register can support a deferral of 6 with a 4th-order coder.
		 */
		typedef struct candStruct {
			uint32_t iState;	///< encoder input associated with this candidate
			uint32_t oState;	///< encoder output associated with this candidate
			float cost;			///< cost (metric value), float to support soft inputs
		} vCand;

		/** Clear a structure. */
		void clear(vCand& v)
		{
			v.iState=0;
			v.oState=0;
			v.cost=0;
		}
		

	private:

		/**@name Survivors and candidates. */
		//@{
		vCand mSurvivors[mIStates];			///< current survivor pool
		vCand mCandidates[2*mIStates];		///< current candidate pool
		//@}

	public:

		unsigned iRate() const { return mIRate; }
		uint32_t cMask() const { return mCMask; }
		uint32_t stateTable(unsigned g, unsigned i) const { return mStateTable[g][i]; }
		unsigned deferral() const { return mDeferral; }
		

        ViterbiR2O41();

		/** Set all cost metrics to zero. */
		void initializeStates();

		/**
			Full cycle of the Viterbi algorithm: branch, metrics, prune, select.
			@return reference to minimum-cost candidate.
		*/
		const vCand& step(uint32_t inSample, const float *probs, const float *iprobs);

	private:

		/** Branch survivors into new candidates. */
		void branchCandidates();

		/** Compute cost metrics for soft-inputs. */
		void getSoftCostMetrics(uint32_t inSample, const float *probs, const float *iprobs);

		/** Select survivors from the candidate set. */
		void pruneCandidates();

		/** Find the minimum cost survivor. */
		const vCand& minCost() const;

		/**
			Precompute the state tables.
			@param g Generator index 0..((1/rate)-1)
		*/
		void computeStateTables(unsigned g);

		/**
			Precompute the generator outputs.
			mCoeffs must be defined first.
		*/
		void computeGeneratorTable();

};




class BitVector1 : public Vector1<char> {


	public:

	/**@name Constructors. */
	//@{

	/**@name Casts of Vector constructors. */
	//@{
    BitVector1(char* wData, char* wStart, char* wEnd)
        :Vector1<char>(wData,wStart,wEnd)
	{ }
    BitVector1(size_t len=0):Vector1<char>(len) {}
    BitVector1(const Vector1<char>& source):Vector1<char>(source) {}
    BitVector1(Vector1<char>& source):Vector1<char>(source) {}
    BitVector1(const Vector1<char>& source1, const Vector1<char> source2):Vector1<char>(source1,source2) {}
	//@}

	/** Construct from a string of "0" and "1". */
    BitVector1(std::string valString);
	//@}

	/** Index a single bit. */
	bool bit(size_t index) const
	{
		// We put this code in .h for fast inlining.
		const char *dp = mStart+index;
		assert(dp<mEnd);
		return (*dp) & 0x01;
	}

	/**@name Casts and overrides of Vector operators. */
	//@{
    BitVector1 segment(size_t start, size_t span)
	{
		char* wStart = mStart + start;
		char* wEnd = wStart + span;
		assert(wEnd<=mEnd);
        return BitVector1(NULL,wStart,wEnd);
	}

    BitVector1 alias()
		{ return segment(0,size()); }

    const BitVector1 segment(size_t start, size_t span) const
        { return (BitVector1)(Vector1<char>::segment(start,span)); }

    BitVector1 head(size_t span) { return segment(0,span); }
    const BitVector1 head(size_t span) const { return segment(0,span); }
    BitVector1 tail(size_t start) { return segment(start,size()-start); }
    const BitVector1 tail(size_t start) const { return segment(start,size()-start); }
	//@}


	void zero() { fill(0); }

	/**@name FEC operations. */
	//@{
	/** Calculate the syndrome of the vector with the given Generator. */
    uint64_t syndrome(Generator1& gen) const;
	/** Calculate the parity word for the vector with the given Generator. */
    uint64_t parity(Generator1& gen) const;
	/** Encode the signal with the GSM rate 1/2 convolutional encoder. */
    void encode(const ViterbiR2O41& encoder, BitVector1& target);
	//@}


	/** Invert 0<->1. */
	void invert();

	/**@name Byte-wise operations. */
	//@{
	/** Reverse an 8-bit vector. */
	void reverse8();
	/** Reverse groups of 8 within the vector (byte reversal). */
	void LSB8MSB();
	//@}

	/**@name Serialization and deserialization. */
	//@{
	uint64_t peekField(size_t readIndex, unsigned length) const;
	uint64_t peekFieldReversed(size_t readIndex, unsigned length) const;
	uint64_t readField(size_t& readIndex, unsigned length) const;
	uint64_t readFieldReversed(size_t& readIndex, unsigned length) const;
	void fillField(size_t writeIndex, uint64_t value, unsigned length);
	void fillFieldReversed(size_t writeIndex, uint64_t value, unsigned length);
	void writeField(size_t& writeIndex, uint64_t value, unsigned length);
	void writeFieldReversed(size_t& writeIndex, uint64_t value, unsigned length);
	//@}

	/** Sum of bits. */
	unsigned sum() const;

	/** Reorder bits, dest[i] = this[map[i]]. */
    void map(const unsigned *map, size_t mapSize, BitVector1& dest) const;

	/** Reorder bits, dest[map[i]] = this[i]. */
    void unmap(const unsigned *map, size_t mapSize, BitVector1& dest) const;

	/** Pack into a char array. */
	void pack(unsigned char*) const;

	/** Unopack from a char array. */
	void unpack(const unsigned char*);

	/** Make a hexdump string. */
	void hex(std::ostream&) const;

};



std::ostream& operator<<(std::ostream&, const BitVector1&);






/**
  The SoftVector class is used to represent a soft-decision signal.
  Values 0..1 represent probabilities that a bit is "true".
 */
class SoftVector1: public Vector1<float> {

	public:

	/** Build a SoftVector of a given length. */
    SoftVector1(size_t wSize=0):Vector1<float>(wSize) {}

	/** Construct a SoftVector from a C string of "0", "1", and "X". */
    SoftVector1(const char* valString);

	/** Construct a SoftVector from a BitVector. */
    SoftVector1(const BitVector1& source);

	/**
		Wrap a SoftVector around a block of floats.
		The block will be delete[]ed upon desctuction.
	*/
    SoftVector1(float *wData, unsigned length)
        :Vector1<float>(wData,length)
	{}

    SoftVector1(float* wData, float* wStart, float* wEnd)
        :Vector1<float>(wData,wStart,wEnd)
	{ }

	/**
		Casting from a Vector<float>.
		Note that this is NOT pass-by-reference.
	*/
    SoftVector1(Vector1<float> source)
        :Vector1<float>(source)
	{}


	/**@name Casts and overrides of Vector operators. */
	//@{
    SoftVector1 segment(size_t start, size_t span)
	{
		float* wStart = mStart + start;
		float* wEnd = wStart + span;
		assert(wEnd<=mEnd);
        return SoftVector1(NULL,wStart,wEnd);
	}

    SoftVector1 alias()
		{ return segment(0,size()); }

    const SoftVector1 segment(size_t start, size_t span) const
        { return (SoftVector1)(Vector1<float>::segment(start,span)); }

    SoftVector1 head(size_t span) { return segment(0,span); }
    const SoftVector1 head(size_t span) const { return segment(0,span); }
    SoftVector1 tail(size_t start) { return segment(start,size()-start); }
    const SoftVector1 tail(size_t start) const { return segment(start,size()-start); }
	//@}

	/** Decode soft symbols with the GSM rate-1/2 Viterbi decoder. */
    void decode(ViterbiR2O41 &decoder, BitVector1& target) const;

	/** Fill with "unknown" values. */
	void unknown() { fill(0.5F); }

	/** Return a hard bit value from a given index by slicing. */
	bool bit(size_t index) const
	{
		const float *dp = mStart+index;
		assert(dp<mEnd);
		return (*dp)>0.5F;
	}

	/** Slice the whole signal into bits. */
    BitVector1 sliced() const;

};



std::ostream& operator<<(std::ostream&, const SoftVector1&);






#endif
// vim: ts=4 sw=4
