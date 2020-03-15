
#include <vector>
#include "utils/string/string.h"

using namespace std;

enum FormatType
{
	FT_NONE,
	FT_WORD,
	FT_LONG,
	FT_PTR
};

union ValueItem
{
	ValueItem(uint16_t value)
	: wordValue(value)
	{
	}

	ValueItem(uint32_t value)
	: longValue(value)
	{
	}

	ValueItem(const char *value)
	: ptr(value)
	{
	}
	
	uint16_t wordValue;
	uint32_t longValue;
	const char *ptr;
};

struct FormatArgument
{
	FormatArgument(void)
	: length(FT_NONE)
	, item((const char *)nullptr)
	{
	}

	FormatArgument(uint16_t value)
	: length(FT_WORD)
	, item(value)
	{
	}

	FormatArgument(uint32_t value)
	: length(FT_WORD)
	, item(value)
	{
	}

	FormatArgument(const char *value)
	: length(FT_WORD)
	, item(value)
	{
	}

	FormatType length;
	ValueItem item;
};

struct Testcase
{
	const char *formatString;
	const char *resultString;
	vector<FormatArgument> arguments;
};


static Testcase gTestcases[] =
{
	 { "", "" }
	,{ "str", "str" }
	,{ "str\n", "str\x0d" }
	,{ "str\r\n", "str\x0a\x0d" }
	,{ "str%u", "str50", { FormatArgument((uint32_t)50), FormatArgument() } }

	,{ nullptr, nullptr }
};
