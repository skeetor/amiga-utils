#ifndef _SORTEDARRAY_H
#define _SORTEDARRAY_H

#include "DynamicArray.h"

#include <string.h>
#include <utils/algorithms/bsearch.h>

template<typename T>
class SortedArray
: public DynamicArray<T>
{
public:
	REGPARAM(1) SortedArray(comparisor comparator = nullptr)
	: DynamicArray<T>()
	{
		setComparator(comparator);
	}

	REGPARAM(1) SortedArray(const SortedArray &o)
	: DynamicArray<T>(o)
	{
		m_comparator = o.m_comparator;
	}

	REGPARAM(0) ~SortedArray(void) override
	{
	}

	REGPARAM(1) void setComparator(comparisor comparator = nullptr)
	{
		if (comparator == nullptr)
			comparator = (comparisor)DefaultComparator<T>;

		if (m_comparator != comparator)
		{
			m_comparator = comparator;
			DynamicArray<T>::sort(comparator);
		}
	}

	/**
	 * Copy the source array into this. If copyFull is set then
	 * also the comparator is copied.
	 * If copyFull is false, then the comparator will not be copied
	 * and the array is resorted according to the current sort order.
	 */
	REGPARAM(2) bool copy(const SortedArray &o, bool copyFull = false)
	{
		if (this == &o)
			return true;

		DynamicArray<T>::operator=(o);
		if(!copyFull)
			setComparator(o.m_comparator);

		return true;
	}

	REGPARAM(1) SortedArray& operator=(const SortedArray &o)
	{
		copy(o, true);

		return *this;
	}

	// Returns the index of the item, or invalid_t if not found
	REGPARAM(2) size_t find(const T &item, size_t start = 0, size_t end = invalid_t) const
	{
		size_t pos;

		if (!tbsearch(&item, &pos, start, end))
			return invalid_t;

		return pos;
	}

	// Adds the item into the array in sorted order.
	// Returns false if the item was not added. In case of an error
	// index is set to invalid_t.
	// So if the item was not added because it already exists, then
	// false is returned and index is != invalid_t.
	REGPARAM(2) bool add(const T &item, size_t *indexPtr = nullptr)
	{
		size_t pos = invalid_t;

		// Item was found, so don't add it and report the position
		if (tbsearch(&item, &pos))
		{
			if (indexPtr)
				*indexPtr = pos;

			return false;
		}

		size_t sz = DynamicArray<T>::size();
		if (!DynamicArray<T>::grow(1))
		{
			if (indexPtr)
				*indexPtr = invalid_t;

			return false;
		}

		T *p = DynamicArray<T>::ptr();

		// If the new entry is located at the end, we don't need to move any memory.
		if (pos != sz)
			memmove(&p[pos + 1], &p[pos], sizeof(T) * (sz - pos));

		p[pos] = item;

		if (indexPtr)
			*indexPtr = pos;

		return true;
	}

protected:
	REGPARAM(0) void init(void)
	{
		DynamicArray<T>::init();
		setComparator(nullptr);
	}

	REGPARAM(1) bool init(const SortedArray &o)
	{
		bool rc = DynamicArray<T>::init(o);
		setComparator(o.m_comparator);

		return rc;
	}

	// Returns true if the item was found.
	// In pos the position before is returned.
	// If the item was not found, pos contains the psoition
	// where it would have to be inserted.
	REGPARAM(4) bool tbsearch(const T *key, size_t *pos, size_t start = 0, size_t end = invalid_t) const
	{
		size_t items = DynamicArray<T>::size();
		if (start > items)
		{
			*pos = invalid_t;

			return false;
		}

		if (end != invalid_t)
			end++;

		if (end > items)
			end = items;

		items = end - start;
		const T *base = &DynamicArray<T>::ptr()[start];
		const T *p;
		bool rc = binarySearch(key, base, items, sizeof(T), (void **)&p, m_comparator);

		if (pos)
			*pos = p - base;

		return rc;
	}

private:
	// Hide this for the sorted array, as we don't need it
	REGPARAM(2) bool insert(size_t index, const T &item)
	{
		return false;
	}

private:
	comparisor m_comparator;
};

#endif // _SORTEDARRAY_H
