# Lib: libAmigaGCCUtils.a

This library contains tools, which require the C runtime environment and thus are not suitable for pure ASM projects. Of course it can be called from asm code as well, but there may be calls to C runtime functions involved.

### Functions

#### Clock

**Name:**`clockToTimeString`:<br>
**Include:** `utils/time.h`<br>
**Description:** Converts a clock_t value to a human readable string.

#### Copper

**Name:**`CreateCopperList`:<br>
**Include:** `utils/hardware/copper.i`<br>
**Description:** Creates a list of copperinstructions for the specified region.<br>
