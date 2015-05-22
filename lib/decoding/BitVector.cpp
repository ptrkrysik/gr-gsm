/*
* Copyright 2008 Free Software Foundation, Inc.
*
* This software is distributed under the terms of the GNU Public License.
* See the COPYING file in the main directory for details.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/




#include "BitVector.h"
#include <iostream>

using namespace std;


/**
  Apply a Galois polymonial to a binary seqeunce.
  @param val The input sequence.
  @param poly The polynomial.
  @param order The order of the polynomial.
  @return Single-bit result.
*/
unsigned applyPoly(uint64_t val, uint64_t poly, unsigned order)
{
	uint64_t prod = val & poly;
	unsigned sum = prod;
	for (unsigned i=1; i<order; i++) sum ^= prod>>i;
	return sum & 0x01;
}






BitVector::BitVector(const char *valString)
	:Vector<char>(strlen(valString))
{
	uint32_t accum = 0;
	for (size_t i=0; i<size(); i++) {
		accum <<= 1;
		if (valString[i]=='1') accum |= 0x01;
		mStart[i] = accum;
	}
}





uint64_t BitVector::peekField(size_t readIndex, unsigned length) const
{
	uint64_t accum = 0;
	char *dp = mStart + readIndex;
	assert(dp+length <= mEnd);
	for (unsigned i=0; i<length; i++) {
		accum = (accum<<1) | ((*dp++) & 0x01);
	}
	return accum;
}


uint64_t BitVector::readField(size_t& readIndex, unsigned length) const
{
	const uint64_t retVal = peekField(readIndex,length);
	readIndex += length;
	return retVal;
}


void BitVector::fillField(size_t writeIndex, uint64_t value, unsigned length)
{
	char *dpBase = mStart + writeIndex;
	char *dp = dpBase + length - 1;
	assert(dp < mEnd);
	while (dp>=dpBase) {
		*dp-- = value & 0x01;
		value >>= 1;
	}
}

void BitVector::writeField(size_t& writeIndex, uint64_t value, unsigned length)
{
	fillField(writeIndex,value,length);
	writeIndex += length;
}


void BitVector::invert()
{
	for (size_t i=0; i<size(); i++) {
		mStart[i] = ~mStart[i];
	}
}




void BitVector::reverse8()
{
	assert(size()>=8);

	char tmp0 = mStart[0];
	mStart[0] = mStart[7];
	mStart[7] = tmp0;

	char tmp1 = mStart[1];
	mStart[1] = mStart[6];
	mStart[6] = tmp1;

	char tmp2 = mStart[2];
	mStart[2] = mStart[5];
	mStart[5] = tmp2;

	char tmp3 = mStart[3];
	mStart[3] = mStart[4];
	mStart[4] = tmp3;
}



void BitVector::LSB8MSB()
{
	size_t size8 = 8*(size()/8);
	size_t iTop = size8 - 8;
	for (size_t i=0; i<=iTop; i+=8) segment(i,8).reverse8();
}



uint64_t BitVector::syndrome(Generator& gen) const
{
	gen.clear();
	const char *dp = mStart;
	while (dp<mEnd) gen.syndromeShift(*dp++);
	return gen.state();
}


uint64_t BitVector::parity(Generator& gen) const
{
	gen.clear();
	const char *dp = mStart;
	while (dp<mEnd) gen.encoderShift(*dp++);
	return gen.state();
}


void BitVector::encode(const ViterbiR2O4& coder, BitVector& target)
{
	size_t sz = size();
	assert(sz*coder.iRate() == target.size());

	// Build a "history" array where each element contains the full history.
	uint32_t history[sz];
	uint32_t accum = 0;
	for (size_t i=0; i<sz; i++) {
		accum = (accum<<1) | bit(i);
		history[i] = accum;
	}

	// Look up histories in the pre-generated state table.
	char *op = target.begin();
	for (size_t i=0; i<sz; i++) {
		unsigned index = coder.cMask() & history[i];
		for (unsigned g=0; g<coder.iRate(); g++) {
			*op++ = coder.stateTable(g,index);
		}
	}
}



unsigned BitVector::sum() const
{
	unsigned sum = 0;
	for (size_t i=0; i<size(); i++) sum += mStart[i] & 0x01;
	return sum;
}




void BitVector::map(const unsigned *map, size_t mapSize, BitVector& dest) const
{
	for (unsigned i=0; i<mapSize; i++) {
		dest.mStart[i] = mStart[map[i]];
	}
}




void BitVector::unmap(const unsigned *map, size_t mapSize, BitVector& dest) const
{
	for (unsigned i=0; i<mapSize; i++) {
		dest.mStart[map[i]] = mStart[i];
	}
}










ostream& operator<<(ostream& os, const BitVector& hv)
{
	for (size_t i=0; i<hv.size(); i++) {
		if (hv.bit(i)) os << '1';
		else os << '0';
	}
	return os;
}




ViterbiR2O4::ViterbiR2O4()
{
	assert(mDeferral < 32);
	mCoeffs[0] = 0x019;
	mCoeffs[1] = 0x01b;
	computeStateTables(0);
	computeStateTables(1);
	computeGeneratorTable();
}




void ViterbiR2O4::initializeStates()
{
	for (unsigned i=0; i<mIStates; i++) clear(mSurvivors[i]);
	for (unsigned i=0; i<mNumCands; i++) clear(mCandidates[i]);
}



void ViterbiR2O4::computeStateTables(unsigned g)
{
	assert(g<mIRate);
	for (unsigned state=0; state<mIStates; state++) {
		// 0 input
		uint32_t inputVal = state<<1;
		mStateTable[g][inputVal] = applyPoly(inputVal, mCoeffs[g], mOrder+1);
		// 1 input
		inputVal |= 1;
		mStateTable[g][inputVal] = applyPoly(inputVal, mCoeffs[g], mOrder+1);
	}
}

void ViterbiR2O4::computeGeneratorTable()
{
	for (unsigned index=0; index<mIStates*2; index++) {
		mGeneratorTable[index] = (mStateTable[0][index]<<1) | mStateTable[1][index];
	}
}






void ViterbiR2O4::branchCandidates()
{
	// Branch to generate new input states.
	const vCand *sp = mSurvivors;
	for (unsigned i=0; i<mNumCands; i+=2) {
		// extend and suffix
		const uint32_t iState0 = (sp->iState) << 1;				// input state for 0
		const uint32_t iState1 = iState0 | 0x01;				// input state for 1
		const uint32_t oStateShifted = (sp->oState) << mIRate;	// shifted output
		const float cost = sp->cost;
		sp++;
		// 0 input extension
		mCandidates[i].cost = cost;
		mCandidates[i].oState = oStateShifted | mGeneratorTable[iState0 & mCMask];
		mCandidates[i].iState = iState0;
		// 1 input extension
		mCandidates[i+1].cost = cost;
		mCandidates[i+1].oState = oStateShifted | mGeneratorTable[iState1 & mCMask];
		mCandidates[i+1].iState = iState1;
	}
}


void ViterbiR2O4::getSoftCostMetrics(const uint32_t inSample, const float *matchCost, const float *mismatchCost)
{
	const float *cTab[2] = {matchCost,mismatchCost};
	for (unsigned i=0; i<mNumCands; i++) {
		vCand& thisCand = mCandidates[i];
		// We examine input bits 2 at a time for a rate 1/2 coder.
		const unsigned mismatched = inSample ^ (thisCand.oState);
		thisCand.cost += cTab[mismatched&0x01][1] + cTab[(mismatched>>1)&0x01][0];
	}
}


void ViterbiR2O4::pruneCandidates()
{
	const vCand* c1 = mCandidates;					// 0-prefix
	const vCand* c2 = mCandidates + mIStates;		// 1-prefix
	for (unsigned i=0; i<mIStates; i++) {
		if (c1[i].cost < c2[i].cost) mSurvivors[i] = c1[i];
		else mSurvivors[i] = c2[i];
	}
}


const ViterbiR2O4::vCand& ViterbiR2O4::minCost() const
{
	int minIndex = 0;
	float minCost = mSurvivors[0].cost;
	for (unsigned i=1; i<mIStates; i++) {
		const float thisCost = mSurvivors[i].cost;
		if (thisCost>=minCost) continue;
		minCost = thisCost;
		minIndex=i;
	}
	return mSurvivors[minIndex];
}


const ViterbiR2O4::vCand& ViterbiR2O4::step(uint32_t inSample, const float *probs, const float *iprobs)
{
	branchCandidates();
	getSoftCostMetrics(inSample,probs,iprobs);
	pruneCandidates();
	return minCost();
}


uint64_t Parity::syndrome(const BitVector& receivedCodeword)
{
	return receivedCodeword.syndrome(*this);
}


void Parity::writeParityWord(const BitVector& data, BitVector& parityTarget, bool invert)
{
	uint64_t pWord = data.parity(*this);
	if (invert) pWord = ~pWord; 
	parityTarget.fillField(0,pWord,size());
}









SoftVector::SoftVector(const BitVector& source)
{
	resize(source.size());
	for (size_t i=0; i<size(); i++) {
		if (source.bit(i)) mStart[i]=1.0F;
		else mStart[i]=0.0F;
	}
}


BitVector SoftVector::sliced() const
{
	size_t sz = size();
	BitVector newSig(sz);
	for (size_t i=0; i<sz; i++) {
		if (mStart[i]>0.5F) newSig[i]=1;
		else newSig[i] = 0;
	}
	return newSig;
}



void SoftVector::decode(ViterbiR2O4 &decoder, BitVector& target) const
{
	const size_t sz = size();
	const unsigned deferral = decoder.deferral();
	const size_t ctsz = sz + deferral;
	assert(sz <= decoder.iRate()*target.size());

	// Build a "history" array where each element contains the full history.
	uint32_t history[ctsz];
	{
		BitVector bits = sliced();
		uint32_t accum = 0;
		for (size_t i=0; i<sz; i++) {
			accum = (accum<<1) | bits.bit(i);
			history[i] = accum;
		}
		// Repeat last bit at the end.
		for (size_t i=sz; i<ctsz; i++) {
			accum = (accum<<1) | (accum & 0x01);
			history[i] = accum;
		}
	}

	// Precompute metric tables.
	float matchCostTable[ctsz];
	float mismatchCostTable[ctsz];
	{
		const float *dp = mStart;
		for (size_t i=0; i<sz; i++) {
			// pVal is the probability that a bit is correct.
			// ipVal is the probability that a bit is correct.
			float pVal = dp[i];
			if (pVal>0.5F) pVal = 1.0F-pVal;
			float ipVal = 1.0F-pVal;
			// This is a cheap approximation to an ideal cost function.
			if (pVal<0.01F) pVal = 0.01;
			if (ipVal<0.01F) ipVal = 0.01;
			matchCostTable[i] = 0.25F/ipVal;
			mismatchCostTable[i] = 0.25F/pVal;
		}
	
		// pad end of table with unknowns
		for (size_t i=sz; i<ctsz; i++) {
			matchCostTable[i] = 0.5F;
			mismatchCostTable[i] = 0.5F;
		}
	}

	{
		decoder.initializeStates();
		// Each sample of history[] carries its history.
		// So we only have to process every iRate-th sample.
		const unsigned step = decoder.iRate();
		// input pointer
		const uint32_t *ip = history + step - 1;
		// output pointers
		char *op = target.begin();
		const char *const opt = target.end();
		// table pointers
		const float* match = matchCostTable;
		const float* mismatch = mismatchCostTable;
		size_t oCount = 0;
		while (op<opt) {
			// Viterbi algorithm
			const ViterbiR2O4::vCand &minCost = decoder.step(*ip, match, mismatch);
			ip += step;
			match += step;
			mismatch += step;
			// output
			if (oCount>=deferral) *op++ = (minCost.iState >> deferral);
			oCount++;
		}
	}
}




ostream& operator<<(ostream& os, const SoftVector& sv)
{
	for (size_t i=0; i<sv.size(); i++) {
		if (sv[i]<0.25) os << "0";
		else if (sv[i]>0.75) os << "1";
		else os << "-";
	}
	return os;
}



void BitVector::pack(unsigned char* targ) const
{
	// Assumes MSB-first packing.
	unsigned bytes = size()/8;
	for (unsigned i=0; i<bytes; i++) {
		targ[i] = peekField(i*8,8);
	}
	unsigned whole = bytes*8;
	unsigned rem = size() - whole;
	if (rem==0) return;
	targ[bytes] = peekField(whole,rem) << (8-rem);
}


void BitVector::unpack(const unsigned char* src)
{
	// Assumes MSB-first packing.
	unsigned bytes = size()/8;
	for (unsigned i=0; i<bytes; i++) {
		fillField(i*8,src[i],8);
	}
	unsigned whole = bytes*8;
	unsigned rem = size() - whole;
	if (rem==0) return;
	fillField(whole,src[bytes],rem);
}

// vim: ts=4 sw=4
