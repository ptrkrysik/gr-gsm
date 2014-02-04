/*
* Copyright 2007 Free Software Foundation, Inc.
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


#ifndef ASSERT_H
#define ASSERT_H

#include "stdio.h"
#include <iostream>

//#define NDEBUG

/**@name Macros for standard messages. */
//@{
#define COUT(text) { std::cout << text << std::endl; }
#define CERR(text) { std::cerr << __FILE__ << ":" << __LINE__ << ": " << text; }
#ifdef NDEBUG
#define DCOUT(text) {}
#define OBJDCOUT(text) {}
#else
#define DCOUT(text) { COUT(__FILE__ << ":" << __LINE__ << " " << text); }
#define OBJDCOUT(text) { DCOUT(this << " " << text); }
#endif
//@}


/** This is thrown by assert() so that gdb can catch it. */

class assertion
{

  public:

    assertion() {
      fprintf( stderr,"Try setting a breakpoint @ %s:%u.\n",__FILE__,__LINE__ );
      return;
    }

};

#ifdef NDEBUG
#define assert(EXPR) {};
#else
/** This replaces the regular assert() with a C++ exception. */
#include "stdio.h"
#define assert(E) { if (!(E)) { fprintf(stderr,"%s:%u failed assertion '%s'\n",__FILE__,__LINE__,#E); throw Assertion(); } }
#endif

#endif
