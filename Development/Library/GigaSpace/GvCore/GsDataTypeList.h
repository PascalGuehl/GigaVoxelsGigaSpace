/*
 * GigaVoxels - GigaSpace
 *
 * Website: http://gigavoxels.inrialpes.fr/
 *
 * Contributors: GigaVoxels Team
 *
 * BSD 3-Clause License:
 *
 * Copyright (C) 2007-2015 INRIA - LJK (CNRS - Grenoble University), All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the organization nor the names  of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * @version 1.0
 */

#ifndef GVDATATYPELIST_H
#define GVDATATYPELIST_H

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GvCore/GsTypeHelpers.h"

// Cuda
//
// NOTE : the CUDA #include <host_defines.h> MUST be placed before the LOKI #include <loki/HierarchyGenerators.h>,
// because LOKI has been modified by adding the CUDA __host__ and __device__ specifiers in one of its class.
#include <host_defines.h>
#include <vector_types.h>

// Loki
#include <loki/Typelist.h>
#include <loki/HierarchyGenerators.h>
#include <loki/TypeManip.h>
#include <loki/NullType.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/


namespace GvCore
{

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @struct DataChannelType
 *
 * @brief The DataChannelType struct provides an access to a particular type from a list of types.
 *
 * @ingroup GvCore
 *
 * Given an index into a list if types, it returns the type at the specified index from the list.
 */
template< class TList, unsigned int index >
struct DataChannelType
{
	/**
	 * Type definition.
	 * Result is the type at the specified index from the list.
	 */
	typedef typename Loki::TL::TypeAt< TList, index >::Result Result;
};

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/**
 * Functor used to serialize/deserialize data pool of a data structure
 */
template< typename TDataTypeList >
struct GvDataTypeInspector
{
	/**
	 * List of data types
	 */
	std::vector< std::string > _dataTypes;

	/**
	 * Generalized functor method used to bound textures.
	 *
	 * @param Loki::Int2Type< i > channel
	 */
	template< int TIndex >
	inline void run( Loki::Int2Type< TIndex > )
	{
		typedef typename GvCore::DataChannelType< TDataTypeList, TIndex >::Result VoxelType;

		//std::cout << "-\t" << GvCore::typeToString< GvCore::DataChannelType< TDataTypeList, TIndex > >() << std::endl;
		const char* type = GvCore::typeToString< VoxelType >();
		_dataTypes.push_back( type );
	}
};

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @struct DataNumChannels
 *
 * @brief The DataNumChannels struct provides the way the access the number of elements in a list of types.
 *
 * @ingroup GvCore
 *
 * Given a list of types, it returns the number of elements in the list.
 */
template< class TList >
struct DataNumChannels
{
	/**
	 * Enumeration definition.
	 * value is equal to the number of elements in the list if types.
	 */
	enum
	{
		value = Loki::TL::Length< TList >::value
	};
};

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @struct DataChannelSize
 *
 * @brief The DataChannelSize struct provides...
 *
 * @ingroup GvCore
 *
 * ...
 */
template< class TList, int index >
struct DataChannelSize
{
	/**
	 * ...
	 */
	enum
	{
		value = sizeof( typename Loki::TL::TypeAt< TList, index >::Result )
	};
};

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @struct DataTotalChannelSize
 *
 * @brief The DataTotalChannelSize struct provides...
 *
 * @ingroup GvCore
 *
 * ...
 */
template< class TList, int index = GvCore::DataNumChannels< TList >::value - 1 >
struct DataTotalChannelSize
{
	/**
	 * ...
	 */
	enum
	{
		value = DataChannelSize< TList, index >::value + DataChannelSize< TList, index - 1 >::value
	};
};

/** 
 * DataTotalChannelSize struct specialization
 */
template< class TList >
struct DataTotalChannelSize< TList, 0 >
{
	/**
	 * ...
	 */
	enum
	{
		value = DataChannelSize< TList, 0 >::value
	};
};

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @struct DataChannelUnitValue
 *
 * @brief The DataChannelUnitValue struct provides...
 *
 * @ingroup GvCore
 *
 * ...
 */
template< class T >
struct DataChannelUnitValue
{
	/**
	 * ...
	 */
	typedef T StorageType;

	/**
	 * ...
	 */
	StorageType value_;
   /* operator StorageType&() { return value_; }
	operator const StorageType&() const { return value_; }*/
};

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @struct DataStruct
 *
 * @brief The DataStruct struct provides...
 *
 * @ingroup GvCore
 *
 * ...
 */
template< class TList >
struct DataStruct
{
	/**
	 * ...
	 */
	typedef Loki::GenScatterHierarchy< TList, DataChannelUnitValue > Result;
};

/////////Type reconition///////

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @struct IsFloatFormat
 *
 * @brief The IsFloatFormat struct provides...
 *
 * @ingroup GvCore
 *
 * ...
 */
template< class T >
struct IsFloatFormat
{
	/**
	 * ...
	 */
	enum
	{
		value = 0
	};
};

/**
 * IsFloatFormat struct specialization
 */
template<>
struct IsFloatFormat< float >
{
	/**
	 * ...
	 */
	enum
	{
		value = 1
	};
};

/**
 * IsFloatFormat struct specialization
 */
template<>
struct IsFloatFormat< float2 >
{
	/**
	 * ...
	 */
	enum
	{
		value = 1
	};
};

/**
 * IsFloatFormat struct specialization
 */
template<>
struct IsFloatFormat< float3 >
{
	/**
	 * ...
	 */
	enum
	{
		value = 1
	};
};

/**
 * IsFloatFormat struct specialization
 */
template<>
struct IsFloatFormat< float4 >
{
	/**
	 * ...
	 */
	enum
	{
		value = 1
	};
};

/**
 * IsFloatFormat struct specialization
 */
template<>
struct IsFloatFormat< half4 >
{
	/**
	 * ...
	 */
	enum
	{
		value = 1
	};
};

////////////////////////////////
//////ReplaceTypeTemplated//////
////////////////////////////////

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @struct ReplaceTypeTemplated
 *
 * @brief The ReplaceTypeTemplated struct provides...
 *
 * @ingroup GvCore
 *
 * Replacement with type templated by replaced type
 */
template< class TList, template< typename > class RT >
struct ReplaceTypeTemplated;

/**
 * ReplaceTypeTemplated struct specialization
 */
template< template< typename > class RT >
struct ReplaceTypeTemplated< Loki::NullType, RT >
{
	typedef Loki::NullType Result;
};

/**
 * ReplaceTypeTemplated struct specialization
 */
template< class T, class Tail, template< typename > class RT >
struct ReplaceTypeTemplated< Loki::Typelist< T, Tail >, RT >
{
	typedef typename Loki::Typelist< RT< T >, typename ReplaceTypeTemplated< Tail, RT >::Result > Result;
};

////////////////////////////////
///////////Static loop//////////
////////////////////////////////

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @class StaticLoop
 *
 * @brief The StaticLoop class provides...
 *
 * @ingroup GvCore
 *
 * ...
 */
template< class SFunctor, int i >
class StaticLoop
{

public:

	/**
	 * ...
	 *
	 * @param f ...
	 */
	inline static void go( SFunctor& f )
	{
		StaticLoop< SFunctor, i - 1 >::go( f );
		f.run( Loki::Int2Type< i >() );
	}

};

/**
 * StaticLoop class specialization
 */
template< class SFunctor >
class StaticLoop< SFunctor, -1 >
{

public:

	/**
	 * ...
	 *
	 * @param f ...
	 */
	inline static void go( SFunctor& f )
	{
	}

};

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @class StaticLoopCallStatic
 *
 * @brief The StaticLoopCallStatic class provides...
 *
 * @ingroup GvCore
 *
 * ...
 */
template< class SFunctor, int i >
class StaticLoopCallStatic
{

public:

	/**
	 * ...
	 */
	__device__ __host__
	inline static void go()
	{
		SFunctor::run( Loki::Int2Type< i >() );
		StaticLoop< SFunctor, i - 1 >::go();
	}
};

/**
 * StaticLoopCallStatic class specialization
 */
template< class SFunctor >
class StaticLoopCallStatic< SFunctor, -1 >
{

public:

	/**
	 * ...
	 */
	__device__ __host__
	inline static void go()
	{
	}

};

} //namespace GvCore

#endif
