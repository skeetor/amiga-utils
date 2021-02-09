#include <cstring>

#include "utils/dos/options.h"

const Option *FindOption(const char *name, const Option *options, uint32_t *index)
{
	uint32_t i = 0;
	while (options->m_name)
	{
		if (strcmp(options->m_name, name) == 0)
		{
			if (index)
				*index = i;

			return options;
		}

		options++;
		i++;
	}

	return nullptr;
}

int ValidateOptions(int argc, char **argv, const Option *options, const Option **option, int argStart)
{
	const Option *cur = nullptr;
	uint32_t count = 0;

	for (int i = argStart; i < argc; i++)
	{
		const char *p = argv[i];
		const Option *op = FindOption(p, options);

		if (op != nullptr)
		{
			if (cur)
			{
				if (count < cur->m_min)
				{
					*option = cur;
					return i;
				}

				if (cur->m_max != -1 && count > cur->m_max)
				{
					*option = cur;
					return i;
				}
			}

			cur = op;
			count = 0;
			continue;
		}

		// Unknown parameter
		if (!cur)
		{
			*option = cur;
			return i;
		}

		count++;
	}

	if (cur)
	{
		if (count < cur->m_min)
		{
			*option = cur;
			return argc-1;
		}

		if (cur->m_max != -1 && count > cur->m_max)
		{
			*option = cur;
			return argc - 1;
		}
	}

	// Now check for mandatory parameters
	for (int i = 0; options->m_name; i++, options++)
	{
		if (!options->m_mandatory)
			continue;

		char found = false;
		uint32_t nameLen = strlen(options->m_name);
		for (int pos = argStart; pos < argc; pos++)
		{
			if (strncmp(options->m_name, argv[pos], nameLen) == 0 && strlen(argv[pos]) == nameLen)
			{
				found = true;
				break;
			}
		}
	
		if (found)
			continue;

		*option = options;
		return i;
	}

	*option = nullptr;
	return -1;
}
