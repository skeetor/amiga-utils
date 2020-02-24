#ifndef _UTILSLIB_DOS_I
#define _UTILSLIB_DOS_I 1

#include <utils/utils_def.h>
#include <dos/dos.h>
#include <exec/memory.h>

typedef Library DOSLibrary;

/**
 * Open-/CloseDOSLibrary
 */
C_FUNCTION DOSLibrary *OpenDOSLibrary(uint32_t nVersion REGISTER_PARAM(d0) = 0);
C_FUNCTION void CloseDOSLibrary(void);

/**
 ************************************************************
 * If the user enters SystemSave() state, the dos library may
 * not work. Before it can be used, EnableDiskIO() must be
 * called. This is not needed if the appropriate DMA and IRQs
 * already have been enabled. This function is enough to call
 * once. It needs to be called again if the DMA/IRQs are changed
 * in between. Calling it multiple times is no problem either.
 
 * INPUT:  None
 *
 * RETURN: None
 *
 ************************************************************
 */
C_FUNCTION void EnableDiskIO(void);

C_FUNCTION uint32_t GetFileSize(const char *filename REGISTER_PARAM(a0));

/**
 ************************************************************
 * Load a file to memory. The memory is allocated and the
 * pointer is returned if the file was successfully loaded.
 * In case of an error a nullptr is returned and FileSize
 * contains a statuscode:
 * If the file was successfully loaded, the caller is
 * responsible for deallocating the memory with FreeMem()
 * when it is no longer needed.
 * In case of an error no memory was allocated.
 *
 * INPUT:  
 *	filename: Name of the file to be loaded
 *	allocationFlags: The flags which indicate which kind of
 *				memory this file should be loaded to. See
 *				AllocMem for the flags.
 *	filesize: If the return value is not a nullptr, then this
 *				will contain the size of the file, otherwise
 *				a statuscode.
 *
 * RETURN: Pointer to loaded file or nullptr.
 *
 ************************************************************
 */

typedef enum
{
	LF_PARAM,		// Invalid pointer for name
	LF_MEM,			// Couldn't allocate memory
	LF_SIZE,		// Unable to get filesize
	LF_OPEN,		// Unable to open file
	LF_READ			// Error while reading file
} LF_STATUS;

C_FUNCTION char *LoadFile(const char *filename REGISTER_PARAM(a0), uint32_t allocationFlags  REGISTER_PARAM(d0), uint32_t *filesize REGISTER_PARAM(a1));

#endif // _UTILSLIB_DOS_I
