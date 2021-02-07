#ifndef _UTILSLIB_ENDIANNES_H
#define _UTILSLIB_ENDIANNES_H

/**
 * Convert BigEndian value to LittleEndian.
 *
 * TODO: For signed values, a seperate function is needed.
 */
static uint16_t flipEndian16(uint16_t value)
{
	return (value >> 8) + (value << 8);
}

static uint32_t flipEndian32(uint32_t value)
{
	return flipEndian16(value >> 16) + (flipEndian16(value & 0xffff) << 16);
}

static bool isLittleEndian(void)
{
	char value[4] = { 0 };
	uint16_t v = 0xff00;
	uint16_t *p = reinterpret_cast<uint16_t *>(&value[0]);
	*p = v;

	return (value[0] != 0xff);
}

#endif // _UTILSLIB_ENDIANNES_H
