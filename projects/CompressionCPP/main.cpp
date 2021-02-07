
#include <cstdio>
#include <cstdlib>

#include <proto/exec.h>
#include <utils/time/time.h>

#include "utils/utils_def.h"

#include <utils/dos/dos.h>
#include <utils/dos/stdio.h>
#include <utils/devices/mouse.h>

#include <utils/tools/zlib/amiga_zlib.h>

// Prepare an uncompressed buffer with some random data...
#define DATA_ELEMENTS	16
const uint32_t uCompData[DATA_ELEMENTS] =
{
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
};

STDARGS int main(int argc, char *argv[])
{
	OpenDOSLibrary();

	uint32_t zCompData[DATA_ELEMENTS];
	uint32_t zuCompData[DATA_ELEMENTS];

	memset(zCompData, 0, sizeof(zCompData));
	memset(zuCompData, 0, sizeof(zuCompData));

	PrintFormatted("Testing zLib %s compression library\r\n\r\n", zlibVersion());

	// ...and calculate the CRC for the uncompressed buffer.
	uLong uCRC32 = 0;
	uCRC32 = crc32(uCRC32, (const Bytef *)uCompData, sizeof(uCompData));
	PrintFormatted("Uncompressed CRC32: $%08lX\r\n", uCRC32);

	uint32_t cLen = zlibCompressData(uCompData, sizeof(uCompData), zCompData, sizeof(zCompData));
	uLong cCRC32 = 0;
	if(cLen != (uint32_t)-1)
		cCRC32 = crc32(cCRC32, (const Bytef *)zCompData, cLen);

	PrintFormatted("Compressed buffer from %lu to %lu bytes. CRC32: $%08lX\r\n", sizeof(uCompData), cLen, cCRC32);

	uint32_t zuLen = zlibUncompressData(zCompData, cLen, zuCompData, sizeof(zuCompData));
	uLong zuCRC32 = 0;
	if(zuLen != (uint32_t)-1)
		zuCRC32 = crc32(zuCRC32, (const Bytef *)zuCompData, sizeof(zuCompData));

	PrintFormatted("Uncompressed buffer from %lu to %lu bytes. CRC32: $%08lX\r\n", cLen, zuLen, zuCRC32);

	if((zuLen != sizeof(uCompData)) || (uCRC32 != zuCRC32))
		PrintFormatted("[ERROR] Selftest failed %ld/$%08lX vs. %ld/$%08lX\r\n", zuLen, uCRC32, sizeof(uCompData), zuCRC32);

	CloseDOSLibrary();

	return 0;
}

/*
https://www.experts-exchange.com/articles/3189/In-Memory-Compression-and-Decompression-Using-ZLIB.html

int CompressData( const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst )
{
    z_stream zInfo ={0};
    zInfo.total_in=  zInfo.avail_in=  nLenSrc;
    zInfo.total_out= zInfo.avail_out= nLenDst;
    zInfo.next_in= (BYTE*)abSrc;
    zInfo.next_out= abDst;

    int nErr, nRet= -1;
    nErr= deflateInit( &zInfo, Z_DEFAULT_COMPRESSION ); // zlib function
    if ( nErr == Z_OK ) {
        nErr= deflate( &zInfo, Z_FINISH );              // zlib function
        if ( nErr == Z_STREAM_END ) {
            nRet= zInfo.total_out;
        }
    }
    deflateEnd( &zInfo );    // zlib function
    return( nRet );
}

int UncompressData( const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst )
{
    z_stream zInfo ={0};
    zInfo.total_in=  zInfo.avail_in=  nLenSrc;
    zInfo.total_out= zInfo.avail_out= nLenDst;
    zInfo.next_in= (BYTE*)abSrc;
    zInfo.next_out= abDst;

    int nErr, nRet= -1;
    nErr= inflateInit( &zInfo );               // zlib function
    if ( nErr == Z_OK ) {
        nErr= inflate( &zInfo, Z_FINISH );     // zlib function
        if ( nErr == Z_STREAM_END ) {
            nRet= zInfo.total_out;
        }
    }
    inflateEnd( &zInfo );   // zlib function
    return( nRet ); // -1 or len of output
}*/
