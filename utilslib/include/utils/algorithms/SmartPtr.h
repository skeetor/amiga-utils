#ifndef _SMARTPTR_H
#define _SMARTPTR_H

#include <utils/utils_def.h>

template <typename T, typename D = T>
class SmartPtr
{
public:
	typedef void (*deleter)(D);

public:
	REGPARAM(2) SmartPtr(T ptr, deleter f)
	: m_ptr(ptr)
	, m_deleter((deleter)f)
	{
	}

	REGPARAM(0) ~SmartPtr(void)
	{
		release();
	}

	REGPARAM(0) void release(void)
	{
		if (m_ptr)
		{
			if (m_deleter)
				m_deleter(m_ptr);
		}

		m_ptr = nullptr;
	}

	REGPARAM(0) operator T(void) const
	{
		return m_ptr;
	}

private:
	T m_ptr;
	deleter m_deleter;
};

#endif // _SMARTPTR_H
