#ifndef _UTILSLIB_STRING_NUMBERS_H
#define _UTILSLIB_STRING_NUMBERS_H

#include <cstdint>
#include <cstdarg>

#include "utils/utils_def.h"

/**
 * Pointer to the numeric characters used for the number to string conversions.
 */
extern const char *numericChar;

/**
 *******************************************************
 * Converts an unsigned 16 bit number to a string. No
 * 0 byte is added and no leading zeroes are included in
 * the string.
 * The number of required bytes is always returned, but
 * if the buffer is to small, it will not be overwritten.
 * In this case the buffer is truncated and may contain
 * an invalid string, so the caller should make sure that
 * enough memory is provided.
 * If the base is invalid, -1 is returned.
 *
 * Using a nullptr or a bufferlen of 0 is valid and will
 * only return the length required.
 *
 * For bases with power of 2 a more specialiced 
 * conversion is used, as these are much faster.
 * These are 2/4/8/16 and 32.
 *******************************************************
 */
C_FUNCTION uint32_t uWordToString(uint16_t value, char *buffer, const uint32_t BufferLen, const uint32_t base = 10u);
C_FUNCTION uint32_t sWordToString(int16_t value, char *buffer, const uint32_t BufferLen, const uint32_t base = 10u);

/**
 *******************************************************
 * Converts an unsigned 32 bit number to a string. No
 * 0 byte is added and no leading zeroes are included in
 * the string.
 * The number of required bytes is always returned, but
 * if the buffer is to small, it will not be overwritten.
 * In this case the buffer is truncated and may contain
 * an invalid string, so the caller should make sure that
 * enough memory is provided.
 * If the base is invalid, -1 is returned.
 *
 * Using a nullptr or a bufferlen of 0 is valid and will
 * only return the length required.
 *
 * For bases with power of 2 a more specialiced 
 * conversion is used, as these are much faster.
 * These are 2/4/8/16 and 32.
 *******************************************************
 */
C_FUNCTION uint32_t uLongToString(uint32_t value, char *buffer, const uint32_t BufferLen, const uint32_t base = 10u);
C_FUNCTION uint32_t sLongToString(int32_t value, char *buffer, const uint32_t BufferLen, const uint32_t base = 10u);


/**
 * Converts a string to an un-/signed 16/32 bit number.
 * The number is converted until either a non-numeric
 * character (for the given base) is encountered or
 * the buffer limit is reached.
 *
 * Note, that the length of the string is not required
 * to end with a c-string 0 byte, so this function can
 * also be used to convert numbers embedded inside a
 * string (i.E. fixed width record or other text).
 *
 * A leading '+' or '-' is considered as valid part
 * of the number, but no whitespaces between.
 *
 * If param state is not a nullptr a status code is
 * returned:
 * RESULT_OK == Converted number is correct
 * RESULT_OVERFLOW == An overflow would occur
 *				 In this case, the returned number
 *				 is the last known value without overflow.
 *				 This might not be an error if the string
 *				 is embedded, but the caller has to decide.
 * RESULT_INVALID_CHAR == Invalid character encountered.
 *				 The return value contains the number
 *				 up to this character. Caller has to
 *				 decide if this is an error or not.
 * RESULT_ERROR == Error. Invalid Base, Buffersize == 0
 *				 or nullptr.
 *
 * If the endpointer is provided, it will return the
 * pointer after the last converted character.
 */
typedef enum
{
	RESULT_OK,
	RESULT_OVERFLOW,
	RESULT_INVALID_CHAR,
	RESULT_ERROR
} RESULT_STATE;

C_FUNCTION int16_t stringToSWord(const char *buffer, const uint32_t BufferLen, uint32_t base = 10u, RESULT_STATE *state = nullptr, char **endPointer = nullptr);
C_FUNCTION uint16_t stringToUWord(const char *buffer, const uint32_t BufferLen, uint32_t base = 10u, RESULT_STATE *state = nullptr, char **endPointer = nullptr);
C_FUNCTION int32_t stringToSLong(const char *buffer, const uint32_t BufferLen, uint32_t base = 10u, RESULT_STATE *state = nullptr, char **endPointer = nullptr);
C_FUNCTION uint32_t stringToULong(const char *buffer, const uint32_t BufferLen, uint32_t base = 10u, RESULT_STATE *state = nullptr, char **endPointer = nullptr);

#endif // _UTILSLIB_STRING_NUMBERS_H
