# Lib: libAmigaUtils.a

The library contains only code, which has no additional dependencies. It can be used in any kind of project (ASM or C/C++).

### Functions

#### Copper

**Name:**`CreateCopperList`:<br>
**Include:** `utils/hardware/copper.i`<br>
**Description:** Creates a list of copperinstructions for the specified region.<br>

#### DOS

#### exports<br>
`DOSName`: Name of the library<br>
`DOSBase`: Library base pointer<br>
`stdout`: Standard output filehandle<br>

**Name:**`OpenDOSLibrary`:<br>
**Include:** <br>
**Description:** Opens the library.<br>

**Name:**`CloseDOSLibrary`:<br>
**Include:** <br>
**Description:** Closes the library.<br>

#### Graphics

#### exports<br>
`GraphicsName`: Name of the library<br>
`GraphicsBase`: Library base pointer<br>

**Name:**`OpenGraphicsLibrary`:<br>
**Include:** <br>
**Description:** Opens the library.<br>

**Name:**`CloseGraphicsLibrary`:<br>
**Include:** <br>
**Description:** Closes the library.<br>

#### strings

**Name:**`puts`:<br>
**Name:**`putsf`:<br>
**Include:** "utils/string/string.h"<br>
**Description:** Prints a 0-terminated string to stdout or the specified file.

**Name:**`formatString`:<br>
**Include:** "utils/string/string.h"<br>
**Description:** This formats a string similar to sprintf. There are some differences though.<br>

	* Floating point formats are not supported and willalways printf "INF"<br>
	* The function will always return the number of character it would need to write to the buffer, even if the buffer is to small. However the function guarantees that there will be no buffer overruns, so the caller can always check if the required characters are more than the supplied buffer, in which case the string will be terminated.<br>
	* passing nullpointers is safe and will never cause it to write there. In case of a string 
	"[null]" is inserted.<br>
	* The function guarantees to always provide a temrinating 0-byte in the outputbuffer, except when the outputbuffer is either a null pointer or the buffersize is 0. In all other cases the resulting string will be null-terminated at the cost of a possible truncated string.<br>

**Name:**`uWordToString`:<br>
**Name:**`sWordToString`:<br>
**Name:**`uLongToString`:<br>
**Name:**`sLongToString`:<br>
**Include:** "utils/string/numbers.h"<br>
**Description:** Prints an signed/unsigned word/long value to a stringbuffer. The resulting number will never be 0 terminated and can thus be used to insert strings in the middle of an existing string. Otherwise the same guarantees apply as for `formatString`.

**Name:**`stringToSWord`:<br>
**Name:**`stringToUWord`:<br>
**Name:**`stringToSLong`:<br>
**Name:**`stringToULong`:<br>
**Include:** "utils/string/numbers.h"<br>
**Description:** Converts a string to a signed/unsigned word/long value. The conversions stops on the first non-numeric character for the given base, or if an overflow would occur. An appropriate returncode can be retrieved, as well as the pointer to the first not-converted character.

#### math

**Name:**`div32s`:<br>
**Include:** <br>
**Description:** Divide 32 bit signed values and returns result and modulo.

**Name:**`div32u`:<br>
**Include:** <br>
**Description:** Divide 32 bit unsigned values and returns result and modulo.

**Name:**`mult32s`:<br>
**Include:** <br>
**Description:** Multiplies 32 bit signed values.

**Name:**`mult32u`:<br>
**Include:** <br>
**Description:** Multiplies 32 bit unsigned values.

#### mem

**Name:**`memcpyw`:<br>
**Include:** <br>
**Description:** Copies N 16 bit values.

**Name:**`memcpyl`:<br>
**Include:** <br>
**Description:** Copies N 32 bit values.

#### strings

**Name:**`strlen`:<br>
**Include:** <br>
**Description:** Returns the length of a c-string.

**Name:**`toDecimalS8`:<br>
**Include:** <br>
**Description:** Converts a signed 8 bit value to a decimal c-string.

**Name:**`toDecimalS16`:<br>
**Include:** <br>
**Description:** Converts a signed 16 bit value to a decimal c-string.

**Name:**`toDecimalS32`:<br>
**Include:** <br>
**Description:** Converts a signed 32 bit value to a decimal c-string.

**Name:**`toDecimalU8`:<br>
**Include:** <br>
**Description:** Converts an unsigned 8 bit value to a decimal c-string.

**Name:**`toDecimalU16`:<br>
**Include:** <br>
**Description:** Converts an unsigned 16 bit value to a decimal c-string.

**Name:**`toDecimalU32`:<br>
**Include:** <br>
**Description:** Converts an unsigned 32 bit value to a decimal c-string.
