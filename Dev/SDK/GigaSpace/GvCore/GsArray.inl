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

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsError.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

	/******************************************************************************
	 * Constructor allocating an array with the given resolution.
	 *
	 * @param pResolution The resolution
	 * @param pMemoryType The memory type
	 ******************************************************************************/
	template< typename T >
	inline Array3D< T >::Array3D( const uint3& pResolution, uint pMemoryType )
	:	_data( NULL )
	{
		_resolution = pResolution;
		_arrayOptions = pMemoryType;

		if ( _arrayOptions == CudaPinnedMemory ) // Uses pinned memory
		{
			GS_CUDA_SAFE_CALL( cudaMallocHost( (void**)&_data, getMemorySize() ) );
		}
		else if ( _arrayOptions == CudaMappedMemory ) // Uses gpu mapped memory
		{
			GS_CUDA_SAFE_CALL( cudaHostAlloc( (void**)&_data, getMemorySize(), cudaHostAllocMapped | cudaHostAllocWriteCombined ) );

			GV_CHECK_CUDA_ERROR( "CUDA Mappable memory creation failled, check cudaSetDeviceFlags(cudaDeviceMapHost | ...) has been called before any cuda operation. " );
		}
		else
		{
			_data = new T[ getNumElements() ];
		}

		GV_CHECK_CUDA_ERROR( "Array3D< T >::Array3D" );
	}

	/******************************************************************************
	 * Constructor taking storage from external allocation.
	 *
	 * @param pData Pointer on an external allocated data
	 * @param pResolution The resolution
	 ******************************************************************************/
	template< typename T >
	inline Array3D< T >::Array3D( T* pData, const uint3& pResolution )
	{
		_resolution = pResolution;
		manualSetDataStorage( pData );

		_arrayOptions = _arrayOptions | static_cast< uint >( SharedData );
	}

	/******************************************************************************
	 * Destructor
	 ******************************************************************************/
	template< typename T >
	inline Array3D< T >::~Array3D()
	{
		if ( ! ( _arrayOptions & static_cast< uint >( SharedData ) ) )
		{
			if ( _arrayOptions == CudaPinnedMemory || _arrayOptions == CudaMappedMemory )
			{
				GS_CUDA_SAFE_CALL( cudaFreeHost( _data ) );
				_data = NULL;
			}
			else
			{
				delete[] _data;
				_data = NULL;
			}
		}
	}

	/******************************************************************************
	 * Return an index in the array given a 3D position. Out of bounds are check.
	 *
	 * @param pPosition 3D position
	 *
	 * @return An index in the array
	 ******************************************************************************/
	template< typename T >
	inline uint3 Array3D< T >::getSecureIndex( uint3 pPosition ) const
	{
		if ( pPosition.x >= _resolution.x )
		{
			pPosition.x = _resolution.x - 1;
		}

		if ( pPosition.y >= _resolution.y )
		{
			pPosition.y = _resolution.y - 1;
		}

		if ( pPosition.z >= _resolution.z )
		{
			pPosition.z = _resolution.z - 1;
		}

		return pPosition;
	}

	/******************************************************************************
	 * Set manual resolution
	 *
	 * @param pResolution The resolution
	 ******************************************************************************/
	template< typename T >
	inline void Array3D< T >::manualSetResolution( const uint3& pResolution )
	{
		assert( pResolution.x * pResolution.y * pResolution.z == getNumElements() );

		_resolution = pResolution;
	}

	/******************************************************************************
	 * Set manual data stotage
	 *
	 * @param pData Pointer on data
	 ******************************************************************************/
	template< typename T >
	inline void Array3D< T >::manualSetDataStorage( T* pData )
	{
		_data = pData;
	}

	/******************************************************************************
	 * Return the current array size.
	 *
	 * @return The array size
	 ******************************************************************************/
	template< typename T >
	inline uint3 Array3D< T >::getResolution() const
	{
		return _resolution;
	}

	/******************************************************************************
	 * Return the number of elements contained in the array.
	 *
	 * @return The number of elements
	 ******************************************************************************/
	template< typename T >
	inline size_t Array3D< T >::getNumElements() const
	{
		return static_cast< size_t >( _resolution.x ) * static_cast< size_t >( _resolution.y ) * static_cast< size_t >( _resolution.z );
	}

	/******************************************************************************
	 * Return the amount of memory used by the array.
	 *
	 * @return The used memory size
	 ******************************************************************************/
	template< typename T >
	inline size_t Array3D< T >::getMemorySize() const
	{
		return getNumElements() * sizeof( T );
	}

	/******************************************************************************
	 * Get the stored value at a given position in the array
	 *
	 * @param pPosition Position in the array
	 *
	 * @return The stored value
	 ******************************************************************************/
	template< typename T >
	inline T& Array3D< T >::get( const uint3& pPosition )
	{
		return _data[ pPosition.x + pPosition.y * _resolution.x + pPosition.z * _resolution.x * _resolution.y ];
	}

	/******************************************************************************
	 * Get the stored value at a given position in the array
	 *
	 * @param pOffset Offset position in the array
	 *
	 * @return The stored value
	 ******************************************************************************/
	template< typename T >
	inline T& Array3D< T >::get( size_t pOffset )
	{
		return _data[ pOffset ];
	}

	/******************************************************************************
	 * Get the stored value at a given position in the array
	 *
	 * @param pPosition Position in the array
	 *
	 * @return The stored value
	 ******************************************************************************/
	template< typename T >
	inline T Array3D< T >::getConst( const uint3& pPosition ) const
	{
		return _data[ pPosition.x + pPosition.y * _resolution.x + pPosition.z * _resolution.x * _resolution.y ];
	}

	/******************************************************************************
	 * Get the stored value at a given position in the array
	 *
	 * @param pPosition Position in the array
	 *
	 * @return The stored value
	 ******************************************************************************/
	template< typename T >
	inline T& Array3D< T >::getSafe( uint3 pPosition ) const
	{
		pPosition = getSecureIndex( pPosition );

		return _data[ pPosition.x + pPosition.y * _resolution.x + pPosition.z* _resolution.x * _resolution.y ];
	}

	/******************************************************************************
	 * Return a pointer to the element located at the given 3D position.
	 * The element cannot be modified.
	 *
	 * @param pPosition The given 3D position
	 *
	 * @return Pointer on the stored value
	 ******************************************************************************/
	template< typename T >
	inline const T* Array3D< T >::getConstPointer( const uint3& pPosition ) const
	{
		return &_data[ pPosition.x + pPosition.y * _resolution.x + pPosition.z * _resolution.x * _resolution.y ];
	}

	/******************************************************************************
	 * Return a pointer to the element located at the given 3D position.
	 *
	 * @param pPosition The given 3D position
	 *
	 * @return Pointer on the stored value
	 ******************************************************************************/
	template< typename T >
	inline T* Array3D< T >::getPointer( const uint3& pPosition ) const
	{
		return &_data[ pPosition.x + pPosition.y * _resolution.x + pPosition.z * _resolution.x * _resolution.y ];
	}

	/******************************************************************************
	 * Return a pointer on the first array element
	 *
	 * @return Pointer on the first array element
	 ******************************************************************************/
	template< typename T >
	inline T* Array3D< T >::getPointer() const
	{
		return _data;
	}

	/******************************************************************************
	 * Return a pointer to the element located at the given 1D position.
	 *
	 * @param pAddress The 1D address position
	 *
	 * @return Pointer on the stored value
	 ******************************************************************************/
	template< typename T >
	inline T* Array3D< T >::getPointer( size_t pAddress ) const
	{
		return &_data[ pAddress ];
	}

	/******************************************************************************
	 * Initialize the array with a given value
	 *
	 * @param pValue The value
	 ******************************************************************************/
	template< typename T >
	inline void Array3D< T >::fill( int pValue )
	{
		memset( _data, pValue, getMemorySize() );
	}

	/******************************************************************************
	 * Initialize the array with 0
	 ******************************************************************************/
	/*template< typename T >
	inline void Array3D< T >::zero()
	{
		memset( _data, 0, getMemorySize() );
	}*/

	/******************************************************************************
	 * GPU related stuff.
	 * Get the CUDA pitch pointer.
	 *
	 * @return the CUDA pitch pointer
	 ******************************************************************************/
	template< typename T >
	inline cudaPitchedPtr Array3D< T >::getCudaPitchedPtr() const
	{
		return make_cudaPitchedPtr( (void *)_data, static_cast< size_t >( _resolution.x ) * sizeof( T ),
									static_cast< size_t >( _resolution.x ),
									static_cast< size_t >( _resolution.y ) );
	}

	/******************************************************************************
	 * Return a pointer on the data mapped into the GPU address space
	 *
	 * @return Pointer on the data
	 ******************************************************************************/
	template< typename T >
	inline T* Array3D< T >::getGPUMappedPointer() const
	{
		if ( _arrayOptions != CudaMappedMemory )
		{
			std::cout << "Array< T >::getGPUMappedPointer: Error, memory was not created as mappable \n";
			return NULL;
		}

		T* devicePointer;
		cudaHostGetDevicePointer( (void**)&devicePointer, (void*)_data, 0 );

		return devicePointer;
	}

	/******************************************************************************
	 * Returns a device array able to access the system memory data through a mapped pointer.
	 *
	 * @return The associated device array
	 ******************************************************************************/
	template< typename T >
	inline GsLinearMemoryKernel< T > Array3D< T >::getDeviceArray() const
	{
		GsLinearMemoryKernel< T > kal;
		kal.init( this->getGPUMappedPointer(), make_uint3( this->_resolution ),	_resolution.x * sizeof( T ) );
		
		return kal;
	}

} // namespace GvCore
