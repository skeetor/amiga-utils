#ifndef _UTILSLIB_OPTIONS_I
#define _UTILSLIB_OPTIONS_I 1

#include <cstdarg>

#include <utils/utils_def.h>

class Option
{
public:
	Option()
	: m_min(0)
	, m_max(0)
	, m_mandatory(false)
	, m_name(nullptr)
	, m_help(nullptr)
	{
	}

	Option(const char *name, const char *help = nullptr)
	: m_min(0)
	, m_max(0)
	, m_mandatory(false)
	, m_name(name)
	, m_help(help)
	{
	}

	Option(const char *name, uint16_t min, uint16_t max, uint16_t mandatory, const char *help = nullptr)
	: m_min(min)
	, m_max(max)
	, m_mandatory(mandatory)
	, m_name(name)
	, m_help(help)
	{
	}

	uint16_t 	m_min;
	uint16_t 	m_max;
	bool 		m_mandatory:1;
	const char *m_name;
	const char *m_help;
};

/**
 * Finds the specified option in the array, and optionally returns also the index.
 */
const Option *FindOption(const char *name, const Option *options, uint32_t *index = nullptr);

/**
 * Validates the provided arguments against the list of supported options.
 * If the arguments are valid, -1 is returned, otherwise the index of the first
 * parameter not matching. In this case also the option is returned that didn't match.
 */
int ValidateOptions(int argc, char **argv, const Option *options, const Option **option, int argStart = 1);

int FindParam(int argc, char **argv, const char *name, int paramIndex = 0, int start = 1);

#endif // _UTILSLIB_OPTIONS_I
