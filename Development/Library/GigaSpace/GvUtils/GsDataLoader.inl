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

// GigaSpace
#include "GvUtils/GsBrickLoaderChannelInitializer.h"
#include "GvCore/GsError.h"
#include "GvStructure/GsReader.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvUtils
{

/******************************************************************************
 * Constructor
 *
 * @param pName filename
 * @param pDataSize volume resolution
 * @param pBlocksize brick resolution
 * @param pBordersize brick border size
 * @param pUseCache  flag to tell wheter or not a cache mechanismn is required when reading files (nodes and bricks)
 ******************************************************************************/
template< typename TDataTypeList >
GsDataLoader< TDataTypeList >
::GsDataLoader( const std::string& pName, const uint3& pBlocksize, int pBordersize, bool pUseCache )
:	GsIDataLoader< TDataTypeList >()
{
	// First, read Meta Data file to retrieve all mandatory information
	uint resolution = 0;
	int parse = this->parseXMLFile( pName.c_str(), resolution );
	assert( parse == 0 );
	// TODO : handle error
	// ...
		
	// Fill member variables
	this->_bricksRes.x = pBlocksize.x;
	this->_bricksRes.y = pBlocksize.y;
	this->_bricksRes.z = pBlocksize.z;
	this->_numChannels = Loki::TL::Length< TDataTypeList >::value;
	this->_volumeRes = make_uint3( resolution );//TO CHANGE
	// LOG
	printf( "%d\n", _volumeRes.x );
	this->_borderSize = pBordersize;
	this->_mipMapOrder = 2;
	this->_useCache = pUseCache;

	// Compute number of mipmaps levels
	int dataResMin = mincc( this->_volumeRes.x, mincc( this->_volumeRes.y, this->_volumeRes.z ) );
	int blocksNumLevels		= static_cast< int >( log( static_cast< float >( this->_bricksRes.x ) ) / log( static_cast< float >( this->_mipMapOrder ) ) );
	this->_numMipMapLevels	= static_cast< int >( log( static_cast< float >( dataResMin ) ) / log( static_cast< float >( this->_mipMapOrder ) ) ) + 1 ;
	this->_numMipMapLevels	= this->_numMipMapLevels - blocksNumLevels;
	if ( this->_numMipMapLevels < 1 )
	{
		this->_numMipMapLevels = 1;
	}

	// Compute full brick resolution (with borders)
	uint3 true_bricksRes = this->_bricksRes + make_uint3( 2 * this->_borderSize );

	// Build the list of all filenames that producer will have to load (nodes and bricks).
	//this->makeFilesNames( pName.c_str() );

	// If cache mechanismn is required, read all files (nodes and bricks),
	// and store data in associated buffers.
	if ( this->_useCache )
	{
		// Iterate through mipmap levels
		
		for ( int level = 0; level < _numMipMapLevels; level++ )
		{
			
			// Retrieve node filename at current mipmap level
			//
			// Files are stored by mipmap level in the list :
			// - first : node file
			// - then : brick file for each channel
			std::string fileNameIndex = _filesNames[ ( _numChannels + 1 ) * level ];
			
			// Open node file
			FILE* fileIndex = fopen( fileNameIndex.c_str(), "rb" );
			if ( fileIndex )
			{
				
				// Retrieve node file size
#ifdef WIN32
				_fseeki64( fileIndex, 0, SEEK_END );

				__int64 size = _ftelli64( fileIndex );
				__int64 expectedSize = (__int64)powf( 8.0f, static_cast< float >( level ) ) * sizeof( unsigned int );
#else
				fseeko( fileIndex, 0, SEEK_END );

				off_t size = ftello( fileIndex );
				off_t expectedSize = (off_t)powf( 8.0f, static_cast< float >( level ) ) * sizeof( unsigned int );
#endif
				// Handle error
				if ( size != expectedSize )
				{					
					std::cerr << "GsDataLoader::GsDataLoader: file size expected = " << expectedSize
								<< ", size returned = " << size << " for " << fileNameIndex << std::endl;
				}

				// Allocate a buffer in which read node data will be stored
				unsigned int* tmpCache = new unsigned int[ size / 4 ];

				// Position file pointer at beginning of file
#ifdef WIN32
				_fseeki64( fileIndex, 0, SEEK_SET );
#else
				fseeko( fileIndex, 0, SEEK_SET );
#endif
				// Read node file and store data in the tmpCache buffer
				if ( fread( tmpCache, 1, static_cast< size_t >( size ), fileIndex ) != size )
				{
					// Handle error if reading node file has failed
					std::cout << "GsDataLoader::GsDataLoader: Unable to open file " << this->_filesNames[ level ] << std::endl;
					
					this->_useCache = false;
				}
				
				// Close node file
				fclose( fileIndex );

				// Store node data in associated cache
				_blockIndexCache.push_back( tmpCache );
			}
			else
			{
				// Handle error if opening node file has failed
				std::cout << "GsDataLoader::GsDataLoader : Unable to open file index " << fileNameIndex << std::endl;
			}

			// For current mipmap level, iterate through channels
			for ( size_t channel = 0; channel < _numChannels; channel++ )
			{
				// Retrieve brick filename at current channel (at current mipmap level)
				//
				// Files are stored by mipmap level in the list :
				// - first : node file
				// - then : brick file for each channel
				
				// Open brick file
				unsigned int fileIndex = ( _numChannels + 1 ) * level + channel + 1;
				if ( fileIndex >= this->_filesNames.size() )
				{
					assert( false );
					std::cout << "GsDataLoader::GsDataLoader() => File index error." << std::endl;
				}
				FILE* brickFile = fopen( this->_filesNames[ fileIndex ].c_str(), "rb" );
				if ( brickFile )
				{
					// Retrieve brick file size
#ifdef WIN32
					_fseeki64( brickFile, 0, SEEK_END );

					__int64 size = _ftelli64( brickFile );
#else
					fseeko( brickFile, 0, SEEK_END );

					off_t size = ftello( brickFile );
#endif
					// Allocate a buffer in which read brick data will be stored
					unsigned char* tmpCache;
#if USE_GPUFETCHDATA
					cudaHostAlloc( (void**)&tmpCache, size, cudaHostAllocMapped | cudaHostAllocWriteCombined );
					
					void* deviceptr;
					cudaHostGetDevicePointer( &deviceptr, tmpCache, 0 );
					std::cout << "Device pointer host mem: " << static_cast< uint >( deviceptr ) << "\n";
#else
					// cudaMallocHost( (void**)&tmpCache, size ); // pinned memory
					// cudaHostAlloc( (void **)&tmpCache, size );
					tmpCache = new unsigned char[ static_cast< size_t >( size ) ];
#endif
					GV_CHECK_CUDA_ERROR( "GsDataLoader::GsDataLoader: cache alloc" );

					// Position file pointer at beginning of file
#ifdef WIN32
					_fseeki64( brickFile, 0, SEEK_SET );
#else
					fseeko( brickFile, 0, SEEK_SET );
#endif
					
					// Read brick file and store data in the tmpCache buffer
					if ( fread( tmpCache, 1, static_cast< size_t >( size ), brickFile ) != size )
					{
						// Handle error if reading brick file has failed
						std::cout << "GsDataLoader::GsDataLoader: Can't read file" << std::endl;
						
						this->_useCache = false;
					}
					
					// Close brick file
					fclose( brickFile );

					// Store brick data in associated cache
					_blockCache.push_back( tmpCache );
				}
				else
				{
					// Handle error if opening brick file has failed
					std::cout << "GsDataLoader::GsDataLoader: Unable to open file " << this->_filesNames[(_numChannels + 1) * level + channel + 1] << std::endl;
					
					this->_useCache = false;
				}
			}
		}
	}

}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< typename TDataTypeList >
GsDataLoader< TDataTypeList >
::~GsDataLoader()
{
	// Free memory of bricks data
	for	( size_t i = 0; i < _blockCache.size(); i++ )
	{
		if ( _blockCache[ i ] )
		{
#if USE_GPUFETCHDATA
			//cudaFree( _blockCache[ i ] );
			cudaFreeHost( _blockCache[ i ] );
#else
			delete[] _blockCache[ i ];
#endif
		}
	}

	// Free memory of nodes data
	for	( size_t i = 0; i < _blockIndexCache.size(); i++ )
	{
		if ( _blockIndexCache[ i ] )
		{
			delete [] _blockIndexCache[ i ];
		}
	}
}

/******************************************************************************
 * Build the list of all filenames that producer will have to load (nodes and bricks).
 * Given a root name, this function create filenames by adding brick resolution,
 * border size, file extension, etc...
 *
 * @param pFilename the root filename from which all filenames will be built
 ******************************************************************************/
template< typename TDataTypeList >
void GsDataLoader< TDataTypeList >
::makeFilesNames( const char* pFilename )
{
	//// Set common filenames parameters
	//std::string sourceFileName = std::string( pFilename );
	//std::string nodesFileNameExt = ".nodes";
	//std::string bricksFileNameExt = ".bricks";

	//// Iterate through mipmap levels
	//for	( int i = 0; i < _numMipMapLevels; i++ )
	//{
	//	std::stringstream ssNodes;

	//	// Build nodes file
	//	ssNodes << sourceFileName << "_BR" << _bricksRes.x << "_B" << _borderSize << "_L" << i << nodesFileNameExt;
	//	_filesNames.push_back( ssNodes.str() );

	//	// Build bricks file
	//	GsFileNameBuilder< TDataTypeList > fnb( _bricksRes.x, _borderSize, i, sourceFileName, bricksFileNameExt, _filesNames );
	//	GvCore::StaticLoop< GsFileNameBuilder< TDataTypeList >, Loki::TL::Length< TDataTypeList >::value - 1 >::go( fnb );
	//}

	//int parse = parseXMLFile(pFilename);
	//assert(parse ==0);
}


/******************************************************************************
 * Parses the XML configuration file
 *
 * @param pFilename the filename of the XML file
 ******************************************************************************/
template< typename TDataTypeList >
int GsDataLoader< TDataTypeList >
::parseXMLFile( const char* pFilename, uint& pResolution )
{
	// Reset internal state
	pResolution = 1;
	_filesNames.clear();

	// Use GigaSpace Meta Data file reader
	GvStructure::GsReader gigaSpaceReader;
	// - read Meta Data file
	bool statusOK = gigaSpaceReader.read( pFilename );
	pResolution = gigaSpaceReader.getModelResolution();
	_filesNames = gigaSpaceReader.getFilenames();
	// Handle error(s)
	assert( statusOK );
	if ( ! statusOK )
	{
		std::cout << "Error during reading GigaSpace's Meta Data file" << std::endl;
		
		return -1;
	}
	
	return 0;
}

/******************************************************************************
 * Retrieve the node encoded address given a mipmap level and a 3D node indexed position
 *
 * @param pLevel mipmap level
 * @param pBlockPos the 3D node indexed position
 *
 * @return the node encoded address
 ******************************************************************************/
template< typename TDataTypeList >
unsigned int GsDataLoader< TDataTypeList >
::getBlockIndex( int pLevel, const uint3& pBlockPos ) const
{
	// Retrieve the resolution at given level (i.e. the number of voxels in each dimension)
	uint3 levelSize = getLevelRes( pLevel );

	// Number of nodes at given level
	uint3 blocksInLevel = levelSize / this->_bricksRes;
	
	unsigned int indexValue = 0;

	// Check whether or not, cache mechanism is used
	if ( _useCache )
	{
		// _blockIndexCache is the buffer containing all read nodes data.
		// This a 2D array containing, for each mipmap level, the list of nodes addresses.

		// Compute the index of the node in the buffer of nodes, given its position
		//
		// Nodes are stored in increasing order from X axis first, then Y axis, then Z axis.
		uint indexPos = pBlockPos.x + pBlockPos.y * blocksInLevel.x + pBlockPos.z * blocksInLevel.x * blocksInLevel.y;

		// Get the node address
		indexValue = _blockIndexCache[ pLevel ][ indexPos ];
	}
	else
	{
		// Compute the index of the node in the buffer of nodes, given its position
		//
		// Nodes are stored in increasing order from X axis first, then Y axis, then Z axis.
#ifdef WIN32
		__int64 indexPos = ( (__int64)pBlockPos.x + (__int64)( pBlockPos.y * blocksInLevel.x ) + (__int64)( pBlockPos.z * blocksInLevel.x * blocksInLevel.y ) ) * sizeof( unsigned int );
#else
		off_t indexPos = ( (off_t)pBlockPos.x + (off_t)( pBlockPos.y * blocksInLevel.x ) + (off_t)( pBlockPos.z * blocksInLevel.x * blocksInLevel.y ) ) * sizeof( unsigned int );
#endif
		// Retrieve node filename at given level
		std::string fileNameIndex = this->_filesNames[ ( _numChannels + 1 ) * pLevel ];

		// Open node file
		FILE* fileIndex = fopen( fileNameIndex.c_str(), "rb" );
		if ( fileIndex )
		{
			// Position file pointer at position corresponding to the requested node information
#ifdef WIN32
			_fseeki64( fileIndex, indexPos, 0 );
#else
			fseeko( fileIndex, indexPos, 0 );
#endif
			// Read node file and store requested node address in indexValue
			if ( fread( &indexValue, sizeof( unsigned int ), 1, fileIndex ) != 1 )
			{
				// Handle error if reading node file has failed
				std::cerr << "GsDataLoader<T>::getBlockIndex(): fread failed." << std::endl;
			}

			// Close node file
			fclose( fileIndex );
		}
		else
		{
			// Handle error if opening node file has failed
			std::cerr << "GsDataLoader<T>::getBlockIndex() : Unable to open file index "<<fileNameIndex << std::endl;
		}
	}

	return indexValue;
}

/******************************************************************************
 * Load a brick given a mipmap level, a 3D node indexed position,
 * the data pool and an offset in the data pool.
 *
 * @param pLevel mipmap level
 * @param pBlockPos the 3D node indexed position
 * @param pDataPool the data pool
 * @param pOffsetInPool offset in the data pool
 *
 * @return a flag telling wheter or not the brick has been loaded (some brick can contain no data).
 ******************************************************************************/
template< typename TDataTypeList >
bool GsDataLoader< TDataTypeList >
::loadBrick( int pLevel, const uint3& pBlockPos, GvCore::GPUPoolHost< GvCore::Array3D, TDataTypeList >* pDataPool, size_t pOffsetInPool )
{
	// Retrieve the resolution at given level (i.e. the number of voxels in each dimension)
	uint3 levelSize = getLevelRes( pLevel );

	//uint3 blocksInLevel = levelSize / this->_bricksRes;	// seem to be not used anymore

	// Compute full brick resolution (with borders)
	uint3 trueBlocksRes = this->_bricksRes + make_uint3( 2 * this->_borderSize );

	// Compute the brick size alignment in memory (with borders)
	size_t blockMemSize = static_cast< size_t >( trueBlocksRes.x * trueBlocksRes.y * trueBlocksRes.z );

	// Retrieve the node encoded address given a mipmap level and a 3D node indexed position
	unsigned int indexVal = getBlockIndex( pLevel, pBlockPos );

	// Test if node contains a brick
	if ( indexVal & GV_VTBA_BRICK_FLAG )
	{
		// Use a channel initializer to read the brick
		GsBrickLoaderChannelInitializer< TDataTypeList > channelInitializer( this, indexVal, blockMemSize, pLevel, pDataPool, pOffsetInPool );
		GvCore::StaticLoop< GsBrickLoaderChannelInitializer< TDataTypeList >, Loki::TL::Length< TDataTypeList >::value - 1 >::go( channelInitializer );

		return true;
	}
	
	return false;
} 

/******************************************************************************
 * Helper function used to determine the type of regions in the data structure.
 * The data structure is made of regions containing data, empty or constant regions.
 *
 * Retrieve the node and associated brick located in this region of space,
 * and depending of its type, if it contains data, load it.
 *
 * @param pPosition position of a region of space
 * @param pSize size of a region of space
 * @param pBrickPool data cache pool. This is where all data reside for each channel (color, normal, etc...)
 * @param pOffsetInPool offset in the brick pool
 *
 * @return the type of the region (.i.e returns constantness information for that region)
 ******************************************************************************/
template< typename TDataTypeList >
GsDataLoader< TDataTypeList >::VPRegionInfo GsDataLoader< TDataTypeList >
::getRegion( const float3& pPosition, const float3& pSize, GvCore::GPUPoolHost< GvCore::Array3D, TDataTypeList >* pBrickPool, size_t pOffsetInPool )
{
	// Retrieve the level of resolution associated to a given size of a region of space.
	int level =	getDataLevel( pSize, _bricksRes );

	// Retrieve the indexed coordinates associated to position of region of space at level of resolution.
	uint3 coordsInLevel = getCoordsInLevel( level, pPosition );
	// Retrieve the indexed coordinates of a block (i.e. a node) in the blocks grid of a region of space at a level of resolution.
	uint3 blockCoords = getBlockCoords( level, pPosition );

	// Check mipmap level bounds
	if ( level >= 0 && level < _numMipMapLevels )
	{
		// Try to load brick given localization parameters
		if ( loadBrick( level, blockCoords, pBrickPool, pOffsetInPool ) )
		{
			return GsDataLoader< TDataTypeList >::VP_UNKNOWN_REGION;
		}
		else
		{
			return GsDataLoader< TDataTypeList >::VP_CONST_REGION; // Correct ?
		}
	}
	else
	{
		// Handle error
		std::cout << "VolProducerBlocksOptim::getZone() : Invalid requested block dimentions" << std::endl;
		return GsDataLoader< TDataTypeList >::VP_CONST_REGION;
	}
}

/******************************************************************************
 * Provides constantness information about a region.
 *
 * @param pPosition position of a region of space
 * @param pSize size of a region of space
 *
 * @return the type of the region (.i.e returns constantness information for that region)
 ******************************************************************************/
template< typename TDataTypeList >
GsDataLoader< TDataTypeList >::VPRegionInfo GsDataLoader< TDataTypeList >
::getRegionInfo( const float3& pPosition, const float3& pSize/*, T* constValueOut*/ )
{
	// Retrieve the level of resolution associated to a given size of a region of space.
	int level =	getDataLevel( pSize, _bricksRes );
	// Retrieve the indexed coordinates of a block (i.e. a node) in the blocks grid of a region of space at a level of resolution.
	uint3 blockPosition = getBlockCoords( level, pPosition );

	// Retrieve the resolution at given level (i.e. the number of voxels in each dimension)
	uint3 levelsize = getLevelRes( level );
	//uint3 blocksinlevel = levelsize / this->_bricksRes;	// seem to be not used anymore...

	uint3 trueBlocksRes = this->_bricksRes+ make_uint3( 2 * this->_borderSize );
	//size_t blockmemsize = (size_t)(trueBlocksRes.x*trueBlocksRes.y*trueBlocksRes.z );

	// Check mipmap level bounds
	if ( level >= 0 && level < _numMipMapLevels )
	{
		// Retrieve the node encoded address given a mipmap level and a 3D node indexed position
		unsigned int indexValue = getBlockIndex( level, blockPosition );

		// If there is a brick
		if ( indexValue & 0x40000000U )
		{
			// If we are on a terminal node
			if ( indexValue & 0x80000000U )
			{
				return GsDataLoader< TDataTypeList >::VP_UNKNOWN_REGION;
			}
			else
			{
				return GsDataLoader< TDataTypeList >::VP_NON_CONST_REGION;
			}
		}
		else
		{
			return GsDataLoader< TDataTypeList >::VP_CONST_REGION;
		}
	}
	else
	{
		return GsDataLoader< TDataTypeList >::VP_CONST_REGION;
	}
}

/******************************************************************************
 * Retrieve the node located in a region of space,
 * and get its information (i.e. address containing its data type region).
 *
 * @param pPosition position of a region of space
 * @param pSize size of a region of space
 *
 * @return the node encoded information
 ******************************************************************************/
template< typename TDataTypeList >
uint GsDataLoader< TDataTypeList >
::getRegionInfoNew( const float3& pPosition, const float3& pSize )
{
	// Retrieve the level of resolution associated to a given size of a region of space.
	int level =	getDataLevel( pSize, _bricksRes );
	// Retrieve the indexed coordinates of a block (i.e. a node) in the blocks grid of a region of space at a level of resolution.
	uint3 blockPosition = getBlockCoords( level, pPosition );

	// Retrieve the resolution at given level (i.e. the number of voxels in each dimension)
	uint3 levelsize = getLevelRes( level );
	//uint3 blocksinlevel = levelsize / this->_bricksRes;	// seem to be not used anymore...

	// Compute full brick resolution (with borders)
	uint3 trueBlocksRes = this->_bricksRes + make_uint3( 2 * this->_borderSize );

	// Check mipmap level bounds
	if ( level >= 0 && level < _numMipMapLevels )
	{
		// Get the node encoded address in the pool.
		//
		// Retrieve the node encoded address given a mipmap level and a 3D node indexed position
		// Apply a mask on the two first bits to retrieve node information
		// (the other 30 bits are for x,y,z address).
		return ( getBlockIndex( level, blockPosition ) & 0xC0000000 );
	}
	return 0;
}

/******************************************************************************
 * Retrieve the resolution at a given level (i.e. the number of voxels in each dimension)
 *
 * @param level the level
 *
 * @return the resolution at given level
 ******************************************************************************/
template< typename TDataTypeList >
inline uint3 GsDataLoader< TDataTypeList >
::getLevelRes( uint level ) const
{
	//return _volumeRes / (1<<level); // WARNING: suppose mipMapOrder==2 !
	return _bricksRes * ( 1 << level ); // WARNING: suppose mipMapOrder==2 !
}

/******************************************************************************
 * Provides the size of the smallest features the producer can generate.
 *
 * @return the size of the smallest features the producer can generate
 ******************************************************************************/
template< typename TDataTypeList >
inline float3 GsDataLoader< TDataTypeList >
::getFeaturesSize() const
{
	return make_float3( 1.0f ) / make_float3( _volumeRes );
}

/******************************************************************************
 * Get the data resolution
 *
 * @return the data resolution
 ******************************************************************************/
template< typename TDataTypeList >
inline uint3 GsDataLoader< TDataTypeList >
::getDataResolution() const
{
	return _volumeRes;
}

/******************************************************************************
 * Read a brick given parameters to retrieve data localization in brick files or in cache of bricks.
 *
 * @param pChannel channel index (i.e. color, normal, density, etc...)
 * @param pIndexVal associated node encoded address of the node in which the brick resides
 * @param pBlockMemSize brick size alignment in memory (with borders)
 * @param pLevel mipmap level
 * @param pData data array corresponding to given channel index in the data pool (i.e. bricks of voxels)
 * @param pOffsetInPool offset in the data pool
 ******************************************************************************/
template< typename TDataTypeList >
template< typename TChannelType >
inline void GsDataLoader< TDataTypeList >
::readBrick( int pChannel, unsigned int pIndexVal, unsigned int pBlockMemSize, unsigned int pLevel, GvCore::Array3D< TChannelType >* pData, size_t pOffsetInPool )
{
	// pIndexVal & 0x3FFFFFFFU : this expression corresponds to the last 30 bits of the 32 bits node encoded address
	// These 30 bits corresponds to the address of the node on x,y,z axes.
	//
	// Compute the offset
	unsigned int filePos = ( pIndexVal & 0x3FFFFFFFU ) * pBlockMemSize * sizeof( TChannelType );

	// Check whether or not, cache mechanism is used
	if ( _useCache )
	{
		if ( _blockCache[ pLevel * _numChannels + pChannel ] )
		{
			// Copy data from cache to the channel array of the data pool
			memcpy( pData->getPointer( pOffsetInPool )/* destination */,
					_blockCache[ pLevel * _numChannels + pChannel ] + filePos/* source */,
					pBlockMemSize * sizeof( TChannelType ) /* number of bytes*/ );
		}
	}
	else
	{
		// Open brick file
		FILE* file = fopen( _filesNames[ pLevel * ( _numChannels + 1 ) + pChannel + 1 ].c_str(), "rb" );
		if ( file )
		{
			// Position file pointer at position corresponding to the requested brick data
#ifdef WIN32
			_fseeki64( file, filePos, 0 );
#else
			fseeko( file, filePos, 0 );
#endif
			// Read brick file and store data in the channel array of the data pool
			fread( pData->getPointer( pOffsetInPool ), sizeof( TChannelType ), pBlockMemSize, file );

			// Close brick file
			fclose( file );
		}
	}
}

/******************************************************************************
 * Retrieve the indexed coordinates of a block (i.e. a node) in the blocks grid
 * associated to a given position of a region of space at a given level of resolution.
 *
 * @param pLevel level of resolution
 * @param pPosition position of a region of space
 *
 * @return the associated indexed coordinates
 ******************************************************************************/
template< typename TDataTypeList >
inline uint3 GsDataLoader< TDataTypeList >
::getBlockCoords( int pLevel, const float3& pPosition ) const
{
	return getCoordsInLevel( pLevel, pPosition ) / this->_bricksRes;
}

/******************************************************************************
 * Retrieve the level of resolution associated to a given size of a region of space.
 *
 * @param pSize size of a region of space
 * @param pResolution resolution of data (i.e. brick)
 *
 * @return the corresponding level
 ******************************************************************************/
template< typename TDataTypeList >
inline int GsDataLoader< TDataTypeList >
::getDataLevel( const float3& pSize, const uint3& pResolution ) const
{
	// Compute the node resolution (i.e. number of nodes in each dimension)
	uint3 numNodes = make_uint3( 1.0f / pSize );
	int level = static_cast< int >( log( static_cast< float >( numNodes.x ) ) / log( static_cast< float >( _mipMapOrder ) ) );

	// uint3 resinfulldata = make_uint3( make_float3( this->_volumeRes ) * pSize );
	// int level = (int)( log( resinfulldata.x / (float)pRes.x ) / log( (float)( _mipMapOrder ) ) );

	return level;
}

/******************************************************************************
 * Retrieve the indexed coordinates associated to a given position of a region of space
 * at a given level of resolution.
 *
 * @param pLevel level of resolution
 * @param pPosition position of a region of space
 *
 * @return the associated indexed coordinates
 ******************************************************************************/
template< typename TDataTypeList >
inline uint3 GsDataLoader< TDataTypeList >
::getCoordsInLevel( int pLevel, const float3& pPosition ) const
{
	// Retrieve the resolution at given level (i.e. the number of voxels in each dimension)
	uint3 levelResolution = getLevelRes( pLevel );
	uint3 coordsInLevel = make_uint3( make_float3( levelResolution ) * pPosition );

	return coordsInLevel;
}

/******************************************************************************
 * Get the flag telling wheter or not to use cache on CPU to load all dataset content.
 * Note : this may require a huge memory consumption.
 *
 * @return pFlag Flag to tell wheter or not to use cache on CPU to load all dataset content.
 ******************************************************************************/
template< typename TDataTypeList >
bool GsDataLoader< TDataTypeList >
::isHostCacheActivated() const
{
	return _useCache;
}

/******************************************************************************
 * Set the flag to tell wheter or not to use cache on CPU to load all dataset content.
 * Note : this may require a huge memory consumption.
 *
 * @param pFlag Flag to tell wheter or not to use cache on CPU to load all dataset content.
 ******************************************************************************/
template< typename TDataTypeList >
void GsDataLoader< TDataTypeList >
::setHostCacheActivated( bool pFlag )
{
	//_useCache = pFlag;

	// TODO
	// ... reinit buffers accordingly
	assert( false );
}

} // namespace GvUtils
