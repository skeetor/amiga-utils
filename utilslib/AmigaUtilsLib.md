# Lib: libAmigaUtils.a

The library contains only code, which has no additional dependencies. It can be used in any kind of project (ASM or C/C++).

### Functions

#### Copper

**Name:** `CreateCopperList`:<br>
**Include:** `utils/hardware/copper.i`<br>
**Description:** Creates a list of copperinstructions for the specified region.<br>

#### DOS

#### exports<br>
`DOSName`: Name of the library<br>
`DOSBase`: Library base pointer<br>
`stdout`: Standard output filehandle<br>

**Name:** `OpenDOSLibrary`:<br>
**Include:** "utils/dos/dos.h"<br>
**Description:** Opens the library.<br>

**Name:** `CloseDOSLibrary`:<br>
**Include:** "utils/dos/dos.h"<br>
**Description:** Closes the library.<br>

**Name:** `GetStdout`:<br>
**Include:** "utils/dos/stdio.h"<br>
**Description:** Set the current stdout to a new filehandle.<br>

**Name:** `GetStdout`:<br>
**Include:** "utils/dos/stdio.h"<br>
**Description:** Get the current stdout filehandle.<br>

**Name:** `EnableDiskIO`:<br>
**Include:** "utils/dos/dos.h"<br>
**Description:** If the user enters SystemSave() state, all IRQs and DMA are disabled. If the user wants to use the dos.library functions for accessing the regular filesystem, this helper function will enable the appropriate settings.<br>
Note that this is not needed if the settings are already enabled, or if EnableDiskIO was already called before. If the user changed the DMA/IRQ settings afterwards this may be needed to be called again before accessing dos.library functions.

**Name:** `LoadFile`:<br>
**Include:** "utils/dos/dos.h"<br>
**Description:** Loads a file into memory. The memory is allocated and the file is loaded. If an error occured the memory is freed and a nullptr is returned.<br> On success the pointer to the allocated memory is returned and the filesize.
The allocated memory is allocated using the allocation flags from the exec library (i.E. MEMF_CHIP, ...) which have to be specified by the caller. The caller is responsible for freeing the memory after it is no longer needed using FreeMem()<br>
If this function is used in SystemSave() state, the disk IO must be enabled for it to work.

**Name:** `PrintString`:<br>
**Include:** "utils/dos/stdio.h"<br>
**Description:** Print a string to stdout.<br>

**Name:** `WriteString`:<br>
**Include:** "utils/dos/stdio.h"<br>
**Description:** Write a string to the specified filehandle.<br>

**Name:** `PrintFormatted`:<br>
**Name:** `vPrintFormatted`:<br>
**Name:** `WriteFormatted`:<br>
**Name:** `vWriteFormatted`:<br>
**Include:** "utils/dos/stdio.h"<br>
**Description:** Similar to printf. See `FormatString` below.

**Name:** ``:FindOption<br>
**Include:** "utils/dos/options.h"<br>
**Description:** Find the option class and it's index in the array<br>

**Name:** ``:ValidateOption<br>
**Include:** "utils/dos/options.h"<br>
**Description:** Validtes if the set of defined options matches the commandline<br>

**Name:** ``:FindParam<br>
**Include:** "utils/dos/options.h"<br>
**Description:** Find the specified param and arguments from the commandline paraeters<br>

#### Graphics

#### exports<br>
`GraphicsName`: Name of the library<br>
`GraphicsBase`: Library base pointer<br>

**Name:** `OpenGraphicsLibrary`:<br>
**Include:** <br>
**Description:** Opens the library.<br>

**Name:** `CloseGraphicsLibrary`:<br>
**Include:** <br>
**Description:** Closes the library.<br>

#### strings

**Name:** `FormatString`:<br>
**Include:** "utils/string/string.h"<br>
**Description:** This formats a string similar to sprintf. There are some differences though.<br>

	* With "%b" you can create binary numbers<br>
	* Floating point formats are not supported and willalways printf "INF"<br>
	* The function will always return the number of character it would need to write to the buffer, even if the buffer is to small. However the function guarantees that there will be no buffer overruns, so the caller can always check if the required characters are more than the supplied buffer, in which case the string is truncated.<br>
	* passing nullpointers is safe and will never cause it to write there. In case of a string 
	"[null]" is inserted.<br>
	* The function guarantees to always provide a terminating 0-byte in the outputbuffer, except when the outputbuffer is either a null pointer or the buffersize is 0. In all other cases the resulting string will be null-terminated at the cost of a possible truncated string.<br>

**Name:** `uWordToString`:<br>
**Name:** `sWordToString`:<br>
**Name:** `uLongToString`:<br>
**Name:** `sLongToString`:<br>
**Include:** "utils/string/numbers.h"<br>
**Description:** Prints an signed/unsigned word/long value to a stringbuffer. The resulting number will never be 0 terminated and can thus be used to insert strings in the middle of an existing string. Otherwise the same guarantees apply as for `formatString`.

**Name:** `stringToSWord`:<br>
**Name:** `stringToUWord`:<br>
**Name:** `stringToSLong`:<br>
**Name:** `stringToULong`:<br>
**Include:** "utils/string/numbers.h"<br>
**Description:** Converts a string to a signed/unsigned word/long value. The conversions stops on the first non-numeric character for the given base, or if an overflow would occur. An appropriate returncode can be retrieved, as well as the pointer to the first not-converted character.
**Name:** `strlen`:<br>
**Include:** <br>
**Description:** Returns the length of a c-string.

**Name:** `toDecimalS8`:<br>
**Include:** <br>
**Description:** Converts a signed 8 bit value to a decimal c-string.

**Name:** `toDecimalS16`:<br>
**Include:** <br>
**Description:** Converts a signed 16 bit value to a decimal c-string.

**Name:** `toDecimalS32`:<br>
**Include:** <br>
**Description:** Converts a signed 32 bit value to a decimal c-string.

**Name:** `toDecimalU8`:<br>
**Include:** <br>
**Description:** Converts an unsigned 8 bit value to a decimal c-string.

**Name:** `toDecimalU16`:<br>
**Include:** <br>
**Description:** Converts an unsigned 16 bit value to a decimal c-string.

**Name:** `toDecimalU32`:<br>
**Include:** <br>
**Description:** Converts an unsigned 32 bit value to a decimal c-string.

#### math

**Name:** `div32s`:<br>
**Include:** <br>
**Description:** Divide 32 bit signed values and returns result and modulo.

**Name:** `div32u`:<br>
**Include:** <br>
**Description:** Divide 32 bit unsigned values and returns result and modulo.

**Name:** `mult32s`:<br>
**Include:** <br>
**Description:** Multiplies 32 bit signed values.

**Name:** `mult32u`:<br>
**Include:** <br>
**Description:** Multiplies 32 bit unsigned values.

**Name:** `random`:<br>
**Include:** "utils/math/math.h"<br>
**Description:** Generate a 32 bit unsigned random value.

#### mem

**Name:** `memcpyw`:<br>
**Include:** <br>
**Description:** Copies N 16 bit values.

**Name:** `memcpyl`:<br>
**Include:** <br>
**Description:** Copies N 32 bit values.

**Name:** `memfill`:<br>
**Include:** <br>
**Description:** A replacement for memset which is up to 60% faster then memse, depending on the buffersize.

**Name:** `memfillBlit`:<br>
**Include:** <br>
**Description:** A memset replacement, which uses the blitter, and can be used synchronously or asynchronously.

#### debug

**Name:** `DebugWait`:<br>
**Include:** "utils/debug/debug.h"<br>
**Description:** Print a message and wait for left mousebutton<br>

**Name:** `PrintCPU`:<br>
**Include:** "utils/debug/debug.h"<br>
**Description:** Print the CPU registers to stdout.<br>

#### tools

#### zlib library 1.2.11

**Name:** `CompressData`:<br>
**Include:** <br>
**Description:** zlib Compression for a fixed size memory block

**Name:** `UncompressData`:<br>
**Include:** <br>
**Description:** zlib Decompression for a fixed size memory block
