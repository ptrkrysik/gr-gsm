/**@file Simplified Vector template with aliases. */
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




#ifndef VECTOR_H
#define VECTOR_H

#include <string.h>
#include <iostream>
#include <assert.h>

/**
	A simplified Vector template with aliases.
	Unlike std::vector, this class does not support dynamic resizing.
	Unlike std::vector, this class does support "aliases" and subvectors.
*/
template <class T> class Vector {

	// TODO -- Replace memcpy calls with for-loops.

	public:

	/**@name Iterator types. */
	//@{
	typedef T* iterator;
	typedef const T* const_iterator;
	//@}

	protected:

	T* mData;		///< allocated data block, if any
	T* mStart;		///< start of useful data
	T* mEnd;		///< end of useful data + 1

	public:

	/** Return the size of the Vector. */
	size_t size() const
	{
		assert(mStart>=mData);
		assert(mEnd>=mStart);
		return mEnd - mStart;
	}

	/** Return size in bytes. */
	size_t bytes() const { return size()*sizeof(T); }

	/** Change the size of the Vector, discarding content. */
	void resize(size_t newSize)
	{
		if (mData!=NULL) delete[] mData;
		if (newSize==0) mData=NULL;
		else mData = new T[newSize];
		mStart = mData;
		mEnd = mStart + newSize;
	}

	/** Release memory and clear pointers. */
	void clear() { resize(0); }


	/** Copy data from another vector. */
	void clone(const Vector<T>& other)
	{
		resize(other.size());
		memcpy(mData,other.mStart,other.bytes());
	}




	//@{

	/** Build an empty Vector of a given size. */
	Vector(size_t wSize=0):mData(NULL) { resize(wSize); }

	/** Build a Vector by shifting the data block. */
	Vector(Vector<T>& other)
		:mData(other.mData),mStart(other.mStart),mEnd(other.mEnd)
	{ other.mData=NULL; }

	/** Build a Vector by copying another. */
	Vector(const Vector<T>& other):mData(NULL) { clone(other); }

	/** Build a Vector with explicit values. */
	Vector(T* wData, T* wStart, T* wEnd)
		:mData(wData),mStart(wStart),mEnd(wEnd)
	{ }

	/** Build a vector from an existing block, NOT to be deleted upon destruction. */
	Vector(T* wStart, size_t span)
		:mData(NULL),mStart(wStart),mEnd(wStart+span)
	{ }

	/** Build a Vector by concatenation. */
	Vector(const Vector<T>& other1, const Vector<T>& other2)
		:mData(NULL)
	{
		resize(other1.size()+other2.size());
		memcpy(mStart, other1.mStart, other1.bytes());
		memcpy(mStart+other1.size(), other2.mStart, other2.bytes());
	}

	//@}

	/** Destroy a Vector, deleting held memory. */
	~Vector() { clear(); }




	//@{

	/** Assign from another Vector, shifting ownership. */
	void operator=(Vector<T>& other)
	{
		clear();
		mData=other.mData;
		mStart=other.mStart;
		mEnd=other.mEnd;
		other.mData=NULL;
	}

	/** Assign from another Vector, copying. */
	void operator=(const Vector<T>& other) { clone(other); }

	//@}


	//@{

	/** Return an alias to a segment of this Vector. */
	Vector<T> segment(size_t start, size_t span)
	{
		T* wStart = mStart + start;
		T* wEnd = wStart + span;
		assert(wEnd<=mEnd);
		return Vector<T>(NULL,wStart,wEnd);
	}

	/** Return an alias to a segment of this Vector. */
	const Vector<T> segment(size_t start, size_t span) const
	{
		T* wStart = mStart + start;
		T* wEnd = wStart + span;
		assert(wEnd<=mEnd);
		return Vector<T>(NULL,wStart,wEnd);
	}

	Vector<T> head(size_t span) { return segment(0,span); }
	const Vector<T> head(size_t span) const { return segment(0,span); }
	Vector<T> tail(size_t start) { return segment(start,size()-start); }
	const Vector<T> tail(size_t start) const { return segment(start,size()-start); }

	/**
		Copy part of this Vector to a segment of another Vector.
		@param other The other vector.
		@param start The start point in the other vector.
		@param span The number of elements to copy.
	*/
	void copyToSegment(Vector<T>& other, size_t start, size_t span) const
	{
		T* base = other.mStart + start;
		assert(base+span<=other.mEnd);
		assert(mStart+span<=mEnd);
		memcpy(base,mStart,span*sizeof(T));
	}

	/** Copy all of this Vector to a segment of another Vector. */
	void copyToSegment(Vector<T>& other, size_t start=0) const { copyToSegment(other,start,size()); }

	void copyTo(Vector<T>& other) const { copyToSegment(other,0,size()); }

	/**
		Copy a segment of this vector into another.
		@param other The other vector (to copt into starting at 0.)
		@param start The start point in this vector.
		@param span The number of elements to copy.
	*/
	void segmentCopyTo(Vector<T>& other, size_t start, size_t span)
	{
		T* base = mStart + start;
		assert(base+span<=mEnd);
		assert(other.mStart+span<=other.mEnd);
		memcpy(other.mStart,base,span*sizeof(T));
	}

	void fill(const T& val)
	{
		T* dp=mStart;
		while (dp<mEnd) *dp++=val;
	}


	//@}


	//@{

	T& operator[](size_t index)
	{
		assert(mStart+index<mEnd);
		return mStart[index];
	}

	const T& operator[](size_t index) const
	{
		assert(mStart+index<mEnd);
		return mStart[index];
	}

	const T* begin() const { return mStart; }
	T* begin() { return mStart; }
	const T* end() const { return mEnd; }
	T* end() { return mEnd; }
	//@}


};




/** Basic print operator for Vector objects. */
template <class T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& v)
{
	for (unsigned i=0; i<v.size(); i++) os << v[i] << " ";
	return os;
}



#endif
// vim: ts=4 sw=4
