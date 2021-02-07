#ifndef _DYNAMICARRAY_H
#define _DYNAMICARRAY_H

#include <cstdlib> 
#include <cstdint>
#include <string.h>

#include "utils/utils_def.h"

template<typename T>
int CDECL DefaultComparator(const T *e1, const T *e2)
{
	if (*e1 < *e2)
		return -1;

	if (*e2 < *e1)
		return 1;

	return 0;
}

typedef int32_t (CDECL *comparisor)(void const *e1, void const *e2);

#ifdef AMIGA_OS
	// TODO: We don't want to pull in std here, so we disable it
	#undef USE_STD
#else
	#define USE_STD
#endif

// Iterator from: https://stackoverflow.com/questions/3582608/how-to-correctly-implement-custom-iterators-and-const-iterators
#ifdef USE_STD
	#include <iterator>
#endif

template<typename T>
class RawIterator
{
public:

#ifdef USE_STD
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
#endif

	using value_type = T;
	using pointer = T * ;
	using reference = T & ;

public:
	REGPARAM(1) RawIterator(T *ptr = nullptr) { m_ptr = ptr; }
	REGPARAM(1) RawIterator(const RawIterator<T> &rawIterator) = default;
	REGPARAM(0) ~RawIterator(void) {}

	REGPARAM(1) RawIterator<T> &operator=(const RawIterator<T> &rawIterator) = default;
	REGPARAM(1) RawIterator<T> &operator=(T *ptr) { m_ptr = ptr; return (*this); }

	REGPARAM(0) operator bool(void) const
	{
		return m_ptr != nullptr;
	}

	REGPARAM(1) bool operator==(const RawIterator<T> &rawIterator) const { return (m_ptr == rawIterator.getConstPtr()); }
	REGPARAM(1) bool operator!=(const RawIterator<T> &rawIterator) const { return (m_ptr != rawIterator.getConstPtr()); }

	REGPARAM(1) RawIterator<T> & operator++(void) { ++m_ptr; return (*this); }
	REGPARAM(1) RawIterator<T> &operator--(void) { --m_ptr; return (*this); }
	REGPARAM(1) RawIterator<T> operator++(int) { auto temp(*this); ++m_ptr; return temp; }
	REGPARAM(1) RawIterator<T> operator--(int) { auto temp(*this); --m_ptr; return temp; }

#ifdef USE_STD
	RawIterator<T> &operator+=(const difference_type &movement) { m_ptr += movement; return (*this); }
	RawIterator<T> &operator-=(const difference_type &movement) { m_ptr -= movement; return (*this); }

	RawIterator<T> operator+(const difference_type &movement) { auto oldPtr = m_ptr; m_ptr += movement; auto temp(*this); m_ptr = oldPtr; return temp; }
	RawIterator<T> operator-(const difference_type &movement) { auto oldPtr = m_ptr; m_ptr -= movement; auto temp(*this); m_ptr = oldPtr; return temp; }

	difference_type operator-(const RawIterator<T> &rawIterator) { return std::distance(rawIterator.getPtr(), this->getPtr()); }
#endif

	REGPARAM(0) T &operator*(void) { return *m_ptr; }
	REGPARAM(0) const T &operator*(void) const { return *m_ptr; }
	REGPARAM(0) T *operator->(void) { return m_ptr; }

	REGPARAM(0) T *getPtr(void) const { return m_ptr; }
	REGPARAM(0) const T *getConstPtr(void) const { return m_ptr; }

protected:
	T *m_ptr;
};

template<typename T>
class RawReverseIterator
: public RawIterator<T>
{
public:
	RawReverseIterator(T *ptr = nullptr) : RawIterator<T>(ptr) {}
	RawReverseIterator(const RawIterator<T> &rawIterator) { this->m_ptr = rawIterator.getPtr(); }
	RawReverseIterator(const RawReverseIterator<T> &rawReverseIterator) = default;
	~RawReverseIterator(void) {}

	RawReverseIterator<T> &operator=(const RawReverseIterator<T> &rawReverseIterator) = default;
	RawReverseIterator<T> &operator=(const RawIterator<T> &rawIterator) { this->m_ptr = rawIterator.getPtr(); return (*this); }
	RawReverseIterator<T> &operator=(T *ptr) { this->setPtr(ptr); return (*this); }

	RawReverseIterator<T> &operator++(void) { --this->m_ptr; return (*this); }
	RawReverseIterator<T> &operator--(void) { ++this->m_ptr; return (*this); }
	RawReverseIterator<T> operator++(int) { auto temp(*this); --this->m_ptr; return temp; }
	RawReverseIterator<T> operator--(int) { auto temp(*this); ++this->m_ptr; return temp; }
	RawReverseIterator<T> operator+(const int& movement) { auto oldPtr = this->m_ptr; this->m_ptr -= movement; auto temp(*this); this->m_ptr = oldPtr; return temp; }
	RawReverseIterator<T> operator-(const int& movement) { auto oldPtr = this->m_ptr; this->m_ptr += movement; auto temp(*this); this->m_ptr = oldPtr; return temp; }

#ifdef USE_STD
	RawReverseIterator<T> &operator+=(const typename RawIterator<T>::difference_type &movement) { this->m_ptr -= movement; return (*this); }
	RawReverseIterator<T> &operator-=(const typename RawIterator<T>::difference_type &movement) { this->m_ptr += movement; return (*this); }
	typename RawIterator<T>::difference_type operator-(const RawReverseIterator<T> &rawReverseIterator) { return std::distance(this->getPtr(), rawReverseIterator.getPtr()); }
#endif

	RawIterator<T> base(void) { RawIterator<T> forwardIterator(this->m_ptr); ++forwardIterator; return forwardIterator; }
};

template<typename T>
class DynamicArray
{
public:
	typedef RawIterator<T> iterator;
	typedef RawIterator<const T> const_iterator;

	typedef RawReverseIterator<T> reverse_iterator;
	typedef RawReverseIterator<const T> const_reverse_iterator;

public:
	REGPARAM(3) DynamicArray(size_t defaultSize = 0u, bool initMem = false, uint8_t value = 0)
	{
		init(defaultSize, initMem, value);
	}

	REGPARAM(1) DynamicArray(const DynamicArray &o)
	{
		init(o);
	}

	REGPARAM(0) virtual ~DynamicArray(void)
	{
		m_defaultSize = 0;
		clear();
	}

	REGPARAM(0) iterator begin(void) { return iterator(&m_array[0]); }
	REGPARAM(0) const_iterator begin(void) const { return const_iterator(&m_array[0]); }
	REGPARAM(0) iterator end(void) { return iterator(&m_array[m_elements]); }
	REGPARAM(0) const_iterator end(void) const { return const_iterator(&m_array[m_elements]); }

	REGPARAM(0) const_iterator cbegin(void) const { return const_iterator(&m_array[0]); }
	REGPARAM(0) const_iterator cend(void) const { return const_iterator(&m_array[m_elements]); }

	REGPARAM(0) reverse_iterator rbegin(void) { return reverse_iterator(&m_array[m_elements - 1]); }
	REGPARAM(0) reverse_iterator rend(void) { return reverse_iterator(&m_array[-1]); }

	REGPARAM(0) const_reverse_iterator crbegin(void) { return const_reverse_iterator(&m_array[m_elements - 1]); }
	REGPARAM(0) const_reverse_iterator crend(void) { return const_reverse_iterator(&m_array[-1]); }

	REGPARAM(0) operator const T *(void) const
	{
		return m_array;
	}

	REGPARAM(0) operator T *(void)
	{
		return m_array;
	}

	REGPARAM(1) bool copy(const DynamicArray &o)
	{
		if (this == &o)
			return true;

		clear();

		if (!resize(o.m_elements))
			return false;

		memcpy(m_array, o.m_array, sizeof(T)*o.m_elements);
		m_elements = o.m_elements;
		m_reserved = o.m_reserved;

		return true;
	}

	REGPARAM(1) DynamicArray& operator=(const DynamicArray &o)
	{
		copy(o);

		return *this;
	}

	REGPARAM(0) bool empty(void) const
	{
		return m_elements == 0;
	}

	REGPARAM(0) size_t size(void) const
	{
		return m_elements;
	}

	REGPARAM(0) const T* ptr(void) const
	{
		return m_array;
	}

	REGPARAM(0) T* ptr(void)
	{
		return m_array;
	}

	// this is not needed as we have the cast operator above, which serves to the same effect.
	/*REGPARAM(1) T& operator [] (size_t index)
	{
		return m_array[index];
	}

	REGPARAM(1) const T& operator [] (size_t index) const
	{
		return m_array[index];
	}*/

	REGPARAM(1) bool add(const T &item)
	{
		if (!grow(1))
			return false;

		m_array[m_elements - 1] = item;

		return true;
	}

	REGPARAM(2) bool insert(size_t index, const T &item)
	{
		if (m_elements == 0 || index >= m_elements)
			return add(item);

		if (!grow(1))
			return false;

		memmove(&m_array[index + 1], &m_array[index], sizeof(T)*(m_elements - index));

		m_array[index] = item;
		return true;
	}

	REGPARAM(1) void sort(comparisor comparator = nullptr)
	{
		if (m_elements < 2)
			return;

		if (!comparator)
			comparator = (comparisor)DefaultComparator<T>;

		qsort(m_array, m_elements, sizeof(T), comparator);
	}

	REGPARAM(1) bool reserve(size_t newsize)
	{
		if (newsize > m_reserved)
		{
			void *p = realloc(m_array, sizeof(T)*newsize);
			if (!p)
				return false;

			m_array = (T *)p;
			m_reserved = newsize;
		}

		return true;
	}

	REGPARAM(2) bool reserve(size_t newsize, const T &value)
	{
		if (newsize > m_reserved)
		{
			size_t diff = newsize - m_reserved;
			if (!reserve(newsize))
				return false;

			fillReserved(value, m_reserved - diff, m_reserved);
		}

		return true;
	}

	/**
	 * Fills the array with a given value. Reserved elements will not be filled, only
	 * items which are actually within the current size.
	 */
	REGPARAM(3) void fill(const T &value, size_t start = 0, size_t end = -1)
	{
		if (!m_array)
			return;

		if (end == -1 || end > m_elements)
			end = m_elements;

		for (size_t i = start; i < end; i++)
			m_array[i] = value;
	}

	/**
	 * Fills the array with a given value. This will also fill items which are reserved
	 * but not yet in size.
	 */
	REGPARAM(3) void fillReserved(const T &value, size_t start = 0, size_t end = -1)
	{
		if (!m_array)
			return;

		if (end == -1 || end > m_reserved)
			end = m_reserved;

		for (size_t i = start; i < end; i++)
			m_array[i] = value;
	}

	/**
	 * If this is 0, resize will
	 * realloc when
	 */
	REGPARAM(1) bool resize(size_t newsize)
	{
		size_t sz = m_elements;
		if (newsize > m_defaultSize)
			m_elements = newsize;
		else
			m_elements = m_defaultSize;

		if (m_elements)
		{
			if ((m_elements > m_reserved) || (m_reserved / 2 > 10 && m_elements < m_reserved / 2))
			{
				m_reserved = m_elements;
				void *p = realloc(m_array, sizeof(T)*m_elements);

				if (p == NULL)
				{
					m_reserved = 0;
					m_elements = sz;

					return false;
				}
				m_array = (T *)p;
			}
		}
		else
			clear();

		return true;
	}

	REGPARAM(1) void remove(size_t index)
	{
		if (m_elements == 1)
			clear();
		else
		{
			for (size_t i = index; i < m_elements - 1; i++)
				m_array[i] = m_array[i + 1];

			m_elements--;
		}
	}

	REGPARAM(0) size_t reserved(void) const
	{
		return m_reserved;
	}

	REGPARAM(0) size_t last(void) const
	{
		if (!m_reserved)
			return m_reserved;

		return m_reserved-1;
	}

	REGPARAM(0) bool init(size_t defaultSize = 0u, bool initMem = false, uint8_t value = 0)
	{
		m_reserved = defaultSize;
		m_elements = defaultSize;
		m_defaultSize = defaultSize;

		if (m_defaultSize)
		{
			m_array = (T *)malloc(sizeof(T)*defaultSize);
			if (!m_array)
			{
				m_reserved = 0;
				m_elements = 0;
				m_defaultSize = 0;

				return false;
			}

			if (initMem)
				memset(m_array, value, m_elements * sizeof(T));
		}
		else
			m_array = nullptr;

		return true;
	}

	REGPARAM(1) bool init(const DynamicArray &o)
	{
		if (o.m_reserved)
		{
			m_array = (T *)malloc(sizeof(T)*o.m_reserved);
			if (m_array == NULL)
			{
				m_defaultSize = 0;
				m_elements = 0;
				m_reserved = 0;

				return false;
			}
		}

		memcpy(m_array, o.m_array, sizeof(T)*o.m_reserved);

		m_reserved = o.m_reserved;
		m_elements = o.m_elements;
		m_defaultSize = o.m_defaultSize;

		return true;
	}

	REGPARAM(0) void clear(void)
	{
		if (m_array)
			free(m_array);

		m_array = NULL;
		m_elements = 0;
		m_reserved = 0;
		m_defaultSize = 0;
	}

protected:
	REGPARAM(0) bool grow(size_t items)
	{
		m_elements += items;

		if (m_elements > m_reserved)
		{
			size_t sz = m_reserved;
			if(items > dyn_array_step)
				m_reserved += items;
			else
				m_reserved += dyn_array_step;

			void *p = realloc(m_array, sizeof(T)*m_reserved);

			if (p == NULL)
			{
				m_elements -= items;
				m_reserved = sz;

				return false;
			}

			m_array = (T *)p;
		}

		return true;
	}

	REGPARAM(1) void setElements(size_t elements)
	{
		m_elements = elements;
	}

	REGPARAM(1) void setReserved(size_t reserved)
	{
		m_reserved = reserved;
	}

	REGPARAM(1) void setPtr(T *array)
	{
		m_array = array;
	}

private:
	T *m_array;
	size_t m_elements;
	size_t m_reserved;
	size_t m_defaultSize;

	const static int dyn_array_step = 1;
};

#endif // _DYNAMICARRAY_H
