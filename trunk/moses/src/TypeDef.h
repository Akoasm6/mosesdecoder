// $Id$

/***********************************************************************
Moses - factored phrase-based language decoder
Copyright (C) 2006 University of Edinburgh

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
***********************************************************************/

#pragma once

#include <list>
#include <limits>

#define PROJECT_NAME		"moses"

#ifndef BOS_
#define BOS_ "<s>" //Beginning of sentence symbol
#endif
#ifndef EOS_ 
#define EOS_ "</s>" //End of sentence symbol
#endif

#define UNKNOWN_FACTOR	"UNK"
#define EPSILON         "*EPS*"

#define NOT_FOUND 			std::numeric_limits<size_t>::max()
#define MAX_NGRAM_SIZE  20

const size_t DEFAULT_MAX_HYPOSTACK_SIZE = 200;
const size_t DEFAULT_MAX_TRANS_OPT_SIZE	= 50;
const size_t ARRAY_SIZE_INCR					= 10; //amount by which a phrase gets resized when necessary
const float LOWEST_SCORE							= -100.0f;
const float DEFAULT_BEAM_THRESHOLD		= 0.00001f;
const size_t DEFAULT_VERBOSE_LEVEL = 1;

///////////////////////////////////////////////// 
// for those using autoconf/automake
#if HAVE_CONFIG_H
#include "config.h"

#define TRACE_ENABLE 1		// REMOVE after we figure this out
#define N_BEST 1					// REMOVE

#  ifdef HAVE_SRILM
#    define LM_SRI 1
#    undef LM_INTERNAL
#  else
#    undef LM_SRI
#    define LM_INTERNAL 1
#  endif

#  ifdef HAVE_IRSTLM
#    define LM_IRST 1
#    undef LM_INTERNAL
#    undef LM_SRI
#  endif

#endif
///////////////////////////////////////////////// 

// enums. 
// must be 0, 1, 2, ..., unless otherwise stated

// can only be 2 at the moment
const int NUM_LANGUAGES = 2;

// count of above
const size_t NUM_FACTORS = 4;

enum FactorDirection
{	
	Input					= 0
	,Output				= 1
};

enum DecodeType
{
	Translate
	,Generate
  ,InsertNullFertilityWord //! an optional step that attempts to insert a few closed-class words to improve LM scores
};

namespace LexReorderType
{
	enum LexReorderType
		{
			Forward
			,Backward
			,Bidirectional
			,Fe
			,F
		};
}

namespace DistortionOrientationType
{
	enum DistortionOrientationOptions
		{
			Monotone
			,Msd
		};	
    // Possible values for orientation.
	enum ORIENTATIONS { MONO, NON_MONO, SWAP, DISC };
		
	
};

enum IOMethod
{
	IOMethodCommandLine
	,IOMethodFile
	,IOMethodMySQL
};

enum LMType
{	
	SRI
	,IRST
	,Chunking
};

// typedef
typedef size_t FactorType;

class Factor;
typedef const Factor * FactorArray[NUM_FACTORS];
