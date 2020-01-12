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
**Include:** <br>
**Description:** Prints a 0-terminated string to stdout or the specified file.

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
